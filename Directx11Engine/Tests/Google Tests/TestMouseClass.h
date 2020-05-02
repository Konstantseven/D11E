#pragma once

#include "gtest/gtest.h"
#include <Mouse/MouseClass.h>

using MouseEventType = MouseEvent::EventType;

class TestMouseClass : public testing::Test {
protected:
	MouseEvent mouseEvent;
	MouseClass mouse;

	const int X;
	const int Y;
	const MousePoint M_POINT;

	TestMouseClass() : X(1), Y(2), M_POINT{X, Y}{
		mouse = MouseClass();
	}

	friend bool operator==(const MousePoint& lMPoint, const MousePoint& rMPoint) {
		return (lMPoint.x == rMPoint.x) && (lMPoint.y == rMPoint.y);
	}
};

TEST_F(TestMouseClass, TestInvalidMouseEvent) {
	mouseEvent = MouseEvent();

	EXPECT_EQ(mouseEvent.GetType(), MouseEventType::Invalid);
}

TEST_F(TestMouseClass, TestNormalMouseEvent) {
	const MouseEventType EVENT_TYPE(MouseEventType::LPress);

	mouseEvent = MouseEvent(EVENT_TYPE, X, Y);

	EXPECT_EQ(mouseEvent.GetType(), EVENT_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);
}

TEST_F(TestMouseClass, TestOnLeftPressed) {
	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::LPress;

	mouse.OnLeftPressed(X, Y);

	EXPECT_EQ(mouse.IsLeftDown(), true);
	EXPECT_EQ(mouse.IsMiddleDown(), false);
	EXPECT_EQ(mouse.IsRightDown(), false);

	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}

TEST_F(TestMouseClass, TestOnLeftReleased) {
	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::LRelease;

	mouse.OnLeftReleased(X, Y);

	EXPECT_EQ(mouse.IsLeftDown(), false);
	EXPECT_EQ(mouse.IsMiddleDown(), false);
	EXPECT_EQ(mouse.IsRightDown(), false);

	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}


TEST_F(TestMouseClass, TestOnRightPressed) {
	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::RPress;

	mouse.OnRightPressed(X, Y);

	EXPECT_EQ(mouse.IsLeftDown(), false);
	EXPECT_EQ(mouse.IsMiddleDown(), false);
	EXPECT_EQ(mouse.IsRightDown(), true);

	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}

TEST_F(TestMouseClass, TestOnRightReleased) {
	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::RRelease;

	mouse.OnRightReleased(X, Y);

	EXPECT_EQ(mouse.IsLeftDown(), false);
	EXPECT_EQ(mouse.IsMiddleDown(), false);
	EXPECT_EQ(mouse.IsRightDown(), false);

	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}

TEST_F(TestMouseClass, TestOnMiddlePressed) {
	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::MPress;

	mouse.OnMiddlePressed(X, Y);

	EXPECT_EQ(mouse.IsLeftDown(), false);
	EXPECT_EQ(mouse.IsMiddleDown(), true);
	EXPECT_EQ(mouse.IsRightDown(), false);

	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}

TEST_F(TestMouseClass, TestOnMiddleReleased) {
	const MouseEventType EXPECTED_ME_TYPE = MouseEventType::MRelease;

	mouse.OnMiddleReleased(X, Y);

	EXPECT_EQ(mouse.IsLeftDown(), false);
	EXPECT_EQ(mouse.IsMiddleDown(), false);
	EXPECT_EQ(mouse.IsRightDown(), false);

	EXPECT_EQ(mouse.IsEventBufferEmpty(), false);

	MouseEvent mouseEvent = mouse.ReadEvent();
	EXPECT_EQ(mouseEvent.GetType(), EXPECTED_ME_TYPE);
	EXPECT_EQ(mouseEvent.GetPosX(), X);
	EXPECT_EQ(mouseEvent.GetPosY(), Y);
	EXPECT_EQ(mouseEvent.GetPos(), M_POINT);

	EXPECT_EQ(mouse.IsEventBufferEmpty(), true);
}
