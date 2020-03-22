#pragma once

#include <string>

class StringConverter
{
	StringConverter() = delete;
public:
	static std::wstring StringToWide(std::string _string);
};

