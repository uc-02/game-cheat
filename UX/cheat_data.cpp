#include "cheat_data.h"
#include "xsgui_menu.h"
#include "VMProtectSDK.h"

_cheat_data* g = NULL;

__MMVA vva = { 0 };
void set_vva(UINT64 aa)
{
	vva.u.Long = aa;
}

bool is_addr(UINT64 aa)
{
	bool b = 0;
	__MMVA va = { 0 };
	va.u.Long = aa;
	if ((va.u.Offset.PML4E == vva.u.Offset.PML4E || va.u.Offset.PML4E == 0xFF) && !va.u.Offset.Partition && !va.u.Offset.PML5E)
		if ((UINT32)aa && (aa > 0x40000 && aa < 0x7FFFFFFE0000))
			b = 1;
	return b;
}

_cheat_data* _cheat::CreateContext()
{
    _cheat_data* ctx =NULL;
    if (g == NULL)
    {
        ctx = XS_NEW(_cheat_data)();
        g = ctx;
    }
    else
        ctx = g;
    return ctx;
}
void   _cheat::DestroyContext(_cheat_data* ctx)
{
    if (ctx == NULL)
        ctx = g;
    //Shutdown(ctx);
    if (g == ctx)
        g = NULL;
    free(ctx);
}

void _cheat::up_cheat_data()
{

    g->updata_player();

    g->updata_materia();


}
void _cheat::free_cheat_data()
{
    g->player_data.clear();
    g->npc.clear();
    g->drop_data.clear();
    //g->materia_data.clear();


}

_cheat_data* _cheat::get_cheat_data()
{
    _cheat_data& c = *g;
    return g ? &c : NULL;
}


void _cheat::run_cheat()
{
	if (g->config_read)
	{
		g->config_read = 0;
		g->aim.aim_config();
		g->esp_draw.esp_config();
		g->player.Player_config();
		g->supplies.supplies_config();
		g->weapon.Weapon_config();
		g->other.Other_config();
	}
	if (g->config_write)
	{
		g->config_write = 0;
		g->aim.aim_config_Write();
		g->esp_draw.esp_config_Write();
		g->player.Player_config_Write();
		g->supplies.supplies_config_Write();
		g->weapon.Weapon_config_Write();
		g->other.Other_config_Write();
	}
    if (g->other.fps)
    {
		char ac[20] = {0};
        sprintf(ac, "%d", g->player_data.Size);
        xs_Gui::GetForegroundDrawList()->AddText(NULL, 18.0f, { 10.0f,20.0f }, 0xFFFFFFFF, ac);
    }

	//{
	//	UINT32 npc_enemy = 0xFFFFFFFF;
	//	for (size_t i = 0; i < g->npc.Size; i++)
	//	{
	//		if (g->npc.Data[i].enemy)
	//		{
	//			npc_enemy = 0;
	//			break;
	//		}
	//	}

	//	xs_Gui::GetForegroundDrawList()->AddRectFilled({ 0.0f,0.0f }, { 4.0f,4.0f }, 0xFFFFFFFF);
	//}

    g->player.player_cheat();
	
        if (g->player_data.Size)
        {
			
            g->aim.aim_player(&g->player_data, 1, g->player_data.Data[0].weapon);
            g->aim.aim_player(&g->npc, 0, g->player_data.Data[0].weapon);
			g->aim.aim_cheat(g->player_data.Data[0]);

            g->supplies.drop_cheat(&g->drop_data, g->player_data.Data[0].position);
            //g->supplies.supplies_cheat(&g->materia_data, g->player_data[0].position);

            g->esp_draw.esp_Drop(&g->drop_data);
            //g->esp_draw.esp_supplies(&g->materia_data);

            g->esp_draw.esp_player(&g->player_data, 1);
            g->esp_draw.esp_player(&g->npc, 0);
            g->esp_draw.esp_aim(g->aim);
            
            g->weapon.weapon_cheat(g->player_data[0].weapon);

            
			
            //Ãûµ¥ÇÐ»»
			if (g->aim.aim_list_key_enable)
			{
				static bool m_4 = 1, m_91 = 1;

				if ((SHORT)stack_hidden_call(GetAsyncKeyState,VK_XBUTTON1) & 0x8000)
				{
					if (m_4)
					{
						g->aim.mouse_list(&g->player_data, g->player_data.Data[0], 1, &g->name_list);
						g->aim.mouse_list(&g->npc, g->player_data.Data[0], 0, &g->name_list);
						m_4 = 0;
					}
				}
				else
					m_4 = 1;

				if ((SHORT)stack_hidden_call(GetAsyncKeyState,VK_MBUTTON) & 0x8000)
				{
					if (m_91)
					{
						g->aim.no_mouse_list(&g->npc);
						m_91 = 0;
					}
				}
				else
					m_91 = 1;
			}
			else 
			{
				if (xs_Gui::GetPressKeyButton(DIK_TAB))
				{
					g->aim.mouse_list(&g->player_data, g->player_data.Data[0], 1, &g->name_list);
					g->aim.mouse_list(&g->npc, g->player_data.Data[0], 0, &g->name_list);
				}
				if (xs_Gui::GetPressKeyButton(DIK_GRAVE))
				{
					g->aim.no_mouse_list(&g->npc);
				}
			}
			if (xs_Gui::GetPressKeyButton(DIK_C))
			{
				g->aim.aim_weak_enable ^= 1;
				if (g->aim.aim_weak_enable)
					xs_Menu_UI::MenuCreation_MnueMessage(0, (char*)VMProtectDecryptStringA("\xe4\xb8\x8d\xe7\x9e\x84\xe5\x87\x86\xe5\xbc\xb1\xe7\x82\xb9 Enable"));
				else
					xs_Menu_UI::MenuCreation_MnueMessage(0, (char*)VMProtectDecryptStringA("\xe4\xb8\x8d\xe7\x9e\x84\xe5\x87\x86\xe5\xbc\xb1\xe7\x82\xb9 Invalid"));
			}
			if (xs_Gui::GetPressKeyButton(DIK_F1))
			{
				g->aim.aim_gun_enable ^= 1;
				if (g->aim.aim_gun_enable)
					xs_Menu_UI::MenuCreation_MnueMessage(0, (char*)VMProtectDecryptStringA("\xe5\xad\x90\xe5\xbc\xb9\xe8\xbf\xbd\xe8\xb8\xaa Enable"));
				else
					xs_Menu_UI::MenuCreation_MnueMessage(0, (char*)VMProtectDecryptStringA("\xe5\xad\x90\xe5\xbc\xb9\xe8\xbf\xbd\xe8\xb8\xaa Invalid"));
			}
			if (xs_Gui::GetPressKeyButton(DIK_F2))
			{
				g->aim.aim_auto_fire_enable ^= 1;
				if (g->aim.aim_auto_fire_enable)
					xs_Menu_UI::MenuCreation_MnueMessage(0, (char*)VMProtectDecryptStringA("\xe8\xaf\x86\xe5\x88\xab\xe5\x88\xb0\xe6\x95\x8c\xe4\xba\xba\xe5\xbc\x80\xe7\x81\xab Enable"));
				else
					xs_Menu_UI::MenuCreation_MnueMessage(0, (char*)VMProtectDecryptStringA("\xe8\xaf\x86\xe5\x88\xab\xe5\x88\xb0\xe6\x95\x8c\xe4\xba\xba\xe5\xbc\x80\xe7\x81\xab Invalid"));
			}
			if (xs_Gui::GetPressKeyButton(DIK_F3))
			{
				g->aim.aim_enable ^= 1;
				if (g->aim.aim_enable)
					xs_Menu_UI::MenuCreation_MnueMessage(0, (char*)VMProtectDecryptStringA("\xe9\x9d\x99\xe9\xbb\x98\x5f\xe7\x9e\x84\xe5\x87\x86 Enable"));
				else
					xs_Menu_UI::MenuCreation_MnueMessage(0, (char*)VMProtectDecryptStringA("\xe9\x9d\x99\xe9\xbb\x98\x5f\xe7\x9e\x84\xe5\x87\x86 Invalid"));
			}
			if (xs_Gui::GetPressKeyButton(DIK_F4))
			{
				g->aim.aim_san_enable ^= 1;
				if (g->aim.aim_san_enable)
					xs_Menu_UI::MenuCreation_MnueMessage(0, (char*)VMProtectDecryptStringA("\xe6\x95\xa3\xe5\xb0\x84\xe8\xbf\xbd\xe8\xb8\xaa Enable"));
				else
					xs_Menu_UI::MenuCreation_MnueMessage(0, (char*)VMProtectDecryptStringA("\xe6\x95\xa3\xe5\xb0\x84\xe8\xbf\xbd\xe8\xb8\xaa Invalid"));
			}
			if ((g->supplies.drop_tp_enable_key ? xs_Gui::GetBounceKeyButton(DIK_BACKSLASH) : 0))
			{
				g->supplies.drop_tp_enable ^= 1;

				if (g->supplies.drop_tp_enable)
					xs_Menu_UI::MenuCreation_MnueMessage(0, (char*)VMProtectDecryptStringA("drop_tp Enable"));
				else
					xs_Menu_UI::MenuCreation_MnueMessage(0, (char*)VMProtectDecryptStringA("drop_tp Invalid"));
			}
			
            //g->message.message_cheat(g->module_addr);
        }
		
        //g->supplies.supplies_cheat(&g->materia_data, g->player_data[0].position);
}





