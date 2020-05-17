#pragma once

#include <queue>
#include "KeyboardEvent.h"

class KeyboardClass
{
public:
	KeyboardClass();

	bool KeyIsPressed(const unsigned char keycode) const { return keyStates[keycode]; };

	bool KeyBufferIsEmpty()  const { return this->keyBuffer.empty(); };
	bool CharBufferIsEmpty() const { return this->charBuffer.empty(); };

	KeyboardEvent ReadKey();
	unsigned char ReadChar();

	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);

	void EnableAutoRepeatKeys();
	void DisableAutoRepeatKeys();
	void EnableAutoRepeatChars();
	void DisableAutoRepeatChars();

	bool IsKeysAutoRepeat() const { return autoRepeatKeys; }
	bool IsCharAutoRepeat() const { return autoRepeatChars; }
	
private:
	bool autoRepeatKeys;
	bool autoRepeatChars;

	bool keyStates[256];

	std::queue<KeyboardEvent> keyBuffer;
	std::queue<unsigned char> charBuffer;
};
