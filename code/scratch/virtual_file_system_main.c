#include <Windows.h>

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;

typedef signed long long size;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef unsigned int bool32;

typedef float float32;
typedef double float64;

typedef unsigned char char8;
typedef unsigned short char16;

typedef unsigned char byte;

#define sizeof(type) (int64)sizeof(type)
#define countof(array) (int64)(sizeof(array) / sizeof(*(array)))
#define lengthof(string) (countof(string) - 1)

#define KILOBYTES(value) ((int64)(value) * 1024)
#define MEGABYTES(value) (KILOBYTES(value) * 1024)
#define GIGABYTES(value) (MEGABYTES(value) * 1024)
#define TERABYTES(value) (GIGABYTES(value) * 1024)

typedef struct
{
    byte* begin;
    byte* current;
    byte* end;
} Arena;

static void arena_initialize(Arena* arena, int64 capacity)
{
    void* memory = VirtualAlloc(0, capacity, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);;
    if (memory)
    {
        arena->begin = memory;
        arena->current = arena->begin;
        arena->end = arena->begin + capacity;
    }
}

static void* arena_allocate(Arena* arena, int64 size)
{
    int32 align = 8;
    int32 remainder = (uint64)arena->current % align;
    if (remainder != 0)
    {
        arena->current += align - remainder;
    }

    void* result = arena->current;

    arena->current += size;

    return result;
}

static void arena_clear(Arena* arena)
{
    arena->current = arena->begin;
}

static void arena_free(Arena* arena)
{
    VirtualFree(arena->begin, 0, MEM_RELEASE);
}

typedef struct
{
    char8** paths;
    int32 count;
} Files;

static int32 get_string_length(char8* string)
{
    int32 length = 0;
    while (*string != '\0')
    {
        string += 1;
        length += 1;
    }
    return length;
}

static void revert_string(char8* string)
{
    int32 length = get_string_length(string);
    for (int32 start = 0, end = length - 1;
        start < end;
        start += 1, end -= 1)
    {
        char8 temporary = string[start];
        string[start] = string[end];
        string[end] = temporary;
    }
}

static void copy_string(char8* destination, char8* source)
{
    while (*source != '\0')
    {
        *destination = *source;
        destination += 1;
        source += 1;
    }
    *destination = '\0';
}

static void append_string(char8* destination, char8* source)
{
    int32 length = get_string_length(destination);
    destination += length;
    copy_string(destination, source);
}

static bool32 is_string_equal(char8* first, char8* second)
{
    if (get_string_length(first) != get_string_length(second))
    {
        return 0;
    }

    while (*first != '\0')
    {
        if (*first != *second)
        {
            return 0;
        }

        first += 1;
        second += 1;
    }

    return 1;
}

static void int_to_string(char8* string, int32 value)
{
    int32 sign = value;
    if (value < 0)
    {
        value = -value;
    }

    int32 index = 0;
    do
    {
        string[index] = (char8)(value % 10) + '0';
        index += 1;
        value /= 10;
    } while (value > 0);

    if (sign < 0)
    {
        string[index] = '-';
        index += 1;
    }

    string[index] = '\0';

    revert_string(string);
}
static Files get_folder_files_recursively(char8* folder_path, Files files)
{
    WIN32_FIND_DATAA find_info;
    HANDLE find_handle = 0;
    char8 search_path[MAX_PATH];
    int64 size = 0;

    copy_string(search_path, folder_path);
    if (search_path[get_string_length(search_path) - 1] != '\\')
    {
        append_string(search_path, "\\");
    }
    append_string(search_path, "*");

    find_handle = FindFirstFileA(search_path, &find_info);
    if (find_handle == INVALID_HANDLE_VALUE)
    {
        return files;
    }

    do
    {
        if (is_string_equal(find_info.cFileName, ".") ||
            is_string_equal(find_info.cFileName, ".."))
        {
            continue;
        }

        if (find_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            char8 full_path[MAX_PATH];
            copy_string(full_path, folder_path);
            if (full_path[get_string_length(full_path) - 1] != '\\')
            {
                append_string(full_path, "\\");
            }
            append_string(full_path, find_info.cFileName);

            files = get_folder_files_recursively(full_path, files);
        }
        else
        {
            files.paths[files.count] = VirtualAlloc(0, KILOBYTES(4), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            copy_string(files.paths[files.count], find_info.cFileName);
            files.count += 1;
        }
    } while (FindNextFile(find_handle, &find_info));

    if (!FindClose(find_handle))
        ExitProcess(1);

    return files;
}

static Files get_folder_files(char8* folder_path)
{
    Files files = { 0 };
    files.paths = VirtualAlloc(0, MEGABYTES(1), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    files = get_folder_files_recursively(folder_path, files);

    return files;
}

void __stdcall mainCRTStartup()
{
    void* stdout = GetStdHandle(STD_OUTPUT_HANDLE);

    byte* files_memory_begin = VirtualAlloc(0, MEGABYTES(256), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    byte* files_memory_current = files_memory_begin;
    int32* file_positions = VirtualAlloc(0, MEGABYTES(32), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    Files files = get_folder_files(".\\");
    for (int32 index = 0; index < files.count; index += 1)
    {
        char8* file_path = files.paths[index];
        
        HANDLE file_handle = CreateFileA(file_path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
        if (file_handle == INVALID_HANDLE_VALUE)
            ExitProcess(1);

        DWORD file_size = GetFileSize(file_handle, 0);
        if (file_size != INVALID_FILE_SIZE)
        {
            DWORD readed_file_size;
            if (ReadFile(file_handle, files_memory_current, file_size, (LPDWORD)&readed_file_size, 0))
            {
                file_positions[index] = (int32)(files_memory_current - files_memory_begin);
                files_memory_current += readed_file_size;
            }
            else
            {
                ExitProcess(1);
            }
        }

        CloseHandle(file_handle);
    }
    char8* result = VirtualAlloc(0, MEGABYTES(512), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    int32 position = 0;
    append_string(result, "static uint8 virtual_file_system_memory[] =\n");
    append_string(result, "{\n");
    for (int32 i = 0; i < (int32)(files_memory_current - files_memory_begin); i += 1)
    {
        // may zero in stringand convert to 0xFF byte
        append_string(result, (char8*)&files_memory_begin[i]);
    }

    append_string(result, "}\n");

    char8 ch[] =
    {
        "static char8* virtual_file_system_paths[] =\n"
        "{\n"
        "   PATHS\n"
        "}\n"
        "\n"
        "static uint64 virtual_file_system_positions[] =\n"
        "{\n"
        "   POSITIONS\n"
        "}\n"
        "\n"
        "static uint8 virtual_file_system_memory[] =\n"
        "{\n"
        "   MEMORY\n"
        "}\n"
    };

    HANDLE file_handle = CreateFileA("virtual_file_system.h", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
    if (file_handle == INVALID_HANDLE_VALUE)
        ExitProcess(1);

    DWORD bytes_written;
    if (WriteFile(file_handle, result, get_string_length(result), &bytes_written, 0))
    {
        
    }

    CloseHandle(file_handle);

    ExitProcess(0);
}