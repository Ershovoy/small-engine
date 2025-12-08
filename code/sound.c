Sound read_sound_file(char8* file_name)
{
    Sound result = { 0 };

    uint32 file_size = platform_api.get_file_size(file_name);
    void* file_memory = platform_api.allocate_memory(file_size);
    if (platform_api.read_file(file_name, file_memory))
    {
        Wav_file_header* wav_file_header = (Wav_file_header*)file_memory;

        result.memory = wav_file_header + 1;
        result.size = wav_file_header->data_size;
        result.sample_rate = wav_file_header->sample_rate;
        result.bits_per_sample = wav_file_header->bits_per_sample;
        result.number_of_channels = wav_file_header->number_of_channels;
        result.is_initialized = 1;
    }

    return result;
}
