// dllmain.cpp : 定义 DLL 应用程序的入口点。

#include <windows.h>
#include <intrin.h>
#include "present.h"
#include "hook_call.h"
#include "cheat_data.h"
#include <stdio.h>
#include "VMProtectSDK.h"

typedef struct _ux_config {
    UINT64 dll_addr;
    UINT64 dll_main;
    UINT64 ux_time;

}_ux_config;
_ux_config* a = NULL;


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call) {
    case (DLL_PROCESS_ATTACH): {
        //DisableThreadLibraryCalls(hModule);
        stack_hidden_call_init();
        a = (_ux_config*)((UINT64)hModule + 0xF60);
        //if (!a->dll_addr /*|| !a->ux_time*/)
        //    break;
        xs_Gui::CreateContext();//xs_gui 初始化
        _cheat::CreateContext();//作弊架构
    

        const WCHAR* _wchar = VMProtectDecryptStringW(L"TheDivision2.exe");//48 85 C9 74 37 53 48 83 EC 20 48 8B DA 48 8B D1
        const char* pattern = VMProtectDecryptStringA("\x48\x85\xC9\x74\x37\x53\x48\x83\xEC\x20\x48\x8B\xDA\x48\x8B\xD1");//90 48 8B 5C 24 40 44 8B 44
        UINT64 old_present_ = findPattern(_wchar, pattern, 16);
		VMProtectFreeString(pattern);

		pattern = VMProtectDecryptStringA("\x48\x8B\x84\xC1????\xC3\x48\x8B\x81");//48 8B 84 C1 ?? ?? ?? ?? C3 48 8B 81
		UINT64 P1_present_ = findPattern(_wchar, pattern, 12);
		P1_present_ += 0xC;

        VMProtectFreeString(pattern);
        VMProtectFreeString(_wchar);
        old_present_ += 0x13;
        old_present_ = Compile_PointAddr(old_present_);
        old_present_ = *(UINT64*)old_present_;

        old_present_ += *(UINT32*)P1_present_;
        old_present_ = *(UINT64*)old_present_;
        old_present_ += 0x88;
        old_present_ = *(UINT64*)old_present_;
        //old_present_ = *(UINT64*)old_present_;
        UINT64 old_present_a_ = (UINT64)stack_hidden_call(malloc, 0x1000);
        memset((void*)old_present_a_, 0, 0x1000);
        memmove((void*)old_present_a_, *(UINT64**)old_present_, 0x1000);

        *(UINT64*)old_present_ = old_present_a_;
        old_present_ = *(UINT64*)old_present_;

        *ret_dll_base() = a->dll_addr;
        *ret_time() = a->ux_time;

        *ret_ole_present() = *(UINT64*)(old_present_+0x40);
        *ret_ole_resize() = *(UINT64*)(old_present_ + 0x68);

        *(UINT64*)(old_present_ + 0x40) = (UINT64)&_present;
        *(UINT64*)(old_present_ + 0x68) = (UINT64)&_resize;
       /* hook_up(old_present_ + 0x40, (UINT64)&_present, 1);
        hook_up(old_present_ + 0x68, (UINT64)&_resize, 1);*/
        memset((void*)a->dll_addr, 0, 0x1000);
        break;
    }
    case (DLL_PROCESS_DETACH): {

        break;
    }
    }

    return TRUE;
}

//extern "C"  __declspec(dllexport) void __stdcall qidong(UINT64 RCX,UINT64 RDX, UINT64 R8)
//{
//    //char dx11_this[256] = { 0 };
//    //sprintf(dx11_this, "RCX %llx  RDX %llx  R8 %llx", RCX, RDX, R8);
//    //////MessageBoxA(0, dx11_this, "", 0);
//
//        (*(BOOL(__stdcall*)//不调用的话macoll会崩溃
//        (HMODULE hModule,
//            DWORD  ul_reason_for_call,
//            LPVOID lpReserved))
//        (RDX))
//        ((HMODULE)R8, 1, 0);
//        xs_Gui::CreateContext();//xs_gui 初始化
//        _cheat::CreateContext();//作弊架构
//    UINT64 old_present_ = findPattern(L"TheDivision2.exe", "\x48\x85\xC9\x74\x37\x53\x48\x83\xEC\x20\x48\x8B\xDA\x48\x8B\xD1");
//    old_present_ += 0x13;
//    old_present_ = Compile_PointAddr(old_present_);
//    old_present_ = *(UINT64*)(old_present_ );
//    old_present_ = *(UINT64*)(old_present_ + 0x508);
//    old_present_ = *(UINT64*)(old_present_ + 0x88);
//    old_present_ = *(UINT64*)(old_present_);
//    old_present_ += 0x40;
//    *ret_ole_present() = *(UINT64*)old_present_;
//
//    
//    //sprintf(dx11_this,"dx11_this %llx   %llx", old_present_, *ret_ole_present());
//    //////MessageBoxA(0, dx11_this, "", 0);
//
//    DWORD oldProtect, oldProtect2;
//    VirtualProtect((LPVOID)old_present_, 8, PAGE_EXECUTE_READWRITE, &oldProtect);
//    *(UINT64*)old_present_ = (UINT64)&_present;
//    VirtualProtect((LPVOID)old_present_, 8, oldProtect, &oldProtect2);
//
//}

//extern "C"  __declspec(dllexport) void* __stdcall qidong(PVOID apc, PVOID dll_地址, PVOID main_地址)
//{
//    //初始化
//    //ULONG64 dllmain_地址 = *(ULONG64*)((ULONG64)&qidong - 0x1020 + 0xf00);
//    //ULONG64 模块_地址 = *(ULONG64*)((ULONG64)&qidong - 0x1020 + 0xf08);
//
//    (*(BOOL(__stdcall*)
//        (HMODULE hModule,
//            DWORD  ul_reason_for_call,
//            LPVOID lpReserved))
//        (main_地址))
//        ((HMODULE)dll_地址, 1, 0);
//
//    UINT64 overlay64地址 = (UINT64)apc;
//    //////MessageBoxA(0, "mian启动", "", 0);
//    //if (overlay64地址)
//    //    ////MessageBoxA(0, "overlay64未初始化", "", 0);
//    //else
//    //    ////MessageBoxA(0, "overlay64未启动", "", 0);
//    //if (!*ret_gui_())
//        *ret_gui_() = overlay64地址;
//    //if (!*ret_ole_present())
//        *ret_ole_present() = *(UINT64*)overlay64地址;
//    //if (!*ret_ole_resize())
//        *ret_ole_resize() = *(UINT64*)(overlay64地址 + 8);
//    //if (!*ret_dll_base())
//        *ret_dll_base() = (UINT64)dll_地址;
//
//
//    *(UINT64*)overlay64地址 = (UINT64)_present;
//    *(UINT64*)(overlay64地址 + 8) = (UINT64)_resize;
// 
//
//    ULONG64 回call = ((ULONG64)dll_地址 + 0xF52);
//    return (*(void*(__fastcall*)())(回call))();
//}

//extern "C"  __declspec(dllexport) void __stdcall qidong()
//{
//    //初始化
//    ULONG64 main_地址 = (ULONG64)&qidong - 0x1020 + 0xf00;
//    ULONG64 hModule = (ULONG64)&qidong - 0x1020 + 0xf08;
//
//    (*(BOOL(__stdcall*)
//        (HMODULE hModule,
//            DWORD  ul_reason_for_call,
//            LPVOID lpReserved))
//        (main_地址))
//        ((HMODULE)hModule, 1, 0);
//
//    ////MessageBoxA(0,"初始化完成","",0);
//
//    ULONG64 回call = (ULONG64)&qidong - 0x1020 + 0xf10;
//
//    
//    return (*(void(__fastcall*)())(回call))();
//}



//
//WNDCLASSEX wc = {};
//wc.cbSize = sizeof(wc);
//wc.style = CS_OWNDC;
//wc.hInstance = GetModuleHandle(NULL);
//wc.lpfnWndProc = DefWindowProc;
//wc.lpszClassName = _T("DummyWindow");
//if (RegisterClassEx(&wc) == 0)
//{
//	MessageBox(NULL, _T("注册window类失败"), _T(""), MB_OK);
//	return 0;
//}
//
//HWND hwnd = CreateWindowEx(0, wc.lpszClassName, _T(""), WS_OVERLAPPEDWINDOW, 0, 0, 640, 480, NULL, NULL, wc.hInstance, NULL);
//if (hwnd == NULL)
//{
//	MessageBox(NULL, _T("创建window失败"), _T(""), MB_OK);
//	return 0;
//}
//
//// 初始化D3D
//
//IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
//if (d3d9 == NULL)
//{
//	MessageBox(NULL, _T("创建D3D失败"), _T(""), MB_OK);
//	DestroyWindow(hwnd);
//	return 0;
//}
//
//D3DPRESENT_PARAMETERS pp = {};
//pp.Windowed = TRUE;
//pp.SwapEffect = D3DSWAPEFFECT_COPY;
//
//IDirect3DDevice9* device;
//if (FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
//	D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp, &device)))
//{
//	MessageBox(NULL, _T("创建设备失败"), _T(""), MB_OK);
//	d3d9->Release();
//	DestroyWindow(hwnd);
//	return 0;
//}
//
//// hook EndScene
//endSceneAddr = (*(void***)device)[42]; // EndScene是IDirect3DDevice9第43个函数
//hook(endSceneAddr, MyEndScene, endSceneOldCode);
//
//// 释放
//d3d9->Release();
//device->Release();
//DestroyWindow(hwnd);//创建Windows只不过是需要