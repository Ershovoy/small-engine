static IXAudio2* xaudio2 = 0;
static IXAudio2MasteringVoice* master_voice = 0;

typedef struct
{
    IXAudio2SourceVoice* source_voices;
    bool32 is_playing;
} XAudio2Source;

#define MAX_SIMULTANEOUS_SOUNDS 64

static XAudio2Source xaudio2_sources[MAX_SIMULTANEOUS_SOUNDS] = { 0 };

static void on_buffer_end(IXAudio2VoiceCallback* this, void* buffer_context)
{
    XAudio2Source* xaudio2_source = (XAudio2Source*)buffer_context;
    if (xaudio2_source)
    {
        IXAudio2SourceVoice_DestroyVoice(xaudio2_source->source_voices);
        xaudio2_source->is_playing = 0;
    }
}

static void on_stream_end(IXAudio2VoiceCallback* this) { }
static void on_voice_processing_pass_end(IXAudio2VoiceCallback* this) { }
static void on_voice_processing_pass_start(IXAudio2VoiceCallback* this, uint32 samples_required) { }
static void on_buffer_start(IXAudio2VoiceCallback* this, void* buffer_context) { }
static void on_loop_end(IXAudio2VoiceCallback* this, void* buffer_context) { }
static void on_voice_error(IXAudio2VoiceCallback* this, void* buffer_context, HRESULT error) { }

static IXAudio2VoiceCallbackVtbl xaudio2_callbacks_vtbl = { 0 };

static IXAudio2VoiceCallback xaudio2_callbacks = { 0 };
