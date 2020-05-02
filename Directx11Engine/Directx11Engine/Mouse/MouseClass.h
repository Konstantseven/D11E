#pragma once

#include "MouseEvent.h"
#include <queue>

class MouseClass
{
public:
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);

	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);

	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);

	void OnMouseMove(int x, int y);
	void OnMouseMoveRaw(int x, int y);

	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);

	inline bool IsLeftDown()         const { return this->leftIsDown; };
	inline bool IsMiddleDown()       const { return this->middleIsDown; };
	inline bool IsRightDown()        const { return this->rightIsDown; };
	inline bool IsEventBufferEmpty() const { return this->eventBuffer.empty(); };
	
	MouseEvent ReadEvent();

private:
	std::queue<MouseEvent> eventBuffer;

	bool leftIsDown   = false;
	bool rightIsDown  = false;
	bool middleIsDown = false;
};
