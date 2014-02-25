//
//  PlayAnimationEvent.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_ContactEvent_h
#define drScience_ContactEvent_h

#include "events/Event.h"

#include <string>

using std::string;

class ContactEvent : public Event
{
public:
    ContactEvent(const EventType& type, PhysicsObject* objA, PhysicsObject* objB);
    
	PhysicsObject* getObjectA();
	PhysicsObject* getObjectB();

protected:
    
private:
	PhysicsObject* _objA;
	PhysicsObject* _objB;
};

#endif
