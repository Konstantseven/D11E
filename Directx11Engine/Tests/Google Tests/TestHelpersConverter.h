#pragma once

#include "gtest/gtest.h"
#include <Helpers/Converter.h>
#include <map>

namespace hcon = helpers::converter;

class TestConverter : public testing::Test 
{
protected:
	const std::string  MESSAGE_STRING  = "Default Message";
	const std::wstring MESSAGE_WSTRING = L"Default Message";

	const std::map<float, float> DegRadPairs = {
	   //Degrees | Radians
		{0.0f,     0.0f},
		{90.0f,    1.5708f},
		{180.0f,   3.14159f},
		{270.0f,   4.71239f},
		{360.0f,   6.28319f}
	};

	// Comparing two floats
	bool cmpf(const float f_1, const float f_2, const float epsilon = 0.005f) const noexcept {
		return fabs(f_1 - f_2) < epsilon;
	}
};

TEST_F(TestConverter, TestStringToWString) {
	// Verify StringToWString() convert to right wstring

	EXPECT_EQ(hcon::StringToWString(MESSAGE_STRING), MESSAGE_WSTRING);
}

TEST_F(TestConverter, TestStringToWStringReturnType) {
	// Verify StringToWString() convert to string of the wstring type

	EXPECT_EQ(typeid(hcon::StringToWString(MESSAGE_STRING)), typeid(MESSAGE_WSTRING));
}

TEST_F(TestConverter, TestDegToRadConverter) {
	// Verify TestDegToRadConverter() converty degrees to radians as expected

	for (const auto& pair : DegRadPairs) {
		const float rad = hcon::DegToRad(pair.first);
		EXPECT_TRUE(cmpf(rad, pair.second));
	}
}

TEST_F(TestConverter, TestRadToDegConverter) {
	// Verify TestDegToRadConverter() converty radians to degress as expected

	for (const auto& pair : DegRadPairs) {
		const float deg = hcon::RadToDeg(pair.second);
		EXPECT_TRUE(cmpf(deg, pair.first));
	}
}
