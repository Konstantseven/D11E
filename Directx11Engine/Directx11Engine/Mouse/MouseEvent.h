#pragma once

struct MousePoint
{
	int x;
	int y;
};

class MouseEvent
{
public:
	enum class EventType
	{// Left bttn | Right bttn | Middle bttn | Move type | Wheel     | Default
		LPress,     RPress,      MPress,       Move,       WheelDown,  Invalid,
		LRelease,   RRelease,    MRelease,     RAW_MOVE,   WheelUp
	};

private:
	// Mouse event type
	EventType type;

	// Coordinates
	int x;
	int y;

public:
	MouseEvent() : type(EventType::Invalid), 
	               x(0), 
				   y(0) {};
	explicit MouseEvent(const EventType type, const int x, const int y) : type(type), 
																		  x(x), 
																		  y(y) {};

	inline bool IsValid() const { return this->type != EventType::Invalid; };

	EventType  GetType() const { return this->type; };
	MousePoint GetPos()  const { return {this->x, this->y}; };
	int        GetPosX() const { return this->x; };
	int        GetPosY() const { return this->y; };
};
