#pragma once
//ID3D11DeviceContext*

#define ReleaseFn(x) (*(ULONG(__stdcall*)(decltype(x)))((*(UINT64**)x)[0x2]))

#define GetBufferFn(x) (*(HRESULT(__stdcall*)(decltype(x),UINT,REFIID,void **))((*(UINT64**)x)[0x9]))
#define RSSetViewportsFn(x) (*(HRESULT(__stdcall*)(decltype(x),UINT,const D3D11_VIEWPORT *))((*(UINT64**)x)[0x2C]))
#define IASetInputLayoutFn(x) (*(HRESULT(__stdcall*)(decltype(x),ID3D11InputLayout *))((*(UINT64**)x)[0x11]))
#define IASetVertexBuffersFn(x) (*(HRESULT(__stdcall*)(decltype(x),UINT,UINT,ID3D11Buffer *const *,const UINT *,const UINT *))((*(UINT64**)x)[0x12]))
#define IASetIndexBufferFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11Buffer *,DXGI_FORMAT,UINT))((*(UINT64**)x)[0x13]))
#define IASetPrimitiveTopologyFn(x) (*(HRESULT(__stdcall*)(decltype(x), D3D11_PRIMITIVE_TOPOLOGY))((*(UINT64**)x)[0x18]))
#define VSSetShaderFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11VertexShader *,ID3D11ClassInstance *const *,UINT))((*(UINT64**)x)[0xB]))
#define VSSetConstantBuffersFn(x) (*(HRESULT(__stdcall*)(decltype(x), UINT,UINT,ID3D11Buffer *const *))((*(UINT64**)x)[0x7]))
#define PSSetShaderFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11PixelShader *,ID3D11ClassInstance *const *,UINT))((*(UINT64**)x)[0x9]))
#define PSSetSamplersFn(x) (*(HRESULT(__stdcall*)(decltype(x), UINT,UINT,ID3D11SamplerState *const *))((*(UINT64**)x)[0xA]))
#define GSSetShaderFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11GeometryShader *,ID3D11ClassInstance *const *,UINT))((*(UINT64**)x)[0x17]))
#define HSSetShaderFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11HullShader *,ID3D11ClassInstance *const *,UINT))((*(UINT64**)x)[0x3C]))
#define DSSetShaderFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11DomainShader *,ID3D11ClassInstance *const *,UINT))((*(UINT64**)x)[0x40]))
#define CSSetShaderFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11ComputeShader *,ID3D11ClassInstance *const *,UINT))((*(UINT64**)x)[0x45]))
#define OMSetBlendStateFn(x) (*(HRESULT(__stdcall*)(decltype(x),ID3D11BlendState *,const FLOAT*,UINT))((*(UINT64**)x)[0x23]))
#define OMSetDepthStencilStateFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11DepthStencilState *,UINT))((*(UINT64**)x)[0x24]))
#define RSSetStateFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11RasterizerState *))((*(UINT64**)x)[0x2B]))

#define RSGetScissorRectsFn(x) (*(HRESULT(__stdcall*)(decltype(x), UINT*,D3D11_RECT *))((*(UINT64**)x)[0x60]))
#define RSGetViewportsFn(x) (*(HRESULT(__stdcall*)(decltype(x), UINT*,D3D11_VIEWPORT *))((*(UINT64**)x)[0x5F]))
#define RSGetStateFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11RasterizerState **))((*(UINT64**)x)[0x5E]))
#define OMGetBlendStateFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11BlendState **,FLOAT *,UINT*))((*(UINT64**)x)[0x5B]))
#define OMGetDepthStencilStateFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11DepthStencilState **,UINT*))((*(UINT64**)x)[0x5C]))
#define PSGetShaderResourcesFn(x) (*(HRESULT(__stdcall*)(decltype(x), UINT,UINT,ID3D11ShaderResourceView **))((*(UINT64**)x)[0x49]))
#define PSGetSamplersFn(x) (*(HRESULT(__stdcall*)(decltype(x), UINT,UINT,ID3D11SamplerState **))((*(UINT64**)x)[0x4B]))
#define PSGetShaderFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11PixelShader **,ID3D11ClassInstance **,UINT *))((*(UINT64**)x)[0x4A]))
#define VSGetShaderFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11VertexShader **,ID3D11ClassInstance **,UINT*))((*(UINT64**)x)[0x4C]))
#define VSGetConstantBuffersFn(x) (*(HRESULT(__stdcall*)(decltype(x), UINT,UINT,ID3D11Buffer **))((*(UINT64**)x)[0x48]))
#define GSGetShaderFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11GeometryShader **,ID3D11ClassInstance **,UINT *))((*(UINT64**)x)[0x52]))
#define IAGetPrimitiveTopologyFn(x) (*(HRESULT(__stdcall*)(decltype(x), D3D11_PRIMITIVE_TOPOLOGY *))((*(UINT64**)x)[0x53]))
#define IAGetIndexBufferFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11Buffer **,DXGI_FORMAT *,UINT *))((*(UINT64**)x)[0x50]))
#define IAGetVertexBuffersFn(x) (*(HRESULT(__stdcall*)(decltype(x), UINT,UINT,ID3D11Buffer **,UINT *,UINT *))((*(UINT64**)x)[0x4F]))
#define IAGetInputLayoutFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11InputLayout **))((*(UINT64**)x)[0x4E]))



#define MapFn(x) (*(HRESULT(__stdcall*)(decltype(x),ID3D11Resource*,UINT,D3D11_MAP,UINT,D3D11_MAPPED_SUBRESOURCE*))((*(UINT64**)x)[0xE]))
#define UnmapFn(x) (*(HRESULT(__stdcall*)(decltype(x), ID3D11Resource *,UINT))((*(UINT64**)x)[0xF]))
#define RSSetScissorRectsFn(x) (*(HRESULT(__stdcall*)(decltype(x), UINT,const D3D11_RECT *))((*(UINT64**)x)[0x2D]))
#define PSSetShaderResourcesFn(x) (*(HRESULT(__stdcall*)(decltype(x), UINT,UINT, ID3D11ShaderResourceView *const *))((*(UINT64**)x)[0x8]))
#define DrawIndexedFn(x) (*(HRESULT(__stdcall*)(decltype(x),  UINT,UINT,INT))((*(UINT64**)x)[0xC]))



//IDXGISwapChain*
#define OMSetRenderTargetsFn(x) (*(void(__stdcall*)(decltype(x),UINT, ID3D11RenderTargetView *const *, ID3D11DepthStencilView *))((*(UINT64**)x)[0x21]))
#define ClearRenderTargetViewFn(x) (*(void(__stdcall*)(decltype(x),ID3D11RenderTargetView *,const FLOAT*))((*(UINT64**)x)[0x32]))
#define PresentFn(x) (*(HRESULT(__stdcall*)(decltype(x), UINT, UINT))((*(UINT64**)x)[8]))
#define GetDescFn(x) (*(HRESULT(__stdcall*)(decltype(x), DXGI_SWAP_CHAIN_DESC*))((*(UINT64**)x)[0xC]))
#define ResizeBuffersFn(x) (*(HRESULT(__stdcall*)(decltype(x),UINT,UINT,UINT,DXGI_FORMAT,UINT))((*(UINT64**)x)[0xD]))
#define GetDeviceFn(x) (*(HRESULT(__stdcall*)(decltype(x), REFIID,void **))((*(UINT64**)x)[0x7]))


//ID3D11Device*
#define GetImmediateContextFn(x) (*(void(__stdcall*)(decltype(x),ID3D11DeviceContext **))((*(UINT64**)x)[0x28]))
#define CreateRenderTargetViewFn(x) (*(HRESULT(__stdcall*)(decltype(x),ID3D11Resource*, const D3D11_RENDER_TARGET_VIEW_DESC *,ID3D11RenderTargetView **))((*(UINT64**)x)[0x9]))
#define QueryInterfaceFn(x) (*(HRESULT(__stdcall*)(decltype(x),REFIID, void **))((*(UINT64**)x)[0x0]))
#define CreateInputLayoutFn(x) (*(HRESULT(__stdcall*)(decltype(x),const D3D11_INPUT_ELEMENT_DESC*,UINT,const void*,SIZE_T,ID3D11InputLayout**))((*(UINT64**)x)[0xB]))
#define CreateBufferFn(x) (*(HRESULT(__stdcall*)(decltype(x),const D3D11_BUFFER_DESC*,const D3D11_SUBRESOURCE_DATA*,ID3D11Buffer**))((*(UINT64**)x)[0x3]))
#define CreatePixelShaderFn(x) (*(HRESULT(__stdcall*)(decltype(x),const void*,SIZE_T,ID3D11ClassLinkage*,ID3D11PixelShader**))((*(UINT64**)x)[0xF]))
#define CreateBlendStateFn(x) (*(HRESULT(__stdcall*)(decltype(x),const D3D11_BLEND_DESC *, ID3D11BlendState **))((*(UINT64**)x)[0x14]))
#define CreateRasterizerStateFn(x) (*(HRESULT(__stdcall*)(decltype(x),const D3D11_RASTERIZER_DESC*, ID3D11RasterizerState **))((*(UINT64**)x)[0x16]))
#define CreateDepthStencilStateFn(x) (*(HRESULT(__stdcall*)(decltype(x),const D3D11_DEPTH_STENCIL_DESC*, ID3D11DepthStencilState **))((*(UINT64**)x)[0x15]))
#define CreateTexture2DFn(x) (*(HRESULT(__stdcall*)(decltype(x),const D3D11_TEXTURE2D_DESC*, const D3D11_SUBRESOURCE_DATA*,ID3D11Texture2D**))((*(UINT64**)x)[0x5]))
#define CreateShaderResourceViewFn(x) (*(HRESULT(__stdcall*)(decltype(x),ID3D11Resource*, const D3D11_SHADER_RESOURCE_VIEW_DESC* ,ID3D11ShaderResourceView**))((*(UINT64**)x)[0x7]))
#define CreateSamplerStateFn(x) (*(HRESULT(__stdcall*)(decltype(x),const D3D11_SAMPLER_DESC *, ID3D11SamplerState **))((*(UINT64**)x)[0x17]))




#define GetParentFn(x) (*(HRESULT(__stdcall*)(decltype(x),REFIID, void **))((*(UINT64**)x)[0x6]))
#define AddRefFn(x) (*(ULONG(__stdcall*)(decltype(x)))((*(UINT64**)x)[0x1]))
#define CreateVertexShaderFn(x) (*(ULONG(__stdcall*)(decltype(x),const void*,SIZE_T,ID3D11ClassLinkage*,ID3D11VertexShader**))((*(UINT64**)x)[0xC]))


