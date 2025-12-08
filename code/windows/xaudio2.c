#include <xaudio2.h>

#include "xaudio2.h"

static bool32 initialize_xaudio2()
{
    // MessageBox(NULL, "Failed to initialize COM!", "FAILURE", MB_OK);
    if (CoInitializeEx(0, COINIT_MULTITHREADED))
        return 0;

    uint32 flags = XAUDIO2_DEFAULT_PROCESSOR;

#if DEBUG
    flags |= XAUDIO2_DEBUG_ENGINE;
#endif

    if (XAudio2Create(&xaudio2, 0, flags))
        return 0;

    if (IXAudio2_CreateMasteringVoice(xaudio2, &master_voice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, 0, AudioCategory_GameEffects))
        return 0;

    if (IXAudio2Voice_SetVolume(master_voice, 0.1f, 0))
        return 0;

    return 1;
}

static void xaudio2_play_sound(void* memory, uint32 size, int32 sample_rate, int32 bits_per_sample, int32 number_of_channels)
{
    int32 bytes_per_block = (number_of_channels * bits_per_sample) / 8;
    WAVEFORMATEX wave_format =
    {
        .wFormatTag = WAVE_FORMAT_PCM,
        .nChannels = (WORD)number_of_channels,
        .nSamplesPerSec = sample_rate,
        .wBitsPerSample = (WORD)bits_per_sample,
        .nBlockAlign = (WORD)bytes_per_block,
        .nAvgBytesPerSec = sample_rate * bytes_per_block,
        .cbSize = 0
    };

    for (int32 i = 0; i < MAX_SIMULTANEOUS_SOUNDS; i += 1)
    {
        XAudio2Source* xaudio2_source = &xaudio2_sources[i];
        if (!xaudio2_sources[i].is_playing)
        {
            if (IXAudio2_CreateSourceVoice(xaudio2, &xaudio2_source->source_voices, &wave_format,
                                           0, XAUDIO2_DEFAULT_FREQ_RATIO, &xaudio2_callbacks, 0, 0))
                break;

            XAUDIO2_BUFFER buffer = { 0 };
            buffer.AudioBytes = size;
            buffer.pAudioData = memory;
            buffer.Flags = XAUDIO2_END_OF_STREAM;
            buffer.LoopCount = 0;
            buffer.pContext = (void*)xaudio2_source;

            if (IXAudio2SourceVoice_SubmitSourceBuffer(xaudio2_source->source_voices, &buffer, 0))
                break;

            if (IXAudio2SourceVoice_Start(xaudio2_source->source_voices, 0, XAUDIO2_COMMIT_NOW))
                break;

            xaudio2_source->is_playing = 1;

            break;
        }
    }
}

static void destroy_xaudio2()
{
    IXAudio2_Release(xaudio2);
}
