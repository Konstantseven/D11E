#pragma once

#include "gtest/gtest.h"
#include <Keyboard/KeyboardClass.h>

using KeyboardEventType = KeyboardEvent::EventType;

class TestKeyboardClass : public testing::Test
{
protected:
	KeyboardEvent keyboardEvent;          // Basic keyboard event
	KeyboardClass keyboardClass;          // Basic keyboard object
	const unsigned char key_uch = 0x0041; // UTF-8 'A'
};

TEST_F(TestKeyboardClass, TestInvalidKeyboardEvent) {
	// Verify KeyboardEvent correctly initialize its object with no parameters

	keyboardEvent = KeyboardEvent();

	EXPECT_EQ(keyboardEvent.IsValid(), false);
	EXPECT_EQ(keyboardEvent.GetKeyCode(), 0u);
}

TEST_F(TestKeyboardClass, TestNornalKeyboardEvent) {
	// Verify KeyboardEvent correctly initialize its object with parameters

	keyboardEvent = KeyboardEvent(KeyboardEventType::Press, key_uch);

	// Checking that created KeyboardEvent updating correctly
	EXPECT_EQ(keyboardEvent.IsValid(), true);
	EXPECT_EQ(keyboardEvent.IsPress(), true);
	EXPECT_EQ(keyboardEvent.IsRelease(), false);
	EXPECT_EQ(keyboardEvent.GetKeyCode(), key_uch);
}

TEST_F(TestKeyboardClass, TestKeyboardKeyBuffer) {
	// Verify KeyBuffer works correctly

	keyboardClass = KeyboardClass();

	// Verify just initialized KeyBuffer has no elements
	EXPECT_EQ(keyboardClass.KeyBufferIsEmpty(), true);

	// Verify right updating of incoming keyboard events
	keyboardClass.OnKeyPressed(key_uch);
	keyboardEvent = keyboardClass.ReadKey();

	EXPECT_EQ(keyboardEvent.GetKeyCode(), key_uch);
	EXPECT_EQ(keyboardEvent.IsPress(), true);
	EXPECT_EQ(keyboardEvent.IsRelease(), false);
	EXPECT_EQ(keyboardEvent.IsValid(), true);

	// Verify right behaviour of ReadKey() in case when KeyBuffer is already empty
	keyboardEvent = keyboardClass.ReadKey();

	EXPECT_EQ(keyboardEvent.GetKeyCode(), 0u);
	EXPECT_EQ(keyboardEvent.IsPress(), false);
	EXPECT_EQ(keyboardEvent.IsRelease(), false);
	EXPECT_EQ(keyboardEvent.IsValid(), false);
}

TEST_F(TestKeyboardClass, TestKeyboardCharBuffer) {
	// Verify CharBuffer works correctly
	keyboardClass = KeyboardClass();

	// Verify just initialized CharBuffer has no elements
	EXPECT_EQ(keyboardClass.CharBufferIsEmpty(), true);

	// Verify right updating of incoming chars
	keyboardClass.OnChar(key_uch);
	unsigned char _char = keyboardClass.ReadChar();

	EXPECT_EQ(_char, key_uch);

	// Verify right behaviour of ReadChar() in case when CharBuffer is already empty
	_char = keyboardClass.ReadChar();

	EXPECT_EQ(_char, 0u);
}
