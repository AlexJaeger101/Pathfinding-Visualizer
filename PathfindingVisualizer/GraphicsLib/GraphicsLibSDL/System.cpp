#include "System.h"
#include "GraphicsSystem.h"
#include "InputSystem.h"
#include "EventSystem.h"

#include <iostream>

using namespace std;

System::System()
{
}

System::~System()
{
	cleanup();
}

bool System::init(unsigned int width, unsigned int height)
{
	if (!mIsInitted)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			mpGraphicsSystem = new GraphicsSystem;
			mpInputSystem = new InputSystem;

			bool ret = mpGraphicsSystem->init(width, height);
			if (!ret)
			{
				cout << "error initting GraphicsSystem\n";
				return false;
			}
			ret = mpInputSystem->init();
			if (!ret)
			{
				cout << "error initting InputSystem\n";
				return false;
			}
		}

		mIsInitted = true;
	}
	return true;
}

void System::cleanup()
{
	if (mIsInitted)
	{
		delete mpInputSystem;
		delete mpGraphicsSystem;

		SDL_Quit();
		mIsInitted = false;
	}
}

