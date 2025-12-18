
#include "present.h"

#include <d3d11.h>
#include "xsgui.h"
#include "xsgui_impl_win32.h"
#include "xsgui_impl_dx11.h"
#include "xsgui_menu.h"

#include "hook_call.h"
#include "cheat_data.h"
#include "dx12.h"
#include "menu_call.h"
#include "xs_font.h"
#include "dx_fn.h"

 bool one = 1;
UINT64 ole_present = 0;
UINT64 ole_resize = 0;
UINT64 dll_base = 0;
UINT64 gui_ = 0;
UINT64 time_ = 0;

UINT64 Current_time = 0;//当前
UINT64 Last_time = 0;//上一次
UINT64 Down_time = 0;//倒计时

UINT64* ret_time()
{

	UINT64* axa = &time_;

	return axa;

}
UINT64* ret_gui_()
{
	return &gui_;
}
UINT64* ret_ole_present()
{
	return &ole_present;
}
UINT64* ret_ole_resize()
{
	return &ole_resize;
}
UINT64* ret_dll_base()
{
	return &dll_base;
}
 ID3D11Device* g_pd3dDevice_ = NULL;
 ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
 ID3D11RenderTargetView* mRenderTargetView = NULL;
//void menu_home(xs_MenuUi& a);
//
//void window_call(xs_windowUI& a);


HRESULT __stdcall _present_DX11(IDXGISwapChain* g_pSwapChain, UINT a0, UINT a1)
{
	if (!g_pd3dDevice_)
	{
		stack_hidden_call(GetDeviceFn(g_pSwapChain), g_pSwapChain,__uuidof(ID3D11Device), (void**)&g_pd3dDevice_);

		if (!g_pd3dDevice_)
			goto GG;
		stack_hidden_call_v(GetImmediateContextFn(g_pd3dDevice_), g_pd3dDevice_, (ID3D11DeviceContext**)&g_pd3dDeviceContext);
		if (!g_pd3dDeviceContext)
			goto GG;
		
		ID3D11Texture2D* backBuffer;
		// 获取一个交换链的后台缓冲区指针

		//g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		stack_hidden_call(GetBufferFn(g_pSwapChain), g_pSwapChain, 0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

		// 创建渲染目标视图
		stack_hidden_call(CreateRenderTargetViewFn(g_pd3dDevice_), g_pd3dDevice_,backBuffer, (D3D11_RENDER_TARGET_VIEW_DESC*)NULL, &mRenderTargetView);

		// 每调用一次GetBuffer方法，后台缓冲区的COM引用计数就会递增一次。我们需要在使用完之后释放它
		stack_hidden_call(ReleaseFn(backBuffer), backBuffer);

		//xs_Gui::CreateContext();//xs_gui 初始化
		//_cheat::CreateContext();//作弊架构

		xsGui_DX11_Init(g_pd3dDevice_, g_pd3dDeviceContext);

		if (one)
		{
			one = 0;

			DXGI_SWAP_CHAIN_DESC sd;
			g_pSwapChain->GetDesc(&sd);
			UINT64 module_addr;
			if (getModuleInformation(VMProtectDecryptStringW(L"TheDivision2.exe"), &module_addr))
				xsGui_Win32_Init(sd.OutputWindow, (HINSTANCE)module_addr);
			xs_Gui::GetIO().xsFont->AddFontFromMemoryTTF(ret_ziti(), 1351437, 18.0f, NULL, xs_Gui::GetIO().xsFont->GetGlyphRangesChineseFull());
			xs_Menu_UI::MenuCreation(window_call);//菜单
		}

		goto GG;

	}

	xsGui_Win32_ReadKeyButton();

	//xsGuiIO& io = xs_Gui::GetIO();
	//g_pd3dDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);
	//UINT vps = 1;
	//D3D11_VIEWPORT viewport;
	//g_pd3dDeviceContext->RSGetViewports(&vps, &viewport);
	//io.DisplaySize= { viewport.Width, viewport.Height };


	xsGui_Win32_GetWind();
	
	//{
	//	CleanupRenderTarget();
	//	g_pSwapChain->ResizeBuffers(0, (UINT)HIWORD(xs_Gui::GetIO().DisplaySize.x), (UINT)HIWORD(xs_Gui::GetIO().DisplaySize.y), DXGI_FORMAT_UNKNOWN, 0);
	//	CreateRenderTarget(g_pSwapChain);
	//	goto GG;
	//}
	xsGui_DX11_NewFrame();
	
	xs_Gui::NewFrame();
	{
		//{
		//	char ac[20] = { 0 };
		//	sprintf(ac, "%lld - %lld", Current_time, Down_time);
		//	xs_Gui::GetForegroundDrawList()->AddText(NULL, 18.0f, { 10.0f,20.0f }, 0xFFFFFFFF, ac);
		//}
		
		_cheat::up_cheat_data();
		_cheat::run_cheat();
	
		_cheat::free_cheat_data();
	}

	xs_Menu_UI::MenuUiFrame();
	
	xs_Gui::Render();//74.92
	
	stack_hidden_call_v(OMSetRenderTargetsFn(g_pd3dDeviceContext), g_pd3dDeviceContext,1, &mRenderTargetView, (ID3D11DepthStencilView*)NULL);
	xsGui_DX11_RenderDrawData(xs_Gui::GetDrawData());
	
GG:
	return 0;
}

HRESULT __stdcall _resize_DX11(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {
	/*Renderer::reset_canvas();*/

	if (mRenderTargetView)
	{
		stack_hidden_call(ReleaseFn(mRenderTargetView), mRenderTargetView);
		mRenderTargetView = NULL;
	}
	if (g_pd3dDeviceContext) {
		stack_hidden_call(ReleaseFn(g_pd3dDeviceContext), g_pd3dDeviceContext);
		g_pd3dDeviceContext = NULL;
	}
	if (g_pd3dDevice_) stack_hidden_call(ReleaseFn(g_pd3dDevice_), g_pd3dDevice_);

	g_pd3dDevice_ = nullptr;

	return  0;
}

VOID KeStallExecutionProcessor(ULONG Microseconds) {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	ULONGLONG cycles = (ULONGLONG)Microseconds * frequency.QuadPart / 1000000;

	ULONGLONG start, end;
	QueryPerformanceCounter((LARGE_INTEGER *)&start);

	do {
		QueryPerformanceCounter((LARGE_INTEGER *)&end);
	} while ((end - start) < cycles);
}

bool DX12 = 0;
bool DX12_one = 0;

HRESULT __stdcall _present(IDXGISwapChain* g_pSwapChain, UINT a0, UINT a1)
{
	if (!DX12_one)
	{
		DX12_one = 1;
		
		stack_hidden_call(SetPriorityClass,(HANDLE)-1, REALTIME_PRIORITY_CLASS);
		stack_hidden_call(SetThreadPriority, (HANDLE)-1, THREAD_PRIORITY_ABOVE_NORMAL);
		//ID3D11Device* d3d11Device = nullptr;
		//ID3D12Device* d3d12Device = nullptr;
		//if (SUCCEEDED(g_pSwapChain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {
		//	DX12 = 0;
		//}
		//else if (SUCCEEDED(g_pSwapChain->GetDevice(IID_PPV_ARGS(&d3d12Device)))) {
		//	DX12 = 1;
		//};
		//DX12 = 0;
		//xs_Gui::CreateContext();//xs_gui 初始化
		//xs_Gui::GetIO().time_Current = 0;
		if (dll_base)
		{
			////time_ = *(UINT64*)(dll_base + 0xFF0);
			UINT64 time_s = 0;
			stack_hidden_call(QueryPerformanceFrequency,(LARGE_INTEGER*)&time_s);
			UINT64 time_current = 0;
			stack_hidden_call(QueryPerformanceCounter,(LARGE_INTEGER*)&time_current);
			Down_time = time_ / 10000000 * time_s;
			Last_time = time_current;
			//time_ = time_ * time_s;
			//time_ = time_ + time_current;



			


			//UINT64 start = __rdtsc();
			//KeStallExecutionProcessor(1000); // 暂停1000微秒
			//UINT64 end = __rdtsc();

			//ULONGLONG timeDifference = end - start;

			//time_ = timeDifference * time_ /** 1000*/;
			//time_ += __rdtsc();

			memset((void*)dll_base, 0, 0x1000);
			dll_base = 0;
		}
	}
	//if (__rdtsc() > time_)
	//	goto GG;
	//if (xs_Gui::GetIO().time_Current > time_)
	//	goto GG;
	{
		if (Down_time < 600000000)
		{
			goto GG;
		}
		stack_hidden_call(QueryPerformanceCounter, (PLARGE_INTEGER)&Current_time);


		if (Current_time >= Last_time)
		{
			UINT64 time_d = Current_time - Last_time;
			Last_time = Current_time;
			if (time_d < 50000000)
			{
				Down_time -= time_d;
			}
			else
			{
				goto GG;
			}

		}
		else
		{
			Last_time = Current_time;
			goto GG;
		}
	}

	/*if (DX12)
		hkPresent_DX12((IDXGISwapChain3*)g_pSwapChain, a0, a1);
	else*/
		_present_DX11(g_pSwapChain, a0, a1);
GG:
	return (*(HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT))(ole_present))(g_pSwapChain, a0, a1);
}
HRESULT __stdcall _resize(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags)
{
	/*if (DX12)
		_resize_DX12(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
	else*/
		_resize_DX11(swapChain, bufferCount, width, height, newFormat, swapChainFlags);

	return  (*(HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT))(ole_resize))(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
} 

