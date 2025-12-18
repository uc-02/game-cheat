#pragma once
#include <d3d11.h>

UINT64* ret_ole_present();
UINT64* ret_ole_resize();

UINT64* ret_dll_base();
UINT64* ret_gui_();
UINT64* ret_time();
HRESULT __stdcall _present(IDXGISwapChain* g_pSwapChain, UINT a0, UINT a1);
HRESULT __stdcall _resize(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags); \
