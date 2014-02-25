//
//  RoomTransitionTrigger.h
//  drScience
//
//  Created by Matthew Parish on 10/15/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_TransitionTrigger_h
#define drScience_TransitionTrigger_h

#include <string>

using std::string;

#include "utils/Constants.h"
#include "events/EventListener.h"

#include "object/components/Component.h"
#include "graphics/Animation.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <vector>

class GameObject;

class RoomTransitionTrigger : public Component, public EventListener
{
public:
    RoomTransitionTrigger(GameObjectPtr parent);
    ~RoomTransitionTrigger();
    
	int initialize(RoomsEnum nextRoom, float nextX, float nextY);
    void update(float elapsed);

	virtual void onEvent(const EventPtr& anEvent);
    RoomsEnum getNextRoomEnum(const string& room);
    const string getNextRoomString(RoomsEnum next);
    
protected:

private:
    RoomsEnum _nextRoom;
    Vector2f _nextPosition;
};


#endif
