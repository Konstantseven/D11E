#pragma once

#include <DirectXMath.h>
#include <string>

namespace helpers 
{
	namespace converter
	{
		constexpr float RadToDeg(const float rad) noexcept {
			return (rad * 180) / DirectX::XM_PI;
		}

		constexpr float DegToRad(const float deg) noexcept {
			return (deg * DirectX::XM_PI) / 180;
		}

		inline std::wstring StringToWString(const std::string& string) noexcept {
			return std::wstring(string.begin(), string.end());
		}
	}
}
