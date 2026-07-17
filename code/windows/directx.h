// static bool32 enable_vertical_sync = 0;

typedef struct
{
    ID3D11Device* device;
    ID3D11DeviceContext* device_context;
    IDXGISwapChain* swap_chain;
    ID3D11RenderTargetView* render_target_view;
    ID3D11RasterizerState* rasterizer_state;
    ID3D11SamplerState* sampler_state;
    ID3D11VertexShader* vertex_shader;
    ID3D11PixelShader* pixel_shader;
    ID3D11Texture2D* texture;
    ID3D11ShaderResourceView* texture_view;

    uint32 render_target_width;
    uint32 render_target_height;
    uint32 texture_width;
    uint32 texture_height;
} Directx;

static Directx directx = { 0 };

//static ID3D11Device* device = { 0 };
//static ID3D11DeviceContext* device_context = { 0 };
// static IDXGISwapChain* swap_chain = { 0 };

// static ID3D11RenderTargetView* render_target_view = { 0 };
// static ID3D11RasterizerState* rasterizer_state = { 0 };
// static ID3D11SamplerState* sampler_state = { 0 };
// static ID3D11VertexShader* vertex_shader = { 0 };
// static ID3D11PixelShader* pixel_shader = { 0 };
// static ID3D11Texture2D* texture = { 0 };
// static ID3D11ShaderResourceView* texture_view = { 0 };
//
// static uint32 render_target_width = 0;
// static uint32 render_target_height = 0;
// static uint32 texture_width = 0;
// static uint32 texture_height = 0;
