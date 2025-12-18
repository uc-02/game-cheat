#include "xsgui_menu.h"
#include "xsgui_internal.h"
#include<stdio.h>

xs_UI_windowList GxsGui_UI;




void xs_Menu_UI::MenuUiFrame()
{
	xs_DrawList* drawList = xs_Gui::GetBackgroundDrawList();
	void* _TextureID = drawList->_TextureIdStack.Data[drawList->_TextureIdStack.Size-1];
	xsFloat4 _ClipRectStack = drawList->_ClipRectStack.Data[drawList->_ClipRectStack.Size-1];
	for (int ck_i = 0; ck_i < GxsGui_UI.windowUI.Size; ck_i++)
	{
		GxsGui_UI.windowUI.Data[ck_i].windowRun(drawList);
	}




	drawList->PushGuideBuffer();
	drawList->PushTextureID(_TextureID);//xsFont纹理
	drawList->PushClipRect(xsFloat2(_ClipRectStack.x, _ClipRectStack.y), xsFloat2(_ClipRectStack.z, _ClipRectStack.w));

}
void xs_Menu_UI::MenuCreation_MnueMessage(int i, char* a)
{
	if (GxsGui_UI.windowUI.Data[0].MenuUi.Size > i)
		GxsGui_UI.windowUI.Data[0].MenuUi.Data[i].MenuUi_PushTextMessage(a);
}

void xs_Menu_UI::MenuCreation(PVOID call)
{
	xs_windowUI a;
	a.call = call;
	GxsGui_UI.windowUI.push_back(a);
}

void xs_windowUI::pushCompon_text(float Text_size,xsFloat2 xy, UINT32 col, char* Text)
{
	xs_componUi a;
	a.pos = xsFloat4(xy.x, xy.y, 0, 0) + xsFloat4(pos.x, pos.y, 0, 0);
	a.col = col;
	a.Text = Text;
	a.Text_size = Text_size;
	a.type = xsMenuUiType_Text;
	compon.push_back(a);
}

void xs_windowUI::pushCompon_Menu(PVOID call, xsFloat4 xy, UINT32 col)
{
	xs_componUi a;
	a.pos = xy ;
	a.col = col;
	a.Text = NULL;
	a.Text_size = 0;
	a.type = xsMenuUiType_Menu;
	
	for (int i = 0; i < MenuUi.Size; i++)//这里列表集 是一个组件一个UI列表
	{
		if (MenuUi.Data[i].MenuUi_call.Data[0].call == call)//这里 遍历检验 call有没有重复的 如果有重复的列表call 执行下面
		{
			a._value_0 = &MenuUi.Data[i];//这里 _value_0 是存储 列表集的地址
			((xs_MenuUi*)a._value_0)->MenuUi_free_posData();
			((xs_MenuUi*)a._value_0)->MenuUi_malloc_posData(a.pos);//这里重置 《posData》
			a.pos = ((xs_MenuUi*)a._value_0)->posData->pos;
			((xs_MenuUi*)a._value_0)->Refresh = 0;////这里重置Refresh 为了重置MenuUi_Data
			compon.push_back(a);
			return;
		}
	}
	MenuUi.push_back(xs_MenuUi());//这里push 一个新的列表集 所以不用Refresh
	 
	a._value_0 = &MenuUi.Data[MenuUi.Size-1];//这里把新的列表集 存储到 _value_0
	((xs_MenuUi&)*(xs_MenuUi*)a._value_0).MenuUi_malloc_posData(a.pos);//这里初始化 《posData》
	a.pos = ((xs_MenuUi*)a._value_0)->posData->pos;//这里的 列表界面pos 存储到 组件Data里
	((xs_MenuUi*)a._value_0)->MenuUi_Pushcall(call);//这里提call 到新的UI列表 Data里 这里的新的UI列表所以不用Refresh---“新的UI列表 默认Refresh当前UI列表,所以在旧的UI列表 提二次call 需要手动RefreshUI列表”

	compon.push_back(a);//这里push 已经初始化好的 列表界面Data 到 UI组件Data里
}


void xs_MenuUiLsitData::MenuList_item_LR_control(xs_MenuUi& UI)
{
	bool Left = xs_Gui::GetTriggerEventKeyButton(DIK_LEFTARROW) ? 1 : xs_Gui::GetTriggerEventKeyButton(DIK_NUMPAD4)
		, right = xs_Gui::GetTriggerEventKeyButton(DIK_RIGHTARROW) ? 1 : xs_Gui::GetTriggerEventKeyButton(DIK_NUMPAD6);

	xs_uint* a = NULL;
	xs_float* b = NULL;
	char* c = NULL;
	switch (type)
	{
	case xsMenuUiList_Type_Text:
		if (right && other && other1)
		{
			*(bool*)other1 = 1;
			UI.MenuUi_PushTextMessage((char*)other);
		}
		break;
	case xsMenuUiList_Type_call:
		if (right)
				UI.MenuUi_Pushcall(other);

		break;
	case xsMenuUiList_Type_Bool:
		if (right&& !*(bool*)other)
		{
			if (other1)
					*(UINT32*)other1 += 1;
			*(bool*)other = 1;
			xs_Gui::Play_Control_Item();
		}
		if (Left&& *(bool*)other)
		{
			if (other1)
					*(UINT32*)other1 -= 1;
			*(bool*)other = 0;
			xs_Gui::Play_Control_Item();
		}
		break;
	case xsMenuUiList_Type_Col_R:case xsMenuUiList_Type_col_G:case xsMenuUiList_Type_col_B:case xsMenuUiList_Type_col_A:
	{
		if(!Left&&!right)
			break;
		xsFloat4 RGBA = xs_Gui::col_Uint32ToFloat_RGBA(*(UINT32*)other);
		xsFloat3 HSV;
		xs_Gui::ColorConvertRGBtoHSV(RGBA.x, RGBA.y, RGBA.z, HSV.x, HSV.y, HSV.z);

		switch (type)
		{
		case xsMenuUiList_Type_Col_R:
			if (Left)
			{HSV.x -= 0.01F; xs_Gui::Play_Control_Item();}
			else if (right&& HSV.x <0.98f)
			{HSV.x += 0.01F; xs_Gui::Play_Control_Item();}
			break;
		case xsMenuUiList_Type_col_G:
			if (Left)
			{
				HSV.y -= 0.01F;
				xs_Gui::Play_Control_Item();
			}
			else if (right)
			{
				HSV.y += 0.01F; xs_Gui::Play_Control_Item();
			}
			break;
		case xsMenuUiList_Type_col_B:
			if (Left)
			{
				HSV.z -= 0.01F; xs_Gui::Play_Control_Item();
			}
			else if (right)
			{
				HSV.z += 0.01F; xs_Gui::Play_Control_Item();
			}
			break;
		case xsMenuUiList_Type_col_A:
			if (Left)
			{
				RGBA.w -= 0.01F; xs_Gui::Play_Control_Item();
			}
			else if (right)
			{
				RGBA.w += 0.01F; xs_Gui::Play_Control_Item();
			}
			break;
		default:
			break;
		}
		xs_Gui::ColorConvertHSVtoRGB( HSV.x, HSV.y, HSV.z, RGBA.x, RGBA.y, RGBA.z);
		*(UINT32*)other = xs_Gui::col_FloatToUint32_RGBA(RGBA);
		break;
	}
	
	case xsMenuUiList_Type_Int:
		a = (xs_uint*)other;
		if (Left && a->value > a->min)
		{
			a->value -= a->speed; xs_Gui::Play_Control_Item();
		}
		else if (right && a->value < a->max)
		{
			a->value += a->speed; xs_Gui::Play_Control_Item();
		}
		break;
	case xsMenuUiList_Type_Float:
		b = (xs_float*)other;
		if (Left && b->value > b->min)
		{
			b->value -= b->speed; xs_Gui::Play_Control_Item();
		}
		else if (right && b->value < b->max)
		{
			b->value += b->speed; xs_Gui::Play_Control_Item();
		}
		if (b->value > b->max)
			b->value = b->max;
		if (b->value < b->min)
			b->value = b->min;
		break;


	default:
		break;
	}

}