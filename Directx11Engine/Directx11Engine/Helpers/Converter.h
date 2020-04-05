#pragma once

#include <DirectXMath.h>

class Converter
{
	Converter() = delete;
public:
	constexpr static float RadToDeg(const float rad) noexcept {
		return (rad * 180) / DirectX::XM_PI;
	};
	constexpr static float DegToRad(const float deg) noexcept {
		return (deg * DirectX::XM_PI) / 180;
	};
};

