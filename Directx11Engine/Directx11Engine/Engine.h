#pragma once

#include "WindowContainer.h"

class Engine : WindowContainer
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int heigh);
	bool ProcessMessages();

	void Update();
	void RenderFrame();
};

