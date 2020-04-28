#pragma once

#include <DirectXMath.h>

struct CB_VS_VERTEXSHADER 
{
	DirectX::XMMATRIX matrix_dxxmm;
};

struct CB_PS_PIXELSHADER
{
	float alpha = 1.0f;
};
