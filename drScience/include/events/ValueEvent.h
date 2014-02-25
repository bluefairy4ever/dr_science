//
//  ValueEvent.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_ValueEvent_h
#define drScience_ValueEvent_h

#include "utils/Constants.h"

#include "events/Event.h"
#include "events/EventTypes.h"

class ValueEvent : public Event
{
public:
	ValueEvent(EventType type) : Event(type) { }
	~ValueEvent() { }

	float getValue() { return _value; }
	void setValue(float value) { _value = value; }

protected:
    
private:
	float _value;    
};

#endif
