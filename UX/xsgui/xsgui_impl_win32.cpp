#include "xsgui_impl_win32.h"

HWND _hwnd = NULL;

void xsGui_Win32_Init(HWND hwnd, HINSTANCE hInstance, xsFloat2 _pos)
{
	//--------------------------------------------------------------------------------------
		// 【DirectInput使用五步曲的前三步】：创设备，设格式，拿权力。在为键盘设备初始化
		//--------------------------------------------------------------------------------------
		// createDirectInput接口和设备
	xsGuiIO& g = xs_Gui::GetIO();
	_hwnd = hwnd;
	stack_hidden_call(QueryPerformanceFrequency,(LARGE_INTEGER*)&g.time_Frequency);
	stack_hidden_call(QueryPerformanceCounter,(LARGE_INTEGER*)&g.time_Current);
	g.KeyButton_Later_trigger = g.time_Frequency / 3;
	g.KeyButton_Later_triggerInterval = g.time_Frequency / 17;

	g.DisplaySize = _pos;

	stack_hidden_call(DirectInput8Create,hInstance, (DWORD)0x0800, IID_IDirectInput8W, (void**)&g.g_pDirectInput, (LPUNKNOWN)NULL);


	g.g_pDirectInput->CreateDevice(GUID_SysKeyboard, &g.g_pKeyboardDevice, NULL);//r9=参数3 r8=参数2 rdx=参数1 参数0rcx=*this 类的指针

	// 设置Data格式和协作级别
	g.g_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	g.g_pKeyboardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//获取设备控制权
	g.g_pKeyboardDevice->Acquire();


}

void xsGui_Win32_Init(HWND hwnd, HINSTANCE hInstance)
{
	//--------------------------------------------------------------------------------------
		// 【DirectInput使用五步曲的前三步】：创设备，设格式，拿权力。在为键盘设备初始化
		//--------------------------------------------------------------------------------------
		// createDirectInput接口和设备
	xsGuiIO& g = xs_Gui::GetIO();
	_hwnd = hwnd;
	stack_hidden_call(QueryPerformanceFrequency,(LARGE_INTEGER*)&g.time_Frequency);
	stack_hidden_call(QueryPerformanceCounter,(LARGE_INTEGER*)&g.time_Current);
	g.KeyButton_Later_trigger = g.time_Frequency / 3;
	g.KeyButton_Later_triggerInterval = g.time_Frequency / 17;

	//g.DisplaySize = _pos;

	DirectInput8Create(hInstance, 0x0800, IID_IDirectInput8W, (void**)&g.g_pDirectInput, NULL);


	g.g_pDirectInput->CreateDevice(GUID_SysKeyboard, &g.g_pKeyboardDevice, NULL);//r9=参数3 r8=参数2 rdx=参数1 参数0rcx=*this 类的指针

	// 设置Data格式和协作级别
	g.g_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	g.g_pKeyboardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//获取设备控制权
	g.g_pKeyboardDevice->Acquire();


//	//鼠标
//	g.g_pDirectInput->CreateDevice(GUID_SysMouse, &g.g_pMouseDevice, NULL);//r9=参数3 r8=参数2 rdx=参数1 参数0rcx=*this 类的指针
//
//// 设置Data格式和协作级别
//	g.g_pMouseDevice->SetDataFormat(&c_dfDIMouse2);
//	g.g_pMouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
//
//	//获取设备控制权
//	g.g_pMouseDevice->Acquire();


}

void xsGui_Win32_ReadKeyButton()
{
	xsGuiIO& g = xs_Gui::GetIO();

	INT64 current_time;
	stack_hidden_call(QueryPerformanceCounter, (LARGE_INTEGER*)&current_time);
	g.DeltaTime = (float)(current_time - g.time_Current) / g.time_Frequency;
	g.time_Current = current_time;


	if (g.g_pDirectInput && g.g_pKeyboardDevice)
	{
		memmove(g.KeyButton_Later, g.KeyButton_Current,256);
		memset(g.KeyButton_Current, 0, 256);

		g.g_pKeyboardDevice->Poll();              // 轮询设备
		g.g_pKeyboardDevice->Acquire();           // 获取设备的控制权
		g.g_pKeyboardDevice->GetDeviceState(256, g.KeyButton_Current);

		g.g_pKeyboardDevice->Acquire();
	}
	//if (g.g_pDirectInput && g.g_pMouseDevice)
	//{
	//	memmove(g.Mouse_Later, g.Mouse_Current, sizeof(DIMOUSESTATE2));
	//	memset(g.Mouse_Current, 0, sizeof(DIMOUSESTATE2));

	//	//g.g_pMouseDevice->Poll();              // 轮询设备
	//	//g.g_pMouseDevice->Acquire();           // 获取设备的控制权
	//	g.g_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), g.Mouse_Current);

	//	//g.g_pMouseDevice->Acquire();
	//}
}



void xsGui_Win32_GetWind()
{
	
	if (!_hwnd)
		return ;
	xsGuiIO& io = xs_Gui::GetIO();
	RECT rect;
	stack_hidden_call(GetClientRect,_hwnd, &rect);
	xsFloat2 windo = xsFloat2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

	io.DisplaySize = windo;
	return ;
}
