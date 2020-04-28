#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height) {
	timer.Start();

	if (!this->renderWindow.Initialize(this, hInstance, windowTitle, windowClass, width, height)) {
		return false;
	}

	if (!grafics.Initialize(this->renderWindow.GetHWND(), width, height)) {
		return false;
	}

	return true;
}

bool Engine::ProcessMessages() {
	return this->renderWindow.ProcessMessages();
}

void Engine::Update() {
	double dTime = timer.GetMilisecondsElapsed();
	timer.Restart();

	while (!keyboard.CharBufferIsEmpty()) {
		unsigned char _char = keyboard.ReadChar();
	}

	while (!keyboard.KeyBufferIsEmpty()) {
		KeyboardEvent kEvent = keyboard.ReadKey();
		unsigned char _char = kEvent.GetKeyCode();
	}

	constexpr float CAMERA_ROTATION_SPEED_COEF = 0.005f;
	while (!mouse.EventBufferIsEmpty()) {
		MouseEvent mEvent = mouse.ReadEvent();
		if (mouse.IsRightDown()) {
			if (mEvent.GetType() == MouseEvent::EventType::RAW_MOVE)
				this->grafics.camera.AdjustRotation(static_cast<float>(mEvent.GetPosY()) * CAMERA_ROTATION_SPEED_COEF, 
													static_cast<float>(mEvent.GetPosX()) * CAMERA_ROTATION_SPEED_COEF, 
													0.0f);
		}
	}

	using DirectX::operator*;

	constexpr double CAMERA_MOVEMENT_SPEED_COEF = 0.002;
	// test
	if (keyboard.KeyIsPressed('W')) {
		this->grafics.camera.AdjustPosition(this->grafics.camera.GetForwardVector() * CAMERA_MOVEMENT_SPEED_COEF * dTime);
	}
	if (keyboard.KeyIsPressed('S')) {
		this->grafics.camera.AdjustPosition(this->grafics.camera.GetBackwardVector() * CAMERA_MOVEMENT_SPEED_COEF * dTime);
	}
	if (keyboard.KeyIsPressed('A')) {
		this->grafics.camera.AdjustPosition(this->grafics.camera.GetLeftVector() * -CAMERA_MOVEMENT_SPEED_COEF * dTime);
	}
	if (keyboard.KeyIsPressed('D')) {
		this->grafics.camera.AdjustPosition(this->grafics.camera.GetRightVector() * CAMERA_MOVEMENT_SPEED_COEF * dTime);
	}
	if (keyboard.KeyIsPressed(VK_SPACE)) {
		this->grafics.camera.AdjustPosition(0.0f, CAMERA_MOVEMENT_SPEED_COEF * dTime, 0.0f);
	}
	if (keyboard.KeyIsPressed('C')) {
		this->grafics.camera.AdjustPosition(0.0f, -CAMERA_MOVEMENT_SPEED_COEF * dTime, 0.0f);
	}
}

void Engine::RenderFrame() {
	grafics.RenderFrame();
}