#include "Converter.h"

float Converter::RadToDeg(const float rad) {
	return (rad * 180) / DirectX::XM_PI;
}

float Converter::DegToRad(const float deg) {
	return (deg * DirectX::XM_PI) / 180;
}