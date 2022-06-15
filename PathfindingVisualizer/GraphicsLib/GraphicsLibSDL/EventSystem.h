#pragma once

#include <map>
#include <Trackable.h>
#include "Event.h"

class EventListener;

using namespace std;

class EventSystem:public Trackable
{
public:

	static EventSystem * getInstance();
	static void initInstance();
	static void cleanupInstance();

	void fireEvent( const Event& theEvent );
	void addListener( Event::EventType type, EventListener* pListener );
	void removeListener(Event::EventType type, EventListener* pListener );
	void removeListenerFromAllEvents( EventListener* pListener );

private:
	static EventSystem* mspInstance;

	multimap< Event::EventType, EventListener* > mListenerMap;

	void dispatchAllEvents( const Event& theEvent );
	EventSystem();
	~EventSystem();

};

