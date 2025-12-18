#include "xsgui.h"
#include "xsgui_internal.h"
#include "imstb_rectpack.h"
#include <math.h>       // sqrtf, fabsf, fmodf, powf, floorf, ceilf, cosf, sinf
#include <stdio.h>
#include <malloc.h>     // alloca
#include "VMProtectSDK.h"
#ifndef STB_RECT_PACK_IMPLEMENTATION                        // in case the user already have an implementation in the _same_ compilation unit (e.g. unity builds)
#ifndef IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION          // in case the user already have an implementation in another compilation unit
#define STBRP_STATIC
#define STBRP_ASSERT(x)     do { IM_ASSERT(x); } while (0)
#define STBRP_SORT          ImQsort
#define STB_RECT_PACK_IMPLEMENTATION
#endif
#ifdef IMGUI_STB_RECT_PACK_FILENAME
#include IMGUI_STB_RECT_PACK_FILENAME
#else
#include "imstb_rectpack.h"
#endif
#endif

#define STBTT_malloc(x,u)   ((void)(u), stack_hidden_call( malloc,x))
#define STBTT_free(x,u)     ((void)(u), stack_hidden_call_v(free,x))
#define STBTT_assert(x)     do { IM_ASSERT(x); } while(0)
#define STBTT_fmod(x,y)     ImFmod(x,y)
#define STBTT_sqrt(x)       ImSqrt(x)
#define STBTT_pow(x,y)      ImPow(x,y)
#define STBTT_fabs(x)       ImFabs(x)
#define STBTT_ifloor(x)     ((int)ImFloor(x))
#define STBTT_iceil(x)      ((int)ImCeil(x))
#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#include "imstb_truetype.h"

#define IM_NORMALIZE2F_OVER_ZERO(VX,VY)     do { float d2 = VX*VX + VY*VY; if (d2 > 0.0f) { float inv_len = 1.0f / sqrtf(d2); VX *= inv_len; VY *= inv_len; } } while (0)
#define IM_FIXNORMAL2F(VX,VY)               do { float d2 = VX*VX + VY*VY; if (d2 < 0.5f) d2 = 0.5f; float inv_lensq = 1.0f / d2; VX *= inv_lensq; VY *= inv_lensq; } while (0)


// Using macros because C++ is a terrible language, we want guaranteed inline, no code in header, and no overhead in Debug builds
#define GetCurrentClipRect()    (_ClipRectStack.Size ? _ClipRectStack.Data[_ClipRectStack.Size-1]  : _SharedData->ClipRectFullscreen)
#define GetCurrentTextureId()   (_TextureIdStack.Size ? _TextureIdStack.Data[_TextureIdStack.Size-1] : (void*)NULL)

int xsTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end);


//增加索引、顶点的Data容量
void xs_DrawList::Capacityretention(UINT idx_count, UINT vtx_count)
{
	
    // Large mesh support (when enabled)
    //IM_ASSERT_PARANOID(idx_count >= 0 && vtx_count >= 0);
    //if (sizeof(xsDrawIdx) == 2 && (_VtxCurrentIdx + vtx_count >= (1 << 16)) && (Flags & ImDrawListFlags_AllowVtxOffset))
    //{
    //    _VtxCurrentOffset = VtxBuffer.Size;
    //    _VtxCurrentIdx = 0;
    //    AddDrawCmd();
    //}

    xsDrawGuide* draw_guide = &GuideBuffer.Data[GuideBuffer.Size - 1];
    draw_guide->ElemCount += idx_count;

    UINT vtx_buffer_old_size = VtxBuffer.Size;
    VtxBuffer.resize(vtx_buffer_old_size + vtx_count);
    _xsDrawVert = VtxBuffer.Data + vtx_buffer_old_size;

    UINT idx_buffer_old_size = IdxBuffer.Size;
    IdxBuffer.resize(idx_buffer_old_size + idx_count);
    _xsDrawIdx = IdxBuffer.Data + idx_buffer_old_size;
	
}

void xs_DrawList::PrimRectUV(const xsFloat2& a, const xsFloat2& c, const xsFloat2& uv_a, const xsFloat2& uv_c, UINT col)
{
	
    xsFloat2 b(c.x, a.y), d(a.x, c.y), uv_b(uv_c.x, uv_a.y), uv_d(uv_a.x, uv_c.y);
    xsDrawIdx idx = (xsDrawIdx)_VtxCurrentIdx;
    _xsDrawIdx[0] = idx; _xsDrawIdx[1] = (xsDrawIdx)(idx + 1); _xsDrawIdx[2] = (xsDrawIdx)(idx + 2);
    _xsDrawIdx[3] = idx; _xsDrawIdx[4] = (xsDrawIdx)(idx + 2); _xsDrawIdx[5] = (xsDrawIdx)(idx + 3);
    _xsDrawVert[0].pos = a; _xsDrawVert[0].uv = uv_a; _xsDrawVert[0].col = col;
    _xsDrawVert[1].pos = b; _xsDrawVert[1].uv = uv_b; _xsDrawVert[1].col = col;
    _xsDrawVert[2].pos = c; _xsDrawVert[2].uv = uv_c; _xsDrawVert[2].col = col;
    _xsDrawVert[3].pos = d; _xsDrawVert[3].uv = uv_d; _xsDrawVert[3].col = col;
    _xsDrawVert += 4;
    _VtxCurrentIdx += 4;
    _xsDrawIdx += 6;
	
}

// TODO: 厚度抗锯齿线帽缺少它们的 AA 边缘。
// 我们避免在此处使用 xsFloat2 数学运算符，以将调试/非内联构建的成本降至最低。
void xs_DrawList::AddPolyline(const xsFloat2* points, const UINT32 points_count, UINT32 col, bool closed, float thickness)
{
	
    if (points_count < 2)
        return;

    const xsFloat2 opaque_uv = _SharedData->TexUvWhitePixel;
    int count = points_count;
    if (!closed)//一般是0
        count = points_count - 1;

    const bool thick_line = (thickness > 1.0f);//0
    if (1)//Flags & ImDrawListFlags_AntiAliasedLines
    {
        // Anti-aliased stroke
        // 抗锯齿笔画
        const float AA_SIZE = 1.0f;
        const unsigned int col_trans = col & ~0xFF000000;

        const int idx_count = thick_line ? count * 18 : count * 12;
        const int vtx_count = thick_line ? points_count * 4 : points_count * 3;
        Capacityretention(idx_count, vtx_count);//初级储备

        // Temporary buffer
        // 临时缓冲区
        xsFloat2* temp_normals = (xsFloat2*)_alloca(points_count * (thick_line ? 5 : 3) * sizeof(xsFloat2)); //-V630
        xsFloat2* temp_points = temp_normals + points_count;

        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
            float dx = points[i2].x - points[i1].x;
            float dy = points[i2].y - points[i1].y;
            IM_NORMALIZE2F_OVER_ZERO(dx, dy);
            temp_normals[i1].x = dy;
            temp_normals[i1].y = -dx;
        }
        if (!closed)
            temp_normals[points_count - 1] = temp_normals[points_count - 2];

        if (!thick_line)
        {
            if (!closed)
            {
                temp_points[0] = points[0] + temp_normals[0] * AA_SIZE;
                temp_points[1] = points[0] - temp_normals[0] * AA_SIZE;
                temp_points[(points_count - 1) * 2 + 0] = points[points_count - 1] + temp_normals[points_count - 1] * AA_SIZE;
                temp_points[(points_count - 1) * 2 + 1] = points[points_count - 1] - temp_normals[points_count - 1] * AA_SIZE;
            }

            // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
            // FIXME-OPT：合并不同的循环，可能删除临时缓冲区。
            unsigned int idx1 = _VtxCurrentIdx;
            for (int i1 = 0; i1 < count; i1++)
            {
                const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
                unsigned int idx2 = (i1 + 1) == points_count ? _VtxCurrentIdx : idx1 + 3;

                // Average normals
                // 平均法线
                float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                IM_FIXNORMAL2F(dm_x, dm_y);
                dm_x *= AA_SIZE;
                dm_y *= AA_SIZE;

                // Add temporary vertices
                // 添加临时顶点
                xsFloat2* out_vtx = &temp_points[i2 * 2];
                out_vtx[0].x = points[i2].x + dm_x;
                out_vtx[0].y = points[i2].y + dm_y;
                out_vtx[1].x = points[i2].x - dm_x;
                out_vtx[1].y = points[i2].y - dm_y;

                // Add indexes
                // 添加索引
                _xsDrawIdx[0] = (xsDrawIdx)(idx2 + 0); _xsDrawIdx[1] = (xsDrawIdx)(idx1 + 0); _xsDrawIdx[2] = (xsDrawIdx)(idx1 + 2);
                _xsDrawIdx[3] = (xsDrawIdx)(idx1 + 2); _xsDrawIdx[4] = (xsDrawIdx)(idx2 + 2); _xsDrawIdx[5] = (xsDrawIdx)(idx2 + 0);
                _xsDrawIdx[6] = (xsDrawIdx)(idx2 + 1); _xsDrawIdx[7] = (xsDrawIdx)(idx1 + 1); _xsDrawIdx[8] = (xsDrawIdx)(idx1 + 0);
                _xsDrawIdx[9] = (xsDrawIdx)(idx1 + 0); _xsDrawIdx[10] = (xsDrawIdx)(idx2 + 0); _xsDrawIdx[11] = (xsDrawIdx)(idx2 + 1);
                _xsDrawIdx += 12;

                idx1 = idx2;
            }

            // Add vertices
            // 添加顶点
            for (int i = 0; i < points_count; i++)
            {
                _xsDrawVert[0].pos = points[i];          _xsDrawVert[0].uv = opaque_uv; _xsDrawVert[0].col = col;
                _xsDrawVert[1].pos = temp_points[i * 2 + 0]; _xsDrawVert[1].uv = opaque_uv; _xsDrawVert[1].col = col_trans;
                _xsDrawVert[2].pos = temp_points[i * 2 + 1]; _xsDrawVert[2].uv = opaque_uv; _xsDrawVert[2].col = col_trans;
                _xsDrawVert += 3;
            }
        }
        else
        {
            const float half_inner_thickness = (thickness - AA_SIZE) * 0.5f;
            if (!closed)
            {
                const int points_last = points_count - 1;
                temp_points[0] = points[0] + temp_normals[0] * (half_inner_thickness + AA_SIZE);
                temp_points[1] = points[0] + temp_normals[0] * (half_inner_thickness);
                temp_points[2] = points[0] - temp_normals[0] * (half_inner_thickness);
                temp_points[3] = points[0] - temp_normals[0] * (half_inner_thickness + AA_SIZE);
                temp_points[points_last * 4 + 0] = points[points_last] + temp_normals[points_last] * (half_inner_thickness + AA_SIZE);
                temp_points[points_last * 4 + 1] = points[points_last] + temp_normals[points_last] * (half_inner_thickness);
                temp_points[points_last * 4 + 2] = points[points_last] - temp_normals[points_last] * (half_inner_thickness);
                temp_points[points_last * 4 + 3] = points[points_last] - temp_normals[points_last] * (half_inner_thickness + AA_SIZE);
            }

            // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
            // FIXME-OPT：合并不同的循环，可能删除临时缓冲区。
            unsigned int idx1 = _VtxCurrentIdx;
            for (int i1 = 0; i1 < count; i1++)
            {
                const int i2 = (i1 + 1) == points_count ? 0 : (i1 + 1); // i2 is the second point of the line segment
                                                                        // i2 是线段的第二个点
                const unsigned int idx2 = (i1 + 1) == points_count ? _VtxCurrentIdx : (idx1 + 4); // Vertex index for end of segment
                                                                                                  // 段结束的顶点索引

                // Average normals
                // 平均法线
                float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                IM_FIXNORMAL2F(dm_x, dm_y);
                float dm_out_x = dm_x * (half_inner_thickness + AA_SIZE);
                float dm_out_y = dm_y * (half_inner_thickness + AA_SIZE);
                float dm_in_x = dm_x * half_inner_thickness;
                float dm_in_y = dm_y * half_inner_thickness;

                // Add temporary vertices
                // 添加临时顶点
                xsFloat2* out_vtx = &temp_points[i2 * 4];
                out_vtx[0].x = points[i2].x + dm_out_x;
                out_vtx[0].y = points[i2].y + dm_out_y;
                out_vtx[1].x = points[i2].x + dm_in_x;
                out_vtx[1].y = points[i2].y + dm_in_y;
                out_vtx[2].x = points[i2].x - dm_in_x;
                out_vtx[2].y = points[i2].y - dm_in_y;
                out_vtx[3].x = points[i2].x - dm_out_x;
                out_vtx[3].y = points[i2].y - dm_out_y;

                // Add indexes
                // 添加索引
                _xsDrawIdx[0] = (xsDrawIdx)(idx2 + 1); _xsDrawIdx[1] = (xsDrawIdx)(idx1 + 1); _xsDrawIdx[2] = (xsDrawIdx)(idx1 + 2);
                _xsDrawIdx[3] = (xsDrawIdx)(idx1 + 2); _xsDrawIdx[4] = (xsDrawIdx)(idx2 + 2); _xsDrawIdx[5] = (xsDrawIdx)(idx2 + 1);
                _xsDrawIdx[6] = (xsDrawIdx)(idx2 + 1); _xsDrawIdx[7] = (xsDrawIdx)(idx1 + 1); _xsDrawIdx[8] = (xsDrawIdx)(idx1 + 0);
                _xsDrawIdx[9] = (xsDrawIdx)(idx1 + 0); _xsDrawIdx[10] = (xsDrawIdx)(idx2 + 0); _xsDrawIdx[11] = (xsDrawIdx)(idx2 + 1);
                _xsDrawIdx[12] = (xsDrawIdx)(idx2 + 2); _xsDrawIdx[13] = (xsDrawIdx)(idx1 + 2); _xsDrawIdx[14] = (xsDrawIdx)(idx1 + 3);
                _xsDrawIdx[15] = (xsDrawIdx)(idx1 + 3); _xsDrawIdx[16] = (xsDrawIdx)(idx2 + 3); _xsDrawIdx[17] = (xsDrawIdx)(idx2 + 2);
                _xsDrawIdx += 18;

                idx1 = idx2;
            }

            // Add vertices
            // 添加顶点
            for (int i = 0; i < points_count; i++)
            {
                _xsDrawVert[0].pos = temp_points[i * 4 + 0]; _xsDrawVert[0].uv = opaque_uv; _xsDrawVert[0].col = col_trans;
                _xsDrawVert[1].pos = temp_points[i * 4 + 1]; _xsDrawVert[1].uv = opaque_uv; _xsDrawVert[1].col = col;
                _xsDrawVert[2].pos = temp_points[i * 4 + 2]; _xsDrawVert[2].uv = opaque_uv; _xsDrawVert[2].col = col;
                _xsDrawVert[3].pos = temp_points[i * 4 + 3]; _xsDrawVert[3].uv = opaque_uv; _xsDrawVert[3].col = col_trans;
                _xsDrawVert += 4;
            }
        }
        _VtxCurrentIdx += (xsDrawIdx)vtx_count;
    }
    else
    {
        // Non Anti-aliased Stroke
        // 非抗锯齿描边
        const int idx_count = count * 6;
        const int vtx_count = count * 4;      // FIXME-OPT: Not sharing edges// FIXME-OPT: 不共享边
        Capacityretention(idx_count, vtx_count);

        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
            const xsFloat2& p1 = points[i1];
            const xsFloat2& p2 = points[i2];

            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            IM_NORMALIZE2F_OVER_ZERO(dx, dy);
            dx *= (thickness * 0.5f);
            dy *= (thickness * 0.5f);

            _xsDrawVert[0].pos.x = p1.x + dy; _xsDrawVert[0].pos.y = p1.y - dx; _xsDrawVert[0].uv = opaque_uv; _xsDrawVert[0].col = col;
            _xsDrawVert[1].pos.x = p2.x + dy; _xsDrawVert[1].pos.y = p2.y - dx; _xsDrawVert[1].uv = opaque_uv; _xsDrawVert[1].col = col;
            _xsDrawVert[2].pos.x = p2.x - dy; _xsDrawVert[2].pos.y = p2.y + dx; _xsDrawVert[2].uv = opaque_uv; _xsDrawVert[2].col = col;
            _xsDrawVert[3].pos.x = p1.x - dy; _xsDrawVert[3].pos.y = p1.y + dx; _xsDrawVert[3].uv = opaque_uv; _xsDrawVert[3].col = col;
            _xsDrawVert += 4;

            _xsDrawIdx[0] = (xsDrawIdx)(_VtxCurrentIdx); _xsDrawIdx[1] = (xsDrawIdx)(_VtxCurrentIdx + 1); _xsDrawIdx[2] = (xsDrawIdx)(_VtxCurrentIdx + 2);
            _xsDrawIdx[3] = (xsDrawIdx)(_VtxCurrentIdx); _xsDrawIdx[4] = (xsDrawIdx)(_VtxCurrentIdx + 2); _xsDrawIdx[5] = (xsDrawIdx)(_VtxCurrentIdx + 3);
            _xsDrawIdx += 6;
            _VtxCurrentIdx += 4;
        }
    }
	
}

void xs_DrawList::AddPolyline(const xsFloat2* points, const UINT32 points_count, UINT32 col, UINT32 col2, bool closed, float thickness)
{
	
    if (points_count < 2)
        return;

    const xsFloat2 opaque_uv = _SharedData->TexUvWhitePixel;
    int count = points_count;
    if (!closed)//一般是0
        count = points_count - 1;

    const bool thick_line = (thickness > 1.0f);//0
    /*if (抗锯齿)//Flags & ImDrawListFlags_AntiAliasedLines
    {
        // Anti-aliased stroke
        // 抗锯齿笔画
        const float AA_SIZE = 1.0f;
        const unsigned int col_trans = col & ~0xFF000000;

        const int idx_count = thick_line ? count * 18 : count * 12;
        const int vtx_count = thick_line ? points_count * 4 : points_count * 3;
        Capacityretention(idx_count, vtx_count);//初级储备

        // Temporary buffer
        // 临时缓冲区
        xsFloat2* temp_normals = (xsFloat2*)alloca(points_count * (thick_line ? 5 : 3) * sizeof(xsFloat2)); //-V630
        xsFloat2* temp_points = temp_normals + points_count;

        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
            float dx = points[i2].x - points[i1].x;
            float dy = points[i2].y - points[i1].y;
            IM_NORMALIZE2F_OVER_ZERO(dx, dy);
            temp_normals[i1].x = dy;
            temp_normals[i1].y = -dx;
        }
        if (!closed)
            temp_normals[points_count - 1] = temp_normals[points_count - 2];

        if (!thick_line)
        {
            if (!closed)
            {
                temp_points[0] = points[0] + temp_normals[0] * AA_SIZE;
                temp_points[1] = points[0] - temp_normals[0] * AA_SIZE;
                temp_points[(points_count - 1) * 2 + 0] = points[points_count - 1] + temp_normals[points_count - 1] * AA_SIZE;
                temp_points[(points_count - 1) * 2 + 1] = points[points_count - 1] - temp_normals[points_count - 1] * AA_SIZE;
            }

            // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
            // FIXME-OPT：合并不同的循环，可能删除临时缓冲区。
            unsigned int idx1 = _VtxCurrentIdx;
            for (int i1 = 0; i1 < count; i1++)
            {
                const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
                unsigned int idx2 = (i1 + 1) == points_count ? _VtxCurrentIdx : idx1 + 3;

                // Average normals
                // 平均法线
                float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                IM_FIXNORMAL2F(dm_x, dm_y);
                dm_x *= AA_SIZE;
                dm_y *= AA_SIZE;

                // Add temporary vertices
                // 添加临时顶点
                xsFloat2* out_vtx = &temp_points[i2 * 2];
                out_vtx[0].x = points[i2].x + dm_x;
                out_vtx[0].y = points[i2].y + dm_y;
                out_vtx[1].x = points[i2].x - dm_x;
                out_vtx[1].y = points[i2].y - dm_y;

                // Add indexes
                // 添加索引
                _xsDrawIdx[0] = (xsDrawIdx)(idx2 + 0); _xsDrawIdx[1] = (xsDrawIdx)(idx1 + 0); _xsDrawIdx[2] = (xsDrawIdx)(idx1 + 2);
                _xsDrawIdx[3] = (xsDrawIdx)(idx1 + 2); _xsDrawIdx[4] = (xsDrawIdx)(idx2 + 2); _xsDrawIdx[5] = (xsDrawIdx)(idx2 + 0);
                _xsDrawIdx[6] = (xsDrawIdx)(idx2 + 1); _xsDrawIdx[7] = (xsDrawIdx)(idx1 + 1); _xsDrawIdx[8] = (xsDrawIdx)(idx1 + 0);
                _xsDrawIdx[9] = (xsDrawIdx)(idx1 + 0); _xsDrawIdx[10] = (xsDrawIdx)(idx2 + 0); _xsDrawIdx[11] = (xsDrawIdx)(idx2 + 1);
                _xsDrawIdx += 12;

                idx1 = idx2;
            }

            // Add vertices
            // 添加顶点
            for (int i = 0; i < points_count; i++)
            {
                _xsDrawVert[0].pos = points[i];          _xsDrawVert[0].uv = opaque_uv; _xsDrawVert[0].col = col;
                _xsDrawVert[1].pos = temp_points[i * 2 + 0]; _xsDrawVert[1].uv = opaque_uv; _xsDrawVert[1].col = col_trans;
                _xsDrawVert[2].pos = temp_points[i * 2 + 1]; _xsDrawVert[2].uv = opaque_uv; _xsDrawVert[2].col = col_trans;
                _xsDrawVert += 3;
            }
        }
        else
        {
            const float half_inner_thickness = (thickness - AA_SIZE) * 0.5f;
            if (!closed)
            {
                const int points_last = points_count - 1;
                temp_points[0] = points[0] + temp_normals[0] * (half_inner_thickness + AA_SIZE);
                temp_points[1] = points[0] + temp_normals[0] * (half_inner_thickness);
                temp_points[2] = points[0] - temp_normals[0] * (half_inner_thickness);
                temp_points[3] = points[0] - temp_normals[0] * (half_inner_thickness + AA_SIZE);
                temp_points[points_last * 4 + 0] = points[points_last] + temp_normals[points_last] * (half_inner_thickness + AA_SIZE);
                temp_points[points_last * 4 + 1] = points[points_last] + temp_normals[points_last] * (half_inner_thickness);
                temp_points[points_last * 4 + 2] = points[points_last] - temp_normals[points_last] * (half_inner_thickness);
                temp_points[points_last * 4 + 3] = points[points_last] - temp_normals[points_last] * (half_inner_thickness + AA_SIZE);
            }

            // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
            // FIXME-OPT：合并不同的循环，可能删除临时缓冲区。
            unsigned int idx1 = _VtxCurrentIdx;
            for (int i1 = 0; i1 < count; i1++)
            {
                const int i2 = (i1 + 1) == points_count ? 0 : (i1 + 1); // i2 is the second point of the line segment
                                                                        // i2 是线段的第二个点
                const unsigned int idx2 = (i1 + 1) == points_count ? _VtxCurrentIdx : (idx1 + 4); // Vertex index for end of segment
                                                                                                  // 段结束的顶点索引

                // Average normals
                // 平均法线
                float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                IM_FIXNORMAL2F(dm_x, dm_y);
                float dm_out_x = dm_x * (half_inner_thickness + AA_SIZE);
                float dm_out_y = dm_y * (half_inner_thickness + AA_SIZE);
                float dm_in_x = dm_x * half_inner_thickness;
                float dm_in_y = dm_y * half_inner_thickness;

                // Add temporary vertices
                // 添加临时顶点
                xsFloat2* out_vtx = &temp_points[i2 * 4];
                out_vtx[0].x = points[i2].x + dm_out_x;
                out_vtx[0].y = points[i2].y + dm_out_y;
                out_vtx[1].x = points[i2].x + dm_in_x;
                out_vtx[1].y = points[i2].y + dm_in_y;
                out_vtx[2].x = points[i2].x - dm_in_x;
                out_vtx[2].y = points[i2].y - dm_in_y;
                out_vtx[3].x = points[i2].x - dm_out_x;
                out_vtx[3].y = points[i2].y - dm_out_y;

                // Add indexes
                // 添加索引
                _xsDrawIdx[0] = (xsDrawIdx)(idx2 + 1); _xsDrawIdx[1] = (xsDrawIdx)(idx1 + 1); _xsDrawIdx[2] = (xsDrawIdx)(idx1 + 2);
                _xsDrawIdx[3] = (xsDrawIdx)(idx1 + 2); _xsDrawIdx[4] = (xsDrawIdx)(idx2 + 2); _xsDrawIdx[5] = (xsDrawIdx)(idx2 + 1);
                _xsDrawIdx[6] = (xsDrawIdx)(idx2 + 1); _xsDrawIdx[7] = (xsDrawIdx)(idx1 + 1); _xsDrawIdx[8] = (xsDrawIdx)(idx1 + 0);
                _xsDrawIdx[9] = (xsDrawIdx)(idx1 + 0); _xsDrawIdx[10] = (xsDrawIdx)(idx2 + 0); _xsDrawIdx[11] = (xsDrawIdx)(idx2 + 1);
                _xsDrawIdx[12] = (xsDrawIdx)(idx2 + 2); _xsDrawIdx[13] = (xsDrawIdx)(idx1 + 2); _xsDrawIdx[14] = (xsDrawIdx)(idx1 + 3);
                _xsDrawIdx[15] = (xsDrawIdx)(idx1 + 3); _xsDrawIdx[16] = (xsDrawIdx)(idx2 + 3); _xsDrawIdx[17] = (xsDrawIdx)(idx2 + 2);
                _xsDrawIdx += 18;

                idx1 = idx2;
            }

            // Add vertices
            // 添加顶点
            for (int i = 0; i < points_count; i++)
            {
                _xsDrawVert[0].pos = temp_points[i * 4 + 0]; _xsDrawVert[0].uv = opaque_uv; _xsDrawVert[0].col = col_trans;
                _xsDrawVert[1].pos = temp_points[i * 4 + 1]; _xsDrawVert[1].uv = opaque_uv; _xsDrawVert[1].col = col;
                _xsDrawVert[2].pos = temp_points[i * 4 + 2]; _xsDrawVert[2].uv = opaque_uv; _xsDrawVert[2].col = col;
                _xsDrawVert[3].pos = temp_points[i * 4 + 3]; _xsDrawVert[3].uv = opaque_uv; _xsDrawVert[3].col = col_trans;
                _xsDrawVert += 4;
            }
        }
        _VtxCurrentIdx += (xsDrawIdx)vtx_count;
    }
    else*/
    {
        // Non Anti-aliased Stroke
        // 非抗锯齿描边
        const int idx_count = count * 6;
        const int vtx_count = count * 4;      // FIXME-OPT: Not sharing edges// FIXME-OPT: 不共享边
        Capacityretention(idx_count, vtx_count);

        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
            const xsFloat2& p1 = points[i1];
            const xsFloat2& p2 = points[i2];

            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            IM_NORMALIZE2F_OVER_ZERO(dx, dy);
            dx *= (thickness * 0.5f);
            dy *= (thickness * 0.5f);

            _xsDrawVert[0].pos.x = p1.x + dy; _xsDrawVert[0].pos.y = p1.y - dx; _xsDrawVert[0].uv = opaque_uv; _xsDrawVert[0].col = col2;
            _xsDrawVert[1].pos.x = p2.x + dy; _xsDrawVert[1].pos.y = p2.y - dx; _xsDrawVert[1].uv = opaque_uv; _xsDrawVert[1].col = col2;
            _xsDrawVert[2].pos.x = p2.x - dy; _xsDrawVert[2].pos.y = p2.y + dx; _xsDrawVert[2].uv = opaque_uv; _xsDrawVert[2].col = col;
            _xsDrawVert[3].pos.x = p1.x - dy; _xsDrawVert[3].pos.y = p1.y + dx; _xsDrawVert[3].uv = opaque_uv; _xsDrawVert[3].col = col;
            _xsDrawVert += 4;

            _xsDrawIdx[0] = (xsDrawIdx)(_VtxCurrentIdx); _xsDrawIdx[1] = (xsDrawIdx)(_VtxCurrentIdx + 1); _xsDrawIdx[2] = (xsDrawIdx)(_VtxCurrentIdx + 2);
            _xsDrawIdx[3] = (xsDrawIdx)(_VtxCurrentIdx); _xsDrawIdx[4] = (xsDrawIdx)(_VtxCurrentIdx + 2); _xsDrawIdx[5] = (xsDrawIdx)(_VtxCurrentIdx + 3);
            _xsDrawIdx += 6;
            _VtxCurrentIdx += 4;
        }
    }
	
}

// We intentionally avoid using xsFloat2 and its math operators here to reduce cost to a minimum for debug/non-inlined builds.
void xs_DrawList::AddConvexPolyFilled(const xsFloat2* points, const UINT32 points_count, UINT32 col)
{
	
    if (points_count < 3)
        return;

    const xsFloat2 uv = _SharedData->TexUvWhitePixel;

    /*if (Flags & ImDrawListFlags_AntiAliasedFill)
    {
        // Anti-aliased Fill
        const float AA_SIZE = 1.0f;
        const unsigned int col_trans = col & ~0xFF000000;
        const int idx_count = (points_count - 2) * 3 + points_count * 6;
        const int vtx_count = (points_count * 2);
        Capacityretention(idx_count, vtx_count);

        // Add indexes for fill
        unsigned int vtx_inner_idx = _VtxCurrentIdx;
        unsigned int vtx_outer_idx = _VtxCurrentIdx + 1;
        for (int i = 2; i < points_count; i++)
        {
            _IdxWritePtr[0] = (xsDrawIdx)(vtx_inner_idx); _IdxWritePtr[1] = (xsDrawIdx)(vtx_inner_idx + ((i - 1) << 1)); _IdxWritePtr[2] = (xsDrawIdx)(vtx_inner_idx + (i << 1));
            _IdxWritePtr += 3;
        }

        // Compute normals
        xsFloat2* temp_normals = (xsFloat2*)alloca(points_count * sizeof(xsFloat2)); //-V630
        for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            const xsFloat2& p0 = points[i0];
            const xsFloat2& p1 = points[i1];
            float dx = p1.x - p0.x;
            float dy = p1.y - p0.y;
            IM_NORMALIZE2F_OVER_ZERO(dx, dy);
            temp_normals[i0].x = dy;
            temp_normals[i0].y = -dx;
        }

        for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            // Average normals
            const xsFloat2& n0 = temp_normals[i0];
            const xsFloat2& n1 = temp_normals[i1];
            float dm_x = (n0.x + n1.x) * 0.5f;
            float dm_y = (n0.y + n1.y) * 0.5f;
            IM_FIXNORMAL2F(dm_x, dm_y);
            dm_x *= AA_SIZE * 0.5f;
            dm_y *= AA_SIZE * 0.5f;

            // Add vertices
            _VtxWritePtr[0].pos.x = (points[i1].x - dm_x); _VtxWritePtr[0].pos.y = (points[i1].y - dm_y); _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;        // Inner
            _VtxWritePtr[1].pos.x = (points[i1].x + dm_x); _VtxWritePtr[1].pos.y = (points[i1].y + dm_y); _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col_trans;  // Outer
            _VtxWritePtr += 2;

            // Add indexes for fringes
            _IdxWritePtr[0] = (xsDrawIdx)(vtx_inner_idx + (i1 << 1)); _IdxWritePtr[1] = (xsDrawIdx)(vtx_inner_idx + (i0 << 1)); _IdxWritePtr[2] = (xsDrawIdx)(vtx_outer_idx + (i0 << 1));
            _IdxWritePtr[3] = (xsDrawIdx)(vtx_outer_idx + (i0 << 1)); _IdxWritePtr[4] = (xsDrawIdx)(vtx_outer_idx + (i1 << 1)); _IdxWritePtr[5] = (xsDrawIdx)(vtx_inner_idx + (i1 << 1));
            _IdxWritePtr += 6;
        }
        _VtxCurrentIdx += (xsDrawIdx)vtx_count;
    }
    else*/
    {
        // Non Anti-aliased Fill
        const int idx_count = (points_count - 2) * 3;
        const int vtx_count = points_count;
        Capacityretention(idx_count, vtx_count);
        for (int i = 0; i < vtx_count; i++)
        {
            _xsDrawVert[0].pos = points[i]; _xsDrawVert[0].uv = uv; _xsDrawVert[0].col = col;
            _xsDrawVert++;
        }
        for (int i = 2; i < points_count; i++)
        {
            _xsDrawIdx[0] = (xsDrawIdx)(_VtxCurrentIdx); _xsDrawIdx[1] = (xsDrawIdx)(_VtxCurrentIdx + i - 1); _xsDrawIdx[2] = (xsDrawIdx)(_VtxCurrentIdx + i);
            _xsDrawIdx += 3;
        }
        _VtxCurrentIdx += (xsDrawIdx)vtx_count;
    }
	
}


void xs_DrawList::PrimRect(const xsFloat2& a, const xsFloat2& c, UINT32 col)
{
	

    xsFloat2 b(c.x, a.y), d(a.x, c.y), uv(_SharedData->TexUvWhitePixel);
    xsDrawIdx idx = (xsDrawIdx)_VtxCurrentIdx;
    _xsDrawIdx[0] = idx; _xsDrawIdx[1] = (xsDrawIdx)(idx + 1); _xsDrawIdx[2] = (xsDrawIdx)(idx + 2);
    _xsDrawIdx[3] = idx; _xsDrawIdx[4] = (xsDrawIdx)(idx + 2); _xsDrawIdx[5] = (xsDrawIdx)(idx + 3);
    _xsDrawVert[0].pos = a; _xsDrawVert[0].uv = uv; _xsDrawVert[0].col = col;
    _xsDrawVert[1].pos = b; _xsDrawVert[1].uv = uv; _xsDrawVert[1].col = col;
    _xsDrawVert[2].pos = c; _xsDrawVert[2].uv = uv; _xsDrawVert[2].col = col;
    _xsDrawVert[3].pos = d; _xsDrawVert[3].uv = uv; _xsDrawVert[3].col = col;
    _xsDrawVert += 4;
    _VtxCurrentIdx += 4;
    _xsDrawIdx += 6;
	
}

void xs_DrawList::PrimRect(const xsFloat2& a, const xsFloat2& c, UINT32 col, UINT32 col2, UINT32 col3, UINT32 col4)
{
	
    xsFloat2 b(c.x, a.y), d(a.x, c.y), uv(_SharedData->TexUvWhitePixel);
    xsDrawIdx idx = (xsDrawIdx)_VtxCurrentIdx;
    _xsDrawIdx[0] = idx; _xsDrawIdx[1] = (xsDrawIdx)(idx + 1); _xsDrawIdx[2] = (xsDrawIdx)(idx + 2);
    _xsDrawIdx[3] = idx; _xsDrawIdx[4] = (xsDrawIdx)(idx + 2); _xsDrawIdx[5] = (xsDrawIdx)(idx + 3);
    _xsDrawVert[0].pos = a; _xsDrawVert[0].uv = uv; _xsDrawVert[0].col = col;
    _xsDrawVert[1].pos = b; _xsDrawVert[1].uv = uv; _xsDrawVert[1].col = col2;
    _xsDrawVert[2].pos = c; _xsDrawVert[2].uv = uv; _xsDrawVert[2].col = col4;
    _xsDrawVert[3].pos = d; _xsDrawVert[3].uv = uv; _xsDrawVert[3].col = col3;
    _xsDrawVert += 4;
    _VtxCurrentIdx += 4;
    _xsDrawIdx += 6;
	
}

void xs_DrawList::_ClearFreeMemory()
{
	
    GuideBuffer.clear();
    IdxBuffer.clear();
    VtxBuffer.clear();

    _VtxCurrentIdx = 0;
    _xsDrawIdx = NULL;
    _xsDrawVert = NULL;


    _Path.clear();


	
}

void xs_DrawList::_ResetForNewFrame()
{
	
    GuideBuffer.resize(0);
    IdxBuffer.resize(0);
    VtxBuffer.resize(0);
    //Flags = _Data->InitialFlags;

    _VtxCurrentIdx = 0;
    _xsDrawVert = NULL;
    _xsDrawIdx = NULL;
    _ClipRectStack.resize(0);
    _TextureIdStack.resize(0);
    _Path.resize(0);
    //_Splitter.Clear();
    GuideBuffer.push_back(xsDrawGuide());
	
}

void xs_DrawList::PushGuideBuffer()
{
	
    xsDrawGuide a;
    a.IdxOffset = IdxBuffer.Size;
    a.VtxOffset = VtxBuffer.Size;
    GuideBuffer.push_back(a);
	
}

void xs_DrawList::PushTextureID(void* texture_id)
{
	
    _TextureIdStack.push_back(texture_id);
    OnChangedTextureID();
	
}

// Render-level scissoring. This is passed down to your render function but not used for CPU-side coarse clipping. Prefer using higher-level ImGui::PushClipRect() to affect logic (hit-testing and widget culling)
void xs_DrawList::PushClipRect(xsFloat2 cr_min, xsFloat2 cr_max, bool intersect_with_current_clip_rect)
{
	
    xsFloat4 cr(cr_min.x, cr_min.y, cr_max.x, cr_max.y);
    if (intersect_with_current_clip_rect && _ClipRectStack.Size)
    {
        xsFloat4 current = _ClipRectStack.Data[_ClipRectStack.Size - 1];
        if (cr.x < current.x) cr.x = current.x;
        if (cr.y < current.y) cr.y = current.y;
        if (cr.z > current.z) cr.z = current.z;
        if (cr.w > current.w) cr.w = current.w;
    }
    cr.z = ImMax(cr.x, cr.z);
    cr.w = ImMax(cr.y, cr.w);

    _ClipRectStack.push_back(cr);
    OnChangedClipRect();
	
}

void xs_DrawList::OnChangedTextureID()
{
	
    void* curr_texture_id = GetCurrentTextureId();
    xsDrawGuide* curr_cmd = &GuideBuffer.Data[GuideBuffer.Size - 1];


    curr_cmd->TextureID = curr_texture_id;
	
}

void xs_DrawList::OnChangedClipRect()
{
	
    // If current command is used with different settings we need to add a new command
    const xsFloat4 curr_clip_rect = GetCurrentClipRect();
    xsDrawGuide* curr_cmd = &GuideBuffer.Data[GuideBuffer.Size - 1];


    curr_cmd->ClipRect = curr_clip_rect;
	
}

void xs_DrawList::PushClipRectFullScreen()
{
	
    PushClipRect(xsFloat2(_SharedData->ClipRectFullscreen.x, _SharedData->ClipRectFullscreen.y), xsFloat2(_SharedData->ClipRectFullscreen.z, _SharedData->ClipRectFullscreen.w));
	
}






void    xsFontAtlas::ClearInputData()
{
	
    //IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    for (ImFontConfig& font_cfg : ConfigData)
        if (font_cfg.FontData && font_cfg.FontDataOwnedByAtlas)
        {
            stack_hidden_call_v(free,font_cfg.FontData);
            font_cfg.FontData = NULL;
        }

    // When clearing this we lose access to the font name and other information used to build the font.
    for (ImFont* font : Fonts)
        if (font->ConfigData >= ConfigData.Data && font->ConfigData < ConfigData.Data + ConfigData.Size)
        {
            font->ConfigData = NULL;
            font->ConfigDataCount = 0;
        }
    ConfigData.clear();
    CustomRects.clear();
    PackIdMouseCursors = PackIdLines = -1;
    // Important: we leave TexReady untouched
	
}

void    xsFontAtlas::ClearTexData()
{
	
    //IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    if (TexPixelsAlpha8)
        stack_hidden_call_v(free,TexPixelsAlpha8);
    if (TexPixelsRGBA32)
        stack_hidden_call_v(free,TexPixelsRGBA32);
    TexPixelsAlpha8 = NULL;
    TexPixelsRGBA32 = NULL;
    TexPixelsUseColors = false;
    // Important: we leave TexReady untouched
	
}

void    xsFontAtlas::ClearFonts()
{
	
    //IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    Fonts.clear_destruct();
    TexReady = false;
	
}

void    xsFontAtlas::Clear()
{
	
    ClearInputData();
    ClearTexData();
    ClearFonts();
	
}

xsFontAtlas::xsFontAtlas()
{
	
    memset(this, 0, sizeof(*this));
    TexGlyphPadding = 1;
    PackIdMouseCursors = PackIdLines = -1;
	
}

xsFontAtlas::~xsFontAtlas()
{
	
    Clear();
	
}

void xs_DrawList::AddLine(const xsFloat2& p1, const xsFloat2& p2, UINT32 col, float thickness)
{
	
    if ((col & XS_COL32_A_col位) == 0)
        return;
    PathLineTo(p1);
    PathLineTo(p2);
    PathStroke(col, 0, thickness); 
	
}

void xs_DrawList::AddLine_s(const xsFloat2& p1, const float& p2, UINT32 col, float thickness, float a1, float a2)
{
	
    if ((col & XS_COL32_A_col) == 0)
        return;
    float a3 = float(a1) / float(a2);
    a3 *= p2;
    xsFloat2 p1_ = p1 - xsFloat2(p2 / 2.0f, 0.0f);

    PathLineTo(p1_);
    PathLineTo(p1_ + xsFloat2(a3, 0.0f));
    PathStroke(col, 0, thickness);
	
}

void xs_DrawList::PathArcTo(const xsFloat2& center, float radius, float a_min,float a_max,int num_segments)
{
	
    if (radius == 0.0f)
    {
        _Path.push_back(center);
        return;
    }


    _Path.reserve(_Path.Size + num_segments+1);
    for (int i = 0; i <= num_segments; i++)
    {
        //这里计算段的弧度 （i/num_segments）分段
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        _Path.push_back(xsFloat2(center.x + cosf(a) * radius, center.y + sinf(a) * radius));
    }
	
}

void xs_DrawList::AddCircle(const xsFloat2& center, float radius, UINT32 col, int num_segments, float thickness)
{
	
    if ((col & XS_COL32_A_col位) == 0 || radius <= 0.0f)
        return;

    num_segments = (num_segments > 3 ? num_segments : 12) > 512 ? 512 : num_segments;


    // 因为我们正在填充一个封闭的形状，所以我们从段/点的计数中删除 1
    const float a_max = (XS_PI * 2.0f) *((float)num_segments - 1.0f) / (float)num_segments;

    PathArcTo(center, radius, 0.0f, a_max, num_segments - 1);
    PathStroke(col, 1, thickness);
	
}

void xs_DrawList::AddCircleFilled(const xsFloat2& center, float radius, UINT32 col, int num_segments)
{
	
    if ((col & XS_COL32_A_col位) == 0 || radius <= 0.0f)
        return;

    num_segments = (num_segments > 3 ? num_segments : 12) > 512 ? 512 : num_segments;


    // 因为我们正在填充一个封闭的形状，所以我们从段/点的计数中删除 1
    const float aa_max = (XS_PI * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;

    PathArcTo(center, radius, 0.0f, aa_max, num_segments - 1);
    PathFillConvex(col); 
	
}

xsFloat2  xs_DrawList::CalcText(const ImFont* font, float font_size, const char* text_begin)
{
	
    const char* text_end = NULL;
    if(text_end == NULL)
    {
        text_end = text_begin + strlen(text_begin);
    }
    if (text_begin == text_end)
        return xsFloat2(0,0);
    if (font == NULL)
        font = _SharedData->xsFont;
    if (font_size == 0.0f)
        font_size = font->FontSize;

	xsFloat2 axa= font->CalcTextSizeA(font_size, FLT_MAX, 0, text_begin, text_end, NULL);
	
	return axa;

}

void xs_DrawList::AddText(const ImFont* font, float font_size, xsFloat2 pos, UINT32 col, const char* text_begin, const char* text_end , bool centered, float wrap_width , const xsFloat4* cpu_fine_clip_rect )
{
	
    if ((col & XS_COL32_A_col位) == 0)
        return;

    bool Text_type = 0;
        if (text_end == NULL)
        {
            text_end = text_begin + strlen(text_begin);
            Text_type = 0;
        }
        if (text_begin == NULL)
        {
            text_begin = text_end + strlen(text_end);
            Text_type = 1;
        }
        if (text_begin == text_end)
            return;

    // Pull default font/size from the shared ImDrawListSharedData instance
    // 从共享的 ImDrawListSharedData 实例中提取默认xsFont/size
    if (font == NULL)
        font = _SharedData->xsFont;
    if (font_size == 0.0f)
        font_size = _SharedData->FontSize;

    //IM_ASSERT(font->ContainerAtlas->TexID == _TextureIdStack.back());  // Use high-level ImGui::PushFont() or low-level ImDrawList::PushTextureId() to change font.

    xsFloat4 clip_rect = _ClipRectStack.back();//这个是全屏的可绘制矩形范围

    if (cpu_fine_clip_rect)//cpu线程剪辑矩形
    {
        clip_rect.x = ImMax(clip_rect.x, cpu_fine_clip_rect->x);
        clip_rect.y = ImMax(clip_rect.y, cpu_fine_clip_rect->y);
        clip_rect.z = ImMin(clip_rect.z, cpu_fine_clip_rect->z);
        clip_rect.w = ImMin(clip_rect.w, cpu_fine_clip_rect->w);
    }
    if (centered)
        if (Text_type)
            pos.x -= font->CalcTextSizeA(font_size, FLT_MAX, wrap_width, text_end, text_begin, NULL).x / 2.0f;
        else
            pos.x -= font->CalcTextSizeA(font_size, FLT_MAX, wrap_width, text_begin, text_end, NULL).x / 2.0f;
    else
        pos.x -= font->CalcTextSizeA(font_size, FLT_MAX, wrap_width, text_end, text_begin, NULL).x;

    if (Text_type)
        font->RenderText(this, font_size, pos, col, clip_rect, text_end, text_begin, wrap_width, cpu_fine_clip_rect != NULL);
    else
        font->RenderText(this, font_size, pos, col, clip_rect, text_begin, text_end, wrap_width, cpu_fine_clip_rect != NULL);
	
}

void xs_DrawList::AddRect(const xsFloat2& p_min, const xsFloat2& p_max, UINT32 col, float thickness)
{
	
    if ((col & XS_COL32_A_col位) == 0)
        return;
    PathLineTo(p_min + xsFloat2(0.50f, 0.50f));
    PathLineTo(xsFloat2(p_max.x, p_min.y) );
    PathLineTo(p_max );
    PathLineTo(xsFloat2(p_min.x, p_max.y) );

    PathStroke(col, 1, thickness);
	
}

void xs_DrawList::AddRect_s(const xsFloat2& p_min, const xsFloat2& p_max, UINT32 col, float thickness)
{
	
    if ((col & XS_COL32_A_col位) == 0)
        return;
    PathLineTo(p_min + xsFloat2(0.50f, 0.50f));
    PathLineTo(xsFloat2(p_max.x, p_min.y) + xsFloat2(0.50f, 0.50f));
    PathLineTo(p_max + xsFloat2(0.50f, 0.50f));
    PathLineTo(xsFloat2(p_min.x, p_max.y) + xsFloat2(0.50f, 0.50f));
    UINT32 col2 = col & 0x00FFFFFF | 0x01000000;
    PathStroke(col, col2, 1, thickness);
	
}

void xs_DrawList::AddRect_S(const xsFloat2& a, const xsFloat2& c, UINT32 col, float thickness)
{
	
    if ((col & XS_COL32_A_col位) == 0)
        return;
    UINT32 col2 = col & 0x00FFFFFF | 0x01000000;
    UINT32 col3 = col & 0x00FFFFFF | 0x10000000;
    UINT32 col4 = col & 0x00FFFFFF | 0x30000000;
    //上
    {
        float thickness_2 = thickness * 0.5F;
        Capacityretention(72, 16);
        xsFloat2 a1(a.x- thickness, a.y- thickness), a2(a.x, a.y - thickness_2), a3(a.x - thickness_2, a.y ), a4(a.x, a.y)
            , b1(c.x+thickness, a.y- thickness), b2(c.x, a.y - thickness_2), b3(c.x + thickness_2, a.y ), b4(c.x, a.y)
            , c1(a.x - thickness, c.y + thickness), c2(a.x- thickness_2, c.y ), c3(a.x, c.y + thickness_2), c4(a.x, c.y)
            , d1(c.x + thickness, c.y + thickness), d2(c.x+ thickness_2, c.y ), d3(c.x, c.y + thickness_2), d4(c.x, c.y)

            , uv(_SharedData->TexUvWhitePixel);

        xsDrawIdx idx = (xsDrawIdx)_VtxCurrentIdx;
        _xsDrawIdx[0] = (xsDrawIdx)(idx    ); _xsDrawIdx[1] = (xsDrawIdx)(idx + 1); _xsDrawIdx[2] = (xsDrawIdx)(idx + 4);
        _xsDrawIdx[3] = (xsDrawIdx)(idx + 5); _xsDrawIdx[4] = (xsDrawIdx)(idx + 1); _xsDrawIdx[5] = (xsDrawIdx)(idx + 4);

        _xsDrawIdx[6] = (xsDrawIdx)(idx    ); _xsDrawIdx[7] = (xsDrawIdx)(idx + 2); _xsDrawIdx[8] = (xsDrawIdx)(idx + 8);
        _xsDrawIdx[9] = (xsDrawIdx)(idx + 9); _xsDrawIdx[10] = (xsDrawIdx)(idx + 2); _xsDrawIdx[11] = (xsDrawIdx)(idx + 8);

        _xsDrawIdx[12] = (xsDrawIdx)(idx + 4); _xsDrawIdx[13] = (xsDrawIdx)(idx + 6); _xsDrawIdx[14] = (xsDrawIdx)(idx + 12);
        _xsDrawIdx[15] = (xsDrawIdx)(idx + 13); _xsDrawIdx[16] = (xsDrawIdx)(idx + 6); _xsDrawIdx[17] = (xsDrawIdx)(idx + 12);

        _xsDrawIdx[18] = (xsDrawIdx)(idx + 10); _xsDrawIdx[19] = (xsDrawIdx)(idx + 14); _xsDrawIdx[20] = (xsDrawIdx)(idx + 8);
        _xsDrawIdx[21] = (xsDrawIdx)(idx + 12); _xsDrawIdx[22] = (xsDrawIdx)(idx + 14); _xsDrawIdx[23] = (xsDrawIdx)(idx + 8);

        _xsDrawIdx[24] = (xsDrawIdx)(idx     ); _xsDrawIdx[25] = (xsDrawIdx)(idx + 1); _xsDrawIdx[26] = (xsDrawIdx)(idx + 2);
        _xsDrawIdx[27] = (xsDrawIdx)(idx + 4); _xsDrawIdx[28] = (xsDrawIdx)(idx + 5); _xsDrawIdx[29] = (xsDrawIdx)(idx + 6);
        _xsDrawIdx[30] = (xsDrawIdx)(idx + 8); _xsDrawIdx[31] = (xsDrawIdx)(idx + 9); _xsDrawIdx[32] = (xsDrawIdx)(idx + 10);
        _xsDrawIdx[33] = (xsDrawIdx)(idx + 12); _xsDrawIdx[34] = (xsDrawIdx)(idx + 13); _xsDrawIdx[35] = (xsDrawIdx)(idx + 14);


        _xsDrawIdx[36] = (xsDrawIdx)(idx + 1); _xsDrawIdx[37] = (xsDrawIdx)(idx + 3); _xsDrawIdx[38] = (xsDrawIdx)(idx + 5);
        _xsDrawIdx[39] = (xsDrawIdx)(idx + 7); _xsDrawIdx[40] = (xsDrawIdx)(idx + 3); _xsDrawIdx[41] = (xsDrawIdx)(idx + 5);

        _xsDrawIdx[42] = (xsDrawIdx)(idx + 2); _xsDrawIdx[43] = (xsDrawIdx)(idx + 3); _xsDrawIdx[44] = (xsDrawIdx)(idx + 9);
        _xsDrawIdx[45] = (xsDrawIdx)(idx + 11); _xsDrawIdx[46] = (xsDrawIdx)(idx + 3); _xsDrawIdx[47] = (xsDrawIdx)(idx + 9);

        _xsDrawIdx[48] = (xsDrawIdx)(idx + 6); _xsDrawIdx[49] = (xsDrawIdx)(idx + 7); _xsDrawIdx[50] = (xsDrawIdx)(idx + 13);
        _xsDrawIdx[51] = (xsDrawIdx)(idx + 15); _xsDrawIdx[52] = (xsDrawIdx)(idx + 7); _xsDrawIdx[53] = (xsDrawIdx)(idx + 13);

        _xsDrawIdx[54] = (xsDrawIdx)(idx + 11); _xsDrawIdx[55] = (xsDrawIdx)(idx + 10); _xsDrawIdx[56] = (xsDrawIdx)(idx + 15);
        _xsDrawIdx[57] = (xsDrawIdx)(idx + 14); _xsDrawIdx[58] = (xsDrawIdx)(idx + 10); _xsDrawIdx[59] = (xsDrawIdx)(idx + 15);

        _xsDrawIdx[60] = (xsDrawIdx)(idx + 1); _xsDrawIdx[61] = (xsDrawIdx)(idx + 2); _xsDrawIdx[62] = (xsDrawIdx)(idx + 3);
        _xsDrawIdx[63] = (xsDrawIdx)(idx + 5); _xsDrawIdx[64] = (xsDrawIdx)(idx + 6); _xsDrawIdx[65] = (xsDrawIdx)(idx + 7);
        _xsDrawIdx[66] = (xsDrawIdx)(idx + 9); _xsDrawIdx[67] = (xsDrawIdx)(idx + 10); _xsDrawIdx[68] = (xsDrawIdx)(idx + 11);
        _xsDrawIdx[69] = (xsDrawIdx)(idx + 13); _xsDrawIdx[70] = (xsDrawIdx)(idx + 14); _xsDrawIdx[71] = (xsDrawIdx)(idx + 15);


        //_xsDrawIdx[72] = (xsDrawIdx)(idx + 16); _xsDrawIdx[73] = (xsDrawIdx)(idx + 17); _xsDrawIdx[74] = (xsDrawIdx)(idx + 16);
        //_xsDrawIdx[75] = (xsDrawIdx)(idx + 17); _xsDrawIdx[76] = (xsDrawIdx)(idx + 19); _xsDrawIdx[77] = (xsDrawIdx)(idx + 17);

        //_xsDrawIdx[78] = (xsDrawIdx)(idx + 19); _xsDrawIdx[43] = (xsDrawIdx)(idx + 18); _xsDrawIdx[79] = (xsDrawIdx)(idx + 19);
        //_xsDrawIdx[80] = (xsDrawIdx)(idx + 18); _xsDrawIdx[81] = (xsDrawIdx)(idx + 16); _xsDrawIdx[82] = (xsDrawIdx)(idx + 18);


        _xsDrawVert[0].pos = a1; _xsDrawVert[0].uv = uv; _xsDrawVert[0].col = col2;
        _xsDrawVert[1].pos = a2; _xsDrawVert[1].uv = uv; _xsDrawVert[1].col = col3;
        _xsDrawVert[2].pos = a3; _xsDrawVert[2].uv = uv; _xsDrawVert[2].col = col3;
        _xsDrawVert[3].pos = a4; _xsDrawVert[3].uv = uv; _xsDrawVert[3].col = col4;

        _xsDrawVert[4].pos = b1; _xsDrawVert[4].uv = uv; _xsDrawVert[4].col = col2;
        _xsDrawVert[5].pos = b2; _xsDrawVert[5].uv = uv; _xsDrawVert[5].col = col3;
        _xsDrawVert[6].pos = b3; _xsDrawVert[6].uv = uv; _xsDrawVert[6].col = col3;
        _xsDrawVert[7].pos = b4; _xsDrawVert[7].uv = uv; _xsDrawVert[7].col = col4;

        _xsDrawVert[8].pos = c1; _xsDrawVert[8].uv = uv; _xsDrawVert[8].col = col2;
        _xsDrawVert[9].pos = c2; _xsDrawVert[9].uv = uv; _xsDrawVert[9].col = col3;
        _xsDrawVert[10].pos = c3; _xsDrawVert[10].uv = uv; _xsDrawVert[10].col = col3;
        _xsDrawVert[11].pos = c4; _xsDrawVert[11].uv = uv; _xsDrawVert[11].col = col4;

        _xsDrawVert[12].pos = d1; _xsDrawVert[12].uv = uv; _xsDrawVert[12].col = col2;
        _xsDrawVert[13].pos = d2; _xsDrawVert[13].uv = uv; _xsDrawVert[13].col = col3;
        _xsDrawVert[14].pos = d3; _xsDrawVert[14].uv = uv; _xsDrawVert[14].col = col3;
        _xsDrawVert[15].pos = d4; _xsDrawVert[15].uv = uv; _xsDrawVert[15].col = col4;


        _xsDrawVert += 16;
        _VtxCurrentIdx += 16;
        _xsDrawIdx += 72;
        AddRect(a,c,col,1.0f);
    }

	
}


void xs_DrawList::AddRectFilled(const xsFloat2& p_min, const xsFloat2& p_max, UINT32 col)
{
	
    if ((col & XS_COL32_A_col位) == 0)
        return;

    Capacityretention(6, 4);
    PrimRect(p_min, p_max, col);
	
}

void xs_DrawList::AddRectFilled(const xsFloat2& p_min, const xsFloat2& p_max, UINT32 col, UINT32 col2, UINT32 col3, UINT32 col4)
{
	
    if ((col & XS_COL32_A_col位) == 0)
        return;

    Capacityretention(6, 4);
    PrimRect(p_min, p_max, col, col2, col3, col4);
	
}

void ImFont::RenderText(xs_DrawList* draw_list, float size, xsFloat2 pos, UINT32 col, const xsFloat4& clip_rect, const char* text_begin, const char* text_end, float wrap_width, bool cpu_fine_clip) const
{
	
    if (!text_end)
        text_end = text_begin + strlen(text_begin); // ImGui:: functions generally already provides a valid text_end, so this is merely to handle direct calls.

    // Align to be pixel perfect
    // 对齐到像素完美  
    pos.x = (float)(pos.x );
    pos.y = (float)(pos.y );
    float x = pos.x;
    float y = pos.y;
    if (y > clip_rect.w)//可视矩形范围
        return;

    const float scale = size / FontSize;//原本xsFontsize 跟 现需xsFontsize 综合之后的xsFontsize
    const float line_height = FontSize * scale; //线高
    const bool word_wrap_enabled = (wrap_width > 0.0f);//是否自行换行
    const char* word_wrap_eol = NULL;

    // Fast-forward to first visible line
    // 计算第一行可见的最后一个字符

    const char* s = text_begin;
    if (y + line_height < clip_rect.y && !word_wrap_enabled)//坐标y + xsFont高度 < 可绘制y坐标 && 自行换行=0  |  才会执行
        while (y + line_height < clip_rect.y && s < text_end)
        {
            s = (const char*)memchr(s, '\n', text_end - s);
            s = s ? s + 1 : text_end;
            y += line_height;
        }

    // For large text, scan for the last visible line in order to avoid over-reserving in the call to Capacityretention()
    // Note that very large horizontal line will still be affected by the issue (e.g. a one megabyte string buffer without a newline will likely crash atm)
    // 对于大Text，扫描最后一个可见行以避免在调用 Capacityretention() 时过度保留
    // 请注意，非常大的水平线仍会受到问题的影响（例如，没有换行符的 1 兆字节__char缓冲区可能会导致 atm 崩溃）
    if (text_end - s > 10000 && !word_wrap_enabled)
    {
        const char* s_end = s;
        float y_end = y;
        while (y_end < clip_rect.w && s_end < text_end)
        {
            s_end = (const char*)memchr(s_end, '\n', text_end - s_end);
            s_end = s_end ? s_end + 1 : text_end;
            y_end += line_height;
        }
        text_end = s_end;
    }
    if (s == text_end)
        return;

    // Reserve vertices for remaining worse case (over-reserving is useful and easily amortized)
    // 为剩余的最坏情况保留顶点（过度保留很有用且易于摊销）  1个字符需要1个矩形
    const int vtx_count_max = (int)(text_end - s) * 4;//__charsize*4   1个矩形 = 2个三角形 = 4个顶点
    const int idx_count_max = (int)(text_end - s) * 6;//__charsize*6   1个矩形需要6个索引 指引
    const int idx_expected_size = draw_list->IdxBuffer.Size + idx_count_max;//预期的索引数量 = 当前索引数量+已备索引数量
    draw_list->Capacityretention(idx_count_max, vtx_count_max);//申请报备需要的顶点空间跟索引空间

    xsDrawVert* vtx_write = draw_list->_xsDrawVert;//当前处在的顶点空间pos
    xsDrawIdx* idx_write = draw_list->_xsDrawIdx;//当前处在的索引空间pos
    unsigned int vtx_current_idx = draw_list->_VtxCurrentIdx;//当前ElemCount

    while (s < text_end)//当前__charpos < __char最后的pos
    {
        if (word_wrap_enabled)//是否 自行换行
        {
            // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
            // 计算我们可以渲染多远。 需要对__charData进行两次传递，但要保持代码简单且不会对本质上不常见的功能造成干扰。
            if (!word_wrap_eol)//这个参数默认是NULL
            {
                word_wrap_eol = CalcWordWrapPositionA(scale, s, text_end, wrap_width - (x - pos.x));//计算自动换行的pos
                if (word_wrap_eol == s) // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
                    // Wrap_width 太小，无法容纳任何东西。 强制display 1 个字符以最小化高度不连续性。
                    word_wrap_eol++;    // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
                // +1 可能不是 UTF-8 中的字符起点，但没关系，因为我们在下面使用 s >= word_wrap_eol
            }

            if (s >= word_wrap_eol)
            {
                x = pos.x;
                y += line_height;
                word_wrap_eol = NULL;
                // 包装跳过即将出现的空白
                // Wrapping skips upcoming blanks
                while (s < text_end)
                {
                    const char c = *s;
                    if (xsCharIsBlankA(c)) { s++; }
                    else if (c == '\n') { s++; break; }
                    else { break; }
                }
                continue;
            }
        }
        // 解码并提前源码
        // Decode and advance source
        unsigned int c = (unsigned int)*s;
        if (c < 0x80)
        {
            s += 1;
        }
        else
        {
            s += xsTextCharFromUtf8(&c, s, text_end);
            if (c == 0) // Malformed UTF-8?// UTF-8 格式错误？
                break;
        }

        if (c < 32)
        {
            if (c == '\n')
            {
                x = pos.x;
                y += line_height;
                if (y > clip_rect.w)
                    break; // break out of main loop// 跳出主循环
                continue;
            }
            if (c == '\r')
                continue;
        }

        const xsFontGlyph* glyph = FindGlyph((UINT16)c);//查找xsFont图pos
        if (glyph == NULL)
            continue;

        float char_width = glyph->AdvanceX * scale;
        if (glyph->Visible)
        {
            // We don't do a second finer clipping test on the Y axis as we've already skipped anything before clip_rect.y and exit once we pass clip_rect.w
            // 我们不会在 Y 轴上进行第二次更精细的裁剪测试，因为我们已经跳过了 clip_rect.y 之前的任何内容，并且一旦通过 clip_rect.w 就退出
            float x1 = x + glyph->X0 * scale;
            float x2 = x + glyph->X1 * scale;
            float y1 = y + glyph->Y0 * scale;
            float y2 = y + glyph->Y1 * scale;
            if (x1 <= clip_rect.z && x2 >= clip_rect.x)
            {
                // Render a character
                // 渲染一个字符
                float u1 = glyph->U0;
                float v1 = glyph->V0;
                float u2 = glyph->U1;
                float v2 = glyph->V1;

                // CPU side clipping used to fit text in their frame when the frame is too small. Only does clipping for axis aligned quads.
                // 当框架太小时，CPU 端裁剪用于将Text放入框架中。 仅对轴对齐的四边形进行裁剪。
                if (cpu_fine_clip)
                {
                    if (x1 < clip_rect.x)
                    {
                        u1 = u1 + (1.0f - (x2 - clip_rect.x) / (x2 - x1)) * (u2 - u1);
                        x1 = clip_rect.x;
                    }
                    if (y1 < clip_rect.y)
                    {
                        v1 = v1 + (1.0f - (y2 - clip_rect.y) / (y2 - y1)) * (v2 - v1);
                        y1 = clip_rect.y;
                    }
                    if (x2 > clip_rect.z)
                    {
                        u2 = u1 + ((clip_rect.z - x1) / (x2 - x1)) * (u2 - u1);
                        x2 = clip_rect.z;
                    }
                    if (y2 > clip_rect.w)
                    {
                        v2 = v1 + ((clip_rect.w - y1) / (y2 - y1)) * (v2 - v1);
                        y2 = clip_rect.w;
                    }
                    if (y1 >= y2)
                    {
                        x += char_width;
                        continue;
                    }
                }

                // We are NOT calling PrimRectUV() here because non-inlined causes too much overhead in a debug builds. Inlined here:
                // 我们没有在这里调用 PrimRectUV()，因为非内联会在调试构建中导致过多的开销。 在这里内联：
                {
                    idx_write[0] = (xsDrawIdx)(vtx_current_idx); idx_write[1] = (xsDrawIdx)(vtx_current_idx + 1); idx_write[2] = (xsDrawIdx)(vtx_current_idx + 2);
                    idx_write[3] = (xsDrawIdx)(vtx_current_idx); idx_write[4] = (xsDrawIdx)(vtx_current_idx + 2); idx_write[5] = (xsDrawIdx)(vtx_current_idx + 3);
                    vtx_write[0].pos.x = x1; vtx_write[0].pos.y = y1; vtx_write[0].col = col; vtx_write[0].uv.x = u1; vtx_write[0].uv.y = v1;
                    vtx_write[1].pos.x = x2; vtx_write[1].pos.y = y1; vtx_write[1].col = col; vtx_write[1].uv.x = u2; vtx_write[1].uv.y = v1;
                    vtx_write[2].pos.x = x2; vtx_write[2].pos.y = y2; vtx_write[2].col = col; vtx_write[2].uv.x = u2; vtx_write[2].uv.y = v2;
                    vtx_write[3].pos.x = x1; vtx_write[3].pos.y = y2; vtx_write[3].col = col; vtx_write[3].uv.x = u1; vtx_write[3].uv.y = v2;
                    vtx_write += 4;
                    vtx_current_idx += 4;
                    idx_write += 6;
                }
            }
        }
        x += char_width;
    }

    // Give back unused vertices (clipped ones, blanks) ~ this is essentially a PrimUnreserve() action.
    // 归还未使用的顶点（剪裁的顶点，空白）~这本质上是一个 PrimUnreserve() 动作。
    draw_list->VtxBuffer.Size = (UINT)(vtx_write - draw_list->VtxBuffer.Data); // Same as calling shrink()// 和调用shrink()一样
    draw_list->IdxBuffer.Size = (UINT)(idx_write - draw_list->IdxBuffer.Data);
    draw_list->GuideBuffer.Data[draw_list->GuideBuffer.Size - 1].ElemCount -= (idx_expected_size - draw_list->IdxBuffer.Size);
    draw_list->_xsDrawVert = vtx_write;
    draw_list->_xsDrawIdx = idx_write;
    draw_list->_VtxCurrentIdx = vtx_current_idx;
	
}



struct ImFontBuildSrcData
{
    stbtt_fontinfo      FontInfo;
    stbtt_pack_range    PackRange;          // Hold the list of codepoints to pack (essentially points to Codepoints.Data)
    stbrp_rect* Rects;              // Rectangle to pack. We first fill in their size and the packer will give us their position.
    stbtt_packedchar* PackedChars;        // Output glyphs
    const UINT16* SrcRanges;          // Ranges as requested by user (user is allowed to request too much, e.g. 0x0020..0xFFFF)
    int                 DstIndex;           // Index into atlas->Fonts[] and dst_tmp_array[]
    int                 GlyphsHighest;      // Highest requested codepoint
    int                 GlyphsCount;        // Glyph count (excluding missing glyphs and glyphs already set by an earlier source font)
    ImBitVector         GlyphsSet;          // Glyph bit map (random access, 1-bit per codepoint. This will be a maximum of 8KB)
    xs_DataFrame<int>       GlyphsList;         // Glyph codepoints list (flattened version of GlyphsSet)
};

// Temporary data for one destination ImFont* (multiple source fonts can be merged into one destination ImFont)
struct ImFontBuildDstData
{
    int                 SrcCount;           // Number of source fonts targeting this destination font.
    int                 GlyphsHighest;
    int                 GlyphsCount;
    ImBitVector         GlyphsSet;          // This is used to resolve collision when multiple sources are merged into a same destination font.
};


static void UnpackBitVectorToFlatIndexList(const ImBitVector* in, xs_DataFrame<int>* out)
{
	
    IM_ASSERT(sizeof(in->Storage.Data[0]) == sizeof(int));
    const UINT* it_begin = in->Storage.begin();
    const UINT* it_end = in->Storage.end();
    for (const UINT* it = it_begin; it < it_end; it++)
        if (UINT entries_32 = *it)
            for (UINT bit_n = 0; bit_n < 32; bit_n++)
                if (entries_32 & ((UINT)1 << bit_n))
                    out->push_back((int)(((it - it_begin) << 5) + bit_n));
	
}

static void ImFontAtlasBuildRenderDefaultTexData(xsFontAtlas* atlas)
{
	
    ImFontAtlasCustomRect* r = atlas->GetCustomRectByIndex(atlas->PackIdMouseCursors);
    IM_ASSERT(r->IsPacked());

    const int w = atlas->TexWidth;
    //if (!(atlas->Flags & ImFontAtlasFlags_NoMouseCursors))
    //{
    //    // Render/copy pixels
    //    IM_ASSERT(r->Width == FONT_ATLAS_DEFAULT_TEX_DATA_W * 2 + 1 && r->Height == FONT_ATLAS_DEFAULT_TEX_DATA_H);
    //    const int x_for_white = r->X;
    //    const int x_for_black = r->X + FONT_ATLAS_DEFAULT_TEX_DATA_W + 1;
    //    if (atlas->TexPixelsAlpha8 != NULL)
    //    {
    //        ImFontAtlasBuildRender8bppRectFromString(atlas, x_for_white, r->Y, FONT_ATLAS_DEFAULT_TEX_DATA_W, FONT_ATLAS_DEFAULT_TEX_DATA_H, FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS, '.', 0xFF);
    //        ImFontAtlasBuildRender8bppRectFromString(atlas, x_for_black, r->Y, FONT_ATLAS_DEFAULT_TEX_DATA_W, FONT_ATLAS_DEFAULT_TEX_DATA_H, FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS, 'X', 0xFF);
    //    }
    //    else
    //    {
    //        ImFontAtlasBuildRender32bppRectFromString(atlas, x_for_white, r->Y, FONT_ATLAS_DEFAULT_TEX_DATA_W, FONT_ATLAS_DEFAULT_TEX_DATA_H, FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS, '.', IM_COL32_WHITE);
    //        ImFontAtlasBuildRender32bppRectFromString(atlas, x_for_black, r->Y, FONT_ATLAS_DEFAULT_TEX_DATA_W, FONT_ATLAS_DEFAULT_TEX_DATA_H, FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS, 'X', IM_COL32_WHITE);
    //    }
    //}
    //else
    {
        // Render 4 white pixels
        IM_ASSERT(r->Width == 2 && r->Height == 2);
        const int offset = (int)r->X + (int)r->Y * w;
        if (atlas->TexPixelsAlpha8 != NULL)
        {
            atlas->TexPixelsAlpha8[offset] = atlas->TexPixelsAlpha8[offset + 1] = atlas->TexPixelsAlpha8[offset + w] = atlas->TexPixelsAlpha8[offset + w + 1] = 0xFF;
        }
        else
        {
            atlas->TexPixelsRGBA32[offset] = atlas->TexPixelsRGBA32[offset + 1] = atlas->TexPixelsRGBA32[offset + w] = atlas->TexPixelsRGBA32[offset + w + 1] = XS_COL32_WHITE;
        }
    }
    atlas->TexUvWhitePixel[0] = (r->X + 0.5f) * atlas->TexUvScale[0];
    atlas->TexUvWhitePixel[1] = (r->Y + 0.5f) * atlas->TexUvScale[1];
	
}

static void ImFontAtlasBuildRenderLinesTexData(xsFontAtlas* atlas)
{
	
    if (atlas->Flags & ImFontAtlasFlags_NoBakedLines)
        return;

    // This generates a triangular shape in the texture, with the various line widths stacked on top of each other to allow interpolation between them
    ImFontAtlasCustomRect* r = atlas->GetCustomRectByIndex(atlas->PackIdLines);
    IM_ASSERT(r->IsPacked());
    for (unsigned int n = 0; n < IM_DRAWLIST_TEX_LINES_WIDTH_MAX + 1; n++) // +1 because of the zero-width row
    {
        // Each line consists of at least two empty pixels at the ends, with a line of solid pixels in the middle
        unsigned int y = n;
        unsigned int line_width = n;
        unsigned int pad_left = (r->Width - line_width) / 2;
        unsigned int pad_right = r->Width - (pad_left + line_width);

        // Write each slice
        IM_ASSERT(pad_left + line_width + pad_right == r->Width && y < r->Height); // Make sure we're inside the texture bounds before we start writing pixels
        if (atlas->TexPixelsAlpha8 != NULL)
        {
            unsigned char* write_ptr = &atlas->TexPixelsAlpha8[r->X + ((r->Y + y) * atlas->TexWidth)];
            for (unsigned int i = 0; i < pad_left; i++)
                *(write_ptr + i) = 0x00;

            for (unsigned int i = 0; i < line_width; i++)
                *(write_ptr + pad_left + i) = 0xFF;

            for (unsigned int i = 0; i < pad_right; i++)
                *(write_ptr + pad_left + line_width + i) = 0x00;
        }
        else
        {
            unsigned int* write_ptr = &atlas->TexPixelsRGBA32[r->X + ((r->Y + y) * atlas->TexWidth)];
            for (unsigned int i = 0; i < pad_left; i++)
                *(write_ptr + i) = XS_COL32(255, 255, 255, 0);

            for (unsigned int i = 0; i < line_width; i++)
                *(write_ptr + pad_left + i) = XS_COL32_WHITE;

            for (unsigned int i = 0; i < pad_right; i++)
                *(write_ptr + pad_left + line_width + i) = XS_COL32(255, 255, 255, 0);
        }

        // Calculate UVs for this line
        xsFloat2 uv0 = xsFloat2((float)(r->X + pad_left - 1) * atlas->TexUvScale[0], (float)(r->Y + y) * atlas->TexUvScale[1]);
        xsFloat2 uv1 = xsFloat2((float)(r->X + pad_left + line_width + 1) * atlas->TexUvScale[0], (float)(r->Y + y + 1) * atlas->TexUvScale[1]);
        float half_v = (uv0.y + uv1.y) * 0.5f; // Calculate a constant V in the middle of the row to avoid sampling artifacts
        atlas->TexUvLines[n] = xsFloat4(uv0.x, half_v, uv1.x, half_v);
    }
	
}

void ImFontAtlasBuildInit(xsFontAtlas* atlas)
{
	
    // Round font size
    // - We started rounding in 1.90 WIP (18991) as our layout system currently doesn't support non-rounded font size well yet.
    // - Note that using io.FontGlobalScale or SetWindowFontScale(), with are legacy-ish, partially supported features, can still lead to unrounded sizes.
    // - We may support it better later and remove this rounding.
    //for (ImFontConfig& cfg : atlas->ConfigData)
    //    cfg.SizePixels = cfg.SizePixels;

    // Register texture region for mouse cursors or standard white pixels
    if (atlas->PackIdMouseCursors < 0)
    {
        //if (!(atlas->Flags & ImFontAtlasFlags_NoMouseCursors))
        //    atlas->PackIdMouseCursors = atlas->AddCustomRectRegular(FONT_ATLAS_DEFAULT_TEX_DATA_W * 2 + 1, FONT_ATLAS_DEFAULT_TEX_DATA_H);
        //else
            atlas->PackIdMouseCursors = atlas->AddCustomRectRegular(2, 2);
    }

    // Register texture region for thick lines
    // The +2 here is to give space for the end caps, whilst height +1 is to accommodate the fact we have a zero-width row
    if (atlas->PackIdLines < 0)
    {
        if (!(atlas->Flags & ImFontAtlasFlags_NoBakedLines))
            atlas->PackIdLines = atlas->AddCustomRectRegular(IM_DRAWLIST_TEX_LINES_WIDTH_MAX + 2, IM_DRAWLIST_TEX_LINES_WIDTH_MAX + 1);
    }
	
}

void ImFontAtlasBuildPackCustomRects(xsFontAtlas* atlas, void* stbrp_context_opaque)
{
	
    stbrp_context* pack_context = (stbrp_context*)stbrp_context_opaque;
    IM_ASSERT(pack_context != NULL);

    xs_DataFrame<ImFontAtlasCustomRect>& user_rects = atlas->CustomRects;
    IM_ASSERT(user_rects.Size >= 1); // We expect at least the default custom rects to be registered, else something went wrong.
#ifdef __GNUC__
    if (user_rects.Size < 1) { __builtin_unreachable(); } // Workaround for GCC bug if IM_ASSERT() is defined to conditionally throw (see #5343)
#endif

    xs_DataFrame<stbrp_rect> pack_rects;
    pack_rects.resize(user_rects.Size);
    memset(pack_rects.Data, 0, (size_t)pack_rects.size_in_bytes());
    for (int i = 0; i < user_rects.Size; i++)
    {
        pack_rects[i].w = user_rects[i].Width;
        pack_rects[i].h = user_rects[i].Height;
    }
    stbrp_pack_rects(pack_context, &pack_rects[0], pack_rects.Size);
    for (int i = 0; i < pack_rects.Size; i++)
        if (pack_rects[i].was_packed)
        {
            user_rects[i].X = (unsigned short)pack_rects[i].x;
            user_rects[i].Y = (unsigned short)pack_rects[i].y;
            IM_ASSERT(pack_rects[i].w == user_rects[i].Width && pack_rects[i].h == user_rects[i].Height);
            atlas->TexHeight = ImMax(atlas->TexHeight, pack_rects[i].y + pack_rects[i].h);
        }
	
}

void    ImFontAtlasBuildMultiplyCalcLookupTable(unsigned char out_table[256], float in_brighten_factor)
{
	
    for (unsigned int i = 0; i < 256; i++)
    {
        unsigned int value = (unsigned int)(i * in_brighten_factor);
        out_table[i] = value > 255 ? 255 : (value & 0xFF);
    }
	
}

void    ImFontAtlasBuildMultiplyRectAlpha8(const unsigned char table[256], unsigned char* pixels, int x, int y, int w, int h, int stride)
{
	
    //IM_ASSERT_PARANOID(w <= stride);
    unsigned char* data = pixels + x + y * stride;
    for (int j = h; j > 0; j--, data += stride - w)
        for (int i = w; i > 0; i--, data++)
            *data = table[*data];
	
}

void ImFontAtlasBuildSetupFont(xsFontAtlas* atlas, ImFont* font, ImFontConfig* font_config, float ascent, float descent)
{
	
    if (!font_config->MergeMode)
    {
        font->ClearOutputData();
        font->FontSize = font_config->SizePixels;
        IM_ASSERT(font->ConfigData == font_config);
        font->ContainerAtlas = atlas;
        font->Ascent = ascent;
        font->Descent = descent;
    }
	
}

void ImFontAtlasBuildFinish(xsFontAtlas* atlas)
{
	
    // Render into our custom data blocks
    IM_ASSERT(atlas->TexPixelsAlpha8 != NULL || atlas->TexPixelsRGBA32 != NULL);
    ImFontAtlasBuildRenderDefaultTexData(atlas);
    ImFontAtlasBuildRenderLinesTexData(atlas);

    // Register custom rectangle glyphs
    for (int i = 0; i < atlas->CustomRects.Size; i++)
    {
        const ImFontAtlasCustomRect* r = &atlas->CustomRects[i];
        if (r->Font == NULL || r->GlyphID == 0)
            continue;

        // Will ignore ImFontConfig settings: GlyphMinAdvanceX, GlyphMinAdvanceY, GlyphExtraSpacing, PixelSnapH
        IM_ASSERT(r->Font->ContainerAtlas == atlas);
        xsFloat2 uv0, uv1;
        atlas->CalcCustomRectUV(r, &uv0, &uv1);
        r->Font->AddGlyph(NULL, (UINT16)r->GlyphID, r->GlyphOffset.x, r->GlyphOffset.y, r->GlyphOffset.x + r->Width, r->GlyphOffset.y + r->Height, uv0.x, uv0.y, uv1.x, uv1.y, r->GlyphAdvanceX);
    }

    // Build all fonts lookup tables
    for (ImFont* font : atlas->Fonts)
        if (font->DirtyLookupTables)
            font->BuildLookupTable();

    atlas->TexReady  = true;
	
}

void ImFontAtlasUpdateConfigDataPointers(xsFontAtlas* atlas)
{
	
    for (ImFontConfig& font_cfg : atlas->ConfigData)
    {
        ImFont* font = font_cfg.DstFont;
        if (!font_cfg.MergeMode)
        {
            font->ConfigData = &font_cfg;
            font->ConfigDataCount = 0;
        }
        font->ConfigDataCount++;
    }
	
}

static bool ImFontAtlasBuildWithStbTruetype(xsFontAtlas* atlas)
{
	
    //IM_ASSERT(atlas->ConfigData.Size > 0);

    ImFontAtlasBuildInit(atlas);

    // Clear atlas
    atlas->TextureID = NULL;
    atlas->TexWidth = atlas->TexHeight = 0;
    atlas->TexUvScale[0] = 0.0f;
    atlas->TexUvScale[1] = 0.0f;
    atlas->TexUvWhitePixel[0] = 0.0f;
    atlas->TexUvWhitePixel[1] = 0.0f;
    atlas->ClearTexData();

    // Temporary storage for building
    xs_DataFrame<ImFontBuildSrcData> src_tmp_array;
    xs_DataFrame<ImFontBuildDstData> dst_tmp_array;
    src_tmp_array.resize(atlas->ConfigData.Size);
    dst_tmp_array.resize(atlas->Fonts.Size);
    memset(src_tmp_array.Data, 0, (size_t)src_tmp_array.size_in_bytes());
    memset(dst_tmp_array.Data, 0, (size_t)dst_tmp_array.size_in_bytes());

    // 1. Initialize font loading structure, check font data validity
    for (int src_i = 0; src_i < atlas->ConfigData.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        ImFontConfig& cfg = atlas->ConfigData[src_i];
        IM_ASSERT(cfg.DstFont && (!cfg.DstFont->IsLoaded() || cfg.DstFont->ContainerAtlas == atlas));

        // Find index from cfg.DstFont (we allow the user to set cfg.DstFont. Also it makes casual debugging nicer than when storing indices)
        src_tmp.DstIndex = -1;
        for (int output_i = 0; output_i < atlas->Fonts.Size && src_tmp.DstIndex == -1; output_i++)
            if (cfg.DstFont == atlas->Fonts[output_i])
                src_tmp.DstIndex = output_i;
        if (src_tmp.DstIndex == -1)
        {
            IM_ASSERT(src_tmp.DstIndex != -1); // cfg.DstFont not pointing within atlas->Fonts[] array?
            return false;
        }
        // Initialize helper structure for font loading and verify that the TTF/OTF data is correct
        const int font_offset = stbtt_GetFontOffsetForIndex((unsigned char*)cfg.FontData, cfg.FontNo);
        IM_ASSERT(font_offset >= 0 && "FontData is incorrect, or FontNo cannot be found.");
        if (!stbtt_InitFont(&src_tmp.FontInfo, (unsigned char*)cfg.FontData, font_offset))
        {
            IM_ASSERT(0 && "stbtt_InitFont(): failed to parse FontData. It is correct and complete? Check FontDataSize.");
            return false;
        }

        // Measure highest codepoints
        ImFontBuildDstData& dst_tmp = dst_tmp_array[src_tmp.DstIndex];
        src_tmp.SrcRanges = cfg.GlyphRanges ? cfg.GlyphRanges : atlas->GetGlyphRangesDefault();
        for (const UINT16* src_range = src_tmp.SrcRanges; src_range[0] && src_range[1]; src_range += 2)
        {
            // Check for valid range. This may also help detect *some* dangling pointers, because a common
            // user error is to setup ImFontConfig::GlyphRanges with a pointer to data that isn't persistent.
            IM_ASSERT(src_range[0] <= src_range[1]);
            src_tmp.GlyphsHighest = ImMax(src_tmp.GlyphsHighest, (int)src_range[1]);
        }
        dst_tmp.SrcCount++;
        dst_tmp.GlyphsHighest = ImMax(dst_tmp.GlyphsHighest, src_tmp.GlyphsHighest);
    }

    // 2. For every requested codepoint, check for their presence in the font data, and handle redundancy or overlaps between source fonts to avoid unused glyphs.
    int total_glyphs_count = 0;
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        ImFontBuildDstData& dst_tmp = dst_tmp_array[src_tmp.DstIndex];
        src_tmp.GlyphsSet.Create(src_tmp.GlyphsHighest + 1);
        if (dst_tmp.GlyphsSet.Storage.empty())
            dst_tmp.GlyphsSet.Create(dst_tmp.GlyphsHighest + 1);

        for (const UINT16* src_range = src_tmp.SrcRanges; src_range[0] && src_range[1]; src_range += 2)
            for (unsigned int codepoint = src_range[0]; codepoint <= src_range[1]; codepoint++)
            {
                if (dst_tmp.GlyphsSet.TestBit(codepoint))    // Don't overwrite existing glyphs. We could make this an option for MergeMode (e.g. MergeOverwrite==true)
                    continue;
                if (!stbtt_FindGlyphIndex(&src_tmp.FontInfo, codepoint))    // It is actually in the font?
                    continue;

                // Add to avail set/counters
                src_tmp.GlyphsCount++;
                dst_tmp.GlyphsCount++;
                src_tmp.GlyphsSet.SetBit(codepoint);
                dst_tmp.GlyphsSet.SetBit(codepoint);
                total_glyphs_count++;
            }
    }

    // 3. Unpack our bit map into a flat list (we now have all the Unicode points that we know are requested _and_ available _and_ not overlapping another)
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        src_tmp.GlyphsList.reserve(src_tmp.GlyphsCount);
        UnpackBitVectorToFlatIndexList(&src_tmp.GlyphsSet, &src_tmp.GlyphsList);
        src_tmp.GlyphsSet.Clear();
        IM_ASSERT(src_tmp.GlyphsList.Size == src_tmp.GlyphsCount);
    }
    for (int dst_i = 0; dst_i < dst_tmp_array.Size; dst_i++)
        dst_tmp_array[dst_i].GlyphsSet.Clear();
    dst_tmp_array.clear();

    // Allocate packing character data and flag packed characters buffer as non-packed (x0=y0=x1=y1=0)
    // (We technically don't need to zero-clear buf_rects, but let's do it for the sake of sanity)
    xs_DataFrame<stbrp_rect> buf_rects;
    xs_DataFrame<stbtt_packedchar> buf_packedchars;
    buf_rects.resize(total_glyphs_count);
    buf_packedchars.resize(total_glyphs_count);
    memset(buf_rects.Data, 0, (size_t)buf_rects.size_in_bytes());
    memset(buf_packedchars.Data, 0, (size_t)buf_packedchars.size_in_bytes());

    // 4. Gather glyphs sizes so we can pack them in our virtual canvas.
    int total_surface = 0;
    int buf_rects_out_n = 0;
    int buf_packedchars_out_n = 0;
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        if (src_tmp.GlyphsCount == 0)
            continue;

        src_tmp.Rects = &buf_rects[buf_rects_out_n];
        src_tmp.PackedChars = &buf_packedchars[buf_packedchars_out_n];
        buf_rects_out_n += src_tmp.GlyphsCount;
        buf_packedchars_out_n += src_tmp.GlyphsCount;

        // Convert our ranges in the format stb_truetype wants
        ImFontConfig& cfg = atlas->ConfigData[src_i];
        src_tmp.PackRange.font_size = cfg.SizePixels * cfg.RasterizerDensity;
        src_tmp.PackRange.first_unicode_codepoint_in_range = 0;
        src_tmp.PackRange.array_of_unicode_codepoints = src_tmp.GlyphsList.Data;
        src_tmp.PackRange.num_chars = src_tmp.GlyphsList.Size;
        src_tmp.PackRange.chardata_for_range = src_tmp.PackedChars;
        src_tmp.PackRange.h_oversample = (unsigned char)cfg.OversampleH;
        src_tmp.PackRange.v_oversample = (unsigned char)cfg.OversampleV;

        // Gather the sizes of all rectangles we will need to pack (this loop is based on stbtt_PackFontRangesGatherRects)
        const float scale = (cfg.SizePixels > 0.0f) ? stbtt_ScaleForPixelHeight(&src_tmp.FontInfo, cfg.SizePixels * cfg.RasterizerDensity) : stbtt_ScaleForMappingEmToPixels(&src_tmp.FontInfo, -cfg.SizePixels * cfg.RasterizerDensity);
        const int padding = atlas->TexGlyphPadding;
        for (int glyph_i = 0; glyph_i < src_tmp.GlyphsList.Size; glyph_i++)
        {
            int x0, y0, x1, y1;
            const int glyph_index_in_font = stbtt_FindGlyphIndex(&src_tmp.FontInfo, src_tmp.GlyphsList[glyph_i]);
            IM_ASSERT(glyph_index_in_font != 0);
            stbtt_GetGlyphBitmapBoxSubpixel(&src_tmp.FontInfo, glyph_index_in_font, scale * cfg.OversampleH, scale * cfg.OversampleV, 0, 0, &x0, &y0, &x1, &y1);
            src_tmp.Rects[glyph_i].w = (stbrp_coord)(x1 - x0 + padding + cfg.OversampleH - 1);
            src_tmp.Rects[glyph_i].h = (stbrp_coord)(y1 - y0 + padding + cfg.OversampleV - 1);
            total_surface += src_tmp.Rects[glyph_i].w * src_tmp.Rects[glyph_i].h;
        }
    }

    // We need a width for the skyline algorithm, any width!
    // The exact width doesn't really matter much, but some API/GPU have texture size limitations and increasing width can decrease height.
    // User can override TexDesiredWidth and TexGlyphPadding if they wish, otherwise we use a simple heuristic to select the width based on expected surface.
    const int surface_sqrt = (int)ImSqrt((float)total_surface) + 1;
    atlas->TexHeight = 0;
    if (atlas->TexDesiredWidth > 0)
        atlas->TexWidth = atlas->TexDesiredWidth;
    else
        atlas->TexWidth = (surface_sqrt >= 4096 * 0.7f) ? 4096 : (surface_sqrt >= 2048 * 0.7f) ? 2048 : (surface_sqrt >= 1024 * 0.7f) ? 1024 : 512;

    // 5. Start packing
    // Pack our extra data rectangles first, so it will be on the upper-left corner of our texture (UV will have small values).
    const int TEX_HEIGHT_MAX = 1024 * 32;
    stbtt_pack_context spc = {};
    stbtt_PackBegin(&spc, NULL, atlas->TexWidth, TEX_HEIGHT_MAX, 0, atlas->TexGlyphPadding, NULL);
    ImFontAtlasBuildPackCustomRects(atlas, spc.pack_info);

    // 6. Pack each source font. No rendering yet, we are working with rectangles in an infinitely tall texture at this point.
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        if (src_tmp.GlyphsCount == 0)
            continue;

        stbrp_pack_rects((stbrp_context*)spc.pack_info, src_tmp.Rects, src_tmp.GlyphsCount);

        // Extend texture height and mark missing glyphs as non-packed so we won't render them.
        // FIXME: We are not handling packing failure here (would happen if we got off TEX_HEIGHT_MAX or if a single if larger than TexWidth?)
        for (int glyph_i = 0; glyph_i < src_tmp.GlyphsCount; glyph_i++)
            if (src_tmp.Rects[glyph_i].was_packed)
                atlas->TexHeight = ImMax(atlas->TexHeight, src_tmp.Rects[glyph_i].y + src_tmp.Rects[glyph_i].h);
    }

    // 7. Allocate texture
    atlas->TexHeight = (atlas->Flags & ImFontAtlasFlags_NoPowerOfTwoHeight) ? (atlas->TexHeight + 1) : ImUpperPowerOfTwo(atlas->TexHeight);
    atlas->TexUvScale[0] = 1.0f / atlas->TexWidth;
    atlas->TexUvScale[1] = 1.0f / atlas->TexHeight;
    atlas->TexPixelsAlpha8 = (unsigned char*)stack_hidden_call( malloc,atlas->TexWidth * atlas->TexHeight);
    memset(atlas->TexPixelsAlpha8, 0, atlas->TexWidth * atlas->TexHeight);
    spc.pixels = atlas->TexPixelsAlpha8;
    spc.height = atlas->TexHeight;

    // 8. Render/rasterize font characters into the texture
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontConfig& cfg = atlas->ConfigData[src_i];
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        if (src_tmp.GlyphsCount == 0)
            continue;

        stbtt_PackFontRangesRenderIntoRects(&spc, &src_tmp.FontInfo, &src_tmp.PackRange, 1, src_tmp.Rects);

        // Apply multiply operator
        if (cfg.RasterizerMultiply != 1.0f)
        {
            unsigned char multiply_table[256];
            ImFontAtlasBuildMultiplyCalcLookupTable(multiply_table, cfg.RasterizerMultiply);
            stbrp_rect* r = &src_tmp.Rects[0];
            for (int glyph_i = 0; glyph_i < src_tmp.GlyphsCount; glyph_i++, r++)
                if (r->was_packed)
                    ImFontAtlasBuildMultiplyRectAlpha8(multiply_table, atlas->TexPixelsAlpha8, r->x, r->y, r->w, r->h, atlas->TexWidth * 1);
        }
        src_tmp.Rects = NULL;
    }

    // End packing
    stbtt_PackEnd(&spc);
    buf_rects.clear();

    // 9. Setup ImFont and glyphs for runtime
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        // When merging fonts with MergeMode=true:
        // - We can have multiple input fonts writing into a same destination font.
        // - dst_font->ConfigData is != from cfg which is our source configuration.
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        ImFontConfig& cfg = atlas->ConfigData[src_i];
        ImFont* dst_font = cfg.DstFont;

        const float font_scale = stbtt_ScaleForPixelHeight(&src_tmp.FontInfo, cfg.SizePixels);
        int unscaled_ascent, unscaled_descent, unscaled_line_gap;
        stbtt_GetFontVMetrics(&src_tmp.FontInfo, &unscaled_ascent, &unscaled_descent, &unscaled_line_gap);

        const float ascent = ImTrunc(unscaled_ascent * font_scale + ((unscaled_ascent > 0.0f) ? +1 : -1));
        const float descent = ImTrunc(unscaled_descent * font_scale + ((unscaled_descent > 0.0f) ? +1 : -1));
        ImFontAtlasBuildSetupFont(atlas, dst_font, &cfg, ascent, descent);
        const float font_off_x = cfg.GlyphOffset.x;
        const float font_off_y = cfg.GlyphOffset.y + (dst_font->Ascent+0.5f);

        const float inv_rasterization_scale = 1.0f / cfg.RasterizerDensity;

        for (int glyph_i = 0; glyph_i < src_tmp.GlyphsCount; glyph_i++)
        {
            // Register glyph
            const int codepoint = src_tmp.GlyphsList[glyph_i];
            const stbtt_packedchar& pc = src_tmp.PackedChars[glyph_i];
            stbtt_aligned_quad q;
            float unused_x = 0.0f, unused_y = 0.0f;
            stbtt_GetPackedQuad(src_tmp.PackedChars, atlas->TexWidth, atlas->TexHeight, glyph_i, &unused_x, &unused_y, &q, 0);
            float x0 = q.x0 * inv_rasterization_scale + font_off_x;
            float y0 = q.y0 * inv_rasterization_scale + font_off_y;
            float x1 = q.x1 * inv_rasterization_scale + font_off_x;
            float y1 = q.y1 * inv_rasterization_scale + font_off_y;
            dst_font->AddGlyph(&cfg, (UINT16)codepoint, x0, y0, x1, y1, q.s0, q.t0, q.s1, q.t1, pc.xadvance * inv_rasterization_scale);
        }
    }

    // Cleanup
    src_tmp_array.clear_destruct();

    ImFontAtlasBuildFinish(atlas);
	
    return true;
	
}


static UINT16 FindFirstExistingGlyph(ImFont* font, const UINT16* candidate_chars, int candidate_chars_count)
{
	
    for (int n = 0; n < candidate_chars_count; n++)
        if (font->FindGlyphNoFallback(candidate_chars[n]) != NULL)
            return candidate_chars[n];
	
	return (UINT16)-1;

}

void xs_DrawListSharedData::SetCircleTessellationMaxError(float max_error)
{
	
    if (CircleSegmentMaxError == max_error)
        return;

    IM_ASSERT(max_error > 0.0f);
    CircleSegmentMaxError = max_error;
    for (int i = 0; i < XS_ARRAYSIZE(CircleSegmentCounts); i++)
    {
        const float radius = (float)i;
        CircleSegmentCounts[i] = (UINT8)((i > 0) ? IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(radius, CircleSegmentMaxError) : IM_DRAWLIST_ARCFAST_SAMPLE_MAX);
    }
    ArcFastRadiusCutoff = IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(IM_DRAWLIST_ARCFAST_SAMPLE_MAX, CircleSegmentMaxError);
	
}


void ImFont::AddGlyph(const ImFontConfig* cfg, UINT16 codepoint, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x)
{
	
    if (cfg != NULL)
    {
        // Clamp & recenter if needed
        const float advance_x_original = advance_x;
        advance_x = ImClamp(advance_x, cfg->GlyphMinAdvanceX, cfg->GlyphMaxAdvanceX);
        if (advance_x != advance_x_original)
        {
            float char_off_x = cfg->PixelSnapH ? ImTrunc((advance_x - advance_x_original) * 0.5f) : (advance_x - advance_x_original) * 0.5f;
            x0 += char_off_x;
            x1 += char_off_x;
        }

        // Snap to pixel
        if (cfg->PixelSnapH)
            advance_x = (advance_x+0x5f);

        // Bake spacing
        advance_x += cfg->GlyphExtraSpacing.x;
    }

    Glyphs.resize(Glyphs.Size + 1);
    xsFontGlyph& glyph = Glyphs.back();
    glyph.Codepoint = (unsigned int)codepoint;
    glyph.Visible = (x0 != x1) && (y0 != y1);
    glyph.Colored = false;
    glyph.X0 = x0;
    glyph.Y0 = y0;
    glyph.X1 = x1;
    glyph.Y1 = y1;
    glyph.U0 = u0;
    glyph.V0 = v0;
    glyph.U1 = u1;
    glyph.V1 = v1;
    glyph.AdvanceX = advance_x;

    // Compute rough surface usage metrics (+1 to account for average padding, +0.99 to round)
    // We use (U1-U0)*TexWidth instead of X1-X0 to account for oversampling.
    float pad = ContainerAtlas->TexGlyphPadding + 0.99f;
    DirtyLookupTables = true;
    MetricsTotalSurface += (int)((glyph.U1 - glyph.U0) * ContainerAtlas->TexWidth + pad) * (int)((glyph.V1 - glyph.V0) * ContainerAtlas->TexHeight + pad);
	
}

void    ImFont::ClearOutputData()
{
	
    FontSize = 0.0f;
    FallbackAdvanceX = 0.0f;
    Glyphs.clear();
    IndexAdvanceX.clear();
    IndexLookup.clear();
    FallbackGlyph = NULL;
    ContainerAtlas = NULL;
    DirtyLookupTables = true;
    Ascent = Descent = 0.0f;
    MetricsTotalSurface = 0;
	
}

void ImFont::BuildLookupTable()
{
	
    int max_codepoint = 0;
    for (int i = 0; i != Glyphs.Size; i++)
        max_codepoint = ImMax(max_codepoint, (int)Glyphs[i].Codepoint);

    // Build lookup table
    IM_ASSERT(Glyphs.Size > 0 && "Font has not loaded glyph!");
    IM_ASSERT(Glyphs.Size < 0xFFFF); // -1 is reserved
    IndexAdvanceX.clear();
    IndexLookup.clear();
    DirtyLookupTables = false;
    memset(Used4kPagesMap, 0, sizeof(Used4kPagesMap));
    GrowIndex(max_codepoint + 1);
    for (int i = 0; i < Glyphs.Size; i++)
    {
        int codepoint = (int)Glyphs[i].Codepoint;
        IndexAdvanceX[codepoint] = Glyphs[i].AdvanceX;
        IndexLookup[codepoint] = (UINT16)i;

        // Mark 4K page as used
        const int page_n = codepoint / 4096;
        Used4kPagesMap[page_n >> 3] |= 1 << (page_n & 7);
    }

    // Create a glyph to handle TAB
    // FIXME: Needs proper TAB handling but it needs to be contextualized (or we could arbitrary say that each string starts at "column 0" ?)
    if (FindGlyph((UINT16)' '))
    {
        if (Glyphs.back().Codepoint != '\t')   // So we can call this function multiple times (FIXME: Flaky)
            Glyphs.resize(Glyphs.Size + 1);
        xsFontGlyph& tab_glyph = Glyphs.back();
        tab_glyph = *FindGlyph((UINT16)' ');
        tab_glyph.Codepoint = '\t';
        tab_glyph.AdvanceX *= 4;
        IndexAdvanceX[(int)tab_glyph.Codepoint] = (float)tab_glyph.AdvanceX;
        IndexLookup[(int)tab_glyph.Codepoint] = (UINT16)(Glyphs.Size - 1);
    }

    // Mark special glyphs as not visible (note that AddGlyph already mark as non-visible glyphs with zero-size polygons)
    SetGlyphVisible((UINT16)' ', false);
    SetGlyphVisible((UINT16)'\t', false);

    // Setup Fallback character
    const UINT16 fallback_chars[] = { (UINT16)0xFFFD, (UINT16)'?', (UINT16)' ' };
    FallbackGlyph = FindGlyphNoFallback(FallbackChar);
    if (FallbackGlyph == NULL)
    {
        FallbackChar = FindFirstExistingGlyph(this, fallback_chars, XS_ARRAYSIZE(fallback_chars));
        FallbackGlyph = FindGlyphNoFallback(FallbackChar);
        if (FallbackGlyph == NULL)
        {
            FallbackGlyph = &Glyphs.back();
            FallbackChar = (UINT16)FallbackGlyph->Codepoint;
        }
    }
    FallbackAdvanceX = FallbackGlyph->AdvanceX;
    for (int i = 0; i < max_codepoint + 1; i++)
        if (IndexAdvanceX[i] < 0.0f)
            IndexAdvanceX[i] = FallbackAdvanceX;

    // Setup Ellipsis character. It is required for rendering elided text. We prefer using U+2026 (horizontal ellipsis).
    // However some old fonts may contain ellipsis at U+0085. Here we auto-detect most suitable ellipsis character.
    // FIXME: Note that 0x2026 is rarely included in our font ranges. Because of this we are more likely to use three individual dots.
    const UINT16 ellipsis_chars[] = { (UINT16)0x2026, (UINT16)0x0085 };
    const UINT16 dots_chars[] = { (UINT16)'.', (UINT16)0xFF0E };
    if (EllipsisChar == (UINT16)-1)
        EllipsisChar = FindFirstExistingGlyph(this, ellipsis_chars, XS_ARRAYSIZE(ellipsis_chars));
    const UINT16 dot_char = FindFirstExistingGlyph(this, dots_chars, XS_ARRAYSIZE(dots_chars));
    if (EllipsisChar != (UINT16)-1)
    {
        EllipsisCharCount = 1;
        EllipsisWidth = EllipsisCharStep = FindGlyph(EllipsisChar)->X1;
    }
    else if (dot_char != (UINT16)-1)
    {
        const xsFontGlyph* glyph = FindGlyph(dot_char);
        EllipsisChar = dot_char;
        EllipsisCharCount = 3;
        EllipsisCharStep = (glyph->X1 - glyph->X0) + 1.0f;
        EllipsisWidth = EllipsisCharStep * 3.0f - 1.0f;
    }
	
}

void ImFont::SetGlyphVisible(UINT16 c, bool visible)
{
	
    if (xsFontGlyph* glyph = (xsFontGlyph*)(void*)FindGlyph((UINT16)c))
        glyph->Visible = visible ? 1 : 0;
	
}

void ImFont::GrowIndex(int new_size)
{
	

    if (new_size <= IndexLookup.Size)
        return;
    IndexAdvanceX.resize(new_size, -1.0f);
    IndexLookup.resize(new_size, (UINT16)-1);
	
}

const xsFontGlyph* ImFont::FindGlyphNoFallback(UINT16 c) const
{
	
    if (c >= (size_t)IndexLookup.Size)
        return NULL;
    const UINT16 i = IndexLookup.Data[c];
    if (i == (UINT16)-1)
        return NULL;
	xsFontGlyph* axa = &Glyphs.Data[i];
	
	return axa;

}

ImFont::ImFont()
{
	
    FontSize = 0.0f;
    FallbackAdvanceX = 0.0f;
    FallbackChar = (UINT16)-1;
    EllipsisChar = (UINT16)-1;
    EllipsisWidth = EllipsisCharStep = 0.0f;
    EllipsisCharCount = 0;
    FallbackGlyph = NULL;
    ContainerAtlas = NULL;
    ConfigData = NULL;
    ConfigDataCount = 0;
    DirtyLookupTables = false;
    Scale = 1.0f;
    Ascent = Descent = 0.0f;
    MetricsTotalSurface = 0;
    memset(Used4kPagesMap, 0, sizeof(Used4kPagesMap));
	
}

ImFont::~ImFont()
{
	
    ClearOutputData();
	
}

void xsFontAtlas::CalcCustomRectUV(const ImFontAtlasCustomRect* rect, xsFloat2* out_uv_min, xsFloat2* out_uv_max) const
{
	
    IM_ASSERT(TexWidth > 0 && TexHeight > 0);   // Font atlas needs to be built before we can calculate UV coordinates
    IM_ASSERT(rect->IsPacked());                // Make sure the rectangle has been packed
    *out_uv_min = xsFloat2((float)rect->X * TexUvScale[0], (float)rect->Y * TexUvScale[1]);
    *out_uv_max = xsFloat2((float)(rect->X + rect->Width) * TexUvScale[0], (float)(rect->Y + rect->Height) * TexUvScale[1]);
	
}

const UINT16* xsFontAtlas::GetGlyphRangesDefault()
{
	
    static const UINT16 ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0,
    };
	UINT16* axa = (UINT16*)&ranges[0];
	
	return axa;
	
}



int xsFontAtlas::AddCustomRectRegular(int width, int height)
{
	
    IM_ASSERT(width > 0 && width <= 0xFFFF);
    IM_ASSERT(height > 0 && height <= 0xFFFF);
    ImFontAtlasCustomRect r;
    r.Width = (unsigned short)width;
    r.Height = (unsigned short)height;
    CustomRects.push_back(r);
	int axa = CustomRects.Size - 1;
	
	return axa; // Return index
	
}

const UINT16*  xsFontAtlas::GetGlyphRangesChineseFull()
{
	
    static const UINT16 ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x2000, 0x206F, // General Punctuation
        0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
        0x31F0, 0x31FF, // Katakana Phonetic Extensions
        0xFF00, 0xFFEF, // Half-width characters
        0xFFFD, 0xFFFD, // Invalid
        0x4e00, 0x9FAF, // CJK Ideograms
        0,
    };
	UINT16* axa= (UINT16*)&ranges[0];
	
	return axa;
	
}

ImFont* xsFontAtlas::AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg_template, const UINT16* glyph_ranges)
{
	
	size_t data_size = 0;
	void* data = xsFileLoadToMemory(filename, VMProtectDecryptStringA("rb"), &data_size, 0);
	if (!data)
	{
		return NULL;
	}
	ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();

	ImFont* axa= AddFontFromMemoryTTF(data, (int)data_size, size_pixels, &font_cfg, glyph_ranges);
	
	return axa;
	
}

ImFont* xsFontAtlas::AddFontFromMemoryTTF(void* ttf_data, int ttf_size, float size_pixels, const ImFontConfig* font_cfg_template, const UINT16* glyph_ranges)
{
	
    //IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
    //IM_ASSERT(font_cfg.FontData == NULL);
    font_cfg.FontData = ttf_data;
    font_cfg.FontDataSize = ttf_size;
    font_cfg.SizePixels = size_pixels;
    if (glyph_ranges)
        font_cfg.GlyphRanges = glyph_ranges;
	ImFont* axa = AddFont(&font_cfg);
	
	return axa;
	
}

ImFont* xsFontAtlas::AddFont(const ImFontConfig* font_cfg)
{
	
    //IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    //IM_ASSERT(font_cfg->FontData != NULL && font_cfg->FontDataSize > 0);
    //IM_ASSERT(font_cfg->SizePixels > 0.0f);

    // Create new font
    if (!font_cfg->MergeMode)
        Fonts.push_back(XS_NEW(ImFont));
    //else
        //IM_ASSERT(!Fonts.empty() && "Cannot use MergeMode for the first font"); // When using MergeMode make sure that a font has already been added before. You can use ImGui::GetIO().Fonts->AddFontDefault() to add the default imgui font.

    ConfigData.push_back(*font_cfg);
    ImFontConfig& new_font_cfg = ConfigData.back();
    if (new_font_cfg.DstFont == NULL)
        new_font_cfg.DstFont = Fonts.back();
    if (!new_font_cfg.FontDataOwnedByAtlas)
    {
        new_font_cfg.FontData = stack_hidden_call( malloc,new_font_cfg.FontDataSize);
        new_font_cfg.FontDataOwnedByAtlas = true;
        memcpy(new_font_cfg.FontData, font_cfg->FontData, (size_t)new_font_cfg.FontDataSize);
    }

    if (new_font_cfg.DstFont->EllipsisChar == (UINT16)-1)
        new_font_cfg.DstFont->EllipsisChar = font_cfg->EllipsisChar;

    ImFontAtlasUpdateConfigDataPointers(this);

    // Invalidate texture
    TexReady = false;
    ClearTexData();

	ImFont* axa= new_font_cfg.DstFont;
	
	return axa;
	
}

void    xsFontAtlas::GetTexDataAsAlpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel)
{
	
    // Build atlas on demand
    if (TexPixelsAlpha8 == NULL)
        Build();

    *out_pixels = TexPixelsAlpha8;
    if (out_width) *out_width = TexWidth;
    if (out_height) *out_height = TexHeight;
    if (out_bytes_per_pixel) *out_bytes_per_pixel = 1;
	
}


bool    xsFontAtlas::Build()
{
	
    //IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");

    // Default font is none are specified
    //if (ConfigData.Size == 0)
    //    AddFontDefault();

    // Select builder
    // - Note that we do not reassign to atlas->FontBuilderIO, since it is likely to point to static data which
    //   may mess with some hot-reloading schemes. If you need to assign to this (for dynamic selection) AND are
    //   using a hot-reloading scheme that messes up static data, store your own instance of ImFontBuilderIO somewhere
    //   and point to it instead of pointing directly to return value of the GetBuilderXXX functions.
    //const ImFontBuilderIO* builder_io = FontBuilderIO;
    //if (builder_io == NULL)
    //{
    //    builder_io = ImFontAtlasGetBuilderForStbTruetype();
    //}

    // Build
	bool axa= ImFontAtlasBuildWithStbTruetype(this);
	
    return axa;

}

void xsFontAtlas::GetFontData_RGBA_32(UCHAR** FontImage, int* out_width, int* out_height, int* out_bytes_per_pixel)
{
	
    //FILE* fp;
    //if ((fp = fopen("C:\\字体", "r")) != NULL)
    //{  //以二进制方式打开
    //    TexPixelsAlpha8 = (unsigned char*)stack_hidden_call( malloc,(size_t)TexWidth * (size_t)TexHeight);
    //    fread(TexPixelsAlpha8, sizeof(char), (size_t)TexWidth * (size_t)TexHeight, fp);
    //    fclose(fp);

    //    if (!TexPixelsRGBA32)
    //    {
    //        TexPixelsRGBA32 = (unsigned int*)stack_hidden_call( malloc,(size_t)TexWidth * (size_t)TexHeight * 4);
    //        const unsigned char* src = (const unsigned char*)TexPixelsAlpha8;
    //        unsigned int* dst = TexPixelsRGBA32;
    //        for (int n = TexWidth * TexHeight; n > 0; n--)
    //            *dst++ = XS_COL32(255, 255, 255, (unsigned int)(*src++));//col四字节
    //    }
    //    stack_hidden_call(free,TexPixelsAlpha8);
    //}
    //else 
    //{
    //    char a[256] = { 0 };
    //    stack_hidden_call(sprintf,a,"Error %d\n", GetLastError());
    //    ////MessageBoxA(0,a,"",0);
    //}
    

    if (!TexPixelsRGBA32)
    {
        unsigned char* pixels = NULL;
        GetTexDataAsAlpha8(&pixels, NULL, NULL);
        if (pixels)
        {
            TexPixelsRGBA32 = (unsigned int*)stack_hidden_call( malloc,(size_t)TexWidth * (size_t)TexHeight * 4);
            const unsigned char* src = pixels;
            unsigned int* dst = TexPixelsRGBA32;
            for (int n = TexWidth * TexHeight; n > 0; n--)
                *dst++ = XS_COL32(255, 255, 255, (unsigned int)(*src++));
            stack_hidden_call_v(free,pixels);
			
        }
        
    }

    *FontImage = (UCHAR*)TexPixelsRGBA32;
    //TexPixelsRGBA32 = NULL;
    if (out_width) *out_width = TexWidth;
    if (out_height) *out_height = TexHeight;
    if (out_bytes_per_pixel) *out_bytes_per_pixel = 4;
	
}

// Convert UTF-8 to 32-bit character, process single character input.
// Based on stb_from_utf8() from github.com/nothings/stb/
// We handle UTF-8 decoding error by skipping forward.
int xsTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end)
{
	
    unsigned int c = (unsigned int)-1;
    const unsigned char* str = (const unsigned char*)in_text;
    if (!(*str & 0x80))
    {
        c = (unsigned int)(*str++);
        *out_char = c;
        return 1;
    }
    if ((*str & 0xe0) == 0xc0)
    {
        *out_char = XS_UNICODE_CODEPOINT_INVALID; // will be invalid but not end of string
        if (in_text_end && in_text_end - (const char*)str < 2) return 1;
        if (*str < 0xc2) return 2;
        c = (unsigned int)((*str++ & 0x1f) << 6);
        if ((*str & 0xc0) != 0x80) return 2;
        c += (*str++ & 0x3f);
        *out_char = c;
        return 2;
    }
    if ((*str & 0xf0) == 0xe0)
    {
        *out_char = XS_UNICODE_CODEPOINT_INVALID; // will be invalid but not end of string
        if (in_text_end && in_text_end - (const char*)str < 3) return 1;
        if (*str == 0xe0 && (str[1] < 0xa0 || str[1] > 0xbf)) return 3;
        if (*str == 0xed && str[1] > 0x9f) return 3; // str[1] < 0x80 is checked below
        c = (unsigned int)((*str++ & 0x0f) << 12);
        if ((*str & 0xc0) != 0x80) return 3;
        c += (unsigned int)((*str++ & 0x3f) << 6);
        if ((*str & 0xc0) != 0x80) return 3;
        c += (*str++ & 0x3f);
        *out_char = c;
        return 3;
    }
    if ((*str & 0xf8) == 0xf0)
    {
        *out_char = XS_UNICODE_CODEPOINT_INVALID; // will be invalid but not end of string
        if (in_text_end && in_text_end - (const char*)str < 4) return 1;
        if (*str > 0xf4) return 4;
        if (*str == 0xf0 && (str[1] < 0x90 || str[1] > 0xbf)) return 4;
        if (*str == 0xf4 && str[1] > 0x8f) return 4; // str[1] < 0x80 is checked below
        c = (unsigned int)((*str++ & 0x07) << 18);
        if ((*str & 0xc0) != 0x80) return 4;
        c += (unsigned int)((*str++ & 0x3f) << 12);
        if ((*str & 0xc0) != 0x80) return 4;
        c += (unsigned int)((*str++ & 0x3f) << 6);
        if ((*str & 0xc0) != 0x80) return 4;
        c += (*str++ & 0x3f);
        // utf-8 encodings of values used in surrogate pairs are invalid
        if ((c & 0xFFFFF800) == 0xD800) return 4;
        // If codepoint does not fit in ImWchar, use replacement character U+FFFD instead
        if (c > XS_UNICODE_CODEPOINT_MAX) c = XS_UNICODE_CODEPOINT_INVALID;
        *out_char = c;
        return 4;
    }
    *out_char = 0;
	
    return 0;
	
}

const char* ImFont::CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const
{
	
    // Simple word-wrapping for English, not full-featured. Please submit failing cases!
    // FIXME: Much possible improvements (don't cut things like "word !", "word!!!" but cut within "word,,,,", more sensible support for punctuations, support for Unicode punctuations, etc.)

    // For references, possible wrap point marked with ^
    //  "aaa bbb, ccc,ddd. eee   fff. ggg!"
    //      ^    ^    ^   ^   ^__    ^    ^

    // List of hardcoded separators: .,;!?'"

    // Skip extra blanks after a line returns (that includes not counting them in width computation)
    // e.g. "Hello    world" --> "Hello" "World"

    // Cut words that cannot possibly fit within one line.
    // e.g.: "The tropical fish" with ~5 characters worth of width --> "The tr" "opical" "fish"

    float line_width = 0.0f;
    float word_width = 0.0f;
    float blank_width = 0.0f;
    wrap_width /= scale; // We work with unscaled widths to avoid scaling every characters

    const char* word_end = text;
    const char* prev_word_end = NULL;
    bool inside_word = true;

    const char* s = text;
    while (s < text_end)
    {
        unsigned int c = (unsigned int)*s;
        const char* next_s;
        if (c < 0x80)
            next_s = s + 1;
        else
            next_s = s + xsTextCharFromUtf8(&c, s, text_end);
        if (c == 0)
            break;

        if (c < 32)
        {
            if (c == '\n')
            {
                line_width = word_width = blank_width = 0.0f;
                inside_word = true;
                s = next_s;
                continue;
            }
            if (c == '\r')
            {
                s = next_s;
                continue;
            }
        }

        const float char_width = ((int)c < IndexAdvanceX.Size ? IndexAdvanceX.Data[c] : FallbackAdvanceX);
        if (xsCharIsBlankW(c))
        {
            if (inside_word)
            {
                line_width += blank_width;
                blank_width = 0.0f;
                word_end = s;
            }
            blank_width += char_width;
            inside_word = false;
        }
        else
        {
            word_width += char_width;
            if (inside_word)
            {
                word_end = next_s;
            }
            else
            {
                prev_word_end = word_end;
                line_width += word_width + blank_width;
                word_width = blank_width = 0.0f;
            }

            // Allow wrapping after punctuation.
            inside_word = !(c == '.' || c == ',' || c == ';' || c == '!' || c == '?' || c == '\"');
        }

        // We ignore blank width at the end of the line (they can be skipped)
        if (line_width + word_width > wrap_width)
        {
            // Words that cannot possibly fit within an entire line will be cut anywhere.
            if (word_width < wrap_width)
                s = prev_word_end ? prev_word_end : word_end;
            break;
        }

        s = next_s;
    }
	
    return s;
	
}

xsFloat2 ImFont::CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end, const char** remaining) const
{
	
    if (!text_end)
        text_end = text_begin + strlen(text_begin); // FIXME-OPT: Need to avoid this.

    const float line_height = size;
    const float scale = size / FontSize;

    xsFloat2 text_size = xsFloat2(0, 0);
    float line_width = 0.0f;

    const bool word_wrap_enabled = (wrap_width > 0.0f);
    const char* word_wrap_eol = NULL;

    const char* s = text_begin;
    while (s < text_end)
    {
        if (word_wrap_enabled)
        {
            // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
            if (!word_wrap_eol)
            {
                word_wrap_eol = CalcWordWrapPositionA(scale, s, text_end, wrap_width - line_width);
                if (word_wrap_eol == s) // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
                    word_wrap_eol++;    // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
            }

            if (s >= word_wrap_eol)
            {
                if (text_size.x < line_width)
                    text_size.x = line_width;
                text_size.y += line_height;
                line_width = 0.0f;
                word_wrap_eol = NULL;

                // Wrapping skips upcoming blanks
                while (s < text_end)
                {
                    const char c = *s;
                    if (xsCharIsBlankA(c)) { s++; }
                    else if (c == '\n') { s++; break; }
                    else { break; }
                }
                continue;
            }
        }

        // Decode and advance source
        const char* prev_s = s;
        unsigned int c = (unsigned int)*s;
        if (c < 0x80)
        {
            s += 1;
        }
        else
        {
            s += xsTextCharFromUtf8(&c, s, text_end);
            if (c == 0) // Malformed UTF-8?
                break;
        }

        if (c < 32)
        {
            if (c == '\n')
            {
                text_size.x = ImMax(text_size.x, line_width);
                text_size.y += line_height;
                line_width = 0.0f;
                continue;
            }
            if (c == '\r')
                continue;
        }

        const float char_width = ((int)c < IndexAdvanceX.Size ? IndexAdvanceX.Data[c] : FallbackAdvanceX) * scale;
        if (line_width + char_width >= max_width)
        {
            s = prev_s;
            break;
        }

        line_width += char_width;
    }

    if (text_size.x < line_width)
        text_size.x = line_width;

    if (line_width > 0 || text_size.y == 0.0f)
        text_size.y += line_height;

    if (remaining)
        *remaining = s;
	
    return text_size;
	
}

// Note: as with every ImDrawList drawing function, this expects that the font atlas texture is bound.
void ImFont::RenderChar(xs_DrawList* draw_list, float size, const xsFloat2& pos, UINT col, UINT16 c) const
{
	
    const xsFontGlyph* glyph = FindGlyph(c);
    if (!glyph || !glyph->Visible)
        return;
    if (glyph->Colored)
        col |= ~0xFF000000;
    float scale = (size >= 0.0f) ? (size / FontSize) : 1.0f;
    float x = (pos.x);
    float y = (pos.y);
    draw_list->Capacityretention(6, 4);
    draw_list->PrimRectUV(xsFloat2(x + glyph->X0 * scale, y + glyph->Y0 * scale), xsFloat2(x + glyph->X1 * scale, y + glyph->Y1 * scale), xsFloat2(glyph->U0, glyph->V0), xsFloat2(glyph->U1, glyph->V1), col);
	
}


const xsFontGlyph* ImFont::FindGlyph(UINT16 c) const
{
	
    if (c >= (size_t)IndexLookup.Size)
        return FallbackGlyph;
    const UINT16 i = IndexLookup.Data[c];
    if (i == (UINT16)-1)
        return FallbackGlyph;
    return &Glyphs.Data[i];

	
}