#include "Vertex.h"

Vertex::Vertex(float&& x, float&& y, float&& z, float&& u, float&& v) {
	position = DirectX::XMFLOAT3(std::forward<float>(x), std::forward<float>(y), std::forward<float>(z));
	textureCoordinate = DirectX::XMFLOAT2(std::forward<float>(u), std::forward<float>(v));
}
