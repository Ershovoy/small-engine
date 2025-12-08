typedef struct
{
    bool32 is_initialized;
    void* memory;
    uint32 size;
    int32 sample_rate;
    int32 bits_per_sample;
    int32 number_of_channels;
} Sound;

typedef struct
{
    // Riff chunk
    char8 riff_id[4];
    uint32 file_size;
    char8 wave_id[4];
    // Format chunk
    char8 format_id[4];
    uint32 format_size;
    uint16 audio_format;
    uint16 number_of_channels;
    uint32 sample_rate;
    uint32 bytes_per_second;
    uint16 bytes_per_block;
    uint16 bits_per_sample;
    // Data chunk
    char8 data_id[4];
    uint32 data_size;
} Wav_file_header;
