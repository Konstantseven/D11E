#pragma once

#include <DirectXMath.h>
#include <utility>


class Vertex
{
public:
	Vertex() {};
	explicit Vertex(float&& x, float&& y);
private:
	DirectX::XMFLOAT2 position;
};

