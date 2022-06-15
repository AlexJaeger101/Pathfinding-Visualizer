#include "InterfaceEvents.h"


MouseMoveEvent::MouseMoveEvent(const Vector2D& location, const MouseButtonState& state)
	:InterfaceEvent(MOUSE_MOVE_EVENT)
	,mLoc(location)
	,mState(state)
{
}

InputSystem::PressState MouseMoveEvent::getPressedState(InputSystem::MouseButton theButton)
{
	return (InputSystem::PressState)mState.getPressedState((MouseButtonState::MouseButton)theButton);
}

MouseButtonEvent::MouseButtonEvent(const Vector2D& location, InputSystem::MouseButton button, InputSystem::PressState state)
	:InterfaceEvent(MOUSE_BUTTON_EVENT)
	,mLoc(location)
	,mButton(button)
	,mState(state)
{
}

KeyEvent::KeyEvent(InputSystem::Key key, InputSystem::PressState state)
	:InterfaceEvent(KEYBOARD_EVENT)
	,mKey(key)
	,mState(state)
{
}
