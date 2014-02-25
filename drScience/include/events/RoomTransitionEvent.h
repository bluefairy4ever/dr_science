//
//  TransitionEvent.h
//  drScience
//
//  Created by Matthew Parish on 10/15/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_TransitionEvent_h
#define drScience_TransitionEvent_h

#include "utils/Constants.h"

#include "events/Event.h"
#include "events/EventTypes.h"

#include "object/GameObject.h"
#include "object/Rooms.h"

#include <vector>

using std::vector;

class RoomTransitionEvent : public Event
{
public:
	RoomTransitionEvent(EventType type) : Event(type) { }
	~RoomTransitionEvent() { }

	const RoomsEnum& getNextRoom() { return _nextRoom; }
    const Vector2f& getNextPosition() { return _nextPosition; }
	void setNextRoom(RoomsEnum nextRoom) { _nextRoom = nextRoom; }
    void setNextPosition(Vector2f nextPosition) { _nextPosition = nextPosition; }

protected:
    
private:
	RoomsEnum _nextRoom;
    Vector2f _nextPosition;
};

#endif
