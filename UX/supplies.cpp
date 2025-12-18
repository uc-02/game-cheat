#include "cheat_data.h"
#include<cmath>

UINT64 supplies_1_olecall = 0;
UINT64 supplies_4_olecall = 0;
UINT64 supplies_F_olecall = 0;
bool supplies_tp_call_enable = 0;



xsFloat3 pos;

xsFloat3* supplies_1_newcall(UINT64* rcx, UINT64* rdx)
{
	xsFloat3* rax = (*(xsFloat3 * (__stdcall*)(UINT64*, UINT64*))(supplies_1_olecall))(rcx, rdx);
	if (supplies_tp_call_enable)
		if (/**(UINT64*)((UINT64)rcx + _type) == materia_type &&*/ !*(UINT8*)((UINT64)rcx + _isPlunder))
			*rax = pos;

	return rax;
}

UINT64 supplies_4_newcall(UINT64* rcx, UINT64* rdx)
{
	UINT64 rax = (*(UINT64(__stdcall*)(UINT64*, UINT64*))(supplies_4_olecall))(rcx, rdx);
	if (supplies_tp_call_enable)
		if (/**(UINT64*)((UINT64)rcx + _type) == materia_type &&*/ !*(UINT8*)((UINT64)rcx + _isPlunder))
			*(xsFloat3*)(rax + 0x30) = pos;
	return rax;
}

void supplies_F_newcall(UINT64 rcx, UINT64 rdx)//F¼ü
{
	(*(void(__stdcall*)(UINT64, UINT64))(supplies_F_olecall))(rcx, rdx);
	if (supplies_tp_call_enable)
		if (/**(UINT64*)((UINT64)rcx + _type) == materia_type &&*/ !*(UINT8*)((UINT64)rcx + _isPlunder))
			*(bool*)(rcx + _Show_Fkey) = 1;
}


inline void _supplies::supplies_hook_Init(_materia_data& data)
{
	UINT64* call1 = data.get_supplies_1_call();
	UINT64* call4 = data.get_supplies_4_call();
	UINT64* callF = data.get_supplies_F_call();
	supplies_1_olecall = *call1;
	supplies_4_olecall = *call4;
	supplies_F_olecall = *callF;

	DWORD oldProtect, oldProtect2;
	//VirtualProtect(call1, 8, PAGE_EXECUTE_READWRITE, &oldProtect);
	*call1 = (UINT64)&supplies_1_newcall;
	//VirtualProtect(call1, 8, oldProtect, &oldProtect2);

	//VirtualProtect(call4, 8, PAGE_EXECUTE_READWRITE, &oldProtect);
	*call4 = (UINT64)&supplies_4_newcall;
	//VirtualProtect(call4, 8, oldProtect, &oldProtect2);

	//VirtualProtect(callF, 8, PAGE_EXECUTE_READWRITE, &oldProtect);
	*callF = (UINT64)&supplies_F_newcall;
	//VirtualProtect(callF, 8, oldProtect, &oldProtect2);
}

inline void _supplies::set_supplies_pos(_materia_data& data)
{
	*data.position2 = pos;
	*data.position3 = pos;
}
inline void _supplies::set_drop_pos(_materia_data& data, xsFloat3* pos_0)
{

	//if (!data.drop_is_Fkey())
	{
		*data.drop_position = *pos_0;
		*data.position1 = *pos_0;
	}
}


bool supplies_hook_Init_ = 0;
void _supplies::supplies_cheat(xs_DataFrame<_materia_data>* c, xsFloat3* pos_0)
{
	pos = *pos_0;
	pos.y += 0.3f;
	//if (!materia_type)
	if (c->Size&& !supplies_hook_Init_)
	{
		supplies_hook_Init_ = 1;
		//get_supplies_id(c->Data[0]);
		supplies_hook_Init(c->Data[0]);
	}
	if ((supplies_tp_enable_key ? xs_Gui::GetBounceKeyButton(DIK_CAPSLOCK) : 0))
		supplies_tp_enable ^= 1;
	if (supplies_tp_enable)
	{
		supplies_tp_call_enable = 1;
		for (size_t i = 0; i < c->Size; i++)
			if (!*c->Data[i].plunder && !*c->Data[i].Show_Fkey)
			{
				*c->Data[i].is_cheat = 1;
				set_supplies_pos(c->Data[i]);
			}
	}
	else
		supplies_tp_call_enable = 0;
	//else 
	//{
	//	for (size_t i = 0; i < c->Size; i++)
	//			if (*c->Data[i].is_cheat == 1)
	//			{
	//				*c->Data[i].is_cheat == 0;
	//				set_supplies_pos(c->Data[i], c->Data[i].position3);
	//			}
	//}
}

char* drop_name[] = {
	(char*)VMProtectDecryptStringA("player_"),
	(char*)VMProtectDecryptStringA("RWItem_bnt_lws_"),
	(char*)VMProtectDecryptStringA("_questitem"),
	(char*)VMProtectDecryptStringA("g_mod_"),
	(char*)VMProtectDecryptStringA("Skyscraper_Key")
};

void _supplies::drop_cheat(xs_DataFrame<_materia_data>* c, xsFloat3* pos_0)
{
	//if (!drop_type)
	//	return;
	
	if (drop_tp_enable)
	{

		for (size_t i = 0, ic = 0; i < c->Size && ic < drop_tp_conut.value; i++)
			if (!*c->Data[i].Show_Fkey && (c->Data[i].is_drop_player(drop_name[0])|| c->Data[i].is_drop_player(drop_name[1])|| c->Data[i].is_drop_player(drop_name[2])|| c->Data[i].is_drop_player(drop_name[3]) || c->Data[i].is_drop_player(drop_name[4])))
			{
				//*c->Data[i].is_cheat = 1;
				//if (c->Data[i].position0->x == 0.0F)
				//	*c->Data[i].position0 = *c->Data[i].position1;
				set_drop_pos(c->Data[i], pos_0);
				ic++;
			}
			//else
			//{
			//	if (*c->Data[i].drop_position != *c->Data[i].position1)
			//	{
			//		*c->Data[i].drop_position = *c->Data[i].position1;
			//	}
			//	
			//}
	}

}

void _supplies::supplies_config()
{
	xs_config iniop;

	iniop.Read_bool_Config(VMProtectDecryptStringA("[supplies]"), VMProtectDecryptStringA("\nsupplies_tp_enable="), &supplies_tp_enable);
	iniop.Read_bool_Config(VMProtectDecryptStringA("[supplies]"), VMProtectDecryptStringA("\ndrop_tp_enable="), &drop_tp_enable);
	iniop.Read_ulong_Config(VMProtectDecryptStringA("[supplies]"), VMProtectDecryptStringA("\ndrop_tp_conut="), &drop_tp_conut.value);
	iniop.Read_bool_Config(VMProtectDecryptStringA("[supplies]"), VMProtectDecryptStringA("\nsupplies_tp_enable_key="), &supplies_tp_enable_key);
	iniop.Read_bool_Config(VMProtectDecryptStringA("[supplies]"), VMProtectDecryptStringA("\ndrop_tp_enable_key="), &drop_tp_enable_key);

	iniop.SaveConfig();
	iniop.OffConfig();
}

void _supplies::supplies_config_Write()
{
	xs_config iniop;

	iniop.Write_bool_Config(VMProtectDecryptStringA("[supplies]"), VMProtectDecryptStringA("supplies_tp_enable="), supplies_tp_enable);
	iniop.Write_bool_Config(VMProtectDecryptStringA("[supplies]"), VMProtectDecryptStringA("drop_tp_enable="), drop_tp_enable);
	iniop.Write_ulong_Config(VMProtectDecryptStringA("[supplies]"), VMProtectDecryptStringA("drop_tp_conut="), drop_tp_conut.value);
	iniop.Write_bool_Config(VMProtectDecryptStringA("[supplies]"), VMProtectDecryptStringA("supplies_tp_enable_key="), supplies_tp_enable_key);
	iniop.Write_bool_Config(VMProtectDecryptStringA("[supplies]"), VMProtectDecryptStringA("drop_tp_enable_key="), drop_tp_enable_key);

	iniop.SaveConfig();
	iniop.OffConfig();
}