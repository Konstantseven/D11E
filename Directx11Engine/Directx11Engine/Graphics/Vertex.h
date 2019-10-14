#pragma once

#include <DirectXMath.h>
#include <utility>

class Vertex
{
public:
	Vertex() = delete;
	explicit Vertex(float&& x, float&& y, float&& z, float&& u, float&& v);
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 textureCoordinate;
};

