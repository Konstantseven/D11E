#pragma once

#include <DirectXMath.h>
#include <utility>

class Vertex
{
public:
	Vertex() = delete;
	explicit Vertex(float&& x, float&& y, float&& z, float&& r, float&& g, float&& b);
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
};

