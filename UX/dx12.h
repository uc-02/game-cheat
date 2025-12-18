#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>

HRESULT __stdcall hkPresent_DX12(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags);
HRESULT __stdcall _resize_DX12(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);
void HookExecuteCommandLists(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists);
UINT64* ExecuteCommandLists_ret();


