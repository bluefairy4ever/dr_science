//
//  EventManager.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_EventManager_h
#define drScience_EventManager_h

#include "utils/Constants.h"
#include "events/Event.h"
#include "events/EventListener.h"
#include "events/EventTypes.h"

#include <vector>
#include <map>

using namespace std;
using std::vector;
using std::map;
using std::pair;

class EventListener;

typedef vector<EventListenerPtr> CallbackList;

class EventManager
{
public:

	static EventManager* get();

	void registerForAll(const EventType& type, const EventListenerPtr& callback);
	void registerForEvents(const EventType& type, const int& objectId, const EventListenerPtr& callback);

	void dispatch(EventPtr anEvent);
	void dispatchImmediate(EventPtr anEvent);

	void update(float elapsed);

	// TODO: allow objects to unregister for events....
	//   most likely occurs when an object dies or is removed from the game.
	//   also occurs every time we load a new level.

protected:
    
private:
	EventManager() {};
	EventManager(EventManager const&) {};
	EventManager& operator=(EventManager const&) {};

	static EventManager* _instance;

    vector<EventPtr> _queue;
	map< EventType, map<int,CallbackList> > _unique;
	map< EventType, CallbackList > _all;
};

#endif
