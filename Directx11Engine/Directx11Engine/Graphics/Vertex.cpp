#include "Vertex.h"

Vertex::Vertex(float&& x, float&& y, float&& z,float&& r, float&& g, float&& b) {
	position = DirectX::XMFLOAT3(std::forward<float>(x), std::forward<float>(y), std::forward<float>(z));
	color = DirectX::XMFLOAT3(std::forward<float>(r), std::forward<float>(g), std::forward<float>(b));
}
