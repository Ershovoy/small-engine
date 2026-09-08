#include "directx.h"

static bool32 read_shader_file()
{

}

static bool32 resize_swap_chain(int32 width, int32 height)
{
    bool32 result = 0;

    directx.device_context->lpVtbl->OMSetRenderTargets(directx.device_context, 0, 0, 0);
    directx.render_target_view->lpVtbl->Release(directx.render_target_view);
    directx.render_target_view = 0;

    result = directx.swap_chain->lpVtbl->ResizeBuffers(directx.swap_chain, 0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

    ID3D11Texture2D* back_buffer = 0;
    directx.swap_chain->lpVtbl->GetBuffer(directx.swap_chain, 0, &IID_ID3D11Texture2D, &back_buffer);
    result = directx.device->lpVtbl->CreateRenderTargetView(directx.device, (ID3D11Resource*)back_buffer, 0, &directx.render_target_view);
    back_buffer->lpVtbl->Release(back_buffer);

    // directx.viewport.Width = (float32)client_width;
    // directx.viewport.Height = (float32)client_height;
    // directx.viewport.MaxDepth = 1;

    return !result;
}

static bool32 initialize_directx(HWND window)
{
    bool32 result = 0;

    uint32 device_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef DEBUG_INFO
    device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL feature_levels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
    };

    result = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, device_flags, feature_levels, countof(feature_levels),
                               D3D11_SDK_VERSION, &directx.device, 0, &directx.device_context);

    IDXGIFactory* dxgi_factory = 0;
    result = CreateDXGIFactory(&IID_IDXGIFactory, &dxgi_factory);

    DXGI_SWAP_CHAIN_DESC swap_chain_description = { 0 };
    swap_chain_description.BufferDesc.Width = (uint32)client_width;
    swap_chain_description.BufferDesc.Height = (uint32)client_height;
    swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_description.SampleDesc.Count = 1;
    swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
    swap_chain_description.BufferCount = 2;
    swap_chain_description.OutputWindow = window;
    swap_chain_description.Windowed = 1;
    swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    result = dxgi_factory->lpVtbl->CreateSwapChain(dxgi_factory, (IUnknown*)directx.device, &swap_chain_description, &directx.swap_chain);

    dxgi_factory->lpVtbl->Release(dxgi_factory);

    result = directx.swap_chain->lpVtbl->GetParent(directx.swap_chain, &IID_IDXGIFactory, &dxgi_factory);
    result = dxgi_factory->lpVtbl->MakeWindowAssociation(dxgi_factory, window, DXGI_MWA_NO_ALT_ENTER);
    dxgi_factory->lpVtbl->Release(dxgi_factory);

    ID3D11Texture2D* back_buffer = 0;
    result = directx.swap_chain->lpVtbl->GetBuffer(directx.swap_chain, 0, &IID_ID3D11Texture2D, &back_buffer);
    result = directx.device->lpVtbl->CreateRenderTargetView(directx.device, (ID3D11Resource*)back_buffer, 0, &directx.render_target_view);
    back_buffer->lpVtbl->Release(back_buffer);

    D3D11_TEXTURE2D_DESC texture_description = { 0 };
    texture_description.Width = GAME_DEFAULT_HORIZONTAL_RESOLUTION;
    texture_description.Height = GAME_DEFAULT_VERTICAL_RESOLUTION;
    texture_description.MipLevels = 1;
    texture_description.ArraySize = 1;
    texture_description.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texture_description.SampleDesc.Count = 1;
    texture_description.Usage = D3D11_USAGE_DYNAMIC;
    texture_description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texture_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    result = directx.device->lpVtbl->CreateTexture2D(directx.device, &texture_description, 0, &directx.texture);
    result = directx.device->lpVtbl->CreateShaderResourceView(directx.device, (ID3D11Resource*)directx.texture, 0, &directx.texture_view);
    directx.texture->lpVtbl->Release(directx.texture);

    directx.viewport.Width = (float32)client_width;
    directx.viewport.Height = (float32)client_height;
    directx.viewport.MaxDepth = 1;

    D3D11_SAMPLER_DESC sampler_description = { 0 };
    sampler_description.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sampler_description.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampler_description.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampler_description.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampler_description.ComparisonFunc = D3D11_COMPARISON_NEVER;

    result = directx.device->lpVtbl->CreateSamplerState(directx.device, &sampler_description, &directx.sampler_state);

    char16 vertex_shader_file_name[] = L"vertex_shader.cso";
    HANDLE file_handle = CreateFileW(vertex_shader_file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (file_handle != INVALID_HANDLE_VALUE)
    {
        DWORD file_size = GetFileSize(file_handle, 0);
        if (file_size != INVALID_FILE_SIZE)
        {
            void* vertex_shader_object = VirtualAlloc(0, file_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

            DWORD readed_file_size = 0;
            if (ReadFile(file_handle, vertex_shader_object, file_size, (DWORD*)&readed_file_size, 0))
            {
                if (readed_file_size == file_size)
                {
                    result = directx.device->lpVtbl->CreateVertexShader(directx.device, vertex_shader_object, file_size, 0, &directx.vertex_shader);
                }
            }
        }

        CloseHandle(file_handle);
    }

    char16 pixel_shader_file_name[] = L"pixel_shader.cso";
    file_handle = CreateFileW(pixel_shader_file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (file_handle != INVALID_HANDLE_VALUE)
    {
        DWORD file_size = GetFileSize(file_handle, 0);
        if (file_size != INVALID_FILE_SIZE)
        {
            void* pixel_shader_object = VirtualAlloc(0, file_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

            DWORD readed_file_size = 0;
            if (ReadFile(file_handle, pixel_shader_object, file_size, (DWORD*)&readed_file_size, 0))
            {
                if (readed_file_size == file_size)
                {
                    result = directx.device->lpVtbl->CreatePixelShader(directx.device, pixel_shader_object, file_size, 0, &directx.pixel_shader);
                }
            }
        }

        CloseHandle(file_handle);
    }

    // char8 vertex_shader_file_name[] = "vertex_shader.cso";
    // uint64 vertex_shader_object_size = get_file_size_implementation(vertex_shader_file_name, lengthof(vertex_shader_file_name));
    // void* vertex_shader_object = VirtualAlloc(0, vertex_shader_object_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    // read_file_implementation(vertex_shader_file_name, lengthof(vertex_shader_file_name), vertex_shader_object);
    // result = directx.device->lpVtbl->CreateVertexShader(directx.device, vertex_shader_object, vertex_shader_object_size, 0, &directx.vertex_shader);
    // VirtualFree(vertex_shader_object, 0, MEM_RELEASE);

    // char8 pixel_shader_file_name[] = "pixel_shader.cso";
    // uint64 pixel_shader_object_size = get_file_size_implementation(pixel_shader_file_name, lengthof(pixel_shader_file_name));
    // void* pixel_shader_object = VirtualAlloc(0, pixel_shader_object_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    // read_file_implementation(pixel_shader_file_name, lengthof(pixel_shader_file_name), pixel_shader_object);
    // result = directx.device->lpVtbl->CreatePixelShader(directx.device, pixel_shader_object, pixel_shader_object_size, 0, &directx.pixel_shader);
    // VirtualFree(pixel_shader_object, 0, MEM_RELEASE);

    return !result;
}

static void directx_draw(void* offscreen_buffer, int32 width, int32 height)
{
    // if (directx.swap_chain && client_width > 0 && client_height > 0)
    //     assert(resize_swap_chain(client_width, client_height));

    // if (!directx.render_target_view)
    //     return;

    float32 clear_color[] = { 1.0f, 0.5f, 0.0f, 1.0f };
    directx.device_context->lpVtbl->ClearRenderTargetView(directx.device_context, directx.render_target_view, clear_color);

    directx.device_context->lpVtbl->IASetPrimitiveTopology(directx.device_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    directx.device_context->lpVtbl->VSSetShader(directx.device_context, directx.vertex_shader, 0, 0);
    directx.device_context->lpVtbl->RSSetViewports(directx.device_context, 1, &directx.viewport);
    directx.device_context->lpVtbl->PSSetShader(directx.device_context, directx.pixel_shader, 0, 0);
    directx.device_context->lpVtbl->PSSetShaderResources(directx.device_context, 0, 1, &directx.texture_view);
    directx.device_context->lpVtbl->PSSetSamplers(directx.device_context, 0, 1, &directx.sampler_state);
    directx.device_context->lpVtbl->OMSetRenderTargets(directx.device_context, 1, &directx.render_target_view, 0);

    D3D11_MAPPED_SUBRESOURCE mapped_subresource;
    directx.device_context->lpVtbl->Map(directx.device_context, (ID3D11Resource*)directx.texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);

    uint32* source_row = (uint32*)offscreen_buffer;
    uint32* destination_row = (uint32*)mapped_subresource.pData;
    for (int32 y = 0; y < height; y += 1)
    {
        uint32* source_pixel = source_row;
        uint32* destination_pixel = destination_row;
        for (int32 x = 0; x < width; x += 1)
        {
            *destination_pixel = *source_pixel;

            source_pixel += 1;
            destination_pixel += 1;
        }
        source_row += width;
        destination_row += mapped_subresource.RowPitch / 4;
    }

    directx.device_context->lpVtbl->Unmap(directx.device_context, (ID3D11Resource*)directx.texture, 0);

    directx.device_context->lpVtbl->Draw(directx.device_context, 4, 0);

    directx.swap_chain->lpVtbl->Present(directx.swap_chain, 1, 0);
}

static void deinitialize_directx(void)
{
    directx.pixel_shader->lpVtbl->Release(directx.pixel_shader);
    directx.vertex_shader->lpVtbl->Release(directx.vertex_shader);
    directx.sampler_state->lpVtbl->Release(directx.sampler_state);
    directx.texture_view->lpVtbl->Release(directx.texture_view);
    directx.render_target_view->lpVtbl->Release(directx.render_target_view);
    directx.swap_chain->lpVtbl->Release(directx.swap_chain);
    directx.device_context->lpVtbl->Release(directx.device_context);
    directx.device->lpVtbl->Release(directx.device);
}
