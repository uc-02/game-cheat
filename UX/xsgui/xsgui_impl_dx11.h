#pragma once
#include "xsgui.h"
#include <d3d11.h>


void xsGui_DX11_NewFrame();
void xsGui_DX11_RenderDrawData(xs_DrawData* draw_data);
bool xsGui_DX11_Init(ID3D11Device* device, ID3D11DeviceContext* device_context);

void xsGui_DX11_InvalidateDeviceObjects();
bool xsGui_DX11_CreateDeviceObjects();