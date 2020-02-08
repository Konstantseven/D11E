#include "Engine.h"

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

int APIENTRY wWinMain( _In_ 	HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
	                   _In_ 	LPWSTR    lpCmdLine,
	                   _In_ 	int       nCmdShow ) {
	Engine* engine = new Engine;

	engine->Initialize(hInstance, "D11 Engine Window", "D11EWindowClass", WIDTH, HEIGHT);
	
	while (engine->ProcessMessages()) {
		engine->Update();
		engine->RenderFrame();
	}

	return 0;
}