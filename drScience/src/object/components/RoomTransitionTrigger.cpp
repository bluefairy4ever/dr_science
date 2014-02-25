//
//  RoomTransitionTrigger.cpp
//  drScience
//
//  Created by Matthew Parish on 10/15/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "events/Event.h"
#include "events/EventTypes.h"
#include "events/ContactEvent.h"
#include "events/RoomTransitionEvent.h"

#include "object/GameObject.h"
#include "object/PhysicsObject.h"
#include "object/components/Component.h"
#include "object/components/RoomTransitionTrigger.h"

#include "systems/EventManager.h"
#include "systems/Blackboard.h"
#include "boost/shared_ptr.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <cmath>

using sf::Vector2f;
using std::cout;
using std::endl;
using std::max;

RoomTransitionTrigger::RoomTransitionTrigger(GameObjectPtr parent) :
	Component(parent)
{

}

RoomTransitionTrigger::~RoomTransitionTrigger()
{

}

/**
 *  @param nextRoom - the screen that should be set to the active screen
 */
int RoomTransitionTrigger::initialize(RoomsEnum nextRoom, float nextX, float nextY)
{
    _nextRoom = nextRoom;
    _nextPosition.x = nextX;
    _nextPosition.y = nextY;

	EventManager::get()->registerForEvents(EVENT_CONTACT_BEGIN, _parent.get()->getId(), EventListenerPtr(this));
	return SUCCESS;
}
    
/**
 *  
 *  @param elapsed - the time that has elapsed since the last update.
 */
void RoomTransitionTrigger::update(float elapsed)
{
}

/**
 *  onEvent is called when something collides with this trigger
 *  @param anEvent - the event we received
 */
void RoomTransitionTrigger::onEvent(const EventPtr& anEvent)
{
    cout << "RoomTransitionTrigger activated\n";

	EventType type = anEvent.get()->getEventType();
	if (type == EVENT_CONTACT_BEGIN) { 
		boost::shared_ptr<ContactEvent> evt = boost::static_pointer_cast<ContactEvent>(anEvent);
		// here you can test to see if the collision needs to be 
		// processed by this Transition trigger.  Things like bullets
		// should not trigger the Transition.
		PhysicsObject* objA = evt.get()->getObjectA();
		PhysicsObject* objB = evt.get()->getObjectB();

		PhysicsObject* recepient = objA;
		if (objA->getId() == _parent.get()->getId()) { 
			recepient = objB;
        }

		// Generate a RoomTransitionEvent for the recipient of this collision
		RoomTransitionEvent* transition = new RoomTransitionEvent(EVENT_ROOM_TRANSITION);
		transition->setNextRoom(_nextRoom);
        transition->setNextPosition(_nextPosition + Blackboard::get()->getControlled()->getDimensions());
		EventPtr tEvent(transition);
		tEvent->addRecipient(recepient->getId());
		EventManager::get()->dispatch(tEvent);
		
        cout << "RoomTransitionEvent dispatched. _nextRoom: " << getNextRoomString(_nextRoom) << endl;
	}
}

//  Takes a door name as a string and returns the RoomsEnum of the door's next room
RoomsEnum RoomTransitionTrigger::getNextRoomEnum(const string& room)
{
    int i=4;    // i = index of first char of current room's name
    string current, next;

    //  Find next capital letter, indicating name of next room,
    //  and store name of current room
    current += room[i++];
    while(room[i] < 'A' || room[i] > 'Z')
        current += room[i++];

    cout << current << endl;

    //  Get complete name of next room
    while(room[i] != NULL)
        next += room[i++];

    cout << next << endl;

    //  Return correct value of next room
    if (next == "Lab") {
        return LAB;
    } else if (next == "Animal") {
        return ANIMAL;
    } else if (next == "Board") {
        return BOARD;
    } else if (next == "Executive") {
        return EXECUTIVE;
    } else if (next == "Lobby") {
        return LOBBY;
    } else if (next == "Locker") {
        return LOCKER;
    } else if (next == "Observation") {
        return OBSERVATION;
    } else if (next == "Record") {
        return RECORD;
    } else if (next == "Security") {
        return SECURITY;
    } else if (next == "Trap") {
        return TRAP;
    } else {
        cout << "ERROR: " << next << " is not defined.\n";
        return LAB;
    }
};

// Takes the RoomsEnum of the next room and returns the name as a string
const string RoomTransitionTrigger::getNextRoomString(RoomsEnum next)
{
    switch(next) {
    case LAB: return "Lab";
    case ANIMAL: return "Animal";
    case BOARD: return "Board";
    case EXECUTIVE: return "Executive";
    case LOBBY: return "Lobby";
    case LOCKER: return "Locker";
    case OBSERVATION: return "Observation";
    case RECORD: return "Record";
    case SECURITY: return "Security";
    case TRAP: return "Trap";
    default: return "Undefined";
    }
};