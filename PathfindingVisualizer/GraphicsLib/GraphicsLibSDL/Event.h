#pragma once

#include <string>
#include <trackable.h>

using namespace std;

class Event :public Trackable
{
public:
	enum EventType
	{
		INVALID_EVENT_TYPE = -1,
		MOUSE_MOVE_EVENT,
		MOUSE_BUTTON_EVENT,
		KEYBOARD_EVENT,
		NUM_EVENT_TYPES
	};


	Event(EventType type);
	virtual ~Event();

	EventType getType() const { return mType; };

private:
	EventType mType;

};