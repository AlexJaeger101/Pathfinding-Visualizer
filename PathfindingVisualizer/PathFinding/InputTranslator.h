#pragma once

#include <EventListener.h>
#include <InterfaceEvents.h>

class InputTranslator :public EventListener
{
public:
	InputTranslator();
	~InputTranslator() {};

private:
	void handleEvent(const Event& theEvent);
	void translateKeyEvent(const KeyEvent& theEvent);
	void translateMouseMoveEvent(const MouseMoveEvent& theEvent);
	void translateMouseButtonEvent(const MouseButtonEvent& theEvent);
};

