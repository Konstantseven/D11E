#include "Vertex.h"

Vertex::Vertex(float&& x, float&& y, float&& r, float&& g, float&& b) {
	position = DirectX::XMFLOAT2(std::forward<float>(x), std::forward<float>(y));
	color = DirectX::XMFLOAT3(std::forward<float>(r), std::forward<float>(g), std::forward<float>(b));
}
