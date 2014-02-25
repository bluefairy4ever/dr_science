//
//  Event.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_Event_h
#define drScience_Event_h

#include "utils/Constants.h"
#include "events/EventTypes.h"

#include <vector>
using std::vector;

class Event
{
public:
    Event(EventType type);
    ~Event();

	EventType getEventType();
    
    void addRecipient(GameObjectPtr obj);
	void addRecipient(int objId);

    vector<int> getRecipients();
    
	void clearRecipients();
    
protected:
    
private:
	EventType _type;
    vector<int> _recipients;
    
};

#endif
