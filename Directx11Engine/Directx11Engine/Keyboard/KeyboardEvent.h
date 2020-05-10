#pragma once

class KeyboardEvent
{
public:
	enum class EventType
	{
		Press,
		Release,
		Invalid
	};

	KeyboardEvent() : type(EventType::Invalid),
		              key(0u) {};
	explicit KeyboardEvent(const EventType type, unsigned char key) : type(type), 
		                                                              key(key) {};

	bool IsPress()   const { return this->type == EventType::Press; };
	bool IsRelease() const { return this->type == EventType::Release; };
	bool IsValid()   const { return this->type != EventType::Invalid; };

	unsigned char GetKeyCode() const { return this->key; }

private: 
	EventType type;

	unsigned char key;
};
