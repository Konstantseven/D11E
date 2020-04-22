#pragma once

#include <Windows.h>
#include <comdef.h>
#include "Converter.h"

namespace helpers
{
	namespace error_logger
	{
		int Log(::std::string message) noexcept;
		int Log(HRESULT hResult, std::string message) noexcept;
		int Log(HRESULT hResult, std::wstring message) noexcept;
	}
}
