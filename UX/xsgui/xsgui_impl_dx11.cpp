
// dear imgui: Renderer Backend for DirectX11
// This needs to be used along with a Platform Backend (e.g. Win32)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'ID3D11ShaderResourceView*' as ImTextureID. Read the FAQ about ImTextureID!
//  [X] Renderer: Support for large meshes (64k+ vertices) with 16-bit indices.

// You can use unmodified imgui_impl_* files in your project. See examples/ folder for examples of using this.
// Prefer including the entire imgui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//  2021-06-29: Reorganized backend to pull data from a single structure to facilitate usage with multiple-contexts (all g_XXXX access changed to XXXX).
//  2021-05-19: DirectX11: Replaced direct access to ImDrawCmd::TextureId with a call to ImDrawCmd::GetTexID(). (will become a requirement)
//  2021-02-18: DirectX11: Change blending equation to preserve alpha in output buffer.
//  2019-08-01: DirectX11: Fixed code querying the Geometry Shader state (would generally error with Debug layer enabled).
//  2019-07-21: DirectX11: Backup, clear and restore Geometry Shader is any is bound when calling ImGui_ImplDX10_RenderDrawData. Clearing Hull/Domain/Compute shaders without backup/restore.
//  2019-05-29: DirectX11: Added support for large mesh (64K+ vertices), enable ImGuiBackendFlags_RendererHasVtxOffset flag.
//  2019-04-30: DirectX11: Added support for special ImDrawCallback_ResetRenderState callback to reset render state.
//  2018-12-03: Misc: Added #pragma comment statement to automatically link with d3dcompiler.lib when using D3DCompile().
//  2018-11-30: Misc: Setting up io.BackendRendererName so it can be displayed in the About Window.
//  2018-08-01: DirectX11: Querying for IDXGIFactory instead of IDXGIFactory1 to increase compatibility.
//  2018-07-13: DirectX11: Fixed unreleased resources in Init and Shutdown functions.
//  2018-06-08: Misc: Extracted imgui_impl_dx11.cpp/.h away from the old combined DX11+Win32 example.
//  2018-06-08: DirectX11: Use draw_data->DisplayPos and draw_data->DisplaySize to setup projection matrix and clipping rectangle.
//  2018-02-16: Misc: Obsoleted the io.RenderDrawListsFn callback and exposed ImGui_ImplDX11_RenderDrawData() in the .h file so you can call it yourself.
//  2018-02-06: Misc: Removed call to ImGui::Shutdown() which is not available from 1.60 WIP, user needs to call CreateContext/DestroyContext themselves.
//  2016-05-07: DirectX11: Disabling depth-write.

#include "xsgui_impl_dx11.h"
#include "dx_fn.h"
// DirectX

#include <d3d11.h>
//#include <d3dcompiler.h>
//#ifdef _MSC_VER
//#pragma comment(lib, "d3dcompiler") // Automatically link with d3dcompiler.lib as we are using D3DCompile() below.
//#endif

#define IM_OFFSETOF(_TYPE,_MEMBER)  ((size_t)&(((_TYPE*)0)->_MEMBER))           // Offset of _MEMBER within _TYPE. Old style macro.


// DirectX11 data

    ID3D11Device* pd3dDevice;
    ID3D11DeviceContext* pd3dDeviceContext;
    IDXGIFactory* pFactory;
    ID3D11Buffer* pVB;
    ID3D11Buffer* pIB;
    ID3D11VertexShader* pVertexShader;
    ID3D11InputLayout* pInputLayout;
    ID3D11Buffer* pVertexConstantBuffer;
    ID3D11PixelShader* pPixelShader;
    ID3D11SamplerState* pFontSampler;
    ID3D11ShaderResourceView* pFontTextureView;
    ID3D11RasterizerState* pRasterizerState;
    ID3D11BlendState* pBlendState;
    ID3D11DepthStencilState* pDepthStencilState;
    int                         VertexBufferSize = 5000;
    int                         IndexBufferSize = 10000;


struct VERTEX_CONSTANT_BUFFER
{
    float   mvp[4][4];
};

void    xsGui_DX11_InvalidateDeviceObjects();

// Functions
static void xsGui_DX11_SetupRenderState(xs_DrawData* draw_data, ID3D11DeviceContext* ctx)
{
    // Setup viewport
    D3D11_VIEWPORT vp;
    memset(&vp, 0, sizeof(D3D11_VIEWPORT));
    vp.Width = draw_data->DisplaySize.x;
    vp.Height = draw_data->DisplaySize.y;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = vp.TopLeftY = 0;
    //ctx->RSSetViewports(1, &vp);//160h
    stack_hidden_call(RSSetViewportsFn(ctx), ctx, 1, &vp);

    // Setup shader and vertex buffers
    unsigned int stride = sizeof(xsDrawVert);
    unsigned int offset = 0;
    //ctx->IASetInputLayout(pInputLayout);//88h
    //ctx->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);//90h
    //ctx->IASetIndexBuffer(pIB, sizeof(xsDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);//98h
    //ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//0C0h
    //ctx->VSSetShader(pVertexShader, NULL, 0);//58h
    //ctx->VSSetConstantBuffers(0, 1, &pVertexConstantBuffer);//38h
    //ctx->PSSetShader(pPixelShader, NULL, 0);//48h
    //ctx->PSSetSamplers(0, 1, &pFontSampler);//50h
    //ctx->GSSetShader(NULL, NULL, 0);//0B8h
    //ctx->HSSetShader(NULL, NULL, 0); // In theory we should backup and restore this as well.. very infrequently used..1E0h
    //ctx->DSSetShader(NULL, NULL, 0); // In theory we should backup and restore this as well.. very infrequently used..200h
    //ctx->CSSetShader(NULL, NULL, 0); // In theory we should backup and restore this as well.. very infrequently used..228h

    stack_hidden_call(IASetInputLayoutFn(ctx), ctx, pInputLayout);
    stack_hidden_call(IASetVertexBuffersFn(ctx), ctx, 0, 1, &pVB, &stride, &offset);
    stack_hidden_call(IASetIndexBufferFn(ctx), ctx, pIB, sizeof(xsDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
    stack_hidden_call(IASetPrimitiveTopologyFn(ctx), ctx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    stack_hidden_call(VSSetShaderFn(ctx), ctx, pVertexShader, (ID3D11ClassInstance* const*)NULL, 0);
    stack_hidden_call(VSSetConstantBuffersFn(ctx), ctx, 0, 1, &pVertexConstantBuffer);
    stack_hidden_call(PSSetShaderFn(ctx), ctx, pPixelShader, (ID3D11ClassInstance* const*)NULL, 0);
    stack_hidden_call(PSSetSamplersFn(ctx), ctx, 0, 1, &pFontSampler);
    stack_hidden_call(GSSetShaderFn(ctx), ctx, (ID3D11GeometryShader*)NULL, (ID3D11ClassInstance* const*)NULL, 0);
    stack_hidden_call(HSSetShaderFn(ctx), ctx, (ID3D11HullShader*)NULL, (ID3D11ClassInstance* const*)NULL, 0);
    stack_hidden_call(DSSetShaderFn(ctx), ctx, (ID3D11DomainShader*)NULL, (ID3D11ClassInstance* const*)NULL, 0);
    stack_hidden_call(CSSetShaderFn(ctx), ctx, (ID3D11ComputeShader*)NULL, (ID3D11ClassInstance* const*)NULL, 0);

    // Setup blend state
    const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
    //ctx->OMSetBlendState(pBlendState, blend_factor, 0xffffffff);//118h
    //ctx->OMSetDepthStencilState(pDepthStencilState, 0);//120h
    //ctx->RSSetState(pRasterizerState);//158h

    stack_hidden_call(OMSetBlendStateFn(ctx), ctx, pBlendState, blend_factor, 0xffffffff);
    stack_hidden_call(OMSetDepthStencilStateFn(ctx), ctx, pDepthStencilState, 0);
    stack_hidden_call(RSSetStateFn(ctx), ctx, pRasterizerState);
}

// Render function
void xsGui_DX11_RenderDrawData(xs_DrawData* draw_data)
{
    // Avoid rendering when minimized
    if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f)
        return;

    ID3D11DeviceContext* ctx = pd3dDeviceContext;

    // Create and grow vertex/index buffers if needed
    if (!pVB || VertexBufferSize < draw_data->TotalVtxCount)
    {
        //if (pVB) { pVB->Release(); pVB = NULL; }
        if (pVB) { stack_hidden_call(ReleaseFn(pVB), pVB); pVB = NULL; }
        VertexBufferSize = draw_data->TotalVtxCount + 5000;
        D3D11_BUFFER_DESC desc;
        memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = VertexBufferSize * sizeof(xsDrawVert);
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        //if (pd3dDevice->CreateBuffer(&desc, NULL, &pVB) < 0)
        if (stack_hidden_call(CreateBufferFn(pd3dDevice), pd3dDevice, & desc, (const D3D11_SUBRESOURCE_DATA*)NULL, &pVB) < 0)
            return;
    }
    if (!pIB || IndexBufferSize < draw_data->TotalIdxCount)
    {
        //if (pIB) { pIB->Release(); pIB = NULL; }
        if (pIB) { stack_hidden_call(ReleaseFn(pIB), pIB); pIB = NULL; }
        IndexBufferSize = draw_data->TotalIdxCount + 10000;
        D3D11_BUFFER_DESC desc;
        memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = IndexBufferSize * sizeof(xsDrawIdx);
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        //if (pd3dDevice->CreateBuffer(&desc, NULL, &pIB) < 0)
        if (stack_hidden_call(CreateBufferFn(pd3dDevice), pd3dDevice, &desc, (const D3D11_SUBRESOURCE_DATA*)NULL, &pIB) < 0)
            return;
    }

    // Upload vertex/index data into a single contiguous GPU buffer
    D3D11_MAPPED_SUBRESOURCE vtx_resource,idx_resource;

    if (stack_hidden_call(MapFn(ctx), ctx, pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource) != S_OK)//70h
        return;
    if (stack_hidden_call(MapFn(ctx), ctx, pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource) != S_OK)//70h
        return;
    xsDrawVert* vtx_dst = (xsDrawVert*)vtx_resource.pData;
    xsDrawIdx* idx_dst = (xsDrawIdx*)idx_resource.pData;
    for (int n = 0; n < draw_data->DrawListsCount; n++)
    {
        const xs_DrawList* cmd_list = draw_data->DrawLists[n];
        memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(xsDrawVert));
        memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(xsDrawIdx));
        vtx_dst += cmd_list->VtxBuffer.Size;
        idx_dst += cmd_list->IdxBuffer.Size;
    }
    stack_hidden_call(UnmapFn(ctx), ctx, pVB, 0);//78h
    stack_hidden_call(UnmapFn(ctx), ctx, pIB, 0);//78h

    // Setup orthographic projection matrix into our constant buffer
    // Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayPos is (0,0) for single viewport apps.
    {
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        if (stack_hidden_call(MapFn(ctx), ctx, pVertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource) != S_OK)
            return;
        VERTEX_CONSTANT_BUFFER* constant_buffer = (VERTEX_CONSTANT_BUFFER*)mapped_resource.pData;
        float L = draw_data->DisplayPos.x;
        float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
        float T = draw_data->DisplayPos.y;
        float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
        float mvp[4][4] =
        {
            { 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
            { 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
            { 0.0f,         0.0f,           0.5f,       0.0f },
            { (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
        };
        memcpy(&constant_buffer->mvp, mvp, sizeof(mvp));
        stack_hidden_call(UnmapFn(ctx), ctx, pVertexConstantBuffer, 0);
    }

    // Backup DX state that will be modified to restore it afterwards (unfortunately this is very ugly looking and verbose. Close your eyes!)
    struct BACKUP_DX11_STATE
    {
        UINT                        ScissorRectsCount, ViewportsCount;
        D3D11_RECT                  ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
        D3D11_VIEWPORT              Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
        ID3D11RasterizerState* RS;
        ID3D11BlendState* BlendState;
        FLOAT                       BlendFactor[4];
        UINT                        SampleMask;
        UINT                        StencilRef;
        ID3D11DepthStencilState* DepthStencilState;
        ID3D11ShaderResourceView* PSShaderResource;
        ID3D11SamplerState* PSSampler;
        ID3D11PixelShader* PS;
        ID3D11VertexShader* VS;
        ID3D11GeometryShader* GS;
        UINT                        PSInstancesCount, VSInstancesCount, GSInstancesCount;
        ID3D11ClassInstance* PSInstances[256], * VSInstances[256], * GSInstances[256];   // 256 is max according to PSSetShader documentation
        D3D11_PRIMITIVE_TOPOLOGY    PrimitiveTopology;
        ID3D11Buffer* IndexBuffer, * VertexBuffer, * VSConstantBuffer;
        UINT                        IndexBufferOffset, VertexBufferStride, VertexBufferOffset;
        DXGI_FORMAT                 IndexBufferFormat;
        ID3D11InputLayout* InputLayout;
    };
    //BACKUP_DX11_STATE old = {};
    //old.ScissorRectsCount = old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
    //ctx->RSGetScissorRects(&old.ScissorRectsCount, old.ScissorRects);//300h
    //ctx->RSGetViewports(&old.ViewportsCount, old.Viewports);//2F8h
    //ctx->RSGetState(&old.RS);//2F0h
    //ctx->OMGetBlendState(&old.BlendState, old.BlendFactor, &old.SampleMask);//2D8h
    //ctx->OMGetDepthStencilState(&old.DepthStencilState, &old.StencilRef);//2E0h
    //ctx->PSGetShaderResources(0, 1, &old.PSShaderResource);//248h
    //ctx->PSGetSamplers(0, 1, &old.PSSampler);//258h
    //old.PSInstancesCount = old.VSInstancesCount = old.GSInstancesCount = 256;
    //ctx->PSGetShader(&old.PS, old.PSInstances, &old.PSInstancesCount);//250h
    //ctx->VSGetShader(&old.VS, old.VSInstances, &old.VSInstancesCount);//260h
    //ctx->VSGetConstantBuffers(0, 1, &old.VSConstantBuffer);//240h
    //ctx->GSGetShader(&old.GS, old.GSInstances, &old.GSInstancesCount);//290h

    //ctx->IAGetPrimitiveTopology(&old.PrimitiveTopology);//298h
    //ctx->IAGetIndexBuffer(&old.IndexBuffer, &old.IndexBufferFormat, &old.IndexBufferOffset);//280h
    //ctx->IAGetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset);//278h
    //ctx->IAGetInputLayout(&old.InputLayout);//270h

    //stack_hidden_call(RSGetScissorRectsFn(ctx), ctx,
    BACKUP_DX11_STATE old = {};
    old.ScissorRectsCount = old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
    stack_hidden_call(RSGetScissorRectsFn(ctx), ctx, &old.ScissorRectsCount, old.ScissorRects);//300h
    stack_hidden_call(RSGetViewportsFn(ctx), ctx,&old.ViewportsCount, old.Viewports);//2F8h
    stack_hidden_call(RSGetStateFn(ctx), ctx, &old.RS);//2F0h
    stack_hidden_call(OMGetBlendStateFn(ctx), ctx, &old.BlendState, old.BlendFactor, &old.SampleMask);//2D8h
    stack_hidden_call(OMGetDepthStencilStateFn(ctx), ctx, &old.DepthStencilState, &old.StencilRef);//2E0h
    stack_hidden_call(PSGetShaderResourcesFn(ctx), ctx,0, 1, &old.PSShaderResource);//248h
    stack_hidden_call(PSGetSamplersFn(ctx), ctx,0, 1, &old.PSSampler);//258h
    old.PSInstancesCount = old.VSInstancesCount = old.GSInstancesCount = 256;
    stack_hidden_call(PSGetShaderFn(ctx), ctx, &old.PS, old.PSInstances, &old.PSInstancesCount);//250h
    stack_hidden_call(VSGetShaderFn(ctx), ctx, &old.VS, old.VSInstances, &old.VSInstancesCount);//260h
    stack_hidden_call(VSGetConstantBuffersFn(ctx), ctx, 0, 1, &old.VSConstantBuffer);//240h
    stack_hidden_call(GSGetShaderFn(ctx), ctx, &old.GS, old.GSInstances, &old.GSInstancesCount);//290h

    stack_hidden_call(IAGetPrimitiveTopologyFn(ctx), ctx, &old.PrimitiveTopology);//298h
    stack_hidden_call(IAGetIndexBufferFn(ctx), ctx, &old.IndexBuffer, &old.IndexBufferFormat, &old.IndexBufferOffset);//280h
    stack_hidden_call(IAGetVertexBuffersFn(ctx), ctx,0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset);//278h
    stack_hidden_call(IAGetInputLayoutFn(ctx), ctx, &old.InputLayout);//270h

    // Setup desired DX state
    xsGui_DX11_SetupRenderState(draw_data, ctx);

    // Render command lists
    // (Because we merged all buffers into a single one, we maintain our own offset into them)
    int global_idx_offset = 0;
    int global_vtx_offset = 0;
    xsFloat2 clip_off = draw_data->DisplayPos;
    for (int n = 0; n < draw_data->DrawListsCount; n++)
    {
        const xs_DrawList* cmd_list = draw_data->DrawLists[n];
        for (int cmd_i = 0; cmd_i < cmd_list->GuideBuffer.Size; cmd_i++)
        {
            const xsDrawGuide* pcmd = &cmd_list->GuideBuffer.Data[cmd_i];
            //if (pcmd->UserCallback != NULL)
            //{
            //    // User callback, registered via ImDrawList::AddCallback()
            //    // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
            //    if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
            //        ImGui_ImplDX11_SetupRenderState(draw_data, ctx);
            //    else
            //        pcmd->UserCallback(cmd_list, pcmd);
            //}
            //else
            {
                // Project scissor/clipping rectangles into framebuffer space
                xsFloat2 clip_min(pcmd->ClipRect.x - clip_off.x, pcmd->ClipRect.y - clip_off.y);
                xsFloat2 clip_max(pcmd->ClipRect.z - clip_off.x, pcmd->ClipRect.w - clip_off.y);
                if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                    continue;

                // Apply scissor/clipping rectangle
                const D3D11_RECT r = { (LONG)clip_min.x, (LONG)clip_min.y, (LONG)clip_max.x, (LONG)clip_max.y };
                stack_hidden_call(RSSetScissorRectsFn(ctx), ctx,1, &r);//168h
                
                // Bind texture, Draw
                ID3D11ShaderResourceView* texture_srv = (ID3D11ShaderResourceView*)pcmd->TextureID;
                stack_hidden_call(PSSetShaderResourcesFn(ctx), ctx, 0, 1, &texture_srv);//40h
                stack_hidden_call(DrawIndexedFn(ctx), ctx, pcmd->ElemCount, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset);//60h

            }
        }
        global_idx_offset += cmd_list->IdxBuffer.Size;
        global_vtx_offset += cmd_list->VtxBuffer.Size;
    }

    // Restore modified DX state
    //ctx->RSSetScissorRects(old.ScissorRectsCount, old.ScissorRects);
    //ctx->RSSetViewports(old.ViewportsCount, old.Viewports);
    //ctx->RSSetState(old.RS); if (old.RS) old.RS->Release();
    //ctx->OMSetBlendState(old.BlendState, old.BlendFactor, old.SampleMask); if (old.BlendState) old.BlendState->Release();
    //ctx->OMSetDepthStencilState(old.DepthStencilState, old.StencilRef); if (old.DepthStencilState) old.DepthStencilState->Release();
    //ctx->PSSetShaderResources(0, 1, &old.PSShaderResource); if (old.PSShaderResource) old.PSShaderResource->Release();
    //ctx->PSSetSamplers(0, 1, &old.PSSampler); if (old.PSSampler) old.PSSampler->Release();
    //ctx->PSSetShader(old.PS, old.PSInstances, old.PSInstancesCount); if (old.PS) old.PS->Release();
    //for (UINT i = 0; i < old.PSInstancesCount; i++) if (old.PSInstances[i]) old.PSInstances[i]->Release();
    //ctx->VSSetShader(old.VS, old.VSInstances, old.VSInstancesCount); if (old.VS) old.VS->Release();
    //ctx->VSSetConstantBuffers(0, 1, &old.VSConstantBuffer); if (old.VSConstantBuffer) old.VSConstantBuffer->Release();
    //ctx->GSSetShader(old.GS, old.GSInstances, old.GSInstancesCount); if (old.GS) old.GS->Release();
    //for (UINT i = 0; i < old.VSInstancesCount; i++) if (old.VSInstances[i]) old.VSInstances[i]->Release();
    //ctx->IASetPrimitiveTopology(old.PrimitiveTopology);
    //ctx->IASetIndexBuffer(old.IndexBuffer, old.IndexBufferFormat, old.IndexBufferOffset); if (old.IndexBuffer) old.IndexBuffer->Release();
    //ctx->IASetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset); if (old.VertexBuffer) old.VertexBuffer->Release();
    //ctx->IASetInputLayout(old.InputLayout); if (old.InputLayout) old.InputLayout->Release();


    stack_hidden_call(RSSetScissorRectsFn(ctx), ctx, old.ScissorRectsCount, old.ScissorRects);
    stack_hidden_call(RSSetViewportsFn(ctx), ctx, old.ViewportsCount, old.Viewports);
    stack_hidden_call(RSSetStateFn(ctx), ctx, old.RS); if (old.RS) stack_hidden_call(ReleaseFn(old.RS), old.RS);
    stack_hidden_call(OMSetBlendStateFn(ctx), ctx, old.BlendState, old.BlendFactor, old.SampleMask); if (old.BlendState) stack_hidden_call(ReleaseFn(old.BlendState), old.BlendState);
    stack_hidden_call(OMSetDepthStencilStateFn(ctx), ctx, old.DepthStencilState, old.StencilRef); if (old.DepthStencilState) stack_hidden_call(ReleaseFn(old.DepthStencilState), old.DepthStencilState);
    stack_hidden_call(PSSetShaderResourcesFn(ctx), ctx, 0, 1, &old.PSShaderResource); if (old.PSShaderResource) stack_hidden_call(ReleaseFn(old.PSShaderResource), old.PSShaderResource);
    stack_hidden_call(PSSetSamplersFn(ctx), ctx, 0, 1, &old.PSSampler); if (old.PSSampler) stack_hidden_call(ReleaseFn(old.PSSampler), old.PSSampler);
    stack_hidden_call(PSSetShaderFn(ctx), ctx, old.PS, old.PSInstances, old.PSInstancesCount); if (old.PS) stack_hidden_call(ReleaseFn(old.PS), old.PS);
    for (UINT i = 0; i < old.PSInstancesCount; i++) if (old.PSInstances[i]) stack_hidden_call(ReleaseFn(old.PSInstances[i]), old.PSInstances[i]);
    stack_hidden_call(VSSetShaderFn(ctx), ctx, old.VS, old.VSInstances, old.VSInstancesCount); if (old.VS) stack_hidden_call(ReleaseFn(old.VS), old.VS);
    stack_hidden_call(VSSetConstantBuffersFn(ctx), ctx, 0, 1, &old.VSConstantBuffer); if (old.VSConstantBuffer) stack_hidden_call(ReleaseFn(old.VSConstantBuffer), old.VSConstantBuffer);
    stack_hidden_call(GSSetShaderFn(ctx), ctx, old.GS, old.GSInstances, old.GSInstancesCount); if (old.GS) stack_hidden_call(ReleaseFn(old.GS), old.GS);
    for (UINT i = 0; i < old.VSInstancesCount; i++) if (old.VSInstances[i]) stack_hidden_call(ReleaseFn(old.VSInstances[i]), old.VSInstances[i]);
    stack_hidden_call(IASetPrimitiveTopologyFn(ctx), ctx, old.PrimitiveTopology);
    stack_hidden_call(IASetIndexBufferFn(ctx), ctx, old.IndexBuffer, old.IndexBufferFormat, old.IndexBufferOffset); if (old.IndexBuffer) stack_hidden_call(ReleaseFn(old.IndexBuffer), old.IndexBuffer);
    stack_hidden_call(IASetVertexBuffersFn(ctx), ctx, 0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset); if (old.VertexBuffer) stack_hidden_call(ReleaseFn(old.VertexBuffer), old.VertexBuffer);
    stack_hidden_call(IASetInputLayoutFn(ctx), ctx, old.InputLayout); if (old.InputLayout) stack_hidden_call(ReleaseFn(old.InputLayout), old.InputLayout);
}

static void xsGui_DX11_CreateFont()
{
    // Build texture atlas
    xsGuiIO& io = xs_Gui::GetIO();

    unsigned char* pixels;
    int width, height;
    io.xsFont->GetFontData_RGBA_32(&pixels, &width, &height);

    // Upload texture to graphics system
    {
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        ID3D11Texture2D* pTexture = NULL;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = pixels;
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        //pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);//28
        stack_hidden_call(CreateTexture2DFn(pd3dDevice), pd3dDevice, &desc, &subResource, &pTexture);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        //pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &pFontTextureView);//38
        //pTexture->Release();
        stack_hidden_call(CreateShaderResourceViewFn(pd3dDevice), pd3dDevice, pTexture, &srvDesc, &pFontTextureView);
        stack_hidden_call(ReleaseFn(pTexture), pTexture);
    }

    // Store our identifier
    io.xsFont->TextureID=(void*)pFontTextureView;
    stack_hidden_call_v(free,pixels);

    // Create texture sampler
    // (Bilinear sampling is required by default. Set 'io.Fonts->Flags |= ImFontAtlasFlags_NoBakedLines' or 'style.AntiAliasedLinesUseTex = false' to allow point/nearest sampling)
    {
        D3D11_SAMPLER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.MipLODBias = 0.f;
        desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        desc.MinLOD = 0.f;
        desc.MaxLOD = 0.f;
        //pd3dDevice->CreateSamplerState(&desc, &pFontSampler);//B8
        stack_hidden_call(CreateSamplerStateFn(pd3dDevice), pd3dDevice, &desc, &pFontSampler);
    }

}

bool    xsGui_DX11_CreateDeviceObjects()
{

    if (!pd3dDevice)
        return false;
    if (pFontSampler)
        xsGui_DX11_InvalidateDeviceObjects();

    // By using D3DCompile() from <d3dcompiler.h> / d3dcompiler.lib, we introduce a dependency to a given version of d3dcompiler_XX.dll (see D3DCOMPILER_DLL_A)
    // If you would like to use this DX11 sample code but remove this dependency you can:
    //  1) compile once, save the compiled shader blobs into a file or source code and pass them to CreateVertexShader()/CreatePixelShader() [preferred solution]
    //  2) use code to detect any version of the DLL and grab a pointer to D3DCompile from the DLL.
    // See https://github.com/ocornut/imgui/pull/638 for sources and details.

    // Create the vertex shader
    {
   //     static const char* vertexShader =
			//VMProtectDecryptStringA("cbuffer vertexBuffer : register(b0) \
   //         {\
   //           float4x4 ProjectionMatrix; \
   //         };\
   //         struct VS_INPUT\
   //         {\
   //           float2 pos : POSITION;\
   //           float4 col : COLOR0;\
   //           float2 uv  : TEXCOORD0;\
   //         };\
   //         \
   //         struct PS_INPUT\
   //         {\
   //           float4 pos : SV_POSITION;\
   //           float4 col : COLOR0;\
   //           float2 uv  : TEXCOORD0;\
   //         };\
   //         \
   //         PS_INPUT main(VS_INPUT input)\
   //         {\
   //           PS_INPUT output;\
   //           output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
   //           output.col = input.col;\
   //           output.uv  = input.uv;\
   //           return output;\
   //         }");
	
        //ID3DBlob* vertexShaderBlob;
        //if (FAILED(D3DCompile(vertexShader, 734, NULL, NULL, NULL, VMProtectDecryptStringA("main"), VMProtectDecryptStringA("vs_4_0"), 0, 0, &vertexShaderBlob, NULL)))
        //    return false; // NB: Pass ID3DBlob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!

		
		//static unsigned char vertexShaderBlob_char[] = { 0x44,0x58,0x42,0x43,0x7A,0x54,0x84,0x96,0xDF,0xD1,0x9E,0x21,0xFD,0x85,0x86,0x3D,0x28,0xD1,0x03,0xAE,0x01,0x00,0x00,0x00,0x6C,0x03,0x00,0x00,0x05,0x00,0x00,0x00,0x34,0x00,0x00,0x00,0x04,0x01,0x00,0x00,0x74,0x01,0x00,0x00,0xE8,0x01,0x00,0x00,0xF0,0x02,0x00,0x00,0x52,0x44,0x45,0x46,0xC8,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x4C,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x1C,0x00,0x00,0x00,0x00,0x04,0xFE,0xFF,0x00,0x01,0x00,0x00,0xA0,0x00,0x00,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x76,0x65,0x72,0x74,0x65,0x78,0x42,0x75,0x66,0x66,0x65,0x72,0x00,0xAB,0xAB,0xAB,0x3C,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x64,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x50,0x72,0x6F,0x6A,0x65,0x63,0x74,0x69,0x6F,0x6E,0x4D,0x61,0x74,0x72,0x69,0x78,0x00,0xAB,0xAB,0xAB,0x03,0x00,0x03,0x00,0x04,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x4D,0x69,0x63,0x72,0x6F,0x73,0x6F,0x66,0x74,0x20,0x28,0x52,0x29,0x20,0x48,0x4C,0x53,0x4C,0x20,0x53,0x68,0x61,0x64,0x65,0x72,0x20,0x43,0x6F,0x6D,0x70,0x69,0x6C,0x65,0x72,0x20,0x31,0x30,0x2E,0x31,0x00,0x49,0x53,0x47,0x4E,0x68,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x00,0x00,0x59,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x5F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x03,0x03,0x00,0x00,0x50,0x4F,0x53,0x49,0x54,0x49,0x4F,0x4E,0x00,0x43,0x4F,0x4C,0x4F,0x52,0x00,0x54,0x45,0x58,0x43,0x4F,0x4F,0x52,0x44,0x00,0x4F,0x53,0x47,0x4E,0x6C,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x5C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x62,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x03,0x0C,0x00,0x00,0x53,0x56,0x5F,0x50,0x4F,0x53,0x49,0x54,0x49,0x4F,0x4E,0x00,0x43,0x4F,0x4C,0x4F,0x52,0x00,0x54,0x45,0x58,0x43,0x4F,0x4F,0x52,0x44,0x00,0xAB,0x53,0x48,0x44,0x52,0x00,0x01,0x00,0x00,0x40,0x00,0x01,0x00,0x40,0x00,0x00,0x00,0x59,0x00,0x00,0x04,0x46,0x8E,0x20,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x5F,0x00,0x00,0x03,0x32,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x5F,0x00,0x00,0x03,0xF2,0x10,0x10,0x00,0x01,0x00,0x00,0x00,0x5F,0x00,0x00,0x03,0x32,0x10,0x10,0x00,0x02,0x00,0x00,0x00,0x67,0x00,0x00,0x04,0xF2,0x20,0x10,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x65,0x00,0x00,0x03,0xF2,0x20,0x10,0x00,0x01,0x00,0x00,0x00,0x65,0x00,0x00,0x03,0x32,0x20,0x10,0x00,0x02,0x00,0x00,0x00,0x68,0x00,0x00,0x02,0x01,0x00,0x00,0x00,0x38,0x00,0x00,0x08,0xF2,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x56,0x15,0x10,0x00,0x00,0x00,0x00,0x00,0x46,0x8E,0x20,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x32,0x00,0x00,0x0A,0xF2,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x46,0x8E,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x46,0x0E,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0xF2,0x20,0x10,0x00,0x00,0x00,0x00,0x00,0x46,0x0E,0x10,0x00,0x00,0x00,0x00,0x00,0x46,0x8E,0x20,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x36,0x00,0x00,0x05,0xF2,0x20,0x10,0x00,0x01,0x00,0x00,0x00,0x46,0x1E,0x10,0x00,0x01,0x00,0x00,0x00,0x36,0x00,0x00,0x05,0x32,0x20,0x10,0x00,0x02,0x00,0x00,0x00,0x46,0x10,0x10,0x00,0x02,0x00,0x00,0x00,0x3E,0x00,0x00,0x01,0x53,0x54,0x41,0x54,0x74,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
		//};
        //static unsigned char* vertexShaderBlob_char = (unsigned char*)VMProtectDecryptStringA("44 58 42 43 7A 54 84 96 DF D1 9E 21 FD 85 86 3D 28 D1 03 AE 01 00 00 00 6C 03 00 00 05 00 00 00 34 00 00 00 04 01 00 00 74 01 00 00 E8 01 00 00 F0 02 00 00 52 44 45 46 C8 00 00 00 01 00 00 00 4C 00 00 00 01 00 00 00 1C 00 00 00 00 04 FE FF 00 01 00 00 A0 00 00 00 3C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 01 00 00 00 76 65 72 74 65 78 42 75 66 66 65 72 00 AB AB AB 3C 00 00 00 01 00 00 00 64 00 00 00 40 00 00 00 00 00 00 00 00 00 00 00 7C 00 00 00 00 00 00 00 40 00 00 00 02 00 00 00 90 00 00 00 00 00 00 00 50 72 6F 6A 65 63 74 69 6F 6E 4D 61 74 72 69 78 00 AB AB AB 03 00 03 00 04 00 04 00 00 00 00 00 00 00 00 00 4D 69 63 72 6F 73 6F 66 74 20 28 52 29 20 48 4C 53 4C 20 53 68 61 64 65 72 20 43 6F 6D 70 69 6C 65 72 20 31 30 2E 31 00 49 53 47 4E 68 00 00 00 03 00 00 00 08 00 00 00 50 00 00 00 00 00 00 00 00 00 00 00 03 00 00 00 00 00 00 00 03 03 00 00 59 00 00 00 00 00 00 00 00 00 00 00 03 00 00 00 01 00 00 00 0F 0F 00 00 5F 00 00 00 00 00 00 00 00 00 00 00 03 00 00 00 02 00 00 00 03 03 00 00 50 4F 53 49 54 49 4F 4E 00 43 4F 4C 4F 52 00 54 45 58 43 4F 4F 52 44 00 4F 53 47 4E 6C 00 00 00 03 00 00 00 08 00 00 00 50 00 00 00 00 00 00 00 01 00 00 00 03 00 00 00 00 00 00 00 0F 00 00 00 5C 00 00 00 00 00 00 00 00 00 00 00 03 00 00 00 01 00 00 00 0F 00 00 00 62 00 00 00 00 00 00 00 00 00 00 00 03 00 00 00 02 00 00 00 03 0C 00 00 53 56 5F 50 4F 53 49 54 49 4F 4E 00 43 4F 4C 4F 52 00 54 45 58 43 4F 4F 52 44 00 AB 53 48 44 52 00 01 00 00 40 00 01 00 40 00 00 00 59 00 00 04 46 8E 20 00 00 00 00 00 04 00 00 00 5F 00 00 03 32 10 10 00 00 00 00 00 5F 00 00 03 F2 10 10 00 01 00 00 00 5F 00 00 03 32 10 10 00 02 00 00 00 67 00 00 04 F2 20 10 00 00 00 00 00 01 00 00 00 65 00 00 03 F2 20 10 00 01 00 00 00 65 00 00 03 32 20 10 00 02 00 00 00 68 00 00 02 01 00 00 00 38 00 00 08 F2 00 10 00 00 00 00 00 56 15 10 00 00 00 00 00 46 8E 20 00 00 00 00 00 01 00 00 00 32 00 00 0A F2 00 10 00 00 00 00 00 46 8E 20 00 00 00 00 00 00 00 00 00 06 10 10 00 00 00 00 00 46 0E 10 00 00 00 00 00 00 00 00 08 F2 20 10 00 00 00 00 00 46 0E 10 00 00 00 00 00 46 8E 20 00 00 00 00 00 03 00 00 00 36 00 00 05 F2 20 10 00 01 00 00 00 46 1E 10 00 01 00 00 00 36 00 00 05 32 20 10 00 02 00 00 00 46 10 10 00 02 00 00 00 3E 00 00 01 53 54 41 54 74 00 00 00 06 00 00 00 01 00 00 00 00 00 00 00 06 00 00 00 03 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 02 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00\n");
		//if (pd3dDevice->CreateVertexShader(vertexShaderBlob_char, sizeof(vertexShaderBlob_char), NULL, &pVertexShader) != S_OK)
        
        char* vertexShaderBlob_char_A = (char*)VMProtectDecryptStringA("44 58 42 43 7A 54 84 96 DF D1 9E 21 FD 85 86 3D 28 D1 03 AE 01 00 00 00 6C 03 00 00 05 00 00 00 34 00 00 00 04 01 00 00 74 01 00 00 E8 01 00 00 F0 02 00 00 52 44 45 46 C8 00 00 00 01 00 00 00 4C 00 00 00 01 00 00 00 1C 00 00 00 00 04 FE FF 00 01 00 00 A0 00 00 00 3C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 01 00 00 00 76 65 72 74 65 78 42 75 66 66 65 72 00 AB AB AB 3C 00 00 00 01 00 00 00 64 00 00 00 40 00 00 00 00 00 00 00 00 00 00 00 7C 00 00 00 00 00 00 00 40 00 00 00 02 00 00 00 90 00 00 00 00 00 00 00 50 72 6F 6A 65 63 74 69 6F 6E 4D 61 74 72 69 78 00 AB AB AB 03 00 03 00 04 00 04 00 00 00 00 00 00 00 00 00 4D 69 63 72 6F 73 6F 66 74 20 28 52 29 20 48 4C 53 4C 20 53 68 61 64 65 72 20 43 6F 6D 70 69 6C 65 72 20 31 30 2E 31 00 49 53 47 4E 68 00 00 00 03 00 00 00 08 00 00 00 50 00 00 00 00 00 00 00 00 00 00 00 03 00 00 00 00 00 00 00 03 03 00 00 59 00 00 00 00 00 00 00 00 00 00 00 03 00 00 00 01 00 00 00 0F 0F 00 00 5F 00 00 00 00 00 00 00 00 00 00 00 03 00 00 00 02 00 00 00 03 03 00 00 50 4F 53 49 54 49 4F 4E 00 43 4F 4C 4F 52 00 54 45 58 43 4F 4F 52 44 00 4F 53 47 4E 6C 00 00 00 03 00 00 00 08 00 00 00 50 00 00 00 00 00 00 00 01 00 00 00 03 00 00 00 00 00 00 00 0F 00 00 00 5C 00 00 00 00 00 00 00 00 00 00 00 03 00 00 00 01 00 00 00 0F 00 00 00 62 00 00 00 00 00 00 00 00 00 00 00 03 00 00 00 02 00 00 00 03 0C 00 00 53 56 5F 50 4F 53 49 54 49 4F 4E 00 43 4F 4C 4F 52 00 54 45 58 43 4F 4F 52 44 00 AB 53 48 44 52 00 01 00 00 40 00 01 00 40 00 00 00 59 00 00 04 46 8E 20 00 00 00 00 00 04 00 00 00 5F 00 00 03 32 10 10 00 00 00 00 00 5F 00 00 03 F2 10 10 00 01 00 00 00 5F 00 00 03 32 10 10 00 02 00 00 00 67 00 00 04 F2 20 10 00 00 00 00 00 01 00 00 00 65 00 00 03 F2 20 10 00 01 00 00 00 65 00 00 03 32 20 10 00 02 00 00 00 68 00 00 02 01 00 00 00 38 00 00 08 F2 00 10 00 00 00 00 00 56 15 10 00 00 00 00 00 46 8E 20 00 00 00 00 00 01 00 00 00 32 00 00 0A F2 00 10 00 00 00 00 00 46 8E 20 00 00 00 00 00 00 00 00 00 06 10 10 00 00 00 00 00 46 0E 10 00 00 00 00 00 00 00 00 08 F2 20 10 00 00 00 00 00 46 0E 10 00 00 00 00 00 46 8E 20 00 00 00 00 00 03 00 00 00 36 00 00 05 F2 20 10 00 01 00 00 00 46 1E 10 00 01 00 00 00 36 00 00 05 32 20 10 00 02 00 00 00 46 10 10 00 02 00 00 00 3E 00 00 01 53 54 41 54 74 00 00 00 06 00 00 00 01 00 00 00 00 00 00 00 06 00 00 00 03 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 02 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00\n");
         static unsigned char vertexShaderBlob_char[876];
        xs_Gui::char_to_value(vertexShaderBlob_char_A, (char*)vertexShaderBlob_char);
        
        if (stack_hidden_call(CreateVertexShaderFn(pd3dDevice), pd3dDevice,vertexShaderBlob_char, sizeof(vertexShaderBlob_char), (ID3D11ClassLinkage*)NULL, &pVertexShader) != S_OK)
        {
            //vertexShaderBlob->Release();
			memset(vertexShaderBlob_char, 0, sizeof(vertexShaderBlob_char));
            return false;
        }
        
        // Create the input layout
        D3D11_INPUT_ELEMENT_DESC local_layout[] =
        {
            { VMProtectDecryptStringA("POSITION"), 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)IM_OFFSETOF(xsDrawVert, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { VMProtectDecryptStringA("TEXCOORD"), 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)IM_OFFSETOF(xsDrawVert, uv),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { VMProtectDecryptStringA("COLOR"),    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (UINT)IM_OFFSETOF(xsDrawVert, col), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        //if (pd3dDevice->CreateInputLayout(local_layout, 3, vertexShaderBlob_char, sizeof(vertexShaderBlob_char), &pInputLayout) != S_OK)
        if (stack_hidden_call(CreateInputLayoutFn(pd3dDevice), pd3dDevice,local_layout, 3, vertexShaderBlob_char, sizeof(vertexShaderBlob_char), &pInputLayout) != S_OK)
        {
            //vertexShaderBlob->Release();
			memset(vertexShaderBlob_char, 0, sizeof(vertexShaderBlob_char));
            return false;
        }
        
        //vertexShaderBlob->Release();
		memset(vertexShaderBlob_char, 0, sizeof(vertexShaderBlob_char));
	
        // Create the constant buffer
        {
            D3D11_BUFFER_DESC desc;
            desc.ByteWidth = sizeof(VERTEX_CONSTANT_BUFFER);
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;
            //pd3dDevice->CreateBuffer(&desc, NULL, &pVertexConstantBuffer);
            stack_hidden_call(CreateBufferFn(pd3dDevice), pd3dDevice, &desc, (D3D11_SUBRESOURCE_DATA*)NULL, &pVertexConstantBuffer);
        }
    }
    
    // Create the pixel shader
    {
   //     static const char* pixelShader =
			//VMProtectDecryptStringA("struct PS_INPUT\
   //         {\
   //         float4 pos : SV_POSITION;\
   //         float4 col : COLOR0;\
   //         float2 uv  : TEXCOORD0;\
   //         };\
   //         sampler sampler0;\
   //         Texture2D texture0;\
   //         \
   //         float4 main(PS_INPUT input) : SV_Target\
   //         {\
   //         float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
   //         return out_col; \
   //         }");

   //     ID3DBlob* pixelShaderBlob;
   //     if (FAILED(D3DCompile(pixelShader, 402, NULL, NULL, NULL, VMProtectDecryptStringA("main"), VMProtectDecryptStringA("ps_4_0"), 0, 0, &pixelShaderBlob, NULL)))
   //         return false; // NB: Pass ID3DBlob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!
		static unsigned char pixelShader_char[] = { 0x44,0x58,0x42,0x43,0x9E,0xCE,0x85,0x72,0xA7,0x97,0x52,0xB4,0x6D,0xC4,0x28,0xFA,0x10,0xC0,0xD2,0xC1,0x01,0x00,0x00,0x00,0x94,0x02,0x00,0x00,0x05,0x00,0x00,0x00,0x34,0x00,0x00,0x00,0xD4,0x00,0x00,0x00,0x48,0x01,0x00,0x00,0x7C,0x01,0x00,0x00,0x18,0x02,0x00,0x00,0x52,0x44,0x45,0x46,0x98,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x1C,0x00,0x00,0x00,0x00,0x04,0xFF,0xFF,0x00,0x01,0x00,0x00,0x6E,0x00,0x00,0x00,0x5C,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x65,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x73,0x61,0x6D,0x70,0x6C,0x65,0x72,0x30,0x00,0x74,0x65,0x78,0x74,0x75,0x72,0x65,0x30,0x00,0x4D,0x69,0x63,0x72,0x6F,0x73,0x6F,0x66,0x74,0x20,0x28,0x52,0x29,0x20,0x48,0x4C,0x53,0x4C,0x20,0x53,0x68,0x61,0x64,0x65,0x72,0x20,0x43,0x6F,0x6D,0x70,0x69,0x6C,0x65,0x72,0x20,0x31,0x30,0x2E,0x31,0x00,0xAB,0xAB,0x49,0x53,0x47,0x4E,0x6C,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x5C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x62,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x03,0x03,0x00,0x00,0x53,0x56,0x5F,0x50,0x4F,0x53,0x49,0x54,0x49,0x4F,0x4E,0x00,0x43,0x4F,0x4C,0x4F,0x52,0x00,0x54,0x45,0x58,0x43,0x4F,0x4F,0x52,0x44,0x00,0xAB,0x4F,0x53,0x47,0x4E,0x2C,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x53,0x56,0x5F,0x54,0x61,0x72,0x67,0x65,0x74,0x00,0xAB,0xAB,0x53,0x48,0x44,0x52,0x94,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x25,0x00,0x00,0x00,0x5A,0x00,0x00,0x03,0x00,0x60,0x10,0x00,0x00,0x00,0x00,0x00,0x58,0x18,0x00,0x04,0x00,0x70,0x10,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0x00,0x00,0x62,0x10,0x00,0x03,0xF2,0x10,0x10,0x00,0x01,0x00,0x00,0x00,0x62,0x10,0x00,0x03,0x32,0x10,0x10,0x00,0x02,0x00,0x00,0x00,0x65,0x00,0x00,0x03,0xF2,0x20,0x10,0x00,0x00,0x00,0x00,0x00,0x68,0x00,0x00,0x02,0x01,0x00,0x00,0x00,0x45,0x00,0x00,0x09,0xF2,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x46,0x10,0x10,0x00,0x02,0x00,0x00,0x00,0x46,0x7E,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x10,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x00,0x07,0xF2,0x20,0x10,0x00,0x00,0x00,0x00,0x00,0x46,0x0E,0x10,0x00,0x00,0x00,0x00,0x00,0x46,0x1E,0x10,0x00,0x01,0x00,0x00,0x00,0x3E,0x00,0x00,0x01,0x53,0x54,0x41,0x54,0x74,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
		};
        //static char* pixelShader_char_A = (char*)VMProtectDecryptStringA("44 58 42 43 9E CE 85 72 A7 97 52 B4 6D C4 28 FA 10 C0 D2 C1 01 00 00 00 94 02 00 00 05 00 00 00 34 00 00 00 D4 00 00 00 48 01 00 00 7C 01 00 00 18 02 00 00 52 44 45 46 98 00 00 00 00 00 00 00 00 00 00 00 02 00 00 00 1C 00 00 00 00 04 FF FF 00 01 00 00 6E 00 00 00 5C 00 00 00 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 65 00 00 00 02 00 00 00 05 00 00 00 04 00 00 00 FF FF FF FF 00 00 00 00 01 00 00 00 0C 00 00 00 73 61 6D 70 6C 65 72 30 00 74 65 78 74 75 72 65 30 00 4D 69 63 72 6F 73 6F 66 74 20 28 52 29 20 48 4C 53 4C 20 53 68 61 64 65 72 20 43 6F 6D 70 69 6C 65 72 20 31 30 2E 31 00 AB AB 49 53 47 4E 6C 00 00 00 03 00 00 00 08 00 00 00 50 00 00 00 00 00 00 00 01 00 00 00 03 00 00 00 00 00 00 00 0F 00 00 00 5C 00 00 00 00 00 00 00 00 00 00 00 03 00 00 00 01 00 00 00 0F 0F 00 00 62 00 00 00 00 00 00 00 00 00 00 00 03 00 00 00 02 00 00 00 03 03 00 00 53 56 5F 50 4F 53 49 54 49 4F 4E 00 43 4F 4C 4F 52 00 54 45 58 43 4F 4F 52 44 00 AB 4F 53 47 4E 2C 00 00 00 01 00 00 00 08 00 00 00 20 00 00 00 00 00 00 00 00 00 00 00 03 00 00 00 00 00 00 00 0F 00 00 00 53 56 5F 54 61 72 67 65 74 00 AB AB 53 48 44 52 94 00 00 00 40 00 00 00 25 00 00 00 5A 00 00 03 00 60 10 00 00 00 00 00 58 18 00 04 00 70 10 00 00 00 00 00 55 55 00 00 62 10 00 03 F2 10 10 00 01 00 00 00 62 10 00 03 32 10 10 00 02 00 00 00 65 00 00 03 F2 20 10 00 00 00 00 00 68 00 00 02 01 00 00 00 45 00 00 09 F2 00 10 00 00 00 00 00 46 10 10 00 02 00 00 00 46 7E 10 00 00 00 00 00 00 60 10 00 00 00 00 00 38 00 00 07 F2 20 10 00 00 00 00 00 46 0E 10 00 00 00 00 00 46 1E 10 00 01 00 00 00 3E 00 00 01 53 54 41 54 74 00 00 00 03 00 00 00 01 00 00 00 00 00 00 00 03 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00\n");
        //static unsigned char pixelShader_char[660];
        //xs_Gui::char_to_value(pixelShader_char_A, (char*)pixelShader_char);
        
        //if (pd3dDevice->CreatePixelShader(pixelShader_char, sizeof(pixelShader_char), NULL, &pPixelShader) != S_OK)
        if (stack_hidden_call(CreatePixelShaderFn(pd3dDevice), pd3dDevice,pixelShader_char, sizeof(pixelShader_char), (ID3D11ClassLinkage*)NULL, &pPixelShader) != S_OK)
        {
            //pixelShaderBlob->Release();
			memset(pixelShader_char, 0, sizeof(pixelShader_char));
            return false;
        }
        //pixelShaderBlob->Release();
		memset(pixelShader_char, 0, sizeof(pixelShader_char));
    }
    
    // Create the blending setup
    {
        D3D11_BLEND_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.AlphaToCoverageEnable = false;
        desc.RenderTarget[0].BlendEnable = true;
        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        //pd3dDevice->CreateBlendState(&desc, &pBlendState);
        stack_hidden_call(CreateBlendStateFn(pd3dDevice), pd3dDevice, &desc, &pBlendState);
    }

    // Create the rasterizer state
    {
        D3D11_RASTERIZER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.FillMode = D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_NONE;
        desc.ScissorEnable = true;
        desc.DepthClipEnable = true;
        //pd3dDevice->CreateRasterizerState(&desc, &pRasterizerState);
        stack_hidden_call(CreateRasterizerStateFn(pd3dDevice), pd3dDevice, &desc, &pRasterizerState);
    }
    
    // Create depth-stencil State
    {
        D3D11_DEPTH_STENCIL_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.DepthEnable = false;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
        desc.StencilEnable = false;
        desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        desc.BackFace = desc.FrontFace;
        //pd3dDevice->CreateDepthStencilState(&desc, &pDepthStencilState);
        stack_hidden_call(CreateDepthStencilStateFn(pd3dDevice), pd3dDevice,&desc, &pDepthStencilState);
    }
    
    xsGui_DX11_CreateFont();
    
    return true;
}

void    xsGui_DX11_InvalidateDeviceObjects()
{

    if (!pd3dDevice)
        return;

    //if (pFontSampler) { pFontSampler->Release(); pFontSampler = NULL; }
    //if (pFontTextureView) { pFontTextureView->Release(); pFontTextureView = NULL; xs_Gui::GetIO().xsFont->TextureID=NULL; } // We copied data->pFontTextureView to io.Fonts->TexID so let's clear that as well.
    //if (pIB) { pIB->Release(); pIB = NULL; }
    //if (pVB) { pVB->Release(); pVB = NULL; }
    //if (pBlendState) { pBlendState->Release(); pBlendState = NULL; }
    //if (pDepthStencilState) { pDepthStencilState->Release(); pDepthStencilState = NULL; }
    //if (pRasterizerState) { pRasterizerState->Release(); pRasterizerState = NULL; }
    //if (pPixelShader) { pPixelShader->Release(); pPixelShader = NULL; }
    //if (pVertexConstantBuffer) { pVertexConstantBuffer->Release(); pVertexConstantBuffer = NULL; }
    //if (pInputLayout) { pInputLayout->Release(); pInputLayout = NULL; }
    //if (pVertexShader) { pVertexShader->Release(); pVertexShader = NULL; }


    if (pFontSampler) { stack_hidden_call(ReleaseFn(pFontSampler), pFontSampler); pFontSampler = NULL; }
    if (pFontTextureView) { stack_hidden_call(ReleaseFn(pFontTextureView), pFontTextureView); pFontTextureView = NULL; xs_Gui::GetIO().xsFont->TextureID = NULL; } // We copied data->pFontTextureView to io.Fonts->TexID so let's clear that as well.
    if (pIB) { stack_hidden_call(ReleaseFn(pIB), pIB); pIB = NULL; }
    if (pVB) { stack_hidden_call(ReleaseFn(pVB), pVB); pVB = NULL; }
    if (pBlendState) { stack_hidden_call(ReleaseFn(pBlendState), pBlendState); pBlendState = NULL; }
    if (pDepthStencilState) { stack_hidden_call(ReleaseFn(pDepthStencilState), pDepthStencilState); pDepthStencilState = NULL; }
    if (pRasterizerState) { stack_hidden_call(ReleaseFn(pRasterizerState), pRasterizerState); pRasterizerState = NULL; }
    if (pPixelShader) { stack_hidden_call(ReleaseFn(pPixelShader), pPixelShader); pPixelShader = NULL; }
    if (pVertexConstantBuffer) { stack_hidden_call(ReleaseFn(pVertexConstantBuffer), pVertexConstantBuffer); pVertexConstantBuffer = NULL; }
    if (pInputLayout) { stack_hidden_call(ReleaseFn(pInputLayout), pInputLayout); pInputLayout = NULL; }
    if (pVertexShader) { stack_hidden_call(ReleaseFn(pVertexShader), pVertexShader); pVertexShader = NULL; }
}

bool    xsGui_DX11_Init(ID3D11Device* device, ID3D11DeviceContext* device_context)
{

    // Get factory from device
    IDXGIDevice* pDXGIDevice = NULL;
    IDXGIAdapter* pDXGIAdapter = NULL;
    IDXGIFactory* _pFactory = NULL;

    //if (device->QueryInterface(IID_PPV_ARGS(&pDXGIDevice)) == S_OK)
    //    if (pDXGIDevice->GetParent(IID_PPV_ARGS(&pDXGIAdapter)) == S_OK)
    //        if (pDXGIAdapter->GetParent(IID_PPV_ARGS(&_pFactory)) == S_OK)
    //        {
    //            pd3dDevice = device;
    //            pd3dDeviceContext = device_context;
    //            pFactory = _pFactory;
    //        }
    if (stack_hidden_call(QueryInterfaceFn(device), device,IID_PPV_ARGS(&pDXGIDevice)) == S_OK)
        if (stack_hidden_call(GetParentFn(pDXGIDevice), pDXGIDevice, IID_PPV_ARGS(&pDXGIAdapter)) == S_OK)
            if (stack_hidden_call(GetParentFn(pDXGIAdapter), pDXGIAdapter,IID_PPV_ARGS(&_pFactory)) == S_OK)
            {
                pd3dDevice = device;
                pd3dDeviceContext = device_context;
                pFactory = _pFactory;
            }
    //if (pDXGIDevice) pDXGIDevice->Release();
    //if (pDXGIAdapter) pDXGIAdapter->Release();
    if (pDXGIDevice)     stack_hidden_call(ReleaseFn(pDXGIDevice), pDXGIDevice);
    if (pDXGIAdapter)     stack_hidden_call(ReleaseFn(pDXGIAdapter), pDXGIAdapter);
    //pd3dDevice->AddRef();
    //pd3dDeviceContext->AddRef();
    stack_hidden_call(AddRefFn(pd3dDevice), pd3dDevice);
    stack_hidden_call(AddRefFn(pd3dDeviceContext), pd3dDeviceContext);
    return true;
}



void xsGui_DX11_NewFrame()
{

    if (!pFontSampler)
        xsGui_DX11_CreateDeviceObjects();

}
