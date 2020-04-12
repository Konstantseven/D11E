#pragma once

#include "gtest/gtest.h"
#include <Helpers/StringConverter.h>

class TestStringConverter : public testing::Test {
protected:
	const std::string  MESSAGE_STRING  = "Default Message";
	const std::wstring MESSAGE_WSTRING = L"Default Message";
};

TEST_F(TestStringConverter, TestStringToWide) {
	EXPECT_EQ(StringConverter::StringToWide(MESSAGE_STRING), MESSAGE_WSTRING);
}

TEST_F(TestStringConverter, TestStringToWideReturnType) {
	EXPECT_EQ(typeid(StringConverter::StringToWide(MESSAGE_STRING)), typeid(MESSAGE_WSTRING));
}


