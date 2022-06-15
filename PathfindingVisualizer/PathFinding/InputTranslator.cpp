#include "InputTranslator.h"
#include "GameEvent.h"
#include "Game.h"
#include "UnitManager.h"
#include "LocationManager.h"
#include "Unit.h"
#include "Location.h"
#include "Item.h"
#include "Recipe.h"
#include "Grid.h"

#include <EventSystem.h>
#include <Event.h>
#include <InterfaceEvents.h>
#include <System.h>
#include <InputSystem.h>
#include <iostream>

using namespace std;

InputTranslator::InputTranslator()
{
	EventSystem* pEventSystem = EventSystem::getInstance();
	pEventSystem->addListener(Event::KEYBOARD_EVENT, this);
	pEventSystem->addListener(Event::MOUSE_MOVE_EVENT, this);
	pEventSystem->addListener(Event::MOUSE_BUTTON_EVENT, this);
}

void InputTranslator::handleEvent(const Event& theEvent)
{
	//cout << theEvent.getEventName() << " received by InputTranslator\n";

	switch (theEvent.getType())
	{
	case Event::KEYBOARD_EVENT:
	{
		const KeyEvent& keyEvent = static_cast<const KeyEvent&>(theEvent);
		//cout << keyEvent.getKey() << " " << keyEvent.getPressedState() << endl;
		translateKeyEvent(keyEvent);
		break;
	}
	case Event::MOUSE_BUTTON_EVENT:
	{
		const MouseButtonEvent& buttonEvent = static_cast<const MouseButtonEvent&>(theEvent);
		//cout << buttonEvent.getButton() << " " << buttonEvent.getPressedState() << " " << buttonEvent.getLocation() << endl;
		translateMouseButtonEvent(buttonEvent);
		break;
	}
	case Event::MOUSE_MOVE_EVENT:
	{
		const MouseMoveEvent& moveEvent = static_cast<const MouseMoveEvent&>(theEvent);
		//cout << moveEvent.getLocation() << endl;
		translateMouseMoveEvent(moveEvent);
		break;
	}
	default:
	{

	}
	}
}

void InputTranslator::translateKeyEvent(const KeyEvent& theEvent)
{
	if (theEvent.getPressedState() == InputSystem::RELEASED)
	{
		//toggle between edit and play modes
		if (theEvent.getKey() == InputSystem::E_KEY)
		{
			if (gpGame->getGameMode() == PLAY_GAME_MODE)
			{
				EventSystem::getInstance()->fireEvent(ChangeGameModeEvent(EDIT_GAME_MODE));
			}
			else if (gpGame->getGameMode() == EDIT_GAME_MODE)
			{
				EventSystem::getInstance()->fireEvent(ChangeGameModeEvent(PLAY_GAME_MODE));
			}
		}
	}

	if (gpGame->getGameMode() == START_GAME_MODE)
	{
		if (theEvent.getKey() == InputSystem::SPACE_KEY && theEvent.getPressedState() == InputSystem::PRESSED)
		{
			EventSystem::getInstance()->fireEvent(ChangeGameModeEvent(PLAY_GAME_MODE));
		}
	}
	else if (gpGame->getGameMode() == PLAY_GAME_MODE)
	{
		if (theEvent.getKey() == InputSystem::SPACE_KEY && theEvent.getPressedState() == InputSystem::PRESSED)
		{
			EventSystem::getInstance()->fireEvent(ChangeGameModeEvent(PAUSE_GAME_MODE));
		}
		else if (theEvent.getKey() == InputSystem::F_KEY && theEvent.getPressedState() == InputSystem::PRESSED)
		{
			EventSystem::getInstance()->fireEvent(ChangePathfindingEvent(DF_PATHFINDING));
		}
		else if (theEvent.getKey() == InputSystem::B_KEY && theEvent.getPressedState() == InputSystem::PRESSED)
		{
			EventSystem::getInstance()->fireEvent(ChangePathfindingEvent(BF_PATHFINDING));
		}
		else if (theEvent.getKey() == InputSystem::D_KEY && theEvent.getPressedState() == InputSystem::PRESSED)
		{
			EventSystem::getInstance()->fireEvent(ChangePathfindingEvent(DIJKSTRAS_PATHFINDING));
		}
		else if (theEvent.getKey() == InputSystem::A_KEY && theEvent.getPressedState() == InputSystem::PRESSED)
		{
			EventSystem::getInstance()->fireEvent(ChangePathfindingEvent(ASTAR_PATHFINDING));
		}
		else if (theEvent.getKey() == InputSystem::ONE_KEY && theEvent.getPressedState() == InputSystem::PRESSED)
		{
			EventSystem::getInstance()->fireEvent(ChangeGameModeEvent(EDIT_GAME_MODE));
		}
	}
	else if (gpGame->getGameMode() == PAUSE_GAME_MODE)
	{
		if (theEvent.getKey() == InputSystem::SPACE_KEY && theEvent.getPressedState() == InputSystem::PRESSED)
		{
			EventSystem::getInstance()->fireEvent(ChangeGameModeEvent(PLAY_GAME_MODE));
		}
		else if (theEvent.getKey() == InputSystem::R_KEY && theEvent.getPressedState() == InputSystem::PRESSED)
		{
			EventSystem::getInstance()->fireEvent(ChangeGameModeEvent(START_GAME_MODE));
		}
	}
	else if (gpGame->getGameMode() == END_GAME_MODE)
	{
		if (theEvent.getKey() == InputSystem::R_KEY && theEvent.getPressedState() == InputSystem::PRESSED)
		{
			EventSystem::getInstance()->fireEvent(ChangeGameModeEvent(START_GAME_MODE));
		}
	}
	else if (gpGame->getGameMode() == EDIT_GAME_MODE)
	{
		if (theEvent.getKey() == InputSystem::L_KEY && theEvent.getPressedState() == InputSystem::PRESSED)
		{
			EventSystem::getInstance()->fireEvent(LoadSaveMapEvent(false));
		}
		else if (theEvent.getKey() == InputSystem::S_KEY && theEvent.getPressedState() == InputSystem::PRESSED)
		{
			EventSystem::getInstance()->fireEvent(LoadSaveMapEvent(true));
		}
		else if (theEvent.getKey() == InputSystem::ONE_KEY && theEvent.getPressedState() == InputSystem::PRESSED)
		{
			EventSystem::getInstance()->fireEvent(ChangeGameModeEvent(PLAY_GAME_MODE));
		}
	}

	//whatever the mode - ESC exits immediately
	if (theEvent.getKey() == InputSystem::ESCAPE_KEY && theEvent.getPressedState() == InputSystem::PRESSED)
	{
		EventSystem::getInstance()->fireEvent(GameEvent(EXIT_GAME));
	}

}

void InputTranslator::translateMouseMoveEvent(const MouseMoveEvent& theEvent)
{
	if (gpGame->getGameMode() == EDIT_GAME_MODE)
	{
		if (gpGame->getSystem()->getInputSystem()->isMouseButtonPressed(InputSystem::LEFT))
		{
			EventSystem::getInstance()->fireEvent(ModTerrainEvent(theEvent.getLocation(), BLOCKING_VALUE, true));
		}
		else if (gpGame->getSystem()->getInputSystem()->isMouseButtonPressed(InputSystem::RIGHT))
		{
			EventSystem::getInstance()->fireEvent(ModTerrainEvent(theEvent.getLocation(), CLEAR_VALUE, true));
		}
	}

	//cout << "mouse moved:" << theEvent.getLocation() << endl;
}

void InputTranslator::translateMouseButtonEvent(const MouseButtonEvent& theEvent)
{
	if (gpGame->getGameMode() == PLAY_GAME_MODE)
	{
		if (theEvent.getPressedState() == InputSystem::PRESSED)
		{
			if (theEvent.getButton() == InputSystem::LEFT)
			{
				//EventSystem::getInstance()->fireEvent(MoveToEvent(PLAYER_UNIT_ID, theEvent.getLocation()));
				EventSystem::getInstance()->fireEvent(PathToEvent(theEvent.getLocation()));
			}
			else if (theEvent.getButton() == InputSystem::RIGHT)
			{
			}
		}
	}
	else if (gpGame->getGameMode() == EDIT_GAME_MODE)
	{
		if (theEvent.getPressedState() == InputSystem::PRESSED)
		{
			if (theEvent.getButton() == InputSystem::LEFT)
			{
				EventSystem::getInstance()->fireEvent(ModTerrainEvent(theEvent.getLocation(), BLOCKING_VALUE, true));
			}
			else if (theEvent.getButton() == InputSystem::RIGHT)
			{
				EventSystem::getInstance()->fireEvent(ModTerrainEvent(theEvent.getLocation(), CLEAR_VALUE, true));
			}
		}
	}
}
