#include "KeyboardClass.h"

KeyboardClass::KeyboardClass() {
	autoRepeatKeys  = false;
	autoRepeatChars = false;

	for (auto& keyState : keyStates)
		keyState = false;
}

KeyboardEvent KeyboardClass::ReadKey() {
	if (!keyBuffer.empty()) {
		KeyboardEvent event = keyBuffer.front();
		keyBuffer.pop();

		return event;
	}

	return KeyboardEvent();

}

unsigned char KeyboardClass::ReadChar() {
	if (!charBuffer.empty()) {
		unsigned char event = charBuffer.front();
		charBuffer.pop();
		return event;
	}

	return 0u;
}

void KeyboardClass::OnKeyPressed(const unsigned char key) {
	keyStates[key] = true;
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void KeyboardClass::OnKeyReleased(const unsigned char key) {
	keyStates[key] = false;
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void KeyboardClass::OnChar(const unsigned char key) {
	charBuffer.push(key);
}

void KeyboardClass::EnableAutoRepeatKeys() {
	autoRepeatKeys = true;
}

void KeyboardClass::DisableAutoRepeatKeys() {
	autoRepeatKeys = false;
}

void KeyboardClass::EnableAutoRepeatChars() {
	autoRepeatChars = true;
}

void KeyboardClass::DisableAutoRepeatChars() {
	autoRepeatChars = false;
}
