#pragma once
// xsgui,1.0版本
#include <float.h>
#include <Dinput.h>
#include <math.h>
//#include "imgui_internal.h"
//#include "imgui_internal.h"
#include "VMProtectSDK.h"

#include "hook_call.h"
#ifdef IMGUI_USE_BGRA_PACKED_COLOR
#define XS_COL32_R_SHIFT    16
#define XS_COL32_G_SHIFT    8
#define XS_COL32_B_SHIFT    0
#define XS_COL32_A_SHIFT    24
#define XS_COL32_A_col     0xFF000000
#else
#define XS_COL32_R_SHIFT    0
#define XS_COL32_G_SHIFT    8
#define XS_COL32_B_SHIFT    16
#define XS_COL32_A_SHIFT    24
#define XS_COL32_A_col     0xFF000000
#endif
#define XS_COL32(R,G,B,A)    (((UINT32)(A)<<XS_COL32_A_SHIFT) | ((UINT32)(B)<<XS_COL32_B_SHIFT) | ((UINT32)(G)<<XS_COL32_G_SHIFT) | ((UINT32)(R)<<XS_COL32_R_SHIFT))
#define XS_COL32_WHITE       XS_COL32(255,255,255,255)  // Opaque white = 0xFFFFFFFF
#define XS_COL32_BLACK       XS_COL32(0,0,0,255)        // Opaque black
#define XS_COL32_BLACK_TRANS XS_COL32(0,0,0,0)          // Transparent black = 0x00000000
#define XS_COL32_ARGB(A,R,G,B)    (((UINT32)(A)<<XS_COL32_A_SHIFT) | ((UINT32)(B)<<XS_COL32_B_SHIFT) | ((UINT32)(G)<<XS_COL32_G_SHIFT) | ((UINT32)(R)<<XS_COL32_R_SHIFT))

#define XS_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))
#define INITGUID

void* xsFileLoadToMemory(const char* filename, const char* mode, size_t* out_file_size, int padding_bytes);

#ifndef IM_ASSERT
#include <assert.h>
#define IM_ASSERT(_EXPR)            assert(_EXPR)                               // You can override the default assert handler by editing imconfig.h
#endif
#ifndef IM_DRAWLIST_TEX_LINES_WIDTH_MAX
#define IM_DRAWLIST_TEX_LINES_WIDTH_MAX     (63)
#endif

struct xsFloat2;
struct xsFloat3;
struct xsFloat4;
struct xsDrawVert;
struct xsDrawGuide;
struct xs_DrawList;
struct xs_DrawData;
struct xsGuiIO;
struct xsFontAtlas;
struct xs_GuiContext;
struct xs_DrawDataBuilder;
struct xs_DrawListSharedData;
struct xsFontGlyph;
struct ImFontConfig;
struct ImFont;

class Matrix3x4;
class Matrix4x4;

struct xs_uint
{
    UINT value;
    UINT speed;
    UINT min;
    UINT max;
    xs_uint() {  value = 0; speed = 0; min = 0; max = 0;}
    xs_uint(UINT _value, UINT _speed, UINT _min, UINT _max) {  value = _value; speed = _speed; min = _min; max = _max;}
};

struct xs_float
{
    float value;
    float speed;
    float min;
    float max;
    xs_float() { value = 0; speed = 0; min = 0; max = 0; }
    xs_float(float _value, float _speed, float _min, float _max) {  value = _value; speed = _speed; min = _min; max = _max; }
};

struct xsFloat2
{
    float                                   x, y;
    xsFloat2() { x = y = 0.0f; }
    xsFloat2(float _x, float _y) {  x = _x; y = _y; }
	float  operator[] (size_t idx) const { return (&x)[idx]; }    // We very rarely use this [] operator, the assert overhead is fine.
    float& operator[] (size_t idx) {  return (&x)[idx];}    // We very rarely use this [] operator, the assert overhead is fine.
};
struct xsFloat3
{
    xsFloat3() = default;
    constexpr xsFloat3(float x, float y, float z) noexcept : x{ x }, y{ y }, z{ z } {}

    constexpr auto operator==(const xsFloat3& v) const noexcept
    {
        return x == v.x && y == v.y && z == v.z;
	}

    constexpr auto operator!=(const xsFloat3& v) const noexcept
    {
        return !(*this == v);
	}

    constexpr xsFloat3& operator+=(const xsFloat3& v) noexcept
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
	}

    constexpr xsFloat3& operator+=(float f) noexcept
    {
        x += f;
        y += f;
        z += f;
        return *this;
	}

    constexpr xsFloat3& operator-=(const xsFloat3& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
	}

    constexpr xsFloat3& operator-=(float f) noexcept
    {
        x -= f;
        y -= f;
        z -= f;
        return *this;
	}

    constexpr auto operator-(const xsFloat3& v) const noexcept
    {
        return xsFloat3{ x - v.x, y - v.y, z - v.z };
	}

    constexpr auto operator+(const xsFloat3& v) const noexcept
    {
        return xsFloat3{ x + v.x, y + v.y, z + v.z };
	}

    constexpr auto operator*(const xsFloat3& v) const noexcept
    {
        return xsFloat3{ x * v.x, y * v.y, z * v.z };
	}

    constexpr xsFloat3& operator/=(float div) noexcept
    {
        x /= div;
        y /= div;
        z /= div;
        return *this;
	}

    constexpr auto operator*(float mul) const noexcept
    {
        return xsFloat3{ x * mul, y * mul, z * mul };
	}

    constexpr auto operator-(float sub) const noexcept
    {
        return xsFloat3{ x - sub, y - sub, z - sub };
	}

    constexpr auto operator+(float add) const noexcept
    {
        return xsFloat3{ x + add, y + add, z + add };
	}

    auto length() const noexcept
    {
        return sqrtf(x * x + y * y + z * z);
	}

    constexpr auto squareLength() const noexcept
    {
        return x * x + y * y + z * z;
	}

    constexpr auto dotProduct(const xsFloat3& v) const noexcept
    {
        return x * v.x + y * v.y + z * v.z;
	}

    constexpr auto transform(const Matrix3x4& mat) const noexcept;
    constexpr auto transform(const Matrix4x4& mat) const noexcept;

    auto distTo(const xsFloat3& v) const noexcept
    {
        return (*this - v).length();
	}

    auto distToSqr(const xsFloat3& v) const noexcept
    {
        return (*this - v).squareLength();
	}
    float                                   x, y, z;
    //xsFloat3() { x = y = z  = 0.0f; }
    //xsFloat3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
};
struct xsFloat4
{
    float                                   x, y, z, w;
    xsFloat4() {  x = y = z = w = 0.0f;}
    xsFloat4(float _x, float _y, float _z, float _w) {  x = _x; y = _y; z = _z; w = _w; }
};
class Matrix4x4 {
    float mat[4][4];
public:
    constexpr auto operator[](int i) const noexcept {  return mat[i];}

    constexpr auto origin() const noexcept;
};
constexpr auto Matrix4x4::origin() const noexcept
{
    return xsFloat3{ mat[3][0], mat[3][1], mat[3][2] };
}

constexpr auto xsFloat3::transform(const Matrix4x4& mat) const noexcept
{
    return xsFloat3{ dotProduct({ mat[0][0], mat[1][0], mat[2][0] }) + mat[3][0],
                   dotProduct({ mat[0][1], mat[1][1], mat[2][1] }) + mat[3][1],
                   dotProduct({ mat[0][2], mat[1][2], mat[2][2] }) + mat[3][2] };

}

class Matrix3x4 {
    float mat[3][4];
public:
    constexpr auto operator[](int i) const noexcept { return mat[i]; }

    constexpr auto origin() const noexcept;
};
constexpr auto Matrix3x4::origin() const noexcept
{
    return xsFloat3{ mat[0][3], mat[1][3], mat[2][3] }; 
}
constexpr auto xsFloat3::transform(const Matrix3x4& mat) const noexcept
{
    return xsFloat3{ dotProduct({ mat[0][0], mat[0][1], mat[0][2] }) + mat[0][3],
                   dotProduct({ mat[1][0], mat[1][1], mat[1][2] }) + mat[1][3],
                   dotProduct({ mat[2][0], mat[2][1], mat[2][2] }) + mat[2][3] }; 
}


typedef unsigned short xsDrawIdx;//索引
struct xsDrawVert//顶点
{
    xsFloat2  pos;
    xsFloat2  uv;
    UINT32     col;
};
struct xsDrawGuide
{
    UINT          ElemCount;          // 4    // Number of indices (multiple of 3) to be rendered as triangles. Vertices are stored in the callee ImDrawList's vtx_buffer[] array, indices in idx_buffer[].
    xsFloat4     ClipRect;           // 4*4  // Clipping rectangle (x1, y1, x2, y2). Subtract ImDrawData->DisplayPos to get clipping rectangle in "viewport" coordinates
    void*         TextureID;              // 默认的 xsDrawGuide 永远都是文字的TextureID
    UINT          VtxOffset;          // 4    // Start offset in vertex buffer. Pre-1.71 or without ImGuiBackendFlags_RendererHasVtxOffset: always 0. With ImGuiBackendFlags_RendererHasVtxOffset: may be >0 to support meshes larger than 64K vertices with 16-bit indices.
    UINT          IdxOffset;          // 4    // Start offset in index buffer. Always equal to sum of ElemCount drawn so far.
    //ImDrawCallback  UserCallback;       // 4-8  // If != NULL, call the function instead of rendering the vertices. clip_rect and texture_id will be set normally.
    //void* UserCallbackData;   // 4-8  // The draw callback code can access this.

    xsDrawGuide() {  ElemCount = 0; TextureID = NULL; VtxOffset = IdxOffset = 0; /*UserCallback = NULL; UserCallbackData = NULL;*/}
};

namespace xs_Gui
{
    //函数集
    void             NewFrame();
    void             Render();

    xs_GuiContext*   CreateContext();
    void             DestroyContext(xs_GuiContext* ctx);

    xsGuiIO&         GetIO();
    char*            GetCurrentKeyButton();
    bool             GetBounceKeyButton(UINT key);
    bool             GetPressKeyButton(UINT key);
    bool             GetTriggerEventKeyButton(UINT key);

	DIMOUSESTATE2*            GetCurrentMouseButton();
	bool             GetBounceMouseButton(UINT key);
	bool             GetPressMouseButton(UINT key);

    void             Play_Control_Item();
    void             Play_Refresh();

    xs_DrawData*     GetDrawData();

    void             SetCurrentFont(ImFont* font);
    ImFont*          GetDefaultFont();
    xs_DrawList*     GetForegroundDrawList();
    xs_DrawList*     GetBackgroundDrawList();


    xsFloat4         col_Uint32ToFloat_RGBA(UINT32 in);
    xsFloat4         col_Uint32ToFloat_ARGB(UINT32 in);
    UINT32           col_FloatToUint32_RGBA(const xsFloat4& in);
    UINT32           col_FloatToUint32_ARGB(const xsFloat4& in);
    void             ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
    void             ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);

    bool char_to_value(const char a, char* b);
    int char_to_value(const char* a, char* b);
}


template<typename T>
struct xs_DataFrame
{
    //临时Data
    UINT Size;//size 当前size
    UINT Capacity;//容量 比当前size 大 用来预防 内存泄露
    T* Data;
    
    // Provide standard typedefs but we don't use them ourselves.


    //构造函数，析构函数
    inline xs_DataFrame()                                         {  Size = Capacity = 0; Data = NULL;}
    inline xs_DataFrame(const xs_DataFrame<T>& src)               {  Size = Capacity = 0; Data = NULL; operator=(src);}
    inline xs_DataFrame<T>& operator=(const xs_DataFrame<T>& src) {  clear(); resize(src.Size); memcpy(Data, src.Data, (size_t)Size * sizeof(T)); return *this; }
    inline ~xs_DataFrame()                                        {  if (Data) { stack_hidden_call_v(free,Data); Data=NULL; }}


    //内联函数
    //*************************************************************************************************
	inline T& operator[](int i) {  IM_ASSERT(i >= 0 && i < Size); T& axa = Data[i]; return axa; }
	inline const T& operator[](int i) const {  IM_ASSERT(i >= 0 && i < Size); T& axa = Data[i]; return axa; }

    //计算新内存_更大
	inline int          _grow_capacity(int sz) const {  int new_capacity = Capacity ? (Capacity + Capacity / 2) : 8; int axa = new_capacity > sz ? new_capacity : sz; return axa; }
    //申请更新更大内存

    inline void         reserve(int new_capacity)
    {
		
        if (new_capacity <= Capacity) 
            return;
        T* new_data = (T*)stack_hidden_call(malloc,(size_t)new_capacity * sizeof(T));
        memset(new_data, 0, (size_t)new_capacity * sizeof(T));
        if (Data) {
            memmove(new_data, Data, (size_t)Size * sizeof(T));

            T* Data_o = Data;
            Data = new_data;
            Capacity = new_capacity;

            stack_hidden_call_v(free, Data_o);
        }
        else {
            Data = new_data;
            Capacity = new_capacity;
        }
	}
    //刷新地址
    inline void         refresh()
    {
        if (Data)
        {
            T* new_data = (T*)stack_hidden_call(malloc, (size_t)Capacity * sizeof(T));
            memset(new_data, 0, (size_t)Capacity * sizeof(T));
            memmove(new_data, Data, (size_t)Size * sizeof(T));

            T* Data_o = Data;
            Data = new_data;
            stack_hidden_call_v(free, Data_o);
        }
    }
    //压进新参数

    inline void         push_back(const T& v) {
		
        if (Size == Capacity)
            reserve(_grow_capacity(Size + 1));
        if (&v)
            memmove(&Data[Size], &v, sizeof(v));
        else 
            memchr(&Data[Size], 0, sizeof(T)); 
        Size++; 
		
	}
    //清除遗留内存

    inline void         clear() {  if (Data) { Size = Capacity = 0;stack_hidden_call_v(free,Data); Data = NULL; }}
    //inline void         clear_delete() { for (int n = 0; n < Size; n++) IM_DELETE(Data[n]); clear(); }     // Important: never called automatically! always explicit.
    inline void         clear_destruct() {  for (int n = 0; n < Size; n++) Data[n].~T(); clear();}           // Important: never called automatically! always explicit.

    //调整size
    inline void         resize(int new_size) {  if (new_size > Capacity) reserve(_grow_capacity(new_size)); Size = new_size;}
    inline void         resize(int new_size, const T& v) {  if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v)); Size = new_size;}
    //1=空 0=有Data
	inline bool         empty() const {  bool axa= Size == 0; return axa; }
    //返回当前pos的Data
	inline T&           back() {  T& axa = Data[Size - 1]; return axa; }
	inline const T&     back() const {  T& axa= Data[Size - 1]; return axa;  }
    //Size-1
    inline void         pop_back() {  Size--;}
    //inline void         pop_back(const UINT sz) { if (Size == Capacity) reserve(_grow_capacity(Size + 1)); if (Size > sz) { for (UINT n = sz; n < Size;n++) memmove(&Data[n], &Data[n + 1],sizeof(T)); Size--; } }
    inline void         pop_back(const UINT sz) {  if (Size == Capacity) reserve(_grow_capacity(Size + 1)); if (Size == sz + 1) { Size--;memset(&Data[sz],0, sizeof(T)); return; } if (Size > sz) { Size--; memmove(&Data[sz], &Data[sz + 1], (Size - sz) * sizeof(T));  }}

	inline int          size_in_bytes() const {  int axa = Size * (int)sizeof(T); return axa; }

	inline T*           begin() {  T* axa = Data; return axa; }
	inline const T*     begin() const { T* axa= Data; return axa; }
	inline T*           end() {  T* axa= Data + Size; return axa; }
	inline const T*     end() const {  T* axa = Data + Size; return axa; }

};


struct xs_DrawList
{
    xs_DataFrame<xsDrawGuide>    GuideBuffer;
    xs_DataFrame<xsDrawIdx>    IdxBuffer;
    xs_DataFrame<xsDrawVert>    VtxBuffer;

    
    xs_DrawListSharedData*         _SharedData;//DrawListSharedData
    UINT                        _VtxCurrentIdx;//当前points_count 已用points_count
    xsDrawIdx*                 _xsDrawIdx;
    xsDrawVert*                 _xsDrawVert;
    xs_DataFrame<xsFloat2>      _Path; //临时传值
    xs_DataFrame<xsFloat4>      _ClipRectStack;     // ClipRectsize
    xs_DataFrame<void*>          _TextureIdStack;    // 第0个TextureID数组 对应 第0个GuideBufferData 以此类推


    xs_DrawList(xs_DrawListSharedData* shared_data) {  _SharedData = shared_data; _xsDrawIdx = NULL; _xsDrawVert = NULL;  _VtxCurrentIdx = 0;}
    ~xs_DrawList() {  _ClearFreeMemory();}

    // 框架函数

    void  _ClearFreeMemory();
    void  _ResetForNewFrame();

    void  Capacityretention(UINT idx_count, UINT vtx_count);//保留索引、顶点的Data容量
    void  PrimRectUV(const xsFloat2& a, const xsFloat2& c, const xsFloat2& uv_a, const xsFloat2& uv_c, UINT col);
    void  PushGuideBuffer();//"翻页绘制"
    void  PushTextureID(void* texture_id);
    void  PushClipRect(xsFloat2 cr_min, xsFloat2 cr_max, bool intersect_with_current_clip_rect = false);
    void  PushClipRectFullScreen();

    void  OnChangedTextureID();//更改更新 纹理ID
    void  OnChangedClipRect();//更改更新 剪辑矩形



    void  AddPolyline(const xsFloat2* points, UINT32 points_count, UINT32 col, bool closed, float thickness);
    void  AddPolyline(const xsFloat2* points, UINT32 points_count, UINT32 col, UINT32 col2, bool closed, float thickness);


    void  AddLine(const xsFloat2& p1, const xsFloat2& p2, UINT32 col, float thickness = 1.0f);
    void  AddLine_s(const xsFloat2& p1, const float& p2, UINT32 col, float thickness, float a1, float a2);
    void  AddConvexPolyFilled(const xsFloat2* points, const UINT32 points_count, UINT32 col);//添加凸多边形填充
    void  PrimRect(const xsFloat2& a, const xsFloat2& c, UINT32 col);//Prim矩形
    void  PrimRect(const xsFloat2& a, const xsFloat2& c, UINT32 col, UINT32 col2, UINT32 col3, UINT32 col4);

    xsFloat2  CalcText(const ImFont* font, float font_size, const char* text_begin);
    void  AddText(const ImFont* font, float font_size, xsFloat2 pos, UINT32 col, const char* text_begin = NULL, const char* text_end = NULL, bool centered=0, float wrap_width = 0.0f, const xsFloat4* cpu_fine_clip_rect = NULL);//添加Text
    void  AddCircle(const xsFloat2& center, float radius, UINT32 col, int num_segments = 20, float thickness = 1.0f);//添加圆
    void  AddCircleFilled(const xsFloat2& center, float radius, UINT32 col, int num_segments);//添加圆 填充
    void  AddRect(const xsFloat2& p_min, const xsFloat2& p_max, UINT32 col, float thickness);
    void  AddRect_s(const xsFloat2& p_min, const xsFloat2& p_max, UINT32 col, float thickness);
    void  AddRect_S(const xsFloat2& p_min, const xsFloat2& p_max, UINT32 col, float thickness);
    void  AddRectFilled(const xsFloat2& p_min, const xsFloat2& p_max, UINT32 col);
    void  AddRectFilled(const xsFloat2& p_min, const xsFloat2& p_max, UINT32 col, UINT32 col2, UINT32 col3, UINT32 col4);
    void            PathArcTo(const xsFloat2& center, float radius, float a_min, float a_max, int num_segments);//顶点圆线位
    inline    void  PathLineTo(const xsFloat2& pos) {  _Path.push_back(pos);}//顶点直线位
    inline    void  PathStroke(UINT32 col, bool closed, float thickness = 1.0f) {  AddPolyline(_Path.Data, _Path.Size, col, closed, thickness); _Path.Size = 0;}//顶点直线_写入
    inline    void  PathStroke(UINT32 col, UINT32 col2, bool closed, float thickness = 1.0f) { AddPolyline(_Path.Data, _Path.Size, col, col2, closed, thickness); _Path.Size = 0;}//顶点直线_写入
    inline    void  PathFillConvex(UINT32 col) {  AddConvexPolyFilled(_Path.Data, _Path.Size, col); _Path.Size = 0;}//顶点填充凸面



};

struct xs_DrawData
{
    bool               Valid;                   //是否valid
    xs_DrawList**      DrawLists;               // Array of ImDrawList* to render. The ImDrawList are owned by ImGuiContext and only pointed to from here.
    UINT               DrawListsCount;           // Number of ImDrawList* to render
    UINT               TotalIdxCount;               // 全部的绘制列表里面的索引 总和
    UINT               TotalVtxCount;               // 全部的绘制列表里面的顶点 总和
    xsFloat2          DisplayPos;               // 要渲染的视口的Left上角pos（== 要使用的正交投影矩阵的Left上角）
    xsFloat2          DisplaySize;               // 要渲染的视口size（== io.DisplaySize 用于主视口）（DisplayPos + DisplaySize == 要使用的正交投影矩阵的right下角）
    xsFloat2          FramebufferScale;       // Amount of pixels for each unit of DisplaySize. Based on io.DisplayFramebufferScale. Generally (1,1) on normal display, (2,2) on OSX with Retina display.

    // Functions
    xs_DrawData() {  Valid = false; Clear();}
    ~xs_DrawData() {  Clear();}
    void Clear() {  Valid = false; DrawLists = NULL; DrawListsCount = TotalIdxCount = TotalVtxCount = 0; DisplayPos = DisplaySize = FramebufferScale = xsFloat2(0.f, 0.f);} // The ImDrawList are owned by ImGuiContext!
    //void  DeIndexAllBuffers();                    // Helper to convert all buffers from indexed to non-indexed, in case you cannot render indexed. Note: this is slow and most likely a waste of resources. Always prefer indexed rendering!
    //void  ScaleClipRects(const xsFloat2& fb_scale); // Helper to scale the ClipRect field of each ImDrawCmd. Use if your final output buffer is at a different scale than Dear ImGui expects, or if there is a difference between your window resolution and framebuffer resolution.

};

struct xsGuiIO
{
    xsFloat2               DisplaySize;//窗口displaysize

    xsFontAtlas*             xsFont;
    float                    FontGlobalScale;                // = 1.0f           // Global scale all fonts
    bool                     FontAllowUserScaling;           // = false          // Allow user scaling text of individual window with CTRL+Wheel.
    ImFont*                  FontDefault;                    // = NULL           // Font to use on NewFrame(). Use NULL to uses Fonts->Fonts[0].

    char*                   KeyButton_Current;//键盘实时
    char*                   KeyButton_Later;//键盘延时
    UINT64                  KeyButton_Later_trigger;//键盘指定时间 触发
    UINT64                  KeyButton_Later_triggerInterval;//键盘 触发gap

	DIMOUSESTATE2*          Mouse_Current;
	DIMOUSESTATE2*          Mouse_Later;



    UINT64                  time_Current;//时间实时
    UINT64                  time_Frequency;//时间一秒频率

    float       DeltaTime;                      // = 1.0f/60.0f     // Time elapsed since last frame, in seconds.
    float       Framerate;                      // Application framerate estimate, in frame per second. Solely for convenience. Rolling average estimation based on io.DeltaTime over 120 frames.

    LPDIRECTINPUT8          g_pDirectInput;
    LPDIRECTINPUTDEVICE8    g_pKeyboardDevice;//键盘
	LPDIRECTINPUTDEVICE8    g_pMouseDevice;//鼠标




    xsGuiIO();
    ~xsGuiIO() {
		
        if (xsFont)stack_hidden_call_v(free,xsFont); if (KeyButton_Current)stack_hidden_call_v(free,KeyButton_Current); if (KeyButton_Later)stack_hidden_call_v(free,KeyButton_Later);
		
	}
};

struct ImFontConfig
{
    void* FontData;               //          // TTF/OTF data
    int             FontDataSize;           //          // TTF/OTF data size
    bool            FontDataOwnedByAtlas;   // true     // TTF/OTF data ownership taken by the container ImFontAtlas (will delete memory itself).
    int             FontNo;                 // 0        // Index of font within TTF/OTF file
    float           SizePixels;             //          // Size in pixels for rasterizer (more or less maps to the resulting font height).
    int             OversampleH;            // 2        // Rasterize at higher quality for sub-pixel positioning. Note the difference between 2 and 3 is minimal. You can reduce this to 1 for large glyphs save memory. Read https://github.com/nothings/stb/blob/master/tests/oversample/README.md for details.
    int             OversampleV;            // 1        // Rasterize at higher quality for sub-pixel positioning. This is not really useful as we don't use sub-pixel positions on the Y axis.
    bool            PixelSnapH;             // false    // Align every glyph to pixel boundary. Useful e.g. if you are merging a non-pixel aligned font with the default font. If enabled, you can set OversampleH/V to 1.
    xsFloat2          GlyphExtraSpacing;      // 0, 0     // Extra spacing (in pixels) between glyphs. Only X axis is supported for now.
    xsFloat2          GlyphOffset;            // 0, 0     // Offset all glyphs from this font input.
    const UINT16* GlyphRanges;            // NULL     // THE ARRAY DATA NEEDS TO PERSIST AS LONG AS THE FONT IS ALIVE. Pointer to a user-provided list of Unicode range (2 value per range, values are inclusive, zero-terminated list).
    float           GlyphMinAdvanceX;       // 0        // Minimum AdvanceX for glyphs, set Min to align font icons, set both Min/Max to enforce mono-space font
    float           GlyphMaxAdvanceX;       // FLT_MAX  // Maximum AdvanceX for glyphs
    bool            MergeMode;              // false    // Merge into previous ImFont, so you can combine multiple inputs font into one ImFont (e.g. ASCII font + icons + Japanese glyphs). You may want to use GlyphOffset.y when merge font of different heights.
    unsigned int    FontBuilderFlags;       // 0        // Settings for custom font builder. THIS IS BUILDER IMPLEMENTATION DEPENDENT. Leave as zero if unsure.
    float           RasterizerMultiply;     // 1.0f     // Linearly brighten (>1.0f) or darken (<1.0f) font output. Brightening small fonts may be a good workaround to make them more readable. This is a silly thing we may remove in the future.
    float           RasterizerDensity;      // 1.0f     // DPI scale for rasterization, not altering other font metrics: make it easy to swap between e.g. a 100% and a 400% fonts for a zooming display. IMPORTANT: If you increase this it is expected that you increase font scale accordingly, otherwise quality may look lowered.
    UINT16         EllipsisChar;           // -1       // Explicitly specify unicode codepoint of ellipsis character. When fonts are being merged first specified ellipsis will be used.

    // [Internal]
    char            Name[40];               // Name (strictly to ease debugging)
    ImFont* DstFont;

    ImFontConfig()
    {
		
        memset(this, 0, sizeof(*this));
        FontDataOwnedByAtlas = true;
        OversampleH = 2;
        OversampleV = 1;
        GlyphMaxAdvanceX = FLT_MAX;
        RasterizerMultiply = 1.0f;
        RasterizerDensity = 1.0f;
        EllipsisChar = (UINT16)-1; 
		//
    }
};

struct xsFontGlyph
{
    unsigned int    Colored : 1;        // Flag to indicate glyph is colored and should generally ignore tinting (make it usable with no shift on little-endian as this is used in loops)
    unsigned int    Visible : 1;        // Flag to indicate glyph has no visible pixels (e.g. space). Allow early out when rendering.
    unsigned int    Codepoint : 30;     // 0x0000..0xFFFF
    float           AdvanceX;           // 到下一个字符的距离（= 来自xsFont的Data + ImFontConfig::GlyphExtraSpacing.x 烘焙）
    float           X0, Y0, X1, Y1;     // 字形角
    float           U0, V0, U1, V1;     // 纹理坐标
};

enum ImFontAtlasFlags_
{
    ImFontAtlasFlags_None = 0,
    ImFontAtlasFlags_NoPowerOfTwoHeight = 1 << 0,   // Don't round the height to next power of two
    ImFontAtlasFlags_NoMouseCursors = 1 << 1,   // Don't build software mouse cursors into the atlas (save a little texture memory)
    ImFontAtlasFlags_NoBakedLines = 1 << 2,   // Don't build thick line textures into the atlas (save a little texture memory, allow support for point/nearest filtering). The AntiAliasedLinesUseTex features uses them, otherwise they will be rendered using polygons (more expensive for CPU/GPU).
};
struct ImFontAtlasCustomRect
{
    unsigned short  Width, Height;  // Input    // Desired rectangle dimension
    unsigned short  X, Y;           // Output   // Packed position in Atlas
    unsigned int    GlyphID;        // Input    // For custom font glyphs only (ID < 0x110000)
    float           GlyphAdvanceX;  // Input    // For custom font glyphs only: glyph xadvance
    xsFloat2          GlyphOffset;    // Input    // For custom font glyphs only: glyph display offset
    ImFont* Font;           // Input    // For custom font glyphs only: target font
    ImFontAtlasCustomRect() {  Width = Height = 0; X = Y = 0xFFFF; GlyphID = 0; GlyphAdvanceX = 0.0f; GlyphOffset = xsFloat2(0, 0); Font = NULL;}
	bool IsPacked() const {  bool axa = X != 0xFFFF; return axa; }
};


struct ImFont
{
    // Members: Hot ~20/24 bytes (for CalcTextSize)
    xs_DataFrame<float>             IndexAdvanceX;      // 12-16 // out //            // Sparse. Glyphs->AdvanceX in a directly indexable way (cache-friendly for CalcTextSize functions which only this this info, and are often bottleneck in large UI).
    float                       FallbackAdvanceX;   // 4     // out // = FallbackGlyph->AdvanceX
    float                       FontSize;           // 4     // in  //            // Height of characters/line, set during loading (don't change after loading)

    // Members: Hot ~28/40 bytes (for CalcTextSize + render loop)
    xs_DataFrame<UINT16>           IndexLookup;        // 12-16 // out //            // Sparse. Index glyphs by Unicode code-point.
    xs_DataFrame<xsFontGlyph>       Glyphs;             // 12-16 // out //            // All glyphs.
    const xsFontGlyph* FallbackGlyph;      // 4-8   // out // = FindGlyph(FontFallbackChar)

    // Members: Cold ~32/40 bytes
    xsFontAtlas*                ContainerAtlas;     // 4-8   // out //            // What we has been loaded into
    const ImFontConfig*         ConfigData;         // 4-8   // in  //            // Pointer within ContainerAtlas->ConfigData
    short                       ConfigDataCount;    // 2     // in  // ~ 1        // Number of ImFontConfig involved in creating this font. Bigger than 1 when merging multiple font sources into one ImFont.
    UINT16                      FallbackChar;       // 2     // out // = FFFD/'?' // Character used if a glyph isn't found.
    UINT16                      EllipsisChar;       // 2     // out // = '...'/'.'// Character used for ellipsis rendering.
    short                       EllipsisCharCount;  // 1     // out // 1 or 3
    float                       EllipsisWidth;      // 4     // out               // Width
    float                       EllipsisCharStep;   // 4     // out               // Step between characters when EllipsisCount > 0
    bool                        DirtyLookupTables;  // 1     // out //
    float                       Scale;              // 4     // in  // = 1.f      // Base font scale, multiplied by the per-window font scale which you can adjust with SetWindowFontScale()
    float                       Ascent, Descent;    // 4+4   // out //            // Ascent: distance from top to bottom of e.g. 'A' [0..FontSize]
    int                         MetricsTotalSurface;// 4     // out //            // Total surface in pixels to get an idea of the font rasterization/texture cost (not exact, we approximate the cost of padding between glyphs)
    unsigned char               Used4kPagesMap[(0xFFFF + 1) / 4096 / 8]; // 2 bytes if ImWchar=ImWchar16, 34 bytes if ImWchar==ImWchar32. Store 1-bit for each block of 4K codepoints that has one active glyph. This is mainly used to facilitate iterations across all used codepoints.

    // Methods
     ImFont();
     ~ImFont();
     const xsFontGlyph* FindGlyph(UINT16 c) const;
     const xsFontGlyph* FindGlyphNoFallback(UINT16 c) const;
	 float                       GetCharAdvance(UINT16 c) const {  float axa= ((int)c < IndexAdvanceX.Size) ? IndexAdvanceX[(int)c] : FallbackAdvanceX; return axa; }
	 bool                        IsLoaded() const {  bool axa = ContainerAtlas != NULL; return axa; }
	 const char* GetDebugName() const { const char* axa = ConfigData ? ConfigData->Name : VMProtectDecryptStringA("<unknown>"); return axa; }

    // 'max_width' stops rendering after a certain width (could be turned into a 2d size). FLT_MAX to disable.
    // 'wrap_width' enable automatic word-wrapping across multiple lines to fit into given width. 0.0f to disable.
     xsFloat2            CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end = NULL, const char** remaining = NULL) const; // utf8
     const char* CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const;
     void              RenderChar(xs_DrawList* draw_list, float size, const xsFloat2& pos, UINT col, UINT16 c) const;
     void RenderText(xs_DrawList* draw_list, float size, xsFloat2 pos, UINT32 col, const xsFloat4& clip_rect, const char* text_begin, const char* text_end, float wrap_width = 0.0f, bool cpu_fine_clip = false) const;

     
    // [Internal] Don't use!
     void              BuildLookupTable();
     void              ClearOutputData();
     void              GrowIndex(int new_size);
     void              AddGlyph(const ImFontConfig* src_cfg, UINT16 c, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x);
     void              AddRemapChar(UINT16 dst, UINT16 src, bool overwrite_dst = true); // Makes 'dst' character/glyph points to 'src' character/glyph. Currently needs to be called AFTER fonts have been built.
     void              SetGlyphVisible(UINT16 c, bool visible);
     bool              IsGlyphRangeUnused(unsigned int c_begin, unsigned int c_last);

};


struct xsFontAtlas
{
    int                   Flags;              // Build flags (see ImFontAtlasFlags_)
    void*                 TextureID;             // User data to refer to the texture once it has been uploaded to user's graphic systems. It is passed back to you during rendering via the ImDrawCmd structure.
    int                         TexDesiredWidth;    // Texture width desired by user before Build(). Must be a power-of-two. If have many glyphs your graphics API have texture size restrictions you may want to increase texture width to decrease height.
    int                         TexGlyphPadding;    // Padding between glyphs within texture in pixels. Defaults to 1. If your rendering method doesn't rely on bilinear filtering you may set this to 0 (will also need to set AntiAliasedLinesUseTex = false).
    bool                        Locked;             // Marked as Locked by ImGui::NewFrame() so attempt to modify the atlas will assert.
    void*                       UserData;           // Store your own atlas related user-data (if e.g. you have multiple font atlas).


    bool                  TexReady;           // Set when texture was built matching current font input
    bool                  TexPixelsUseColors; // Tell whether our texture data is known to use colors (rather than just alpha channel), in order to help backend select a format.
    unsigned char*        TexPixelsAlpha8;    // 1 component per pixel, each component is unsigned 8-bit. Total size = TexWidth * TexHeight
    unsigned int*         TexPixelsRGBA32;    // 4 component per pixel, each component is unsigned 8-bit. Total size = TexWidth * TexHeight * 4
    int                   TexWidth;           //512
    int                   TexHeight;          //64
    float                 TexUvScale[2];      //-		TexUvScale	{x=0.00195312 y=0.015625}	xsFloat2
    float                 TexUvWhitePixel[2]; //-		TexUvWhitePixel	{x=0.000976562 y=0.0078125}	xsFloat2
    xs_DataFrame<ImFont*>               Fonts;              // Hold all the fonts returned by AddFont*. Fonts[0] is the default font upon calling ImGui::NewFrame(), use ImGui::PushFont()/PopFont() to change the current font.
    xs_DataFrame<ImFontAtlasCustomRect> CustomRects;    // Rectangles for packing custom texture data into the atlas.
    xs_DataFrame<ImFontConfig>          ConfigData;         // Configuration data
    xsFloat4                            TexUvLines[IM_DRAWLIST_TEX_LINES_WIDTH_MAX + 1];  // UVs for baked anti-aliased lines



    int                         PackIdMouseCursors; // Custom texture rectangle ID for white pixel and mouse cursors
    int                         PackIdLines;        // Custom texture rectangle ID for baked anti-aliased lines


    xsFontAtlas();
    ~xsFontAtlas();
     void              ClearInputData();           // Clear input data (all ImFontConfig structures including sizes, TTF data, glyph ranges, etc.) = all the data used to build the texture and fonts.
     void              ClearTexData();             // Clear output texture data (CPU side). Saves RAM once the texture has been copied to graphics memory.
     void              ClearFonts();               // Clear output font data (glyphs storage, UV coordinates).
     void              Clear();                    // Clear all input and output.

    void GetFontData_RGBA_32(UCHAR** FontImage, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);

    
    
    void GetTexDataAsAlpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);
    bool Build();
    int  AddCustomRectRegular(int width, int height);

    const UINT16* GetGlyphRangesDefault();
    void  CalcCustomRectUV(const ImFontAtlasCustomRect* rect, xsFloat2* out_uv_min, xsFloat2* out_uv_max) const;


   
    
    const UINT16* GetGlyphRangesChineseFull();

	ImFontAtlasCustomRect* GetCustomRectByIndex(int index) {  ImFontAtlasCustomRect* axa= &CustomRects[index]; return axa; }
	ImFont* AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg_template, const UINT16* glyph_ranges);
	ImFont* AddFontFromMemoryTTF(void* ttf_data, int ttf_size, float size_pixels, const ImFontConfig* font_cfg_template, const UINT16* glyph_ranges);
    ImFont* AddFont(const ImFontConfig* font_cfg);
};



struct ImNewDummy {};
inline void* operator new(size_t, ImNewDummy, void* ptr) { return ptr; }
inline void  operator delete(void*, ImNewDummy, void*) {} // This is only required so we can use the symmetrical new()
#define XS_NEW(_TYPE)                       new(ImNewDummy(),stack_hidden_call( malloc,sizeof(_TYPE))) _TYPE
template<typename T> void XS_DELETE(T* p) {  if (p) { p->~T(); stack_hidden_call_v(free,(void*)p); } }


#ifdef IMGUI_USE_BGRA_PACKED_COLOR
#define XS_COL32_R_SHIFT    16
#define XS_COL32_G_SHIFT    8
#define XS_COL32_B_SHIFT    0
#define XS_COL32_A_SHIFT    24
#define XS_COL32_A_col位     0xFF000000
#else
#define XS_COL32_R_SHIFT    0
#define XS_COL32_G_SHIFT    8
#define XS_COL32_B_SHIFT    16
#define XS_COL32_A_SHIFT    24
#define XS_COL32_A_col位     0xFF000000
#endif
#define XS_COL32(R,G,B,A)    (((UINT32)(A)<<XS_COL32_A_SHIFT) | ((UINT32)(B)<<XS_COL32_B_SHIFT) | ((UINT32)(G)<<XS_COL32_G_SHIFT) | ((UINT32)(R)<<XS_COL32_R_SHIFT))
#define XS_COL32_WHITE       XS_COL32(255,255,255,255)  // Opaque white = 0xFFFFFFFF
#define XS_COL32_BLACK       XS_COL32(0,0,0,255)        // Opaque black
#define XS_COL32_BLACK_TRANS XS_COL32(0,0,0,0)          // Transparent black = 0x00000000
#define XS_COL32_ARGB(A,R,G,B)    (((UINT32)(A)<<XS_COL32_A_SHIFT) | ((UINT32)(B)<<XS_COL32_B_SHIFT) | ((UINT32)(G)<<XS_COL32_G_SHIFT) | ((UINT32)(R)<<XS_COL32_R_SHIFT))

#define XS_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*_ARR)))       // Size of a static C-style array. Don't use on pointers!


#define XS_UNICODE_CODEPOINT_INVALID 0xFFFD     // Invalid Unicode code point (standard value).
#ifdef XSGUI_USE_WCHAR32
#define XS_UNICODE_CODEPOINT_MAX     0x10FFFF   // Maximum Unicode code point supported by this build.
#else
#define XS_UNICODE_CODEPOINT_MAX     0xFFFF     // Maximum Unicode code point supported by this build.
#endif

#define XS_PI                           3.14159265358979323846f



static inline xsFloat2 operator*(const xsFloat2& lhs, const float rhs) {  xsFloat2 axa = xsFloat2(lhs.x * rhs, lhs.y * rhs); return axa; }
static inline xsFloat2 operator/(const xsFloat2& lhs, const float rhs) {  xsFloat2 axa = xsFloat2(lhs.x / rhs, lhs.y / rhs); return axa; }
static inline xsFloat2 operator+(const xsFloat2& lhs, const xsFloat2& rhs)     { xsFloat2 axa = xsFloat2(lhs.x + rhs.x, lhs.y + rhs.y); return axa;}
static inline xsFloat2 operator-(const xsFloat2& lhs, const xsFloat2& rhs)     { xsFloat2 axa = xsFloat2(lhs.x - rhs.x, lhs.y - rhs.y); return axa;}
static inline xsFloat2 operator*(const xsFloat2& lhs, const xsFloat2& rhs)     { xsFloat2 axa = xsFloat2(lhs.x * rhs.x, lhs.y * rhs.y); return axa;}
static inline xsFloat2 operator/(const xsFloat2& lhs, const xsFloat2& rhs)     { xsFloat2 axa = xsFloat2(lhs.x / rhs.x, lhs.y / rhs.y); return axa;}
static inline xsFloat2& operator*=(xsFloat2& lhs, const float rhs)              {  lhs.x *= rhs; lhs.y *= rhs; return lhs;}
static inline xsFloat2& operator/=(xsFloat2& lhs, const float rhs)              {  lhs.x /= rhs; lhs.y /= rhs; return lhs;}
static inline xsFloat2& operator+=(xsFloat2& lhs, const xsFloat2& rhs)         {  lhs.x += rhs.x; lhs.y += rhs.y; return lhs;}
static inline xsFloat2& operator-=(xsFloat2& lhs, const xsFloat2& rhs)         {  lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs;}
static inline xsFloat2& operator*=(xsFloat2& lhs, const xsFloat2& rhs)         {  lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs;}
static inline xsFloat2& operator/=(xsFloat2& lhs, const xsFloat2& rhs)         { lhs.x /= rhs.x; lhs.y /= rhs.y; return lhs;}
static inline xsFloat4 operator+(const xsFloat4& lhs, const xsFloat4& rhs)     { xsFloat4 axa = xsFloat4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); return axa;}
static inline xsFloat4 operator-(const xsFloat4& lhs, const xsFloat4& rhs)     { xsFloat4 axa = xsFloat4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); return axa;}
static inline xsFloat4 operator*(const xsFloat4& lhs, const xsFloat4& rhs)     { xsFloat4 axa = xsFloat4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); return axa;}
