//
//  DeathEvent.h
//  Bloody Hell
//
//  Created by Matthew Parish on 10/19/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef BloodyHell_DeathEvent_h
#define BloodyHell_DeathEvent_h

#include "utils/Constants.h"

#include "events/Event.h"
#include "events/EventTypes.h"

class DeathEvent : public Event
{
public:
	DeathEvent(EventType type) : Event(type) { }
	~DeathEvent() { }

	int getObjectID() { return _id; }
	void setObjectID(int id) { _id = id; }

protected:
    
private:
	int _id;
};

#endif