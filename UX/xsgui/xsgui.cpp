#include "xsgui.h"
#include "xsgui_internal.h"
#include "config.h"
#include "VMProtectSDK.h"
xs_GuiContext* GxsGui = NULL;

FILE* xsFileOpen(const char* filename, const char* mode)
{
	
#if defined(_WIN32) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS) && !defined(__CYGWIN__) && !defined(__GNUC__)
    // We need a fopen() wrapper because MSVC/Windows fopen doesn't handle UTF-8 filenames.
    // Previously we used ImTextCountCharsFromUtf8/ImTextStrFromUtf8 here but we now need to support ImWchar16 and ImWchar32!
    const int filename_wsize = stack_hidden_call(MultiByteToWideChar,CP_UTF8, 0, filename, -1, (LPWSTR)NULL, 0);
    const int mode_wsize = stack_hidden_call(MultiByteToWideChar,CP_UTF8, 0, mode, -1, (LPWSTR)NULL, 0);
    xs_DataFrame<WCHAR> buf;
    buf.resize(filename_wsize + mode_wsize);
    stack_hidden_call(MultiByteToWideChar,CP_UTF8, 0, filename, -1, (wchar_t*)&buf.Data[0], filename_wsize);
    stack_hidden_call(MultiByteToWideChar,CP_UTF8, 0, mode, -1, (wchar_t*)&buf.Data[filename_wsize], mode_wsize);
    return stack_hidden_call(_wfopen,(const wchar_t*)&buf.Data[0], (const wchar_t*)&buf.Data[filename_wsize]);
#else
    return fopen(filename, mode);
#endif
	
}

// We should in theory be using fseeko()/ftello() with off_t and _fseeki64()/_ftelli64() with __int64, waiting for the PR that does that in a very portable pre-C++11 zero-warnings way.
bool    xsFileClose(FILE* f) { return stack_hidden_call(fclose,f) == 0; }
UINT64   xsFileGetSize(FILE* f) { long off = 0, sz = 0; return ((off = stack_hidden_call(ftell,f)) != -1 && !stack_hidden_call(fseek,f, 0, SEEK_END) && (sz = stack_hidden_call(ftell,f)) != -1 && !stack_hidden_call(fseek,f, off, SEEK_SET)) ? (UINT64)sz : (UINT64)-1; }
UINT64   xsFileRead(void* data, UINT64 sz, UINT64 count, FILE* f) { return stack_hidden_call(fread,data, (size_t)sz, (size_t)count, f); }
UINT64   xsFileWrite(const void* data, UINT64 sz, UINT64 count, FILE* f) { return stack_hidden_call(fwrite,data, (size_t)sz, (size_t)count, f); }


// Helper: Load file content into memory
// Memory allocated with IM_ALLOC(), must be freed by user using IM_stack_hidden_call_v(free,) == ImGui::Memstack_hidden_call_v(free,)
// This can't really be used with "rt" because fseek size won't match read size.
void* xsFileLoadToMemory(const char* filename, const char* mode, size_t* out_file_size, int padding_bytes)
{
	
    if (out_file_size)
        *out_file_size = 0;

    FILE* f;
    if ((f = xsFileOpen(filename, mode)) == NULL)
        return NULL;

    size_t file_size = (size_t)xsFileGetSize(f);
    if (file_size == (size_t)-1)
    {
        xsFileClose(f);
        return NULL;
    }

    void* file_data = stack_hidden_call( malloc,file_size + padding_bytes);
    if (file_data == NULL)
    {
        xsFileClose(f);
        return NULL;
    }
    if (xsFileRead(file_data, 1, file_size, f) != file_size)
    {
        xsFileClose(f);
        stack_hidden_call_v(free,file_data);
        return NULL;
    }
    if (padding_bytes > 0)
        memset((void*)(((char*)file_data) + file_size), 0, (size_t)padding_bytes);

    xsFileClose(f);
    if (out_file_size)
        *out_file_size = file_size;
	
    return file_data;
	
}
bool xs_config::InitConfig()
{
	
    if ((ConfigHandle = xsFileOpen(ConfigPath, VMProtectDecryptStringA("rb+"))) == NULL)
    {//文件不存在 create一个
        ConfigHandle = xsFileOpen(ConfigPath, VMProtectDecryptStringA("wb+"));
        return 1;
    }
	
    return 0;
}
bool xs_config::SaveConfig()
{
	
    if (Data && size)
    {
        if (!ConfigHandle)
            return 0;
        xsFileClose(ConfigHandle);
        ConfigHandle = NULL;
        ConfigHandle = xsFileOpen(ConfigPath, VMProtectDecryptStringA("wb+"));
        if (!ConfigHandle)
            return 0;
        //xsFileWrite(Data, 1, 1, ConfigHandle);
        xsFileWrite(Data,1,size,ConfigHandle);
        
        stack_hidden_call_v(free,Data);
        Data = NULL;
        size = 0;
        //InitConfig();
        return 1;
    }
	
    return 0;

}

void xs_config::OffConfig()
{
    if (ConfigHandle)
        xsFileClose(ConfigHandle);
    ConfigHandle = NULL;
    if (Data)
        stack_hidden_call_v(free, Data);
    Data = NULL;
    size = 0;
}
char* xs_config::Read_ulong_Config(const char* itme, const char* subitem, UINT32* value)
{

	if (Data == NULL)
	{
		size = (size_t)xsFileGetSize(ConfigHandle);
		if (!size)
		{
			size = 0x100;
			Data = stack_hidden_call(malloc, size);
			memset(Data, 0, size);
		}
		else {
			Data = stack_hidden_call(malloc, size + 0x10);
			memset(Data, 0, size + 0x10);
			if (xsFileRead(Data, 1, size, ConfigHandle) != size)
			{
				stack_hidden_call_v(free, Data);
				Data = NULL;
				return 0;
			}
		}
	}

	char* itme_addr = NULL;
	if (itme_addr = strstr((char*)Data, itme))
	{
		itme_addr = itme_addr + 1;
		char* itme_Tailaddr = strchr(itme_addr, '[');

		char* subitem_addr = strstr(itme_addr, subitem);

		if (subitem_addr)
		{
			if (itme_Tailaddr == NULL)
				goto ret;
			if ((UINT64)subitem_addr < (UINT64)itme_Tailaddr)
			{
			ret:

				char* subitem_value = strchr(subitem_addr, '=') + 1;

				if (subitem_value)
					*value = TextTovalue(subitem_value);

				return subitem_value;
			}

		}


	}


	return 0;

}
char* xs_config::Write_ulong_Config(const char* itme, const char* subitem, UINT32 value)
{
	ULONG32 subitemvalue = 0;
	char* subitem_value = Read_ulong_Config(itme, subitem, &subitemvalue);
	if (subitem_value)
	{
		char* Text_Insert_top = strchr(subitem_value, '\n');

		char* Textvalue = (char*)stack_hidden_call(malloc, 0x256);
		valueToText(value, Textvalue);

		char* new_data = insert_text((char*)Data, subitem_value, Text_Insert_top, Textvalue);

		if (Data && size)
		{
			memset(Data, 0, size);
			stack_hidden_call_v(free, Data);

		}
		stack_hidden_call_v(free, Textvalue);
		Data = new_data;
		size = strlen(new_data);
	}
	else
	{
		char* itme_addr = NULL;
		if (Data == NULL)
		{
			Data = stack_hidden_call(malloc, 0x100);
			memset(Data, 0, 0x100);
		}
		//先搜itme
		if (itme_addr = strstr((char*)Data, itme))
		{
			char Textvalue[8] = { 0 };

			{
				char* Text_Insert_top = strchr(itme_addr, '\n') + 1;

				valueToText(value, Textvalue);
				char subitem_[256] = { 0 };
				stack_hidden_call(sprintf, subitem_, VMProtectDecryptStringA("%s%s\n"), subitem, Textvalue);

				char* new_data = insert_text((char*)Data, Text_Insert_top, Text_Insert_top, subitem_);

				if (Data && size)
				{
					memset(Data, 0, size);
					stack_hidden_call_v(free, Data);

				}
				Data = new_data;
				size = strlen(new_data);
			}

		}
		else
		{
			char Textvalue[8] = { 0 };

			{

				valueToText(value, Textvalue);
				char subitem_[256] = { 0 };
				stack_hidden_call(sprintf, subitem_, VMProtectDecryptStringA("%s\n%s%s\n"), itme, subitem, Textvalue);

				char* new_data = insert_text((char*)Data, NULL, NULL, subitem_);

				if (Data && size)
				{
					memset(Data, 0, size);
					stack_hidden_call_v(free, Data);

				}
				Data = new_data;
				size = strlen(new_data);
			}

		}


	}

	return 0;

}

static void AddDrawList(xs_DataFrame<xs_DrawList*>* out_list, xs_DrawList* draw_list)
{
	
    if (draw_list->GuideBuffer.Size == 0)
        return;

    // 如果未使用，删除尾随命令
    xsDrawGuide* curr_cmd = &draw_list->GuideBuffer.back();
    while (curr_cmd->ElemCount == 0)
    {
        draw_list->GuideBuffer.pop_back();
        if (draw_list->GuideBuffer.Size == 0)
            return;
        curr_cmd = &draw_list->GuideBuffer.back();
    }
    //if (curr_cmd->ElemCount == 0 /* && curr_cmd->UserCallback == NULL*/)
    //{
    //    draw_list->GuideBuffer.pop_back();
    //    if (draw_list->GuideBuffer.Size == 0)
    //        return;
    //}

    // Draw list sanity check. Detect mismatch between Capacityretention() calls and incrementing _VtxCurrentIdx, _VtxWritePtr etc.
    // May trigger for you if you are using PrimXXX functions incorrectly.
    //IM_ASSERT(draw_list->VtxBuffer.Size == 0 || draw_list->_VtxWritePtr == draw_list->VtxBuffer.Data + draw_list->VtxBuffer.Size);
    //IM_ASSERT(draw_list->IdxBuffer.Size == 0 || draw_list->_IdxWritePtr == draw_list->IdxBuffer.Data + draw_list->IdxBuffer.Size);
    //if (!(draw_list->Flags & ImDrawListFlags_AllowVtxOffset))
    //    IM_ASSERT((int)draw_list->_VtxCurrentIdx == draw_list->VtxBuffer.Size);

    // Check that draw_list doesn't use more vertices than indexable (default xsDrawIdx = unsigned short = 2 bytes = 64K vertices per ImDrawList = per window)
    // If this assert triggers because you are drawing lots of stuff manually:
    // - First, make sure you are coarse clipping yourself and not trying to draw many things outside visible bounds.
    //   Be mindful that the ImDrawList API doesn't filter vertices. Use the Metrics window to inspect draw list contents.
    // - If you want large meshes with more than 64K vertices, you can either:
    //   (A) Handle the ImDrawCmd::VtxOffset value in your renderer back-end, and set 'io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset'.
    //       Most example back-ends already support this from 1.71. Pre-1.71 back-ends won't.
    //       Some graphics API such as GL ES 1/2 don't have a way to offset the starting vertex so it is not supported for them.
    //   (B) Or handle 32-bit indices in your renderer back-end, and uncomment '#define xsDrawIdx unsigned int' line in imconfig.h.
    //       Most example back-ends already support this. For example, the OpenGL example code detect index size at compile-time:
    //         glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(xsDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
    //       Your own engine or render API may use different parameters or function calls to specify index sizes.
    //       2 and 4 bytes indices are generally supported by most graphics API.
    // - If for some reason neither of those solutions works for you, a workaround is to call BeginChild()/EndChild() before reaching
    //   the 64K limit to split your draw commands in multiple draw lists.
    /*if (sizeof(xsDrawIdx) == 2)
        IM_ASSERT(draw_list->_VtxCurrentIdx < (1 << 16) && "Too many vertices in ImDrawList using 16-bit indices. Read comment above");*/

    out_list->push_back(draw_list);
	
}

static void SetDrawData(xs_DataFrame<xs_DrawList*>* draw_lists, xs_DrawData* draw_data)
{
	
    xsGuiIO& io = xs_Gui::GetIO();
    draw_data->Valid = true;
    draw_data->DrawLists = (draw_lists->Size > 0) ? draw_lists->Data : NULL;
    draw_data->DrawListsCount = draw_lists->Size;
    draw_data->TotalVtxCount = draw_data->TotalIdxCount = 0;
    draw_data->DisplayPos = xsFloat2(0.0f, 0.0f);
    draw_data->DisplaySize = io.DisplaySize;
    draw_data->FramebufferScale = xsFloat2(1.0f, 1.0f);
    for (int n = 0; n < draw_lists->Size; n++)
    {
        draw_data->TotalVtxCount += draw_lists->Data[n]->VtxBuffer.Size;
        draw_data->TotalIdxCount += draw_lists->Data[n]->IdxBuffer.Size;
    }
	
}

void xs_Gui::NewFrame()
{
	//
    xs_GuiContext& g = *GxsGui;


    // Setup current font and draw list shared data
    g.IO.xsFont->Locked = true;
    SetCurrentFont(GetDefaultFont());




    g.FramerateSecPerFrameAccum += g.IO.DeltaTime - g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx];
    g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx] = g.IO.DeltaTime;
    g.FramerateSecPerFrameIdx = (g.FramerateSecPerFrameIdx + 1) % XS_ARRAYSIZE(g.FramerateSecPerFrame);
    g.IO.Framerate = (g.FramerateSecPerFrameAccum > 0.0f) ? (1.0f / (g.FramerateSecPerFrameAccum / (float)XS_ARRAYSIZE(g.FramerateSecPerFrame))) : FLT_MAX;


    g.DrawListSharedData.ClipRectFullscreen = xsFloat4(0.0f, 0.0f, g.IO.DisplaySize.x, g.IO.DisplaySize.y);

    g.BackgroundDrawList._ResetForNewFrame();
    g.BackgroundDrawList.PushTextureID(g.IO.xsFont->TextureID);
    g.BackgroundDrawList.PushClipRectFullScreen();

    g.ForegroundDrawList._ResetForNewFrame();
    g.ForegroundDrawList.PushTextureID(g.IO.xsFont->TextureID);
    g.ForegroundDrawList.PushClipRectFullScreen();

    // Mark rendering data as invalid to prevent user who may have a handle on it to use it.
    g.DrawData.Clear();
	//
}

void xs_Gui::Render()
{
	//
    xs_GuiContext& g = *GxsGui;

    // Unlock font atlas
    g.IO.xsFont->Locked = false;

    g.DrawDataBuilder.Clear();

    //前置绘制
    if (!g.BackgroundDrawList.VtxBuffer.empty())
        AddDrawList(&g.DrawDataBuilder.Layers[0], &g.BackgroundDrawList);


    if (!g.ForegroundDrawList.VtxBuffer.empty())
        AddDrawList(&g.DrawDataBuilder.Layers[0], &g.ForegroundDrawList);

    // 最终Render的 DrawData结构
    SetDrawData(&g.DrawDataBuilder.Layers[0], &g.DrawData);
	//
}

ImFont* xs_Gui::GetDefaultFont() {  ImFont*axa = GxsGui->IO.xsFont->Fonts[0];  return axa; }
xs_GuiContext* xs_Gui::CreateContext()
{
	
    //xs_GuiContext* ctx = (xs_GuiContext*)stack_hidden_call( malloc,sizeof(xs_GuiContext));
    xs_GuiContext* ctx = XS_NEW(xs_GuiContext)();
    if (GxsGui == NULL)
        GxsGui = ctx;
    //if (!GxsGui->IO.xsFont)
    //    GxsGui->IO.xsFont = (xmxsFont图集*)stack_hidden_call( malloc,sizeof(xmxsFont图集));
    //Initialize(ctx);
	
    return ctx;
}

void xs_Gui::DestroyContext(xs_GuiContext* ctx)
{
	//
    if (ctx == NULL)
        ctx = GxsGui;
    //Shutdown(ctx);
    if (GxsGui == ctx)
        GxsGui = NULL;
    stack_hidden_call_v(free,ctx);
	//
}

xs_DrawData* xs_Gui::GetDrawData()
{
    xs_GuiContext& g = *GxsGui;
    return g.DrawData.Valid ? &g.DrawData : NULL;
}

xsGuiIO& xs_Gui::GetIO()
{
    //IM_ASSERT(GImGui != NULL && "No current context. Did you call ImGui::CreateContext() and ImGui::SetCurrentContext() ?");

    return GxsGui->IO;
}

char* xs_Gui::GetCurrentKeyButton()
{
    return GxsGui->IO.KeyButton_Current;
}

bool xs_Gui::GetBounceKeyButton(UINT key)
{
    if (GxsGui->IO.KeyButton_Later[key] && !GxsGui->IO.KeyButton_Current[key])
        return 1;
    return 0;
}

bool xs_Gui::GetPressKeyButton(UINT key)
{
	
    if (!GxsGui->IO.KeyButton_Later[key] && GxsGui->IO.KeyButton_Current[key])
        return 1;
	
    return 0;
	//
}

UINT64 Record_TriggerKeyTime = 0;

bool xs_Gui::GetTriggerEventKeyButton(UINT key)
{
	
    bool ret = 0;
    if (GetPressKeyButton(key))
    {
            Record_TriggerKeyTime = GxsGui->IO.time_Current + GxsGui->IO.KeyButton_Later_trigger;
            ret = 1;

    }
    else if(Record_TriggerKeyTime&&GetCurrentKeyButton()[key])
    {
        if (GetBounceKeyButton(key))
            Record_TriggerKeyTime = 0;
        else if (Record_TriggerKeyTime <= GxsGui->IO.time_Current)
            if (GxsGui->IO.time_Current - Record_TriggerKeyTime >= GxsGui->IO.KeyButton_Later_triggerInterval)
            {
                Record_TriggerKeyTime = GxsGui->IO.time_Current;
                ret = 1;
            }
    }
	
    return ret;
	//
}

DIMOUSESTATE2* xs_Gui::GetCurrentMouseButton()
{
	return GxsGui->IO.Mouse_Current;
}

bool xs_Gui::GetBounceMouseButton(UINT key)
{
	if (GxsGui->IO.Mouse_Later->rgbButtons[key] && !GxsGui->IO.Mouse_Current->rgbButtons[key])
		return 1;
	return 0;
}

bool xs_Gui::GetPressMouseButton(UINT key)
{

	if (!GxsGui->IO.Mouse_Later->rgbButtons[key] && GxsGui->IO.Mouse_Current->rgbButtons[key])
		return 1;

	return 0;
	//
}


void xs_Gui::Play_Control_Item()
{
    //播放声音((WAVEFORMATEX*)&wfx_1, (const BYTE*)选itme_移动_Data_1, sizeof(选itme_移动_Data_1));
    //播放声音((WAVEFORMATEX*)&wfx_0, (const BYTE*)选itme_移动_Data_0, sizeof(选itme_移动_Data_0));


}

// Important: this alone doesn't alter current ImDrawList state. This is called by PushFont/PopFont only.
void xs_Gui::SetCurrentFont(ImFont* font)
{
	//
    xs_GuiContext& g = *GxsGui;
    IM_ASSERT(font && font->IsLoaded());    // Font Atlas not created. Did you call io.Fonts->GetTexDataAsRGBA32 / GetTexDataAsAlpha8 ?
    IM_ASSERT(font->Scale > 0.0f);
    g.Font = font;
    g.FontBaseSize = ImMin(1.0f, g.IO.FontGlobalScale * g.Font->FontSize * g.Font->Scale);
    g.FontSize =  0.0f;

    xsFontAtlas* atlas = g.Font->ContainerAtlas;
    g.DrawListSharedData.TexUvWhitePixel = { atlas->TexUvWhitePixel[0], atlas->TexUvWhitePixel[1] };
    g.DrawListSharedData.TexUvLines = atlas->TexUvLines;
    g.DrawListSharedData.xsFont = g.Font;
    g.DrawListSharedData.FontSize = g.FontSize;
	//
}

xs_DrawList* xs_Gui::GetBackgroundDrawList()
{
	
	xs_DrawList* axa= &GxsGui->BackgroundDrawList;
	
    return axa;
	
}

xs_DrawList* xs_Gui::GetForegroundDrawList()
{
	
	xs_DrawList*axa = &GxsGui->ForegroundDrawList;
	
	return axa;
	
}

xsGuiIO::xsGuiIO()
{
    time_Current = 0;
    time_Frequency = 0;
    Framerate = 0;

    g_pDirectInput = NULL;
    g_pKeyboardDevice = NULL;
	g_pMouseDevice = NULL;

    KeyButton_Current=(char*)stack_hidden_call( malloc,256);
    KeyButton_Later = (char*)stack_hidden_call( malloc,256);

	Mouse_Current=(DIMOUSESTATE2*)stack_hidden_call(malloc, sizeof(DIMOUSESTATE2));
	Mouse_Later = (DIMOUSESTATE2*)stack_hidden_call(malloc, sizeof(DIMOUSESTATE2));

    memset(KeyButton_Current,0,256);
    memset(KeyButton_Later, 0, 256);
    xsFont = NULL;
    FontGlobalScale = 1.0f;
    FontDefault = NULL;
    FontAllowUserScaling = false;

    DisplaySize = { -1.0f,-1.0f };
    DeltaTime = 1.0f / 60.0f;
	
}


xsFloat4 xs_Gui::col_Uint32ToFloat_RGBA(UINT32 in)
{
	//
    float s = 1.0f / 255.0f;
    return xsFloat4(
        ((in >> XS_COL32_R_SHIFT) & 0xFF) * s,
        ((in >> XS_COL32_G_SHIFT) & 0xFF) * s,
        ((in >> XS_COL32_B_SHIFT) & 0xFF) * s,
        ((in >> XS_COL32_A_SHIFT) & 0xFF) * s);
	//
}

xsFloat4 xs_Gui::col_Uint32ToFloat_ARGB(UINT32 in)
{
	//
    float s = 1.0f / 255.0f;
    return xsFloat4(
        ((in >> XS_COL32_B_SHIFT) & 0xFF) * s,
        ((in >> XS_COL32_G_SHIFT) & 0xFF) * s,
        ((in >> XS_COL32_R_SHIFT) & 0xFF) * s,
        ((in >> XS_COL32_A_SHIFT) & 0xFF) * s);
	//
}

UINT32 xs_Gui::col_FloatToUint32_ARGB(const xsFloat4& in)
{
	
    UINT32 out;
    out = ((UINT32)XS_F32_TO_INT8_SAT(in.x)) << XS_COL32_B_SHIFT;
    out |= ((UINT32)XS_F32_TO_INT8_SAT(in.y)) << XS_COL32_G_SHIFT;
    out |= ((UINT32)XS_F32_TO_INT8_SAT(in.z)) << XS_COL32_R_SHIFT;
    out |= ((UINT32)XS_F32_TO_INT8_SAT(in.w)) << XS_COL32_A_SHIFT;
	
    return out;
	
}

UINT32 xs_Gui::col_FloatToUint32_RGBA(const xsFloat4& in)
{
	
    UINT32 out;
    out = ((UINT32)XS_F32_TO_INT8_SAT(in.x)) << XS_COL32_R_SHIFT;
    out |= ((UINT32)XS_F32_TO_INT8_SAT(in.y)) << XS_COL32_G_SHIFT;
    out |= ((UINT32)XS_F32_TO_INT8_SAT(in.z)) << XS_COL32_B_SHIFT;
    out |= ((UINT32)XS_F32_TO_INT8_SAT(in.w)) << XS_COL32_A_SHIFT;
	
    return out;
	
}

// Convert rgb floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
void xs_Gui::ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
{
	//
    float K = 0.f;
    if (g < b)
    {
        ImSwap(g, b);
        K = -1.f;
    }
    if (r < g)
    {
        ImSwap(r, g);
        K = -2.f / 6.f - K;
    }

    const float chroma = r - (g < b ? g : b);
    out_h = ImFabs(K + (g - b) / (6.f * chroma + 1e-20f));
    out_s = chroma / (r + 1e-20f);
    out_v = r;
	//
}

// Convert hsv floats ([0-1],[0-1],[0-1]) to rgb floats ([0-1],[0-1],[0-1]), from Foley & van Dam p593
// also http://en.wikipedia.org/wiki/HSL_and_HSV
void xs_Gui::ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
	//
    if (s == 0.0f)
    {
        // gray
        out_r = out_g = out_b = v;
        return;
    }

    h = ImFmod(h, 1.0f) / (60.0f / 360.0f);
    int   i = (int)h;
    float f = h - (float)i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i)
    {
    case 0: out_r = v; out_g = t; out_b = p; break;
    case 1: out_r = q; out_g = v; out_b = p; break;
    case 2: out_r = p; out_g = v; out_b = t; break;
    case 3: out_r = p; out_g = q; out_b = v; break;
    case 4: out_r = t; out_g = p; out_b = v; break;
    case 5: default: out_r = v; out_g = p; out_b = q; break;
    }
	//
}

bool xs_Gui::char_to_value(const char a, char* b)
{
    //'\n'10
    switch (a)
    {
    case '0':
        b[0] = 0;
        break;
    case '1':
        b[0] = 1;
        break;
    case '2':
        b[0] = 2;
        break;
    case '3':
        b[0] = 3;
        break;
    case '4':
        b[0] = 4;
        break;
    case '5':
        b[0] = 5;
        break;
    case '6':
        b[0] = 6;
        break;
    case '7':
        b[0] = 7;
        break;
    case '8':
        b[0] = 8;
        break;
    case '9':
        b[0] = 9;
        break;

    case 'a':
        b[0] = 0xA;
        break;
    case 'A':
        b[0] = 0xA;
        break;
    case 'b':
        b[0] = 0xB;
        break;
    case 'B':
        b[0] = 0xB;
        break;
    case 'c':
        b[0] = 0xC;
        break;
    case 'C':
        b[0] = 0xC;
        break;
    case 'd':
        b[0] = 0xD;
        break;
    case 'D':
        b[0] = 0xD;
        break;
    case 'e':
        b[0] = 0xE;
        break;
    case 'E':
        b[0] = 0xE;
        break;
    case 'f':
        b[0] = 0xF;
        break;
    case 'F':
        b[0] = 0xF;
        break;
    default:
        return 0;
        break;
    }
    return 1;
}

int xs_Gui::char_to_value(const char* a, char* b)
{
    //'\n'10
    int sen = 0;
    int sen2 = 0;
    while (true)
    {
        if (a[sen] == (char)' ')
        {
            sen++;
            goto ent;
        }

        if (a[sen] == (char)'?')
        {
            b[sen2] = (char)'?';

            sen2++;
            sen++;
            goto ent;
        }

        char value = 0;
        if (!char_to_value(a[sen], &value))
            return 0;
        char value2 = 0;
        if (!char_to_value(a[sen + 1], &value2))
            return 0;

        value <<= 4;
        value |= value2;
        b[sen2] = value;


        sen2++;
        sen += 2;
    ent:
        if (a[sen] == (char)'\n')
            return sen2;
    }

}