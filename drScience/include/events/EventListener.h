//
//  EventListener.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_EventListener_h
#define drScience_EventListener_h

#include "utils/Constants.h"

class EventListener
{
public:

    virtual void onEvent(const EventPtr& event)=0;
    
protected:
    
private:
    
};


#endif
