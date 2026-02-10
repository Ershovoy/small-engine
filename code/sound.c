Sound read_sound_file(char8* file_name, int64 file_name_length, void* buffer)
{
    Sound result = { 0 };

    int64 file_size = get_file_size(file_name, file_name_length);
    if (read_file(file_name, file_name_length, buffer))
    {
        Wav_file_header* wav_file_header = (Wav_file_header*)buffer;

        result.memory = wav_file_header + 1;
        result.size = wav_file_header->data_size;
        result.sample_rate = wav_file_header->sample_rate;
        result.bits_per_sample = wav_file_header->bits_per_sample;
        result.number_of_channels = wav_file_header->number_of_channels;
        result.is_initialized = 1;
    }

    return result;
}
