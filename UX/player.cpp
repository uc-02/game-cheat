#include "cheat_data.h"
#include<cmath>


static float global_speed_ole_value = 0;


UINT64 ret_addr_player_speed = 0;
UINT64 ole_player_speed = 0;
xsFloat4* speed__ = NULL;


inline  void hook_player_speed(UINT64 rcx)
{
	//1B000100000 - 48 B8 29FA5713F77F0000 - mov rax, TheDivision2.exe+30EFA29{ (72) }
//1B00010000A - 48 39 04 24 - cmp [rsp], rax
//1B00010000E - 74 0E - je 1B00010001E
//1B000100010 - FF25 00000000 205BB423FE7F0000 - jmp KERNEL32.DLL+15B20
//1B00010001E - 48 B8 00011000B0010000 - mov rax, 000001B000100100{ (0) }
//1B000100028 - 0F28 30 - movaps xmm6, [rax]
//1B00010002B - 0F59 B5 80000000 - mulps xmm6, [rbp+00000080]
//1B000100032 - 0F29 B5 80000000 - movaps [rbp+00000080], xmm6
//1B000100039 - FF25 00000000 205BB423FE7F0000 - jmp KERNEL32.DLL+15B20

	__asm {
		////pop rbp
		//mov rax, ret_addr_player_speed
		//cmp qword ptr[rsp+0x18], rax
		//jne not_equal
		//mov rax, speed__
		//movaps xmm6, [rax]
		//mulps xmm6, [rbp + 0x70]
		//movaps [rbp + 0x70], xmm6
		//not_equal :
		//call ole_player_speed
		mov rax, ret_addr_player_speed
		cmp qword ptr[rsp + 0x48], rax
		jne no_speed_xyz

		movaps xmm6, xmmword ptr[rsp + 0xDC]
		movaps xmm7, xmmword ptr[rsp + 0xC0]
		movaps xmm8, xmmword ptr[rsp + 0xC4]

		xorps xmm1, xmm1
		ucomiss xmm6, xmm1
		jz no_speed_xyz
		jnp no_speed_xyz

		xorps xmm1, xmm1
		ucomiss xmm8, xmm1
		jz no_speed_xyz
		jnp no_speed_xyz


		mov rax, speed__


		mulss xmm6, dword ptr[rax]
		mulss xmm7, dword ptr[rax + 0x4]
		mulss xmm8, dword ptr[rax + 0x8]


		movaps xmmword ptr[rsp + 0xDC], xmm6
		movaps xmmword ptr[rsp + 0xC0], xmm7
		movaps xmmword ptr[rsp + 0xC4], xmm8


		movaps xmm8, xmmword ptr[rbp - 0x10]
		movaps xmm7, xmmword ptr[rbp - 0x20]
		movaps xmm6, xmmword ptr[rbp - 0x30]

		no_speed_xyz :
			add rsp,0x40
			pop rbp
				mov rax, ole_player_speed
			jmp rax
	}

}

void _Player::hook_speed_init()
{
	const WCHAR* TheDivision2_wchar = VMProtectDecryptStringW(L"TheDivision2.exe");
	const char* pattern = VMProtectDecryptStringA("\x0F\x57\xC0\xF2\x48\x0F\x2A\xC1\x48\x85\xC9");//0F 57 C0 F2 48 0F 2A C1 48 85 C9
	UINT64 find_addr = findPattern(TheDivision2_wchar, pattern, 11);
	VMProtectFreeString(pattern);
	
	find_addr += 0x1D;
	find_addr = Compile_PointAddr(find_addr);
	find_addr += 4;
	speed__ = player_speed_value;
	global_speed_value = (float*)(find_addr);
	global_speed_ole_value = *global_speed_value;
	//char aa[256];
	//sprintf(aa, "%llx %f", global_speed_value, *global_speed_value);
	//////MessageBoxA(0, aa, "", 0);

	/*
	//48 8B CB FF 90 80 02 00 00 44 38 +0x9
	pattern = VMProtectDecryptStringA("\x48\x8B\xCB\xFF\x90\x80\x02??\x44\x38");
	find_addr = findPattern(TheDivision2_wchar, pattern, 11);
	VMProtectFreeString(pattern);
	find_addr += 0x9;
	ret_addr_player_speed = find_addr;


	//48 8B 8B 68 01 00 00 48 8D 54 24 20 -0xE
	////MessageBoxA(0, VMProtectDecryptStringA("13-0"), 0, 0);
	
	pattern = VMProtectDecryptStringA("\x48\x8B\x8B\x68\x01??\x48\x8D\x54\x24\x20");
	find_addr = findPattern(TheDivision2_wchar, pattern, 12);
	VMProtectFreeString(pattern);
	find_addr -= 0xE;

	ole_player_speed = find_addr;

	find_addr = findPattern(TheDivision2_wchar, find_addr, 1);

	//char aa[256];
	//sprintf(aa, "%llx  %llx %llx", ret_addr_player_speed, ole_player_speed, find_addr);
	//MessageBoxA(0, aa, 0, 0);

	//hook_up(find_addr, (UINT64)&hook_player_speed, 1);
	*/
	VMProtectFreeString(TheDivision2_wchar);
}

bool speed_bool_inti = 0;
bool speed_bool_inti_shift = 0;
bool speed_bool_inti_x = 0;
bool speed_bool_inti_f = 0;
inline void _Player::player_speed()
{
	if (speed__)
		*speed__ = { speed.value,speed.value,speed.value,1.0f };

	if (global_speed_value&&global_speed_bool)
	{
		if (!shift_g_speed && !x_g_speed && !f_g_speed)
		{
			if (!speed_bool_inti)
			{
				speed_bool_inti = 1;
				*global_speed_value = global_speed_ole_value + global_speed_ole_value * (g_speed.value * 0.1f);
			}
		}
		else
		{
			if (!speed_bool_inti_x && !speed_bool_inti_f)
				if (shift_g_speed&&xs_Gui::GetCurrentKeyButton()[DIK_LSHIFT] && !speed_bool_inti)
				{
					speed_bool_inti = 1;
					speed_bool_inti_shift = 1;
					*global_speed_value = global_speed_ole_value + global_speed_ole_value * (g_speed.value * 0.1f);
				}
				else if (shift_g_speed && !xs_Gui::GetCurrentKeyButton()[DIK_LSHIFT] && speed_bool_inti)
				{
					speed_bool_inti = 0;
					speed_bool_inti_shift = 0;
					*global_speed_value = global_speed_ole_value;
				}

			if (!speed_bool_inti_shift && !speed_bool_inti_f)
				if (x_g_speed&&xs_Gui::GetCurrentKeyButton()[DIK_X] && !speed_bool_inti)
				{
					speed_bool_inti = 1;
					speed_bool_inti_x = 1;
					*global_speed_value = global_speed_ole_value + global_speed_ole_value * (g_speed.value * 0.1f);
				}
				else if (x_g_speed && !xs_Gui::GetCurrentKeyButton()[DIK_X] && speed_bool_inti)
				{
					speed_bool_inti = 0;
					speed_bool_inti_x = 0;
					*global_speed_value = global_speed_ole_value;
				}

			if (!speed_bool_inti_shift && !speed_bool_inti_x)
				if (f_g_speed&&xs_Gui::GetCurrentKeyButton()[DIK_F] && !speed_bool_inti)
				{
					speed_bool_inti = 1;
					speed_bool_inti_f = 1;
					*global_speed_value = global_speed_ole_value + global_speed_ole_value * (g_speed.value * 0.1f);
				}
				else if (f_g_speed && !xs_Gui::GetCurrentKeyButton()[DIK_F] && speed_bool_inti)
				{
					speed_bool_inti = 0;
					speed_bool_inti_f = 0;
					*global_speed_value = global_speed_ole_value;
				}
		}
	}
	else if (speed_bool_inti)
	{
		speed_bool_inti = 0;
		*global_speed_value = global_speed_ole_value;
	}

}



void _Player::player_cheat()
{
	player_speed();
}

void _Player::Player_config()
{
	xs_config iniop;

	iniop.Read_float_Config(VMProtectDecryptStringA("[Player]"), VMProtectDecryptStringA("\ngg_speed="), &g_speed.value);
	iniop.Read_float_Config(VMProtectDecryptStringA("[Player]"), VMProtectDecryptStringA("\nsspeed="), &speed.value);
	iniop.Read_bool_Config(VMProtectDecryptStringA("[Player]"), VMProtectDecryptStringA("\nshift_g_speed="), &shift_g_speed);

	iniop.Read_bool_Config(VMProtectDecryptStringA("[Player]"), VMProtectDecryptStringA("\nglobal_speed_bool="), &global_speed_bool);
	iniop.Read_bool_Config(VMProtectDecryptStringA("[Player]"), VMProtectDecryptStringA("\nx_g_speed="), &x_g_speed);
	iniop.Read_bool_Config(VMProtectDecryptStringA("[Player]"), VMProtectDecryptStringA("\nf_g_speed="), &f_g_speed);

	iniop.SaveConfig();
	iniop.OffConfig();
}
void _Player::Player_config_Write()
{
	xs_config iniop;

	iniop.Write_float_Config(VMProtectDecryptStringA("[Player]"), VMProtectDecryptStringA("gg_speed="), g_speed.value);
	iniop.Write_float_Config(VMProtectDecryptStringA("[Player]"), VMProtectDecryptStringA("sspeed="), speed.value);
	iniop.Write_bool_Config(VMProtectDecryptStringA("[Player]"), VMProtectDecryptStringA("shift_g_speed="), shift_g_speed);

	iniop.Write_bool_Config(VMProtectDecryptStringA("[Player]"), VMProtectDecryptStringA("global_speed_bool="), global_speed_bool);
	iniop.Write_bool_Config(VMProtectDecryptStringA("[Player]"), VMProtectDecryptStringA("x_g_speed="), x_g_speed);
	iniop.Write_bool_Config(VMProtectDecryptStringA("[Player]"), VMProtectDecryptStringA("f_g_speed="), f_g_speed);
	iniop.SaveConfig();
	iniop.OffConfig();
}