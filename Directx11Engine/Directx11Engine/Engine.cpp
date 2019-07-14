#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height) {
	return this->renderWindow.Initialize(this, hInstance, windowTitle, windowClass, width, height);
}

bool Engine::ProcessMessages() {
	return this->renderWindow.ProcessMessages();
}

void Engine::Update() {
	while (!keyboard.CharBufferIsEmpty()) {
		unsigned char _char = keyboard.ReadChar();
	}
	while (!keyboard.KeyBufferIsEmpty()) {
		KeyboardEvent kEvent = keyboard.ReadKey();
		unsigned char _char = kEvent.GetKeyCode();
	}
}