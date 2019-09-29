#pragma once

#include <DirectXMath.h>
#include <utility>

class Vertex
{
public:
	Vertex() = delete;
	explicit Vertex(float&& x, float&& y, float&& r, float&& g, float&& b);
private:
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT3 color;
};

