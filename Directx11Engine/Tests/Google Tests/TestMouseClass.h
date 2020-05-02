#pragma once

#include "gtest/gtest.h"
#include <Mouse/MouseClass.h>

using MouseEventType = MouseEvent::EventType;

class TestMouseClass : public testing::Test {
protected:
	MouseEvent mouseEvent;

	friend bool operator==(const MousePoint& lMPoint, const MousePoint& rMPoint) {
		return (lMPoint.x == rMPoint.x) && (lMPoint.y == rMPoint.y);
	}
};

TEST_F(TestMouseClass, TestInvalidMouseEventInitialization) {
	mouseEvent = MouseEvent();

	EXPECT_EQ(mouseEvent.GetType(), MouseEventType::Invalid);
}

TEST_F(TestMouseClass, TestNormalMouseEventInitialization) {
	const MouseEventType eventType = MouseEventType::LPress;
	const int x = 1;
	const int y = 2;
	const MousePoint mPoint{x, y};

	mouseEvent = MouseEvent(eventType, x, y);

	EXPECT_EQ(mouseEvent.GetType(), eventType);
	EXPECT_EQ(mouseEvent.GetPosX(), x);
	EXPECT_EQ(mouseEvent.GetPosY(), y);
	EXPECT_EQ(mouseEvent.GetPos(), mPoint);
}
