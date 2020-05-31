#pragma once

#include "gtest/gtest.h"
#include <Mouse/MouseClass.h>
#include <vector>
#include <utility>
#include <functional>

using MouseEventType = MouseEvent::EventType;

class TestMouseClass : public testing::Test 
{
protected:
	MouseEvent mouseEvent; // Basic mouse event
	MouseClass mouse;      // Basic mouse object

	const int X;              // Basic X coordinate
	const int Y;              // Basic Y coordinate
	const MousePoint M_POINT; // Basic MousePoint Object of the X and Y coordinates above

	// Default TestMouseClass constructor
	TestMouseClass() : X(1), Y(2), M_POINT{X, Y} {
		mouse = MouseClass();
	}

	// Comparing two MousePoint objects
	friend bool operator==(const MousePoint& lMPoint, const MousePoint& rMPoint) {
		return (lMPoint.x == rMPoint.x) && (lMPoint.y == rMPoint.y);
	}
};

TEST_F(TestMouseClass, TestInvalidMouseEvent) {
	// Verify MouseEvent correctly initialize its object with no parameters

	mouseEvent = MouseEvent();

	EXPECT_EQ(mouseEvent.GetType(), MouseEventType::Invalid);
}

TEST_F(TestMouseClass, TestNormalMouseEvent) {
	// Verify MouseEvent correctly initialize its object with parameters

	const MouseEventType EVENT_TYPE(MouseEventType::LPress);

	mouseEvent = MouseEvent(EVENT_TYPE, X, Y);

	// Verify that all parameters are updated correctly
	EXPECT_EQ(mouseEvent.GetType(), EVENT_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);
}

TEST_F(TestMouseClass, TestOnLeftPressed) {
	// Verify mouse correctly updates pressing of the left buttom

	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::LPress;

	mouse.OnLeftPressed(X, Y);

	// Verify that mouse event is updated correctly 
	EXPECT_EQ(mouse.IsLeftDown(), true);
	EXPECT_EQ(mouse.IsMiddleDown(), false);
	EXPECT_EQ(mouse.IsRightDown(), false);

	// Verify mouse event buffer has an event
	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	// Verify that all parameters are updated correctly
	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	// Verify mouse event buffer is empty
	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}

TEST_F(TestMouseClass, TestOnLeftReleased) {
	// Verify mouse correctly updates releasing of the left buttom

	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::LRelease;

	mouse.OnLeftReleased(X, Y);

	// Verify that mouse event is updated correctly 
	EXPECT_EQ(mouse.IsLeftDown(), false);
	EXPECT_EQ(mouse.IsMiddleDown(), false);
	EXPECT_EQ(mouse.IsRightDown(), false);

	// Verify mouse event buffer has an event
	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	// Verify that all parameters are updated correctly
	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	// Verify mouse event buffer is empty
	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}


TEST_F(TestMouseClass, TestOnRightPressed) {
	// Verify mouse correctly updates pressing of the right buttom

	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::RPress;

	mouse.OnRightPressed(X, Y);

	// Verify that mouse event is updated correctly 
	EXPECT_EQ(mouse.IsLeftDown(), false);
	EXPECT_EQ(mouse.IsMiddleDown(), false);
	EXPECT_EQ(mouse.IsRightDown(), true);

	// Verify mouse event buffer has an event
	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	// Verify that all parameters are updated correctly
	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	// Verify mouse event buffer is empty
	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}

TEST_F(TestMouseClass, TestOnRightReleased) {
	// Verify mouse correctly updates releasing of the right buttom

	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::RRelease;

	mouse.OnRightReleased(X, Y);

	// Verify that mouse event is updated correctly 
	EXPECT_EQ(mouse.IsLeftDown(), false);
	EXPECT_EQ(mouse.IsMiddleDown(), false);
	EXPECT_EQ(mouse.IsRightDown(), false);

	// Verify mouse event buffer has an event
	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	// Verify that all parameters are updated correctly
	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	// Verify mouse event buffer is empty
	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}

TEST_F(TestMouseClass, TestOnMiddlePressed) {
	// Verify mouse correctly updates pressing of the middle buttom

	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::MPress;

	mouse.OnMiddlePressed(X, Y);

	// Verify that mouse event is updated correctly 
	EXPECT_EQ(mouse.IsLeftDown(), false);
	EXPECT_EQ(mouse.IsMiddleDown(), true);
	EXPECT_EQ(mouse.IsRightDown(), false);

	// Verify mouse event buffer has an event
	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	// Verify that all parameters are updated correctly
	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	// Verify mouse event buffer is empty
	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}

TEST_F(TestMouseClass, TestOnMiddleReleased) {
	// Verify mouse correctly updates releasing of the middle buttom

	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::MRelease;

	mouse.OnMiddleReleased(X, Y);

	// Verify that mouse event is updated correctly 
	EXPECT_EQ(mouse.IsLeftDown(), false);
	EXPECT_EQ(mouse.IsMiddleDown(), false);
	EXPECT_EQ(mouse.IsRightDown(), false);

	// Verify mouse event buffer has an event
	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	// Verify that all parameters are updated correctly
	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	// Verify mouse event buffer is empty
	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}

TEST_F(TestMouseClass, TestOnWheelUp) {
	// Verify mouse correctly updates wheeling up

	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::WheelUp;

	mouse.OnWheelUp(X, Y);

	// Verify that mouse event is updated correctly 
	EXPECT_EQ(mouse.IsLeftDown(), false);
	EXPECT_EQ(mouse.IsMiddleDown(), false);
	EXPECT_EQ(mouse.IsRightDown(), false);

	// Verify mouse event buffer has an event
	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	// Verify that all parameters are updated correctly
	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	// Verify mouse event buffer is empty
	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}

TEST_F(TestMouseClass, TestOnWheelDown) {
	// Verify mouse correctly updates wheeling down

	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::WheelDown;

	mouse.OnWheelDown(X, Y);

	// Verify that mouse event is updated correctly 
	EXPECT_EQ(mouse.IsLeftDown(), false);
	EXPECT_EQ(mouse.IsMiddleDown(), false);
	EXPECT_EQ(mouse.IsRightDown(), false);

	// Verify mouse event buffer has an event
	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	// Verify that all parameters are updated correctly
	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	// Verify mouse event buffer is empty
	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}

TEST_F(TestMouseClass, TestOnMouseMove) {
	// Verify mouse correctly updates moving by MouseEventType::Move

	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::Move;

	mouse.OnMouseMove(X, Y);

	// Verify that mouse event is updated correctly 
	EXPECT_EQ(mouse.IsLeftDown(), false);
	EXPECT_EQ(mouse.IsMiddleDown(), false);
	EXPECT_EQ(mouse.IsRightDown(), false);

	// Verify mouse event buffer has an event
	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	// Verify that all parameters are updated correctly
	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	// Verify mouse event buffer is empty
	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}

TEST_F(TestMouseClass, TestOnMouseMoveRaw) {
	// Verify mouse correctly updates moving by MouseEventType::RAW_MOVE

	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::RAW_MOVE;

	mouse.OnMouseMoveRaw(X, Y);

	// Verify that mouse event is updated correctly 
	EXPECT_EQ(mouse.IsLeftDown(), false);
	EXPECT_EQ(mouse.IsMiddleDown(), false);
	EXPECT_EQ(mouse.IsRightDown(), false);

	// Verify mouse event buffer has an event
	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	// Verify that all parameters are updated correctly
	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	// Verify mouse event buffer is empty
	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}

TEST_F(TestMouseClass, TestStressSituation) {
	// Verify mouse correctly updates stressful number of clicks

	const int CLICK_NUMBERS = 5000; // Amount number of Press->Release clicks

	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);

	// Pulling all events to the mouse buffer
	for (int i = CLICK_NUMBERS; i > 0; --i) {
		mouse.OnLeftPressed(X, Y);
		mouse.OnLeftReleased(X, Y);
	}

	// Verify mouse event buffer has an events
	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	// Verify that all pulled events are stored and updated correctly
	for (int i = CLICK_NUMBERS; i > 0; --i) {
		MouseEvent mouseEvent = mouse.ReadEvent();
		EXPECT_EQ(mouseEvent.GetType(), MouseEventType::LPress);
		EXPECT_EQ(mouseEvent.GetPosX(), X);
		EXPECT_EQ(mouseEvent.GetPosY(), Y);
		EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

		mouseEvent = mouse.ReadEvent();
		EXPECT_EQ(mouseEvent.GetType(), MouseEventType::LRelease);
		EXPECT_EQ(mouseEvent.GetPosX(), X);
		EXPECT_EQ(mouseEvent.GetPosY(), Y);
		EXPECT_EQ(mouseEvent.GetPos(), M_POINT);
	}

	// Verify mouse event buffer is empty
	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}
