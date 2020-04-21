#pragma once

#include <string>

class StringConverter
{
	StringConverter() = delete;
	StringConverter(const StringConverter&) = delete;
	StringConverter& operator=(StringConverter&) = delete;

public:
	inline static std::wstring StringToWide(const std::string& _string) noexcept {
		return std::wstring(_string.begin(), _string.end());
	}
};

