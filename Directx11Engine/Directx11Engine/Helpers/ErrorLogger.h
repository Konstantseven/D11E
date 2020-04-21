#pragma once

#include <Windows.h>
#include <comdef.h>
#include "StringConverter.h"

class ErrorLogger
{
	ErrorLogger() = delete;
	ErrorLogger(const ErrorLogger&) = delete;
	ErrorLogger& operator=(ErrorLogger&) = delete;

public:	
	static int Log(std::string message) noexcept {
		std::string errorMessage = "Error: " + message;
		return MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_ICONERROR);
	}

	static int Log(HRESULT hResult, std::string message) noexcept {
		_com_error error(hResult);
		std::wstring errorMessage = L"Error: " + StringConverter::StringToWide(message) + L"\n" + error.ErrorMessage();
		return MessageBoxW(NULL, errorMessage.c_str(), L"Error", MB_ICONERROR);
	}

	static int Log(HRESULT hResult, std::wstring message) noexcept {
		_com_error error(hResult);
		std::wstring errorMessage = L"Error: " + message + L"\n" + error.ErrorMessage();
		return MessageBoxW(NULL, errorMessage.c_str(), L"Error", MB_ICONERROR);
	}
};

