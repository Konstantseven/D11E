#include "MouseClass.h"

void MouseClass::OnLeftPressed(int x, int y) {
	this->leftIsDown = true;

	const MouseEvent mEvent(MouseEvent::EventType::LPress, x, y);
	this->eventBuffer.push(mEvent);
}

void MouseClass::OnLeftReleased(int x, int y) {
	this->leftIsDown = false;

	const MouseEvent mEvent(MouseEvent::EventType::LRelease, x, y);
	this->eventBuffer.push(mEvent);
}

void MouseClass::OnRightPressed(int x, int y) {
	this->rightIsDown = true;

	const MouseEvent mEvent(MouseEvent::EventType::RPress, x, y);
	this->eventBuffer.push(mEvent);
}

void MouseClass::OnRightReleased(int x, int y) {
	this->rightIsDown = false;

	const MouseEvent mEvent(MouseEvent::EventType::RRelease, x, y);
	this->eventBuffer.push(mEvent);
}

void MouseClass::OnMiddlePressed(int x, int y) {
	this->middleIsDown = true;

	const MouseEvent mEvent(MouseEvent::EventType::MPress, x, y);
	this->eventBuffer.push(mEvent);
}

void MouseClass::OnMiddleReleased(int x, int y) {
	this->middleIsDown = false;

	const MouseEvent mEvent(MouseEvent::EventType::MRelease, x, y);
	this->eventBuffer.push(mEvent);
}

void MouseClass::OnWheelUp(int x, int y) {
	const MouseEvent mEvent(MouseEvent::EventType::WheelUp, x, y);
	this->eventBuffer.push(mEvent);
}

void MouseClass::OnWheelDown(int x, int y) {
	const MouseEvent mEvent(MouseEvent::EventType::WheelDown, x, y);
	this->eventBuffer.push(mEvent);
}

void MouseClass::OnMouseMove(int x, int y) {
	const MouseEvent mEvent(MouseEvent::EventType::Move, x, y);
	this->eventBuffer.push(mEvent);
}

void MouseClass::OnMouseMoveRaw(int x, int y) {
	const MouseEvent mEvent(MouseEvent::EventType::RAW_MOVE, x, y);
	this->eventBuffer.push(mEvent);
}

MouseEvent MouseClass::ReadEvent() {
	if (!this->IsEventBufferEmpty()) {
		MouseEvent mEvent = this->eventBuffer.front();
		this->eventBuffer.pop();
		return mEvent;
	}

	return MouseEvent();
}
