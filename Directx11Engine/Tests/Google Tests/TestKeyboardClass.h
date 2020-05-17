#pragma once

#include "gtest/gtest.h"
#include <Keyboard/KeyboardClass.h>

using KeyboardEventType = KeyboardEvent::EventType;

class TestKeyboardClass : public testing::Test
{
protected:
	KeyboardEvent keyboardEvent;
	KeyboardClass keyboardClass;
	const unsigned char key_uch = 0x0041; // UTF-8 'A'
};

TEST_F(TestKeyboardClass, TestInvalidKeyboardEvent) {
	keyboardEvent = KeyboardEvent();

	EXPECT_EQ(keyboardEvent.IsValid(), false);
	EXPECT_EQ(keyboardEvent.GetKeyCode(), 0u);
}

TEST_F(TestKeyboardClass, TestNornalKeyboardEvent) {
	keyboardEvent = KeyboardEvent(KeyboardEventType::Press, key_uch);

	EXPECT_EQ(keyboardEvent.IsValid(), true);
	EXPECT_EQ(keyboardEvent.IsPress(), true);
	EXPECT_EQ(keyboardEvent.IsRelease(), false);
	EXPECT_EQ(keyboardEvent.GetKeyCode(), key_uch);
}

TEST_F(TestKeyboardClass, TestKeyboardKeyBuffer) {
	keyboardClass = KeyboardClass();

	EXPECT_EQ(keyboardClass.KeyBufferIsEmpty(), true);

	keyboardClass.OnKeyPressed(key_uch);
	keyboardEvent = keyboardClass.ReadKey();

	EXPECT_EQ(keyboardEvent.GetKeyCode(), key_uch);
	EXPECT_EQ(keyboardEvent.IsPress(), true);
	EXPECT_EQ(keyboardEvent.IsRelease(), false);
	EXPECT_EQ(keyboardEvent.IsValid(), true);

	keyboardEvent = keyboardClass.ReadKey();

	EXPECT_EQ(keyboardEvent.GetKeyCode(), 0u);
	EXPECT_EQ(keyboardEvent.IsPress(), false);
	EXPECT_EQ(keyboardEvent.IsRelease(), false);
	EXPECT_EQ(keyboardEvent.IsValid(), false);
}

TEST_F(TestKeyboardClass, TestKeyboardCharBuffer) {
	keyboardClass = KeyboardClass();

	EXPECT_EQ(keyboardClass.CharBufferIsEmpty(), true);

	keyboardClass.OnChar(key_uch);
	unsigned char _char = keyboardClass.ReadChar();

	EXPECT_EQ(_char, key_uch);

	_char = keyboardClass.ReadChar();

	EXPECT_EQ(_char, 0u);
}
