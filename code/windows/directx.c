#include "directx.h"

static bool32 initialize_directx(HWND window)
{
    bool32 result = 0;

    uint32 device_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef DEBUG
    device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL feature_levels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
    };

    result = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, device_flags, feature_levels, countof(feature_levels),
                               D3D11_SDK_VERSION, &directx.device, 0, &directx.device_context);



    return result;
}