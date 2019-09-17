#pragma once

#include <DirectXMath.h>

class Vertex
{
	Vertex() {};
	explicit Vertex(float x, float y) : pos(x, y) {};

	DirectX::XMFLOAT2 pos;
};

