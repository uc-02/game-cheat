#pragma once
#include <d3d10.h>
#include "xsgui.h"
#include <stdio.h>



struct xs_MenuUiLsitData;

struct xs_UIcall;
struct xs_MenuUi;


enum xsMenu_UIType
{
    xsMenuUiType_Nop = 0,
    xsMenuUiType_Text = 1,
    xsMenuUiType_Menu = 2,
};

enum xsMenu_UIList_Type
{
    xsMenuUiList_Type_Text = 0,
    xsMenuUiList_Type_call = 1,
    xsMenuUiList_Type_Bool = 2,
    xsMenuUiList_Type_Int = 3,
    xsMenuUiList_Type_Float = 4,
    xsMenuUiList_Type_Theme = 5,//主题
    xsMenuUiList_Type_Number = 6,//页数
    xsMenuUiList_Type_Col_R = 7,
    xsMenuUiList_Type_col_G = 8,
    xsMenuUiList_Type_col_B = 9,
    xsMenuUiList_Type_col_A = 10
};

struct xs_MenuUiList
{
    UINT Visible_OptionNumber;

    float Menu_FontSize;
    float TipMenu_FontSize;

    xsFloat2 MenuFont_Originalxy;

    float Menulist_interval;
    float TipMenulist_interval;

    xsFloat4 TopTipMenulist_pos;
    xsFloat4 Menulist_pos;
    xsFloat4 LowTipMenulist_pos;
    xsFloat4 Reminder_pos;//提示TextMessage位置


    xsFloat4 pos;

    inline xs_MenuUiList(xsFloat4 _pos, UINT VisibleOptionNumber, float Fontsize)
    {
        pos = _pos;//{ _pos.x,_pos.y,_pos.x + 400.0f,_pos.y + 600.0f };

        Visible_OptionNumber = VisibleOptionNumber;

        Menu_FontSize = Fontsize;
        TipMenu_FontSize = Menu_FontSize - 2.0f;

        MenuFont_Originalxy = { 10.0f,5.0f };

        Menulist_interval = Menu_FontSize + MenuFont_Originalxy.y * 2.0f;
        TipMenulist_interval= TipMenu_FontSize + MenuFont_Originalxy.y * 2.0f;

        TopTipMenulist_pos = pos;
        TopTipMenulist_pos.w = TopTipMenulist_pos.y + TipMenulist_interval;

        Menulist_pos = pos;
        Menulist_pos.y = TopTipMenulist_pos.w + 1.0f;
        Menulist_pos.w = Menulist_pos.y + (float)Visible_OptionNumber * Menulist_interval;

        LowTipMenulist_pos = pos;
        LowTipMenulist_pos.y = Menulist_pos.w + 1.0f;
        LowTipMenulist_pos.w = LowTipMenulist_pos.y + TipMenulist_interval;

        Reminder_pos = pos;
        Reminder_pos.y = LowTipMenulist_pos.w + 5.0f;
        Reminder_pos.w = Reminder_pos.y;



        pos.w = LowTipMenulist_pos.w;

    }

};

//提示TextMessage
struct xs_MenuUiReminder
{
    char* TextMessage;
    UINT  col;
    UINT64  timecol;
    inline xs_MenuUiReminder() { TextMessage = NULL; col = 0xFFFFFFFF; timecol = xs_Gui::GetIO().time_Current; };
    inline xs_MenuUiReminder(char* a) { TextMessage = a; col = 0xFFFFFFFF;timecol = xs_Gui::GetIO().time_Current; };
};

struct xs_MenuUiLsitData
{
    char*  Text;
    UINT   Text_col;//Text颜色
    char*  itemText;//选itmeText
    UINT   itemText_col;
    void*  other; //这里可能是下一链(页面)的call. 如果使用需提xs_UIcall, 不可直接调用这个call,必须提xs_UIcall 让"Refresh功能"调用这个call
    void*  other1; //这里可能是下一链(页面)的call. 如果使用需提xs_UIcall, 不可直接调用这个call,必须提xs_UIcall 让"Refresh功能"调用这个call
    UINT   type;

    inline xs_MenuUiLsitData() { Text_col = itemText_col = type = 0; itemText = Text = NULL; other = NULL; other1 = NULL; }
    inline xs_MenuUiLsitData(char* a, UINT a1, char* b, UINT b1, void* c, UINT d)
    {
        Text = a; Text_col = a1; itemText = b; itemText_col = b1; other = c; type = d; other1 = c;
    }
    inline xs_MenuUiLsitData(char* a, UINT a1, char* b, UINT b1, void* c, void* c1, UINT d)
    {
        Text = a; Text_col = a1; itemText = b; itemText_col = b1; other = c; other1 = c1; type = d;
    }
  
    //列表_计算pos_Left
    xsFloat2 MenuList_CountPos_left(xs_MenuUiList& pos, int& i) { return (xsFloat2&)pos.Menulist_pos + xsFloat2(0, i * pos.Menulist_interval) + pos.MenuFont_Originalxy; }
    //列表_计算pos_right
    xsFloat2 MenuList_CountPos_right(xs_MenuUiList& pos, int& i) { return xsFloat2(pos.Menulist_pos.z, pos.Menulist_pos.y) + xsFloat2(0, i * pos.Menulist_interval) + xsFloat2(-pos.MenuFont_Originalxy.x, pos.MenuFont_Originalxy.y); }
    //选itme_Leftright移动
    void MenuList_item_LR_control(xs_MenuUi& UI);
    //列表运作
    void MenuList_run(xs_DrawList* drawList, xs_MenuUiList& pos,int& i)
    {
        char itemText[30] = {0};
        switch (type)
        {
        case xsMenuUiList_Type_Text:
            drawList->AddText(NULL, pos.Menu_FontSize, MenuList_CountPos_left(pos,i), Text_col, Text);
            break;
        case xsMenuUiList_Type_call:
            drawList->AddText(NULL, pos.Menu_FontSize, MenuList_CountPos_left(pos, i), Text_col, Text);
            
            drawList->AddText(NULL, pos.Menu_FontSize, MenuList_CountPos_right(pos, i), Text_col, NULL, VMProtectDecryptStringA("\xe9\xbe\xac"));//标识
            break;
        case xsMenuUiList_Type_Bool:
            drawList->AddText(NULL, pos.Menu_FontSize, MenuList_CountPos_left(pos, i), Text_col, Text);
            if (*(bool*)other)
                drawList->AddText(NULL, pos.Menu_FontSize, MenuList_CountPos_right(pos, i), itemText_col, NULL, VMProtectDecryptStringA("\xe9\xbe\xa6"));//标识
            else
                drawList->AddText(NULL, pos.Menu_FontSize, MenuList_CountPos_right(pos, i), Text_col, NULL, VMProtectDecryptStringA("\xe9\xbe\xa7"));//标识
            break;
        case xsMenuUiList_Type_Int:
            drawList->AddText(NULL, pos.Menu_FontSize, MenuList_CountPos_left(pos, i), Text_col, Text);
            stack_hidden_call(sprintf,itemText, VMProtectDecryptStringA("<%d>"), *(UINT*)other);
            drawList->AddText(NULL, pos.Menu_FontSize, MenuList_CountPos_right(pos, i), Text_col, NULL, itemText);//标识

            break;
        case xsMenuUiList_Type_Col_R :case xsMenuUiList_Type_col_G:case xsMenuUiList_Type_col_B:case xsMenuUiList_Type_col_A:
            {
                xsFloat4 RGBA = xs_Gui::col_Uint32ToFloat_RGBA(*(UINT32*)other);
                xsFloat3 HSV;
                xs_Gui::ColorConvertRGBtoHSV(RGBA.x, RGBA.y, RGBA.z, HSV.x, HSV.y, HSV.z);
                drawList->AddText(NULL, pos.Menu_FontSize, MenuList_CountPos_left(pos, i), *(UINT32*)other, Text);
                switch (type)
                {
                case xsMenuUiList_Type_Col_R:
                    stack_hidden_call(sprintf,itemText, VMProtectDecryptStringA("<%.2f>"), HSV.x);
                    break;
                case xsMenuUiList_Type_col_G:
                    stack_hidden_call(sprintf,itemText, VMProtectDecryptStringA("<%.2f>"), HSV.y);
                    break;
                case xsMenuUiList_Type_col_B:
                    stack_hidden_call(sprintf,itemText, VMProtectDecryptStringA("<%.2f>"), HSV.z);
                    break;
                case xsMenuUiList_Type_col_A:
                    stack_hidden_call(sprintf,itemText, VMProtectDecryptStringA("<%.2f>"), RGBA.w);
                    break;
                default:
                    break;
                }

                drawList->AddText(NULL, pos.Menu_FontSize, MenuList_CountPos_right(pos, i), *(UINT32*)other, NULL, itemText);//标识

            }

            break;
        case xsMenuUiList_Type_Float:
            drawList->AddText(NULL, pos.Menu_FontSize, MenuList_CountPos_left(pos, i), Text_col, Text);
            stack_hidden_call(sprintf,itemText, VMProtectDecryptStringA("<%.2f>"), *(float*)other);
            drawList->AddText(NULL, pos.Menu_FontSize, MenuList_CountPos_right(pos, i), Text_col, NULL, itemText);//标识

            break;
        case xsMenuUiList_Type_Theme:
            drawList->AddRectFilled((xsFloat2&)pos.TopTipMenulist_pos, (xsFloat2&)pos.TopTipMenulist_pos.z, 0xA0000000);
            drawList->AddText(NULL, pos.TipMenu_FontSize, (xsFloat2&)pos.TopTipMenulist_pos + pos.MenuFont_Originalxy, Text_col, Text);
            break;
        case xsMenuUiList_Type_Number:
            drawList->AddRectFilled((xsFloat2&)pos.LowTipMenulist_pos, (xsFloat2&)pos.LowTipMenulist_pos.z, 0x80000000);
            drawList->AddText(NULL, pos.TipMenu_FontSize, (xsFloat2&)pos.LowTipMenulist_pos + pos.MenuFont_Originalxy, 0xFFFFFFFF, Text);
            stack_hidden_call(sprintf,itemText, VMProtectDecryptStringA("%d/%d"), i, *(UINT*)other-2);
            drawList->AddText(NULL, pos.TipMenu_FontSize, xsFloat2(pos.LowTipMenulist_pos.z, pos.LowTipMenulist_pos.y) -xsFloat2(pos.TipMenu_FontSize, -pos.MenuFont_Originalxy.y), 0xFFFFFFFF, NULL, itemText);

            break;
        default:
            break;
        }
    }


};

struct xs_UIcall
{
    PVOID                        call;
    UINT                         TopLtem;//顶部itme  TopLtem ToLtem -TopLtem =当前选itmepos 26 -25=1
    UINT                         ToLtem;//选中itme

    inline xs_UIcall() { call = NULL;  TopLtem = ToLtem = 1; }
    inline xs_UIcall(PVOID a) { call = a; TopLtem = ToLtem = 1; }

    inline float ToLtem_Listpos() { return (float)(ToLtem - TopLtem); }//选中itme目列表位置

    xsFloat2 ToLtem_xypos(const xsFloat2& a, const float& gap) { return a + xsFloat2(0, ToLtem_Listpos() * gap); }
    xsFloat2 ToLtem_zwpos(const xsFloat2& a, const float& gap) { return a + xsFloat2(0, (ToLtem_Listpos() + 1) * gap); }
    //选itme_上下移动
    void MenuList_item_UD_control(UINT& Toitme_Capacity,UINT& visual_itmeCount)
    {
        if ( ToLtem > 1)
            if(xs_Gui::GetTriggerEventKeyButton(DIK_UPARROW) ? 1 : xs_Gui::GetTriggerEventKeyButton(DIK_NUMPAD8))//上
            {
                ToLtem--;
                xs_Gui::Play_Control_Item();
                if (ToLtem < TopLtem)
                    TopLtem--;
            }
        if (ToLtem < Toitme_Capacity -2)
            if (xs_Gui::GetTriggerEventKeyButton(DIK_DOWNARROW) ? 1 : xs_Gui::GetTriggerEventKeyButton(DIK_NUMPAD5))//下
            {
                ToLtem++;
                xs_Gui::Play_Control_Item();
                if (ToLtem - TopLtem >= visual_itmeCount)
                    TopLtem++;
            }
    }

};



struct xs_MenuUi
{
    xs_MenuUiList* posData;//设置菜单参数
    xs_DataFrame<xs_UIcall> MenuUi_call;//下一页的菜单call
    xs_DataFrame<xs_MenuUiLsitData> MenuUi_Data;//

	xs_MenuUiReminder MenuUi_TextMessage;

	bool Refresh, display;
	UINT         display_keyValue;
    bool display_TopList, display_LowList;
    UINT32* previous_coladdr;

    inline xs_MenuUi() { MenuUi_call.clear(); MenuUi_Data.clear(); display_keyValue = DIK_INSERT; display = 1; Refresh = display_TopList = display_LowList = 0; posData = NULL; previous_coladdr= NULL; }
    inline ~xs_MenuUi() { MenuUi_call.clear(); MenuUi_Data.clear(); MenuUi_free_posData(); }

	void         display_back() { display ^= xs_Gui::GetPressKeyButton(display_keyValue); }

    inline xs_UIcall&       MenuUI_GetCurrentUicall() { return MenuUi_call.Data[MenuUi_call.Size - 1]; }
    inline xs_MenuUiLsitData&   MenuUI_GetMenuUiLsitData(UINT i) { return MenuUi_Data.Data[i];}
    inline xs_MenuUiLsitData&   MenuUI_GetMenuUiLsitData() { return MenuUi_Data.Data[MenuUi_Data.Size - 1];}
    inline int              MenuUI_UicallCount() { return posData->Visible_OptionNumber < MenuUi_Data.Size - 2 ? MenuUI_GetCurrentUicall().TopLtem+posData->Visible_OptionNumber : MenuUI_GetCurrentUicall().TopLtem+MenuUi_Data.Size - 2; }

    static void col(xs_MenuUi* a, UINT32* RBGA)
    {
        char* cc = (char*)RBGA;
        a->MenuUi_PushTopName((char*)VMProtectDecryptStringA("col"));
        a->MenuUi_PushLowName((char*)VMProtectDecryptStringA("\xe9\xbe\xa1"));
        a->MenuUi_PushR_ToItme((char*)VMProtectDecryptStringA("H"), *RBGA, (char*)VMProtectDecryptStringA("tone"), *RBGA, RBGA);
        a->MenuUi_PushG_ToItme((char*)VMProtectDecryptStringA("S"), *RBGA, (char*)VMProtectDecryptStringA("saturation"), *RBGA, RBGA);
        a->MenuUi_PushB_ToItme((char*)VMProtectDecryptStringA("V"), *RBGA, (char*)VMProtectDecryptStringA("value"), *RBGA, RBGA);
        a->MenuUi_PushA_ToItme((char*)VMProtectDecryptStringA("A"), *RBGA, (char*)VMProtectDecryptStringA("transparency"), *RBGA, RBGA);
    }

    //选中项 提call
    void  MenuUi_Pushcall(void* call) { MenuUi_call.push_back(xs_UIcall(call)); Refresh = 0; }//这里重置Refresh 为了重置MenuUi_Data
    //选中项 提消息
	void  MenuUi_PushTextMessage(char* Text) { MenuUi_TextMessage = (xs_MenuUiReminder(Text)); }//这里重置Refresh 为了重置MenuUi_Data
    //选中项 绘制背景
    void  MenuUi_ToItme_DrawBackground(xs_DrawList* drawList,UINT32 col) { drawList->AddRectFilled(MenuUI_GetCurrentUicall().ToLtem_xypos((xsFloat2&)posData->Menulist_pos, posData->Menulist_interval), MenuUI_GetCurrentUicall().ToLtem_zwpos(xsFloat2(posData->Menulist_pos.z, posData->Menulist_pos.y), posData->Menulist_interval), col);}
    //选中项 文本颜色
    UINT  MenuUi_ToItme_Text_col() { UINT& col = MenuUI_GetMenuUiLsitData(MenuUI_GetCurrentUicall().ToLtem).Text_col; UINT col2 = col; col = 0xE0000000; return col2; }
    void  MenuUi_ToItme_Text_col(UINT col) { MenuUI_GetMenuUiLsitData(MenuUI_GetCurrentUicall().ToLtem).Text_col = col; }
    //选中项的参数 文本颜色
    UINT  MenuUi_ToItme_In_Text_col() { UINT& col = MenuUI_GetMenuUiLsitData(MenuUI_GetCurrentUicall().ToLtem).itemText_col; UINT col2 = col; col = 0xE0000000; return col2; }
    void  MenuUi_ToItme_In_Text_col(UINT col) { MenuUI_GetMenuUiLsitData(MenuUI_GetCurrentUicall().ToLtem).itemText_col = col; }
    //选中项 调制颜色call
    UINT32* MenuUi_ToItme_ModColcall() {if (MenuUi_Data.Size)return (UINT32*)MenuUI_GetMenuUiLsitData(MenuUI_GetCurrentUicall().ToLtem).other1; else return NULL; }
    UINT32* MenuUi_ToItme_ModColcall_col() { if (MenuUi_Data.Size)return (UINT32*)MenuUI_GetMenuUiLsitData(MenuUI_GetCurrentUicall().ToLtem).other; else return NULL; }
    //选中项 输出消息内容
    char* MenuUi_ToItme_GetTextMessage() { return MenuUI_GetMenuUiLsitData(MenuUI_GetCurrentUicall().ToLtem).itemText; }
    //选中项 消息内容绘制
    void  MenuUi_ToItme_DrawTextMessage(xs_DrawList* drawList, UINT32 col,char* Text) { 

        posData->Reminder_pos.w = posData->Reminder_pos.y + drawList->CalcText(NULL, posData->Menu_FontSize, Text).y + posData->MenuFont_Originalxy.y * 2.0f;
        drawList->AddRectFilled((xsFloat2&)posData->Reminder_pos, (xsFloat2&)posData->Reminder_pos.z, col); 
        drawList->AddText(NULL, posData->Menu_FontSize, (xsFloat2&)posData->Reminder_pos + posData->MenuFont_Originalxy- xsFloat2(0,1.0f), 0xFFFFFFFF, Text);

    }
    //选中项 计算调制颜色窗口位置
    xsFloat2 MenuUi_ToItme_ModColPos() { return (xsFloat2&)posData->Menulist_pos + xsFloat2(0, MenuUI_GetCurrentUicall().ToLtem * posData->Menulist_interval) - xsFloat2(10.0f, 100.0f); }
    // 消息提示call出来的  绘制
    void  MenuUi_DrawTextMessage(xs_DrawList* drawList, xs_MenuUiReminder* a,float gap) {

		if (!a->TextMessage)
			return;
		xsFloat4 pos = { 0,0,0,0 };
		(xsFloat2&)pos += xsFloat2(20.f, 14.f);
		//(xsFloat2&)pos.z = (xsFloat2&)pos + xsFloat2(100.f, 20.f);
		xsFloat2 text_pos = drawList->CalcText(NULL, posData->Menu_FontSize, a->TextMessage);
		(xsFloat2&)pos.z = (xsFloat2&)pos + text_pos + posData->MenuFont_Originalxy* 2.0f;
		//pos.w = pos.y + drawList->CalcText(NULL, posData->Menu_FontSize, a->TextMessage).y + posData->MenuFont_Originalxy.y * 2.0f;
		drawList->AddRectFilled((xsFloat2&)pos, (xsFloat2&)pos.z, 0x80000000);
		drawList->AddText(NULL, posData->Menu_FontSize, (xsFloat2&)pos + posData->MenuFont_Originalxy - xsFloat2(0, 1.0f), a->col, a->TextMessage);

    }
    //菜单 背景绘制
    void  MenuUi_Menu_DrawBackground(xs_DrawList* drawList, UINT32 col) { drawList->AddRectFilled((xsFloat2&)posData->Menulist_pos, (xsFloat2&)posData->Menulist_pos.z, col);}
    void  MenuUi_malloc_posData(xsFloat4 pos) { if (!posData)posData = XS_NEW(xs_MenuUiList)(pos, 10, 18.0F);}
    void  MenuUi_free_posData(){if (posData) XS_DELETE(posData);}
    bool* MenuUi_Refresh() { return &Refresh; }



    void MenuUi_PushTopName(char* Name)                                                { if (!display_TopList) {MenuUi_Data.push_back(xs_MenuUiLsitData(Name, 0xFFFFFFFF, NULL, 0, NULL, xsMenuUiList_Type_Theme));display_TopList = 1;}}//
    void MenuUi_PushLowName(char* Name)                                                { if (!display_LowList) {MenuUi_Data.push_back(xs_MenuUiLsitData(Name, 0, NULL, 0 , &MenuUi_Data.Size, xsMenuUiList_Type_Number)); display_LowList = 1; } }//
    void MenuUi_PushText_ToItme(char* Text, UINT32 col, char* itemText)                                  { MenuUi_Data.push_back(xs_MenuUiLsitData(Text, col, itemText, 0, NULL, xsMenuUiList_Type_Text)); }//
    void MenuUi_Pushcall_ToItme(char* Text, UINT32 col, char* itemText, PVOID call)                      { MenuUi_Data.push_back(xs_MenuUiLsitData(Text, col, itemText, 0, call, xsMenuUiList_Type_call)); }//
    void MenuUi_PushEnable_ToItme(char* Text, UINT32 col, char* itemText, bool& Enable)                      { MenuUi_Data.push_back(xs_MenuUiLsitData(Text, col, itemText, 0xFF78B64D, &Enable, xsMenuUiList_Type_Bool)); }    //
    void MenuUi_PushInt_ToItme(char* Text, UINT32 col, char* itemText, UINT32 col_2, void* Int)       { MenuUi_Data.push_back(xs_MenuUiLsitData(Text, col, itemText, col_2, Int, xsMenuUiList_Type_Int)); }     //
    void MenuUi_PushFloat_ToItme(char* Text, UINT32 col, char* itemText, UINT32 col_2, void* Float)   { MenuUi_Data.push_back(xs_MenuUiLsitData(Text, col, itemText, col_2, Float, xsMenuUiList_Type_Float)); }    //
    void MenuUi_PushR_ToItme(char* Text, UINT32 col, char* itemText, UINT32 col_2, void* Int) { MenuUi_Data.push_back(xs_MenuUiLsitData(Text, col, itemText, col_2, Int, xsMenuUiList_Type_Col_R)); }     //
    void MenuUi_PushG_ToItme(char* Text, UINT32 col, char* itemText, UINT32 col_2, void* Int) { MenuUi_Data.push_back(xs_MenuUiLsitData(Text, col, itemText, col_2, Int, xsMenuUiList_Type_col_G)); }     //
    void MenuUi_PushB_ToItme(char* Text, UINT32 col, char* itemText, UINT32 col_2, void* Int) { MenuUi_Data.push_back(xs_MenuUiLsitData(Text, col, itemText, col_2, Int, xsMenuUiList_Type_col_B)); }     //
    void MenuUi_PushA_ToItme(char* Text, UINT32 col, char* itemText, UINT32 col_2, void* Int) { MenuUi_Data.push_back(xs_MenuUiLsitData(Text, col, itemText, col_2, Int, xsMenuUiList_Type_col_A)); }     //

    void MenuUi_Pushcolcall_ToItme(char* Text, UINT32 col_, char* itemText, PVOID colAddr) { MenuUi_Data.push_back(xs_MenuUiLsitData(Text, col_, itemText, 0, &col, colAddr, xsMenuUiList_Type_call)); }//
    
    void MenuUi_PushText_ToItme(char* Text, UINT32 col, char* itemText, bool& Enable, char* message) { MenuUi_Data.push_back(xs_MenuUiLsitData(Text, col, itemText, 0, message, &Enable, xsMenuUiList_Type_Text)); }//
    void MenuUi_PushEnable_ToItme(char* Text, UINT32 col, char* itemText, bool& Enable, UINT32* size) { MenuUi_Data.push_back(xs_MenuUiLsitData(Text, col, itemText, 0xFF78B64D, &Enable, size, xsMenuUiList_Type_Bool)); }    //
    void MenuUi_PushInt_ToItme(char* Text, UINT32 col, char* itemText, UINT32 col_2, void* Int, UINT32* size) { MenuUi_Data.push_back(xs_MenuUiLsitData(Text, col, itemText, col_2, Int, size, xsMenuUiList_Type_Int)); }     //
    void MenuUi_PushFloat_ToItme(char* Text, UINT32 col, char* itemText, UINT32 col_2, void* Int, UINT32* size) { MenuUi_Data.push_back(xs_MenuUiLsitData(Text, col, itemText, col_2, Int, size, xsMenuUiList_Type_Float)); }    //


    void MenuUi_itme_ret_control() { if (xs_Gui::GetPressKeyButton(DIK_NUMPAD0) || xs_Gui::GetPressKeyButton(DIK_0)) if(MenuUi_call.Size-1){MenuUi_call.pop_back(); xs_Gui::Play_Control_Item(); Refresh = 0; } }
    void MenuUi_itme_UD_control() { MenuUI_GetCurrentUicall().MenuList_item_UD_control(MenuUi_Data.Size, posData->Visible_OptionNumber); }
    void MenuUi_itme_LR_control() { MenuUI_GetMenuUiLsitData(MenuUI_GetCurrentUicall().ToLtem).MenuList_item_LR_control((xs_MenuUi&)*this); }

    void MenuList_run(xs_DrawList* drawList)
    {
		xsGuiIO& io = xs_Gui::GetIO();

		char* RGBA = (char*)&MenuUi_TextMessage.col;
		if (io.time_Current - MenuUi_TextMessage.timecol >= io.time_Frequency / 150 && RGBA[3])
		{
			MenuUi_TextMessage.timecol = io.time_Current; RGBA[3]--;
		}
		if (RGBA[3])
			MenuUi_DrawTextMessage(drawList, &MenuUi_TextMessage, (float)0);

        PVOID callAddr = MenuUi_call.Data[MenuUi_call.Size - 1].call;
        
        if (!callAddr)
            return;
		display_back();
		if (!display)
			return;
        if(!Refresh)
        {
            Refresh = 1;
            display_TopList = 0;
            display_LowList = 0;
           
            MenuUi_Data.clear();
            
            if (callAddr == (PVOID)&col)
                ((void(__fastcall*)(xs_MenuUi&, UINT32*))(callAddr))((xs_MenuUi&)*this, previous_coladdr);
            else
            ((void(__fastcall*)(xs_MenuUi&))(callAddr))((xs_MenuUi&)*this);
            Maintenance();
        }
        previous_coladdr = MenuUi_ToItme_ModColcall();
        MenuUi_Menu_DrawBackground(drawList,0x40000000);//绘制 列表背景
        MenuUi_ToItme_DrawBackground(drawList,0xC0FFFFFF);//绘制 选中背景的Valid
        UINT temp_col1 = MenuUi_ToItme_Text_col();//改变 选中itme的Text col为黑色 这里可以优化一下
        UINT temp_col2 = MenuUi_ToItme_In_Text_col();//改变 选中itme的Text col为黑色 这里可以优化一下
        int itmeCount = MenuUI_UicallCount();
        if (display_TopList)// 等于0 的话 是有
            MenuUI_GetMenuUiLsitData(0).MenuList_run(drawList, (xs_MenuUiList&)*posData, itmeCount);//这是头副选itme
        if (display_LowList)
            MenuUI_GetMenuUiLsitData().MenuList_run(drawList, (xs_MenuUiList&)*posData, (int&)MenuUI_GetCurrentUicall().ToLtem); //这是尾副选itme

        for (int i = MenuUI_GetCurrentUicall().TopLtem,ic=0; i < itmeCount; i++,ic++)//这是 列表//最多循环14次
        {
            MenuUI_GetMenuUiLsitData((UINT)i).MenuList_run(drawList, (xs_MenuUiList&)*posData, ic);
        }

        //提示Text
        char* ToItme_text = MenuUi_ToItme_GetTextMessage();
        if (ToItme_text)
            MenuUi_ToItme_DrawTextMessage(drawList, 0x80000000, ToItme_text);
        else
            posData->Reminder_pos.w= posData->Reminder_pos.y;



        if (callAddr == (PVOID)&col)
        {
            UINT32 RGBA =*MenuUi_ToItme_ModColcall_col();
            xsFloat4 RGBA_F = xs_Gui::col_Uint32ToFloat_RGBA(RGBA);
            UINT32 RGBA_A = xs_Gui::col_FloatToUint32_RGBA({ RGBA_F.x,  RGBA_F.y,  RGBA_F.z, 0.01f });
            xsFloat3 HSV;
            xs_Gui::ColorConvertRGBtoHSV(RGBA_F.x, RGBA_F.y, RGBA_F.z, HSV.x, HSV.y, HSV.z);
            xsFloat2 RBGA_pos[2] = { MenuUi_ToItme_ModColPos() - xsFloat2(200.0f,0), MenuUi_ToItme_ModColPos() + xsFloat2(0,250.0f) };
            drawList->AddRectFilled(RBGA_pos[0], RBGA_pos[1], 0x80000000);//背景框

            RBGA_pos[0] += xsFloat2{ 15.0f,15.0f };
            RBGA_pos[1] -= xsFloat2{ 15.0f,65.0f };
            drawList->AddRectFilled(RBGA_pos[0] , RBGA_pos[1] , 0xffffffff);//饱和 鲜艳 调色
            drawList->AddRectFilled(RBGA_pos[0], RBGA_pos[1], RGBA_A, *MenuUi_ToItme_ModColcall_col(), RGBA_A, *MenuUi_ToItme_ModColcall_col());
            drawList->AddRectFilled(RBGA_pos[0], RBGA_pos[1], 0x01000000, 0x01000000, 0xFF000000, 0xFF000000);

            float H = (200.0F - 30.0F) * HSV.x;
            float S = (200.0F - 30.0F) * HSV.y;
            float V = (200.0F - 30.0F) * HSV.z;
            float A = (200.0F - 30.0F) * RGBA_F.w;
            drawList->AddLine( RBGA_pos[0]+ xsFloat2(S,0), xsFloat2{ RBGA_pos[0].x+S , RBGA_pos[1].y }, 0xffffffff);
            drawList->AddLine(xsFloat2(RBGA_pos[0].x, RBGA_pos[1].y  -V), RBGA_pos[1]-xsFloat2{ 0 , V }, 0xffffffff);

            drawList->AddRect(RBGA_pos[0], RBGA_pos[1] + xsFloat2{ 1.0f,1.0f }, 0xff000000, 1.0F);
            //彩色调
            RBGA_pos[0].y += 185.0f;
            RBGA_pos[1].y += 30.0f;
            drawList->AddRectFilled(RBGA_pos[0], RBGA_pos[1], RGBA_A, RGBA, RGBA_A, RGBA);//透明调
            drawList->AddLine(RBGA_pos[0] + xsFloat2(A, 0), xsFloat2{ RBGA_pos[0].x + A , RBGA_pos[1].y }, 0xffffffff);

            RBGA_pos[0].y += 25.0f;
            RBGA_pos[1].y += 25.0f;
            RBGA_pos[1].x = RBGA_pos[0].x;
            float col = (200.0F - 30.0F) / 6.0F;
            RBGA_pos[1].x += col;

            {
                drawList->AddRectFilled(RBGA_pos[0], RBGA_pos[1], 0xff0000ff, 0xff00ffff, 0xff0000ff, 0xff00ffff);//红到黄

                RBGA_pos[0].x += col; RBGA_pos[1].x += col;
                drawList->AddRectFilled(RBGA_pos[0], RBGA_pos[1], 0xff00ffff, 0xff00ff00, 0xff00ffff, 0xff00ff00);//黄到绿

                RBGA_pos[0].x += col; RBGA_pos[1].x += col;
                drawList->AddRectFilled(RBGA_pos[0], RBGA_pos[1], 0xff00ff00, 0xffffff00, 0xff00ff00, 0xffffff00);//绿到青

                RBGA_pos[0].x += col; RBGA_pos[1].x += col;
                drawList->AddRectFilled(RBGA_pos[0], RBGA_pos[1], 0xffffff00, 0xffff0000, 0xffffff00, 0xffff0000);//青到蓝

                RBGA_pos[0].x += col; RBGA_pos[1].x += col;
                drawList->AddRectFilled(RBGA_pos[0], RBGA_pos[1], 0xffff0000, 0xffff00ff, 0xffff0000, 0xffff00ff);//蓝到粉

                RBGA_pos[0].x += col; RBGA_pos[1].x += col;
                drawList->AddRectFilled(RBGA_pos[0], RBGA_pos[1], 0xffff00ff, 0xff0000ff, 0xffff00ff, 0xff0000ff);//粉到红
            }
            RBGA_pos[0].x -= (200.0F -60.0F);
            drawList->AddLine(RBGA_pos[0] + xsFloat2(H, 0), xsFloat2{ RBGA_pos[0].x + H , RBGA_pos[1].y }, 0xffffffff);
        }

        MenuUi_ToItme_Text_col(temp_col1);//改回去之前的Text_col
        MenuUi_ToItme_In_Text_col(temp_col2);//改回去之前的Text_col
        //这里键盘操作界面
        MenuUi_itme_UD_control();
        MenuUi_itme_LR_control();//选itme移动
        MenuUi_itme_ret_control();
    }

    //Refresh重置 MenuUi_Data  修复列表固定顺序
    void Maintenance()
    {
        xs_DataFrame<xs_MenuUiLsitData>* temp_MenuUiLsitData = XS_NEW(xs_DataFrame<xs_MenuUiLsitData>)();

        temp_MenuUiLsitData->clear();

        UINT Top_data = -1, Low_data = -1;

        for (int i = 0; i < MenuUi_Data.Size; i++)
        {
            if (MenuUi_Data.Data[i].type == xsMenuUiList_Type_Theme)
            {
                Top_data = i;
                continue;
            }
            if (MenuUi_Data.Data[i].type == xsMenuUiList_Type_Number)
            {
                Low_data = i;
                continue;
            }
            temp_MenuUiLsitData->push_back(MenuUi_Data.Data[i]);
        }
        if (Top_data != -1)
            temp_MenuUiLsitData->push_back(MenuUi_Data.Data[Top_data]);//xsMenuUiList_Type_Theme
        else
            temp_MenuUiLsitData->push_back(xs_MenuUiLsitData(NULL, 0, NULL, 0, NULL, xsMenuUiList_Type_Theme));
        Top_data = temp_MenuUiLsitData->Size;


        if (Low_data != -1)
            temp_MenuUiLsitData->push_back(MenuUi_Data.Data[Low_data]);//xsMenuUiList_Type_Number
        else
            MenuUi_Data.push_back(xs_MenuUiLsitData(NULL, 0, NULL, 0, &MenuUi_Data.Size, xsMenuUiList_Type_Number));
        Low_data = temp_MenuUiLsitData->Size;


        MenuUi_Data.clear();//清除旧 没顺序的列表Data

        //按照下面顺序 进行pushData  1、Top_data 2、用户选itmeData 3、Low_data
        MenuUi_Data.push_back(temp_MenuUiLsitData->Data[Top_data - 1]);

        for (int z = 0; z < temp_MenuUiLsitData->Size-2; z++)
            MenuUi_Data.push_back(temp_MenuUiLsitData->Data[z]);

        MenuUi_Data.push_back(temp_MenuUiLsitData->Data[Low_data - 1]);


        temp_MenuUiLsitData->clear();//清除 零食Data
        XS_DELETE(temp_MenuUiLsitData);//清除干净 避免内存溢出
    }

};


struct xs_componUi
{
    UINT32     type;
    xsFloat4  pos;
    UINT32     col;

    char*      Text;
    float      Text_size;
    void*      _value_0;
    void*      _value_1;
    void*      _value_2;
    void*      _value_3;
    inline  xs_componUi() { Text_size = type = col = 0; pos = xsFloat4(); Text = NULL; _value_0 = _value_1 = _value_2 = _value_3 = NULL; }

    bool ComponRun(xs_DrawList* drawList)
    {
        bool ret = 0;
        switch (type)
        {
        case xsMenuUiType_Nop:

            break;

        case xsMenuUiType_Text:
            drawList->AddText(NULL, Text_size, (xsFloat2&)pos, col, Text);
            break;

        case xsMenuUiType_Menu:
            //这里进入MenuList_run 实现绘制操作  (组件的Refresh,对MenuList_run不影响,仅仅只是组件的Refresh)
            // _value_0 = xs_MenuUi-地址
            if (_value_0)
                ((xs_MenuUi*)_value_0)->MenuList_run(drawList);
            break;


        default:
            break;
        }

        return ret;
    }

};

struct xs_windowUI
{
    //UINT         当数位;
    PVOID        call;
    xsFloat4    pos;
    UINT32       col;
    bool         Refresh,display;
    UINT         display_keyValue;
    xs_DataFrame<xs_componUi> compon;
    xs_DataFrame<xs_MenuUi> MenuUi;

    inline xs_windowUI() { compon.clear(); MenuUi.clear(); pos = xsFloat4(); col = 0; call = NULL; Refresh = 0; display = 1; display_keyValue = 0; }
    inline ~xs_windowUI() 
    {
        compon.clear();
        for (int i = 0; i < MenuUi.Size; i++) 
            MenuUi.Data[i].~xs_MenuUi(); 
        MenuUi.clear();
    }



    void windowRun(xs_DrawList* drawList)
    {
        if (!call)
            return;
        display_back();
        if (!display)
            return;

        if (!Refresh)
        {
            Refresh = 1;
            compon.clear();
            void* A= this;
            ((void(__fastcall*)(xs_windowUI&))(call))((xs_windowUI&)*this);
            RefreshMenu();
        }

        drawList->PushGuideBuffer();
        drawList->PushTextureID(drawList->_TextureIdStack.Data[0]);//xsFont纹理
        drawList->PushClipRect(xsFloat2(pos.x, pos.y), xsFloat2(pos.z, pos.w));

        drawList->AddRectFilled(xsFloat2(pos.x, pos.y), xsFloat2(pos.z, pos.w), col);

        for (int i = 0; i < compon.Size; i++)
        {
            if (compon.Data[i].ComponRun(drawList))
                Refreshcall();
        }

    }

    void         display_back() { display ^= xs_Gui::GetPressKeyButton(display_keyValue); }
    inline void  display_SetkeyValue(UINT Value) { display_keyValue = Value; }
    inline void  Refreshcall() { Refresh = 0; }//组件的Refresh
    void         RefreshMenu()
    {
        xs_DataFrame<xs_MenuUi>* a = XS_NEW(xs_DataFrame<xs_MenuUi>)();
        a->clear();
        for (int i = 0; i < compon.Size; i++)//这里 将type_列表 的_value_0 push到临时列表 记录
            if (compon.Data[i].type == xsMenuUiType_Menu)
                a->push_back(*(xs_MenuUi*)compon.Data[i]._value_0);//过滤 没有使用到的列表

        if (a->Size != 0)
        {
            for (int i = 0, ic=0; i < MenuUi.Size; i++)
            {
                if(MenuUi.Data[i].MenuUi_call.Data == a->Data[ic].MenuUi_call.Data)
                    ic++;
                else
                    MenuUi.Data[i].~xs_MenuUi();
            }
            MenuUi.clear();
            for (int ic = 0; ic < a->Size; ic++)//这里 将记录的临时列表 重新push到 正式的列表集里
                MenuUi.push_back(a->Data[ic]);
            for (int id = 0, ie = 0; id < compon.Size; id++)//这里 讲新的正式的 &列表地址 替换失效的 旧的 &列表地址
                if (compon.Data[id].type == xsMenuUiType_Menu)
                {
                    compon.Data[id]._value_0 = &MenuUi.Data[ie];
                    ie++;
                }
        }
        a->clear();
        XS_DELETE(a);
    }



    void  pushCompon_text(float Text_size, xsFloat2 xy, UINT32 col, char* Text);
    void  pushCompon_Menu(PVOID call, xsFloat4 xy, UINT32 col);//需UIcall检查是否  防止Refresh的时候又再次申请列表 导致内存溢出



};



struct xs_UI_windowList
{
    xs_DataFrame<xs_windowUI> windowUI;


    bool display;



    inline xs_UI_windowList(){ display = 0; windowUI.clear();}//4*8 32
    inline ~xs_UI_windowList() { windowUI.clear();  }


    
};



namespace xs_Menu_UI
{
    void MenuUiFrame();
    void MenuCreation(PVOID call);
	void MenuCreation_MnueMessage(int i, char* a);

    void window_call_1(xs_windowUI& a);
    void window_call_2(xs_windowUI& a);
    void window_call_3(xs_windowUI& a);

    void Menuhome(xs_MenuUi& a);
    void Menu2(xs_MenuUi& a);
};



