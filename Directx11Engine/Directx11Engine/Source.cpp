#include "Engine.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")

#define WIDTH 800
#define HEIGHT 600

int APIENTRY wWinMain( _In_ 	HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
	                   _In_ 	LPWSTR    lpCmdLine,
	                   _In_ 	int       nCmdShow ) {
	Engine* engine = new Engine;

	engine->Initialize(hInstance, "EngineWindow", "MyWindowClass", WIDTH, HEIGHT);
	
	while (engine->ProcessMessages()) {
		engine->Update();
	}

	return 0;
}