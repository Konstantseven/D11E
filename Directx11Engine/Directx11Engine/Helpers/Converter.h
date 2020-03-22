#pragma once

#include <DirectXMath.h>

class Converter
{
	Converter() = delete;
public:
	static float RadToDeg(const float rad);
	static float DegToRad(const float deg);
};

