#pragma once

#include <Trackable.h>
#include <Vector2D.h>

class GraphicsSystem;
class InputSystem;

class System :public Trackable
{
public:

	System();
	virtual ~System();

	bool init(unsigned int width, unsigned int height);
	void cleanup();
	GraphicsSystem* getGraphicsSystem() const { return mpGraphicsSystem; };
	InputSystem* getInputSystem() const { return mpInputSystem; };

private:
	bool mIsInitted = false;
	GraphicsSystem* mpGraphicsSystem = NULL;
	InputSystem* mpInputSystem = NULL;


};
