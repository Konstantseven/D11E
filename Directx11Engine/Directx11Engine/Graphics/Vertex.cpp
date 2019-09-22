#include "Vertex.h"

Vertex::Vertex(float&& x, float&& y) {
	position = DirectX::XMFLOAT2(std::forward<float>(x), std::forward<float>(y));
}
