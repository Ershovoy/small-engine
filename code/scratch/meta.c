#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>  // Для работы с файлами и директориями в Windows

#define OUTPUT_FILE "virtual_file_system.h"

// Функция для записи данных в заголовочный файл
void write_header_file(const char* file_paths[], size_t offsets[], unsigned char* file_data, size_t data_size, int file_count)
{
    FILE* file = fopen(OUTPUT_FILE, "w");
    if (!file)
    {
        perror("Unable to create the output file");
        exit(EXIT_FAILURE);
    }

    // Запись путей к файлам
    fprintf(file, "static const char8* vfs_file_paths[] =\n{\n");
    for (int i = 0; i < file_count; i++) 
    {
        fprintf(file, "    \"%s\",\n", file_paths[i]);
    }
    fprintf(file, "};\n\n");

    // Запись смещений файлов
    fprintf(file, "static const int32 vfs_file_offsets[] =\n{\n");
    for (int i = 0; i < file_count; i++)
    {
        fprintf(file, "    %zu,\n", offsets[i]);
    }
    fprintf(file, "};\n\n");

    // Запись бинарных данных файлов
    fprintf(file, "static const byte vfs_file_data[] =\n{\n");
    for (size_t i = 0; i < data_size; i++)
    {
        if (i % 12 == 0)
        {
            fprintf(file, "    ");  // Форматирование вывода
        }
        fprintf(file, "0x%02X, ", file_data[i]);
        if ((i + 1) % 12 == 0)
        {
            fprintf(file, "\n");
        }
    }
    fprintf(file, "\n};\n\n");

    fclose(file);
}

// Функция для чтения файла в бинарном формате
unsigned char* read_file(const char* filename, size_t* size)
{
    FILE* file = fopen(filename, "rb");
    if (!file)
    {
        perror("Unable to open file for reading");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    rewind(file);

    unsigned char* data = (unsigned char*)malloc(*size);
    if (!data)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    fread(data, 1, *size, file);
    fclose(file);
    return data;
}

// Функция для корректного формирования пути (избегает лишнего обратного слеша)
void form_path(char* dest, const char* dir_name, const char* file_name) {
    if (strcmp(dir_name, ".") == 0 || strcmp(dir_name, ".\\") == 0 || strcmp(dir_name, "./") == 0) {
        // Если директория текущая (.), просто сохраняем имя файла
        snprintf(dest, 512, "%s", file_name);
    } else if (dir_name[strlen(dir_name) - 1] == '\\' || dir_name[strlen(dir_name) - 1] == '/') {
        snprintf(dest, 512, "%s%s", dir_name, file_name);  // Если путь уже оканчивается на слеш, просто добавляем имя файла
    } else {
        snprintf(dest, 512, "%s\\%s", dir_name, file_name);  // Иначе добавляем обратный слеш перед именем файла
    }
}

// Функция для обработки файлов в директории с использованием Windows API
void process_directory(const char* dir_name, const char* file_paths[], size_t* offsets, unsigned char** file_data, size_t* total_size, int* file_count)
{
    WIN32_FIND_DATA find_data;
    HANDLE hFind;

    char search_path[512];
    snprintf(search_path, sizeof(search_path), "%s\\*", dir_name);  // Формирование пути для поиска всех файлов в директории

    hFind = FindFirstFile(search_path, &find_data);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    do
    {
        if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            // Получение полного пути к файлу
            char file_path[512];
            form_path(file_path, dir_name, find_data.cFileName);

            // Чтение файла
            size_t file_size;
            unsigned char* file_content = read_file(file_path, &file_size);

            // Сохранение пути к файлу
            file_paths[*file_count] = _strdup(file_path);  // Используем _strdup вместо strdup

            // Сохранение смещения
            offsets[*file_count] = *total_size;

            // Добавление данных файла в общий массив данных
            *file_data = (unsigned char*)realloc(*file_data, *total_size + file_size);
            memcpy(*file_data + *total_size, file_content, file_size);

            *total_size += file_size;
            (*file_count)++;

            free(file_content);
        }
    } while (FindNextFile(hFind, &find_data) != 0);

    FindClose(hFind);
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <directories>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* file_paths[1024];  // Массив для хранения путей к файлам (до 1024 файлов)
    size_t file_offsets[1024];     // Массив для хранения смещений файлов
    unsigned char* file_data = NULL;  // Массив для хранения всех данных файлов
    size_t total_data_size = 0;
    int file_count = 0;

    // Обработка каждой директории, переданной в аргументах командной строки
    for (int i = 1; i < argc; i++)
    {
        process_directory(argv[i], file_paths, file_offsets, &file_data, &total_data_size, &file_count);
    }

    // Генерация заголовочного файла виртуальной файловой системы
    write_header_file(file_paths, file_offsets, file_data, total_data_size, file_count);

    // Освобождение памяти
    for (int i = 0; i < file_count; i++)
    {
        free((void*)file_paths[i]);
    }
    free(file_data);

    printf("VFS header file generated successfully as '%s'.\n", OUTPUT_FILE);
    return EXIT_SUCCESS;
}
