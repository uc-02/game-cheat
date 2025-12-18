
#include "dx12.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include "xsgui.h"
#include "xsgui_impl_win32.h"
//#include "xmgui\xsgui_执行_dx12.h"
#include "xsgui_menu.h"

#include "hook_call.h"
#include "cheat_data.h"
#include "menu_call.h"

namespace DirectX12Interface {
	ID3D12Device* Device = nullptr;
	ID3D12DescriptorHeap* DescriptorHeapBackBuffers;
	ID3D12DescriptorHeap* DescriptorHeapImGuiRender;
	ID3D12GraphicsCommandList* CommandList;
	ID3D12CommandQueue* CommandQueue;

	struct _FrameContext {
		ID3D12CommandAllocator* CommandAllocator;
		ID3D12Resource* Resource;
		D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle;
	};

	UINT64 BuffersCounts = -1;
	_FrameContext* FrameContext;
}
UINT64 ExecuteCommandLists = 0;

UINT64* ExecuteCommandLists_ret()
{
	return &ExecuteCommandLists;
}


bool one_DX12 = 1;
bool one_DX12_2 = 1;

//CreateHook(54, (void**)&oExecuteCommandLists, hkExecuteCommandLists);
//CreateHook(140, (void**)&oPresent, hkPresent);
//=========================================================================================================================//

HRESULT __stdcall hkPresent_DX12(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags) {
	if (one_DX12) {
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D12Device), (void**)&DirectX12Interface::Device))) {
			DXGI_SWAP_CHAIN_DESC Desc;
			pSwapChain->GetDesc(&Desc);

			DirectX12Interface::BuffersCounts = Desc.BufferCount;
			DirectX12Interface::FrameContext = new DirectX12Interface::_FrameContext[DirectX12Interface::BuffersCounts];

			D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender = {};
			DescriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			DescriptorImGuiRender.NumDescriptors = DirectX12Interface::BuffersCounts;
			DescriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

			if (DirectX12Interface::Device->CreateDescriptorHeap(&DescriptorImGuiRender, IID_PPV_ARGS(&DirectX12Interface::DescriptorHeapImGuiRender)) != S_OK)
				goto GG;

			ID3D12CommandAllocator* Allocator;
			if (DirectX12Interface::Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator)) != S_OK)
				goto GG;

			for (size_t i = 0; i < DirectX12Interface::BuffersCounts; i++) {
				DirectX12Interface::FrameContext[i].CommandAllocator = Allocator;
			}

			if (DirectX12Interface::Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL, IID_PPV_ARGS(&DirectX12Interface::CommandList)) != S_OK ||
				DirectX12Interface::CommandList->Close() != S_OK)
				goto GG;

			D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers;
			DescriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			DescriptorBackBuffers.NumDescriptors = DirectX12Interface::BuffersCounts;
			DescriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DescriptorBackBuffers.NodeMask = 1;

			if (DirectX12Interface::Device->CreateDescriptorHeap(&DescriptorBackBuffers, IID_PPV_ARGS(&DirectX12Interface::DescriptorHeapBackBuffers)) != S_OK)
				goto GG;

			const auto RTVDescriptorSize = DirectX12Interface::Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = DirectX12Interface::DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart();

			for (size_t i = 0; i < DirectX12Interface::BuffersCounts; i++) {
				ID3D12Resource* pBackBuffer = nullptr;
				DirectX12Interface::FrameContext[i].DescriptorHandle = RTVHandle;
				pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
				DirectX12Interface::Device->CreateRenderTargetView(pBackBuffer, nullptr, RTVHandle);
				DirectX12Interface::FrameContext[i].Resource = pBackBuffer;
				RTVHandle.ptr += RTVDescriptorSize;
			}
			//xs_Gui::CreateContext();//xs_gui 初始化
			//_cheat::CreateContext();//作弊架构
			//xmGui_执行DX12_初始(DirectX12Interface::Device, DirectX12Interface::BuffersCounts, DXGI_FORMAT_R8G8B8A8_UNORM, DirectX12Interface::DescriptorHeapImGuiRender, DirectX12Interface::DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(), DirectX12Interface::DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());

			/*DirectX12Interface::CommandQueue = (ID3D12CommandQueue*)*(UINT64*)(_cheat::get_cheat_data()->module_addr + 0x66E4968);*/
			if (one_DX12_2)
			{
				one_DX12_2 = 0;
				//xmGui_执行Win32_初始(Desc.OutputWindow, (HINSTANCE)_cheat::get_cheat_data()->module_addr);
				//xs_Gui_UI::xsGui_UI_创建window(window_call);//菜单


				//DirectX12Interface::CommandQueue = (ID3D12CommandQueue*)*(UINT64*)(_cheat::get_cheat_data()->module_addr + 0x66E4A08);
			}
			one_DX12 = 0;
		}
		
	}

	if (DirectX12Interface::CommandQueue == nullptr)
		goto GG;


	//xmGui_执行Win32_读取键盘();
	//xsGuiIO& io = xs_Gui::GetIO();
	//g_pd3dDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);
	//UINT vps = 1;
	//D3D11_VIEWPORT viewport;
	//g_pd3dDeviceContext->RSGetViewports(&vps, &viewport);
	//io.DisplaySize= { viewport.Width, viewport.Height };

	//xsGui_执行Win32_获取window信息();
	//xsGui_执行DX12_新框架();

	//xs_Gui::预提();
	{
		//_cheat::up_cheat_data();

		//_cheat::run_cheat();

		//_cheat::free_cheat_data();
	}
	//xs_Gui_UI::xsGui_UI预提();

	DirectX12Interface::_FrameContext& CurrentFrameContext = DirectX12Interface::FrameContext[pSwapChain->GetCurrentBackBufferIndex()];
	CurrentFrameContext.CommandAllocator->Reset();

	D3D12_RESOURCE_BARRIER Barrier;
	Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	Barrier.Transition.pResource = CurrentFrameContext.Resource;
	Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	DirectX12Interface::CommandList->Reset(CurrentFrameContext.CommandAllocator, nullptr);
	DirectX12Interface::CommandList->ResourceBarrier(1, &Barrier);
	DirectX12Interface::CommandList->OMSetRenderTargets(1, &CurrentFrameContext.DescriptorHandle, FALSE, nullptr);
	DirectX12Interface::CommandList->SetDescriptorHeaps(1, &DirectX12Interface::DescriptorHeapImGuiRender);

	//xs_Gui::上提();//74.92
	//xsGui_执行DX12_上提绘制Data(xs_Gui::获取绘制Data(), DirectX12Interface::CommandList);

	Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	DirectX12Interface::CommandList->ResourceBarrier(1, &Barrier);
	DirectX12Interface::CommandList->Close();
	DirectX12Interface::CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&DirectX12Interface::CommandList));
GG:
	return 0;
}

//=========================================================================================================================//

HRESULT __stdcall _resize_DX12(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {
	/*Renderer::reset_canvas();*/
	//xsGui_执行DX12_销毁();
	//DirectX12Interface::CommandQueue = nullptr;
	//delete[] DirectX12Interface::FrameContext;
 //   DirectX12Interface::FrameContext = nullptr;

	//DirectX12Interface::CommandList = nullptr;
	//DirectX12Interface::DescriptorHeapImGuiRender = nullptr;
	//DirectX12Interface::DescriptorHeapBackBuffers = nullptr;
	//one_DX12 = 1;
	return 0;
}



void HookExecuteCommandLists(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists) {
	if (!DirectX12Interface::CommandQueue/* && queue->GetDesc().Type == D3D12_COMMAND_LIST_TYPE_DIRECT*/) {
		DirectX12Interface::CommandQueue = queue;
	}

	return (*(void(__stdcall*)(ID3D12CommandQueue*, UINT, ID3D12CommandList*))(ExecuteCommandLists))(queue, NumCommandLists, ppCommandLists);
}

