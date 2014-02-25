//
//  ActionEvent.h
//  drScience
//
//  Created by Matthew Parish on 10/18/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_ActionEvent_h
#define drScience_ActionEvent_h

#include "utils/Constants.h"

#include "events/Event.h"
#include "events/EventTypes.h"

class ActionEvent : public Event
{
public:
    ActionEvent(EventType type) : Event(type) {;}
    ~ActionEvent() {;}

protected:
    
private:
};

#endif
