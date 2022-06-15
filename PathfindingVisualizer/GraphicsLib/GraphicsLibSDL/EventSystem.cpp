#include "EventSystem.h"
#include "Event.h"
#include "EventListener.h"

#include <cassert>

EventSystem* EventSystem::mspInstance = NULL;

EventSystem::EventSystem()
{
}

EventSystem::~EventSystem()
{
}

void EventSystem::addListener(Event::EventType type, EventListener* pListener)
{
	//make sure we are not adding a duplicate
	//pair<multimap<Event::EventType, EventListener*>::iterator, multimap<Event::EventType, EventListener*>::iterator> ret;

	auto ret = mListenerMap.equal_range(type);
	//multimap<Event::EventType, EventListener*>::iterator iter;

	for (auto iter = ret.first; iter != ret.second; ++iter)
	{
		if (iter->second == pListener)
		{
			return;//already found so just return
		}
	}

	mListenerMap.insert( pair< Event::EventType, EventListener* >(type, pListener) );
}

void EventSystem::removeListener(Event::EventType type, EventListener *pListener)
{
	pair<multimap<Event::EventType,EventListener*>::iterator,multimap<Event::EventType,EventListener*>::iterator> ret;

	ret = mListenerMap.equal_range( type );
	multimap<Event::EventType,EventListener*>::iterator iter;

	for( iter = ret.first; iter != ret.second; ++iter )
	{
		if( iter->second == pListener )
		{
			mListenerMap.erase( iter );
			break;//to prevent using invalidated iterator
		}
	}
}

void EventSystem::removeListenerFromAllEvents( EventListener* pListener )
{
	multimap<Event::EventType,EventListener*>::iterator iter;

	bool allTheWayThrough = false;

	while( !allTheWayThrough )
	{
		allTheWayThrough = true;
		for( iter = mListenerMap.begin(); iter != mListenerMap.end(); ++iter )
		{
			if( iter->second == pListener )
			{
				mListenerMap.erase( iter );
				allTheWayThrough = false; //didn't make it the whole way through
				break;//to prevent using invalidated iterator
			}
		}
	}
}

EventSystem * EventSystem::getInstance()
{
	if (mspInstance == NULL)
	{
		cout << "EventSystem instance - trying to get before initted\n";
		assert(false);
		return NULL;
	}
	else
	{
		return mspInstance;
	}
}

void EventSystem::initInstance()
{
	if (mspInstance != NULL)
	{
		cout << "EventSystem instance - trying to init a non-null instance\n";
		//assert(false);
	}
	else
	{
		mspInstance = new EventSystem;
	}
}

void EventSystem::cleanupInstance()
{
	if (mspInstance != NULL)
	{
		delete mspInstance;
		mspInstance = NULL;
	}
	else
	{
		cout << "EventSystem instance - cleaning up a NULL instance\n";
	}
}

void EventSystem::fireEvent( const Event& theEvent )
{
 	dispatchAllEvents( theEvent );
}

void EventSystem::dispatchAllEvents( const Event& theEvent )
{
	pair<multimap<Event::EventType,EventListener*>::iterator,multimap<Event::EventType,EventListener*>::iterator> ret;
	ret = mListenerMap.equal_range( theEvent.getType() );

	multimap<Event::EventType,EventListener*>::iterator iter;
	for( iter = ret.first; iter != ret.second; ++iter )
	{
		iter->second->handleEvent( theEvent );
	}

}