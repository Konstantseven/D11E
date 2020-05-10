#pragma once

#include "gtest/gtest.h"
#include <Keyboard/KeyboardClass.h>

using KeyboardEventType = KeyboardEvent::EventType;

class TestKeyboardClass : public testing::Test
{
protected:
	KeyboardEvent keyboardEvent;

	const unsigned char key_uch = 0x0041; // UTF-8 'A'
};

TEST_F(TestKeyboardClass, TestInvalidKeyboardEvent) {
	keyboardEvent = KeyboardEvent();

	EXPECT_EQ(keyboardEvent.IsValid(), false);
}

TEST_F(TestKeyboardClass, TestNornalKeyboardEvent) {
	keyboardEvent = KeyboardEvent(KeyboardEventType::Press, key_uch);

	EXPECT_EQ(keyboardEvent.IsValid(), true);
	EXPECT_EQ(keyboardEvent.IsPress(), true);
	EXPECT_EQ(keyboardEvent.IsRelease(), false);
	EXPECT_EQ(keyboardEvent.GetKeyCode(), key_uch);
}
