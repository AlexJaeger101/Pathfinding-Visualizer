#pragma once

#include <Vector2D.h>
#include <SDL.h>
#include "Event.h"
#include "InputSystem.h"

class InterfaceEvent: public Event
{
public:
	/*enum MouseButton
	{
		LEFT=InputSystem::LEFT,
		RIGHT= InputSystem::RIGHT,
		CENTER= InputSystem::CENTER
	};*/

	/*enum Key
	{
		A_KEY = InputSystem::F_KEY,
		S_KEY = InputSystem::S_KEY,
		L_KEY = InputSystem::L_KEY,
		ENTER_KEY = InputSystem::ENTER_KEY,
		RETURN_KEY = InputSystem::RETURN_KEY,
		SPACE_KEY = InputSystem::SPACE_KEY,
		ESCAPE_KEY = InputSystem::ESCAPE_KEY
	};*/

	/*
	enum PressState
	{
		PRESSED = InputSystem::PRESSED,
		RELEASED = InputSystem::RELEASED
	};*/


	InterfaceEvent(EventType type) :Event(type) {};
	~InterfaceEvent() {};
};

class MouseMoveEvent : public InterfaceEvent
{
public:
	MouseMoveEvent(const Vector2D& location, const MouseButtonState& state);
	~MouseMoveEvent() {};

	const Vector2D& getLocation() const { return mLoc; };
	InputSystem::PressState getPressedState(InputSystem::MouseButton theButton);

private:
	Vector2D mLoc;
	MouseButtonState mState;
};

class MouseButtonEvent : public InterfaceEvent
{
public:
	MouseButtonEvent(const Vector2D& location, InputSystem::MouseButton button, InputSystem::PressState state);
	~MouseButtonEvent() {};

	const Vector2D& getLocation() const { return mLoc; };
	InputSystem::MouseButton getButton() const { return mButton; };
	InputSystem::PressState getPressedState() const { return mState; };
private:
	Vector2D mLoc;
	InputSystem::MouseButton mButton;
	InputSystem::PressState mState;
};

class KeyEvent : public InterfaceEvent
{
public:
	KeyEvent(InputSystem::Key key, InputSystem::PressState state);
	~KeyEvent() {};

	InputSystem::Key getKey() const { return mKey; };
	InputSystem::PressState getPressedState() const { return mState; };
private:
	InputSystem::Key mKey;
	InputSystem::PressState mState;
};

