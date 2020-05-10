#include "ErrorLogger.h"

int helpers::error_logger::Log(std::string message) noexcept {
	const std::string errorMessage = "Error: " + message;
	return MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_ICONERROR);
}

int helpers::error_logger::Log(HRESULT hResult, std::string message) noexcept {
	const _com_error error(hResult);
	const std::wstring errorMessage = L"Error: " + converter::StringToWString(message) + L"\n" + error.ErrorMessage();
	return MessageBoxW(NULL, errorMessage.c_str(), L"Error", MB_ICONERROR);
}

int helpers::error_logger::Log(HRESULT hResult, std::wstring message) noexcept {
	const _com_error error(hResult);
	const std::wstring errorMessage = L"Error: " + message + L"\n" + error.ErrorMessage();
	return MessageBoxW(NULL, errorMessage.c_str(), L"Error", MB_ICONERROR);
}
