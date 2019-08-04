#include "MouseClass.h"

void MouseClass::OnLeftPressed(int x, int y) {
	this->leftIsDown = true;
	MouseEvent mEvent(MouseEvent::EventType::LPress, x, y);
	this->eventBuffer.push(mEvent);
}

void MouseClass::OnLeftReleased(int x, int y) {
	this->leftIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void MouseClass::OnRightPressed(int x, int y) {
	this->rightIsDown = true;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x, y));
}

void MouseClass::OnRightReleased(int x, int y) {
	this->rightIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RRelease, x, y));
}

void MouseClass::OnMiddlePressed(int x, int y) {
	this->mButtonDown = true;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::MPress, x, y));
}

void MouseClass::OnMiddleReleased(int x, int y) {
	this->mButtonDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::MRelease, x, y));
}

void MouseClass::OnWheelUp(int x, int y) {
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void MouseClass::OnWheelDown(int x, int y) {
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

void MouseClass::OnMouseMove(int x, int y) {
	this->x = x;
	this->y = y;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

bool MouseClass::IsLeftDown() const {
	return this->leftIsDown;
}

bool MouseClass::IsMiddleDown() const {
	return this->mButtonDown;
}

bool MouseClass::IsRightDown() const {
	return this->rightIsDown;
}

int MouseClass::GetPosX() const {
	return this->x;
}

int MouseClass::GetPosY() const {
	return this->y;
}

MousePoint MouseClass::GetPos() const {
	return{ this->x, this->y };
}

bool MouseClass::EventBufferIsEmpty() const {
	return this->eventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent() {
	if (this->eventBuffer.empty()) {
		return MouseEvent();
	}
	else {
		MouseEvent e = this->eventBuffer.front();
		this->eventBuffer.pop();
		return e;
	}
}