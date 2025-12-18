#pragma once

#include <stdio.h>      // FILE*, sscanf
#include <stdlib.h>     // NULL, malloc, free, qsort, atoi, atof
#include <math.h>       // sqrtf, fabsf, fmodf, powf, floorf, ceilf, cosf, sinf
#include <limits.h>     // INT_MIN, INT_MAX

#define XS_F32_TO_INT8_SAT(_VAL)        ((int)(xsSaturate(_VAL) * 255.0f + 0.5f))               // Saturated, always output 0..255

struct xs_DrawListSharedData;

// Enforce cdecl calling convention for functions called by the standard library, in case compilation settings changed the default to e.g. __vectorcall
#ifdef _MSC_VER
#define IMGUI_CDECL __cdecl
#else
#define IMGUI_CDECL
#endif

// Helpers: Sorting
#ifndef ImQsort
static inline void      ImQsort(void* base, size_t count, size_t size_of_element, int(IMGUI_CDECL* compare_func)(void const*, void const*)) { if (count > 1) qsort(base, count, size_of_element, compare_func); }
#endif


static inline float  xsSaturate(float f) { return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; }




// - Wrapper for standard libs functions. (Note that imgui_demo.cpp does _not_ use them to keep the code easy to copy)

#define ImFabs(X)           fabsf(X)
#define ImSqrt(X)           sqrtf(X)
#define ImFmod(X, Y)        fmodf((X), (Y))
#define ImCos(X)            cosf(X)
#define ImSin(X)            sinf(X)
#define ImAcos(X)           acosf(X)
#define ImAtan2(Y, X)       atan2f((Y), (X))
#define ImAtof(STR)         atof(STR)
#define ImCeil(X)           ceilf(X)
static inline float  ImPow(float x, float y) { return powf(x, y); }          // DragBehaviorT/SliderBehaviorT uses ImPow with either float/double and need the precision
static inline double ImPow(double x, double y) { return pow(x, y); }
static inline float  ImLog(float x) { return logf(x); }             // DragBehaviorT/SliderBehaviorT uses ImLog with either float/double and need the precision
static inline double ImLog(double x) { return log(x); }
static inline int    ImAbs(int x) { return x < 0 ? -x : x; }
static inline float  ImAbs(float x) { return fabsf(x); }
static inline double ImAbs(double x) { return fabs(x); }
static inline float  ImSign(float x) { return (x < 0.0f) ? -1.0f : (x > 0.0f) ? 1.0f : 0.0f; } // Sign operator - returns -1, 0 or 1 based on sign of argument
static inline double ImSign(double x) { return (x < 0.0) ? -1.0 : (x > 0.0) ? 1.0 : 0.0; }
#ifdef IMGUI_ENABLE_SSE
static inline float  ImRsqrt(float x) { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x))); }
#else
static inline float  ImRsqrt(float x) { return 1.0f / sqrtf(x); }
#endif
static inline double ImRsqrt(double x) { return 1.0 / sqrt(x); }



template<typename T> static inline T ImMin(T lhs, T rhs) { return lhs < rhs ? lhs : rhs; }
template<typename T> static inline T ImMax(T lhs, T rhs) { return lhs >= rhs ? lhs : rhs; }
template<typename T> static inline T ImClamp(T v, T mn, T mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }
template<typename T> static inline T ImLerp(T a, T b, float t) { return (T)(a + (b - a) * t); }
template<typename T> static inline void ImSwap(T& a, T& b) { T tmp = a; a = b; b = tmp; }
template<typename T> static inline T ImAddClampOverflow(T a, T b, T mn, T mx) { if (b < 0 && (a < mn - b)) return mn; if (b > 0 && (a > mx - b)) return mx; return a + b; }
template<typename T> static inline T ImSubClampOverflow(T a, T b, T mn, T mx) { if (b > 0 && (a < mn + b)) return mn; if (b < 0 && (a > mx + b)) return mx; return a - b; }

static inline xsFloat2 ImMin(const xsFloat2& lhs, const xsFloat2& rhs) { return xsFloat2(lhs.x < rhs.x ? lhs.x : rhs.x, lhs.y < rhs.y ? lhs.y : rhs.y); }
static inline xsFloat2 ImMax(const xsFloat2& lhs, const xsFloat2& rhs) { return xsFloat2(lhs.x >= rhs.x ? lhs.x : rhs.x, lhs.y >= rhs.y ? lhs.y : rhs.y); }
static inline xsFloat2 ImClamp(const xsFloat2& v, const xsFloat2& mn, xsFloat2 mx) { return xsFloat2((v.x < mn.x) ? mn.x : (v.x > mx.x) ? mx.x : v.x, (v.y < mn.y) ? mn.y : (v.y > mx.y) ? mx.y : v.y); }
static inline xsFloat2 ImLerp(const xsFloat2& a, const xsFloat2& b, float t) { return xsFloat2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t); }
static inline xsFloat2 ImLerp(const xsFloat2& a, const xsFloat2& b, const xsFloat2& t) { return xsFloat2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y); }
static inline xsFloat4 ImLerp(const xsFloat4& a, const xsFloat4& b, float t) { return xsFloat4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t); }
static inline float  ImSaturate(float f) { return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; }
static inline float  ImLengthSqr(const xsFloat2& lhs) { return (lhs.x * lhs.x) + (lhs.y * lhs.y); }
static inline float  ImLengthSqr(const xsFloat4& lhs) { return (lhs.x * lhs.x) + (lhs.y * lhs.y) + (lhs.z * lhs.z) + (lhs.w * lhs.w); }
static inline float  ImInvLength(const xsFloat2& lhs, float fail_value) { float d = (lhs.x * lhs.x) + (lhs.y * lhs.y); if (d > 0.0f) return ImRsqrt(d); return fail_value; }
static inline float  ImTrunc(float f) { return (float)(int)(f); }
static inline xsFloat2 ImTrunc(const xsFloat2& v) { return xsFloat2((float)(int)(v.x), (float)(int)(v.y)); }
static inline float  ImFloor(float f) { return (float)((f >= 0 || (float)(int)f == f) ? (int)f : (int)f - 1); } // Decent replacement for floorf()
static inline xsFloat2 ImFloor(const xsFloat2& v) { return xsFloat2(ImFloor(v.x), ImFloor(v.y)); }
static inline int    ImModPositive(int a, int b) { return (a + b) % b; }
static inline float  ImDot(const xsFloat2& a, const xsFloat2& b) { return a.x * b.x + a.y * b.y; }
static inline xsFloat2 ImRotate(const xsFloat2& v, float cos_a, float sin_a) { return xsFloat2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a); }
static inline float  ImLinearSweep(float current, float target, float speed) { if (current < target) return ImMin(current + speed, target); if (current > target) return ImMax(current - speed, target); return current; }
static inline xsFloat2 ImMul(const xsFloat2& lhs, const xsFloat2& rhs) { return xsFloat2(lhs.x * rhs.x, lhs.y * rhs.y); }
static inline bool   ImIsFloatAboveGuaranteedIntegerPrecision(float f) { return f <= -16777216 || f >= 16777216; }
static inline float  ImExponentialMovingAverage(float avg, float sample, int n) { avg -= avg / n; avg += sample / n; return avg; }


static inline bool      ImIsPowerOfTwo(int v) { return v != 0 && (v & (v - 1)) == 0; }
static inline bool      ImIsPowerOfTwo(UINT64 v) { return v != 0 && (v & (v - 1)) == 0; }
static inline int       ImUpperPowerOfTwo(int v) { v--; v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16; v++; return v; }



static inline bool      xsCharIsBlankA(char c) { return c == ' ' || c == '\t'; }
static inline bool      xsCharIsBlankW(unsigned int c) { return c == ' ' || c == '\t' || c == 0x3000; }




// Helper: ImBitArray
#define         IM_BITARRAY_TESTBIT(_ARRAY, _N)                 ((_ARRAY[(_N) >> 5] & ((UINT)1 << ((_N) & 31))) != 0) // Macro version of ImBitArrayTestBit(): ensure args have side-effect or are costly!
#define         IM_BITARRAY_CLEARBIT(_ARRAY, _N)                ((_ARRAY[(_N) >> 5] &= ~((UINT)1 << ((_N) & 31))))    // Macro version of ImBitArrayClearBit(): ensure args have side-effect or are costly!
inline size_t   ImBitArrayGetStorageSizeInBytes(int bitcount) { return (size_t)((bitcount + 31) >> 5) << 2; }
inline void     ImBitArrayClearAllBits(UINT* arr, int bitcount) { memset(arr, 0, ImBitArrayGetStorageSizeInBytes(bitcount)); }
inline bool     ImBitArrayTestBit(const UINT* arr, int n) { UINT mask = (UINT)1 << (n & 31); return (arr[n >> 5] & mask) != 0; }
inline void     ImBitArrayClearBit(UINT* arr, int n) { UINT mask = (UINT)1 << (n & 31); arr[n >> 5] &= ~mask; }
inline void     ImBitArraySetBit(UINT* arr, int n) { UINT mask = (UINT)1 << (n & 31); arr[n >> 5] |= mask; }
inline void     ImBitArraySetBitRange(UINT* arr, int n, int n2) // Works on range [n..n2)
{
    n2--;
    while (n <= n2)
    {
        int a_mod = (n & 31);
        int b_mod = (n2 > (n | 31) ? 31 : (n2 & 31)) + 1;
        UINT mask = (UINT)(((UINT64)1 << b_mod) - 1) & ~(UINT)(((UINT64)1 << a_mod) - 1);
        arr[n >> 5] |= mask;
        n = (n + 32) & ~31;
    }
}

// Helper: ImBitVector
// Store 1-bit per value.
struct  ImBitVector
{
    xs_DataFrame<UINT> Storage;
    void            Create(int sz) { Storage.resize((sz + 31) >> 5); memset(Storage.Data, 0, (size_t)Storage.Size * sizeof(Storage.Data[0])); }
    void            Clear() { Storage.clear(); }
    bool            TestBit(int n) const { IM_ASSERT(n < (Storage.Size << 5)); return IM_BITARRAY_TESTBIT(Storage.Data, n); }
    void            SetBit(int n) { IM_ASSERT(n < (Storage.Size << 5)); ImBitArraySetBit(Storage.Data, n); }
    void            ClearBit(int n) { IM_ASSERT(n < (Storage.Size << 5)); ImBitArrayClearBit(Storage.Data, n); }
};


#define IM_ROUNDUP_TO_EVEN(_V)                                  ((((_V) + 1) / 2) * 2)
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN                     4
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX                     512
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(_RAD,_MAXERROR)    ImClamp(IM_ROUNDUP_TO_EVEN((int)ImCeil(XS_PI / ImAcos(1 - ImMin((_MAXERROR), (_RAD)) / (_RAD)))), IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN, IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX)

// Raw equation from IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC rewritten for 'r' and 'error'.
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(_N,_MAXERROR)    ((_MAXERROR) / (1 - ImCos(XS_PI / ImMax((float)(_N), XS_PI))))
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_ERROR(_N,_RAD)     ((1 - ImCos(XS_PI / ImMax((float)(_N), XS_PI))) / (_RAD))

// ImDrawList: Lookup table size for adaptive arc drawing, cover full circle.
#ifndef IM_DRAWLIST_ARCFAST_TABLE_SIZE
#define IM_DRAWLIST_ARCFAST_TABLE_SIZE                          48 // Number of samples in lookup table.
#endif
#define IM_DRAWLIST_ARCFAST_SAMPLE_MAX                          IM_DRAWLIST_ARCFAST_TABLE_SIZE // Sample index _PathArcToFastEx() for 360 angle.

// Data shared between all ImDrawList instances
// You may want to create your own instance of this if you want to use ImDrawList completely without ImGui. In that case, watch out for future changes to this structure.
struct xs_DrawListSharedData
{
    xsFloat2       TexUvWhitePixel;            // UV of white pixel in the atlas 空白纹理
    ImFont*         xsFont;                       // Current/default font (optional, for simplified AddText overload)
    float           FontSize;                   // Current/default font size (optional, for simplified AddText overload)
    float           CurveTessellationTol;       // Tessellation tolerance when using PathBezierCurveTo()
    float           CircleSegmentMaxError;      // Number of circle segments to use per pixel of radius for AddCircle() etc
    xsFloat4        ClipRectFullscreen;         // Value for PushClipRectFullscreen()
    int             InitialFlags;               // Initial flags at the beginning of the frame (it is possible to alter flags on a per-drawlist basis afterwards)

    // [Internal] Lookup tables
    xsFloat2          ArcFastVtx[IM_DRAWLIST_ARCFAST_TABLE_SIZE];  // FIXME: Bake rounded corners fill/borders in atlas
    float           ArcFastRadiusCutoff;                        // Cutoff radius after which arc drawing will fallback to slower PathArcTo()
    UINT8            CircleSegmentCounts[64];    // Precomputed segment count for given radius before we calculate it dynamically (to avoid calculation overhead)
    const xsFloat4* TexUvLines;                 // UV of anti-aliased lines in the atlas

    xs_DrawListSharedData()
    {
        memset(this, 0, sizeof(*this));
        for (int i = 0; i < XS_ARRAYSIZE(ArcFastVtx); i++)
        {
            const float a = ((float)i * 2 * XS_PI) / (float)XS_ARRAYSIZE(ArcFastVtx);
            ArcFastVtx[i] = xsFloat2(ImCos(a), ImSin(a));
        }
        ArcFastRadiusCutoff = IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(IM_DRAWLIST_ARCFAST_SAMPLE_MAX, CircleSegmentMaxError);
    }
    void SetCircleTessellationMaxError(float max_error);
};





//全局性 间接 接口
struct xs_DrawDataBuilder
{
    xs_DataFrame<xs_DrawList*>   Layers[2];    //2层画面接口 背景画面 前景画面  // Global layers for: regular, tooltip
    
    void Clear() { for (int n = 0; n < XS_ARRAYSIZE(Layers); n++) Layers[n].resize(0); }
    void ClearFreeMemory() { for (int n = 0; n < XS_ARRAYSIZE(Layers); n++) Layers[n].clear(); }
    //void FlattenIntoSingleLayer();
};

struct xs_GuiContext
{

    float                   FramerateSecPerFrame[120];          // Calculate estimate of framerate for user over the last 2 seconds.
    int                     FramerateSecPerFrameIdx;
    float                   FramerateSecPerFrameAccum;


    xsGuiIO                  IO;                          //用户Data 不是绘制的Data
    ImFont*                  Font;                               // (Shortcut) == FontStack.empty() ? IO.Font : FontStack.back()
    float                    FontSize;                           // (Shortcut) == FontBaseSize * g.CurrentWindow->FontWindowScale == window->FontSize(). Text height for current window.
    float                    FontBaseSize;                       // (Shortcut) == IO.FontGlobalScale * Font->Scale * Font->FontSize. Base text height.
    xs_DrawListSharedData       DrawListSharedData;


    //Render 结构体
	xs_DrawData              DrawData;                           // Main ImDrawData instance to pass render information to the user
    xs_DrawDataBuilder          DrawDataBuilder;
	xs_DrawList              BackgroundDrawList;                 // First draw list to be rendered.
	xs_DrawList              ForegroundDrawList;                 // Last draw list to be rendered. This is where we the render software mouse cursor (if io.MouseDrawCursor is set) and most debug overlays.

    xs_GuiContext() :BackgroundDrawList(&DrawListSharedData) ,ForegroundDrawList(&DrawListSharedData)
    {
        Font = NULL;
        FontSize = FontBaseSize = 0.0f;

        memset(&FramerateSecPerFrame, 0, sizeof(FramerateSecPerFrame));
        FramerateSecPerFrameIdx = 0;
        FramerateSecPerFrameAccum = 0.0f;


        IO.xsFont = XS_NEW(xsFontAtlas)();//(xmxsFont图集*)stack_hidden_call( malloc,sizeof(xmxsFont图集));
        //DrawListSharedData.xsFont = IO.xsFont;
    }




};