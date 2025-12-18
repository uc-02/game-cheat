#include "cheat_data.h"
#include<cmath>
#include "hook_call.h"


UINT64 local_this = 0;

UINT64 ole_weapon_speed = 0;
UINT64  ret_rsp_weapon_speed = 0;
UINT*   hook_speed_value = 0;
bool*   hook_speed_enable = 0;

//射速
//inline void weapon_speed(UINT64 _rcx) __attribute((__annotate__(("nobcf"))));
//inline void weapon_speed(UINT64 _rcx) __attribute((__annotate__(("nosub"))));
//inline void weapon_speed(UINT64 _rcx) __attribute((__annotate__(("nosplit"))));
//inline void weapon_speed(UINT64 _rcx) __attribute((__annotate__(("nofla"))));
inline void weapon_speed(UINT64 _rcx)
{
	UINT64 _rsp = 0;
	UINT hook_ecx = *hook_speed_value;
	__asm {
		mov rax, [rsp + 0x38]
		mov _rsp, rax}
	if (_rsp == ret_rsp_weapon_speed && local_this == _rcx && *hook_speed_enable)
	{
		__asm {
			mov ecx, hook_ecx
			mov rax, _rsp
			add rax, 0x27
			mov[rsp + 0x38], rax
		}
	}
	else
		(*(void *(__stdcall*)(UINT64))(ole_weapon_speed)) (_rcx);
}

UINT64   ole_weapon_bullet = 0;
UINT64   ret_rsp_weapon_bullet = 0;
UINT*    hook_bullet_value = 0;
bool*    hook_bullet_enable = 0;

//倍攻
//inline UINT weapon_bullet(UINT64 _rcx, UINT64 _rdx) __attribute((__annotate__(("nobcf"))));
//inline UINT weapon_bullet(UINT64 _rcx, UINT64 _rdx) __attribute((__annotate__(("nosub"))));
//inline UINT weapon_bullet(UINT64 _rcx, UINT64 _rdx) __attribute((__annotate__(("nosplit"))));
//inline UINT weapon_bullet(UINT64 _rcx, UINT64 _rdx) __attribute((__annotate__(("nofla"))));
inline UINT weapon_bullet(UINT64 _rcx, UINT64 _rdx)
{
	UINT64 _rsp = 0;
	UINT hook_rax = *hook_bullet_value;
	__asm {
		mov rax, [rsp + 0x48]
		mov _rsp, rax}

	if (_rsp == ret_rsp_weapon_bullet && local_this == _rcx && *hook_bullet_enable)
	{
		(*(UINT(__stdcall*)(UINT64, UINT64))(ole_weapon_bullet)) (_rcx, _rdx);
		__asm {
			test al, al
			je end_ret
			mov rax, _rsp
			add rax, 0x4A
			mov[rsp + 0x48], rax
			jmp end_ret2
			end_ret :
			mov hook_rax, rax
				end_ret2 :
		}
		return hook_rax;
	}
	else
		return (*(UINT(__stdcall*)(UINT64, UINT64))(ole_weapon_bullet)) (_rcx, _rdx);
}

UINT64   ole_weapon_reload = 0;
UINT64   ret_rsp_weapon_reload = 0;
bool*    hook_reload_enable = 0;


//inline UINT weapon_reload(UINT64 _rcx, UINT64 _rdx) __attribute((__annotate__(("nobcf"))));
//inline UINT weapon_reload(UINT64 _rcx, UINT64 _rdx) __attribute((__annotate__(("nosub"))));
//inline UINT weapon_reload(UINT64 _rcx, UINT64 _rdx) __attribute((__annotate__(("nosplit"))));
//inline UINT weapon_reload(UINT64 _rcx, UINT64 _rdx) __attribute((__annotate__(("nofla"))));
inline UINT weapon_reload(UINT64 _rcx, UINT64 _rdx)
{
	UINT64 rsp_ = 0;
	__asm {
		mov rax, [rsp + 0x38]
		mov rsp_, rax
	}
	if ((UINT32)local_this && (local_this > 0x40000 && local_this < 0x7FFFFFFF0000))
		if (rsp_ == ret_rsp_weapon_reload && _rcx == *(UINT64*)(local_this + 0x3720) && *hook_reload_enable)
		{
			__asm {
				mov rsi, 0
				mov[rsp + 0x30], rsi
			}
		}
	return (*(UINT(__stdcall*)(UINT64, UINT64))(ole_weapon_reload)) (_rcx, _rdx);
}


UINT64   ole_weapon_deceit_bullets = 0;
UINT64   ole_weapon_deceit_bullets_2 = 0;
UINT64   ret_rsp_weapon_deceit_bullets = 0;
UINT64 hook_deceit_bullets_enable = 0;

//无线子弹
//inline void weapon_deceit_bullets(UINT64 _rcx) __attribute((__annotate__(("nobcf"))));
//inline void weapon_deceit_bullets(UINT64 _rcx) __attribute((__annotate__(("nosub"))));
//inline void weapon_deceit_bullets(UINT64 _rcx) __attribute((__annotate__(("nosplit"))));
//inline void weapon_deceit_bullets(UINT64 _rcx) __attribute((__annotate__(("nofla"))));
inline void weapon_deceit_bullets(UINT64 _rcx)
{

	(*(void(__stdcall*)(UINT64))(ole_weapon_deceit_bullets)) (_rcx);
	UINT64 rsp_ = 0;
	UINT64 rbp_ = 0;
	__asm {
		mov rax, [rsp + 0x38]
		mov rsp_, rax
		mov rbp_, rsi
	}
	if (rsp_ == ret_rsp_weapon_deceit_bullets /*&& rbp_== local_this*/)
	{
		__asm {
			lock inc dword ptr[rbx + 0x8]
			mov r8, rsi
			mov rdx, rdi
			mov rcx, rbx
			cmp r8, local_this//local_this
			jne not_equal
			mov rax, ole_weapon_deceit_bullets_2
			call rax

			//if (*hook_deceit_bullets_enable)
			//mov rax, [hook_deceit_bullets_enable]
			//cmp byte ptr [rax], 0
			//jz not_equal

			mov rax, 0
			mov[rbx + 0x28], rax
			mov rax, 0
			mov[rbx + 0x30], rax
			

			not_equal :

			mov rax, 0x5D4179F15B7BD754
				mov[rbx + 0x38], rax
				mov rax, 0x000302BA7EFB8878
				mov[rbx + 0x40], rax

			mov rax, [rsp + 0x38]
				add rax, 0x12
				mov[rsp + 0x38], rax
		}
	}
}

UINT64 weapon_auto_this_ = 0;
UINT64 weapon_No_auto_this_ = 0;
float* weapon_recoil_ = 0;

void _Weapon::weapon_Init(UINT64* weapon_speed_call,UINT64* weapon_bullet_call,UINT64* weapon_deceit_bullets_call,UINT64* weapon_reload_call)
{
	const WCHAR* TheDivision2_wchar = VMProtectDecryptStringW(L"TheDivision2.exe");
	//const char* pattern = NULL;
	const char* pattern = VMProtectDecryptStringA("\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xDA\x48\x8B\xF9\xE8????\xF6\xC3\x01\x74\x0D\xBA\xD0\x01");

    //半自动 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 48 89 5C 24 08 57 48 83 EC 20 8B DA 48 8B F9 E8 ?? ?? ?? ?? F6 C3 01 74 0D BA D0 01
    UINT64 weapon_No_auto_this = findPattern(TheDivision2_wchar, pattern,42);
	VMProtectFreeString(pattern);

	//MessageBoxA(0, VMProtectDecryptStringA("12-0-"), 0, 0);
	weapon_No_auto_this += 0xE;
	weapon_No_auto_this_ = findPattern(TheDivision2_wchar, weapon_No_auto_this, 1);


	pattern = VMProtectDecryptStringA("\x48\x8D\x15????\x48\x8B\xCF\xE8????\x48\x8D\x8B\x60\x01");

	//全自动 48 8D 15 ?? ?? ?? ?? 48 8B CF E8 ?? ?? ?? ?? 48 8D 8B 60 01
	UINT64 weapon_auto_this = findPattern(TheDivision2_wchar, pattern, 20);
	VMProtectFreeString(pattern);

	//MessageBoxA(0, VMProtectDecryptStringA("12-0"), 0, 0);
	weapon_auto_this -= 0x2D;
	weapon_auto_this_ = findPattern(TheDivision2_wchar, weapon_auto_this, 1);
	weapon_auto_this_ -= 0x8;

    //无后座 F3 0F 10 05 ?? ?? ?? ?? 48 8D ?? ?? ?? F3 0F 59 C7
    //MessageBoxA(0, VMProtectDecryptStringA("12-1"), 0, 0);
	pattern = VMProtectDecryptStringA("\xF3\x0F\x10\x05????\x48\x8D???\xF3\x0F\x59\xC7");
    UINT64 weapon_recoil = findPattern(TheDivision2_wchar, pattern, 17);
	VMProtectFreeString(pattern); 
	weapon_recoil_ = (float*)Compile_PointAddr(weapon_recoil + 4);

    //无视换弹动作
	//MessageBoxA(0, VMProtectDecryptStringA("12-2"), 0, 0);
    //hook_reload_enable = &weapon_reload_bullets_enable;
    //ret_rsp_weapon_reload = weapon_reload_call[1];
    //ole_weapon_reload = *(UINT64*)weapon_reload_call[0];
    //hook_up(weapon_reload_call[0], (UINT64)&weapon_reload, 1);


    //无限备弹
	//MessageBoxA(0, VMProtectDecryptStringA("12-3"), 0, 0);
	pattern = VMProtectDecryptStringA("\xF0\xFF\x43\x08");
	UINT64 my_weapon_deceit = findPattern((UINT64)&weapon_deceit_bullets, (UINT64)0x200, pattern, 4);
	VMProtectFreeString(pattern);
	memmove((void*)my_weapon_deceit, (void*)weapon_deceit_bullets_call[1], 0xD);

	//MessageBoxA(0, VMProtectDecryptStringA("12-4"), 0, 0);
    hook_deceit_bullets_enable = (UINT64)&deceit_bullets_enable;
    ole_weapon_deceit_bullets = *(UINT64*)weapon_deceit_bullets_call[0];
    ole_weapon_deceit_bullets_2 = weapon_deceit_bullets_call[2];
	ret_rsp_weapon_deceit_bullets = weapon_deceit_bullets_call[1];
    hook_up(weapon_deceit_bullets_call[0], (UINT64)&weapon_deceit_bullets, 1);
	//MessageBoxA(0, VMProtectDecryptStringA("12-5"), 0, 0);
    //weapon_bullet 子弹数量
	
    hook_bullet_value = &bullet_value.value;
    hook_bullet_enable = &bullet_enable;
    ret_rsp_weapon_bullet = weapon_bullet_call[1];
    ole_weapon_bullet = *(UINT64*)weapon_bullet_call[0];
    hook_up(weapon_bullet_call[0], (UINT64)&weapon_bullet, 1);
	//MessageBoxA(0, VMProtectDecryptStringA("12-6"), 0, 0);
    //weapon_speed 射速
    hook_speed_value = &speed_value.value;
    hook_speed_enable = &speed_enable;
    ret_rsp_weapon_speed = weapon_speed_call[1];
    ole_weapon_speed = *(UINT64*)weapon_speed_call[0];
    hook_up(weapon_speed_call[0], (UINT64)&weapon_speed, 1);
	VMProtectFreeString(TheDivision2_wchar);
	//MessageBoxA(0, VMProtectDecryptStringA("12-7"), 0, 0);
}


inline void _Weapon::weapon_auto(_weapon_data& data)
{
    if (!weapon_auto_enable)
        return;
	if (weapon_auto_enable)
		weapon_auto_enable = 0;

    UINT64* weapon_auto_this = data.get_bullet_config_list(data.table_this);

	if (weapon_auto_this && *weapon_auto_this == weapon_No_auto_this_)
		*weapon_auto_this = weapon_auto_this_;
}


inline void _Weapon::weapon_no_recoil(_weapon_data& data)
{
    if (!weapon_recoil_enable)
        return;
	if (weapon_recoil_enable)
		weapon_recoil_enable = 0;
    *weapon_recoil_ = 0.0f;
}

bool bullets_lock_call = 0;
inline void _Weapon::weapon_lock_bullet(_weapon_data& data)
{
	//if (bullets_lock_enable != bullets_lock_call)
	if (is_addr((UINT64)data.no_less_bullets))
	{
		//bullets_lock_call = bullets_lock_enable;

		if(bullets_lock_enable)
			*data.no_less_bullets = 1;
		else
			*data.no_less_bullets = 0;
	}
       
}


inline void _Weapon::weapon_config(_weapon_data& data)
{
    static UINT64 hook_weapon_config_this = 0;
	//static UINT64 hook_weapon_clips_this = 0;
    if (weapon_read_confg_enable)
    {
        weapon_read_confg_enable = 0;
        hook_weapon_config_this = *data.bullet_confg_list;
		//hook_weapon_clips_this = *data.bullet_clips_list;
    }

    if (weapon_write_confg_enable)
    {
        weapon_write_confg_enable = 0;
        if (hook_weapon_config_this)
            *data.bullet_confg_list = hook_weapon_config_this;
		//if (hook_weapon_clips_this)
		//	*data.bullet_clips_list = hook_weapon_clips_this;
    }
}


inline void _Weapon::weapon_scatter(_weapon_data& data)
{
    if (weapon_scatter_enable)
    {
        //*data.bullet_scatter0 = 0;
        //*data.bullet_scatter1 = 0;
        //*data.bullet_scatter2 = 0;
        //*data.bullet_scatter3 = 0;
    }
}


void _Weapon::weapon_cheat(_weapon_data& data)
{
	if (is_addr((UINT64)data.table_this))
		if (is_addr(*data.table_this))
			local_this = *data.table_this;

    weapon_auto(data);
    weapon_no_recoil(data);
    weapon_lock_bullet(data);
    weapon_config(data);
    //weapon_scatter(data);
}

void _Weapon::Weapon_config()
{
	xs_config iniop;

	iniop.Read_ulong_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("\nspeed_value="), &speed_value.value);


	iniop.Read_bool_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("\nspeed_enable="), &speed_enable);
	iniop.Read_ulong_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("\nbullet_value="), &bullet_value.value);

	iniop.Read_bool_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("\nbullet_enable="), &bullet_enable);
	iniop.Read_bool_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("\nweapon_read_confg_enable="), &weapon_read_confg_enable);
	iniop.Read_bool_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("\nweapon_write_confg_enable="), &weapon_write_confg_enable);
	iniop.Read_bool_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("\ndeceit_bullets_enable="), &deceit_bullets_enable);
	iniop.Read_bool_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("\nbullets_lock_enable="), &bullets_lock_enable);

	//iniop.Read_bool_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("\nweapon_recoil_enable="), &weapon_recoil_enable);

	iniop.SaveConfig();
	iniop.OffConfig();
}

void _Weapon::Weapon_config_Write()
{
	xs_config iniop;

	iniop.Write_ulong_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("speed_value="), speed_value.value);


	iniop.Write_bool_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("speed_enable="), speed_enable);
	iniop.Write_ulong_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("bullet_value="), bullet_value.value);

	iniop.Write_bool_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("bullet_enable="), bullet_enable);
	iniop.Write_bool_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("weapon_read_confg_enable="), weapon_read_confg_enable);
	iniop.Write_bool_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("weapon_write_confg_enable="), weapon_write_confg_enable);
	iniop.Write_bool_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("deceit_bullets_enable="), deceit_bullets_enable);
	iniop.Write_bool_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("bullets_lock_enable="), bullets_lock_enable);

	//iniop.Write_bool_Config(VMProtectDecryptStringA("[Weapon]"), VMProtectDecryptStringA("weapon_recoil_enable="), weapon_recoil_enable);

	iniop.SaveConfig();
	iniop.OffConfig();
}