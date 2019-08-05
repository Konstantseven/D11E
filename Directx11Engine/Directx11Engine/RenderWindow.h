#pragma once
#include "ErrorLogger.h"
#include <memory>

class WindowContainer;

class RenderWindow
{
public:
	bool Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessages();
	~RenderWindow();
private:
	void RegisterWindowClass();
	HWND handle = NULL; //Handle to this window
	HINSTANCE hInstance = NULL; //Handle to application instance
	std::string windowTitle = "";
	std::wstring windowTitleWide = L""; //Wide string representation of window title
	std::string windowClass = "";
	std::wstring windowClassWide = L""; //Wide string representation of window class name
	int width = 0;
	int height = 0;
};
