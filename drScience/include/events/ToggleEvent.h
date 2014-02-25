//
//  ToggleEvent.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_ToggleEvent_h
#define drScience_ToggleEvent_h

#include "utils/Constants.h"

#include "events/Event.h"
#include "events/EventTypes.h"

#include <vector>
using std::vector;

class ToggleEvent : public Event
{
public:
	ToggleEvent(EventType type) : Event(type) { }
	~ToggleEvent() { }

	bool getStatus() { return _status; }
	void setStatus(bool status) { _status = status; }

protected:
    
private:
	bool _status;    
};

#endif
