//
//  PlayAnimationEvent.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_PlayAnimationEvent_h
#define drScience_PlayAnimationEvent_h

#include "events/Event.h"

#include <string>

using std::string;

class PlayAnimationEvent : public Event
{
public:
    PlayAnimationEvent(const EventType& type, const string& name);
    const string& getName();
    
protected:
    
private:
    string _name;
};

#endif
