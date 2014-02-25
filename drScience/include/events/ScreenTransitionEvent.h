//
//  ScreenTransitionEvent.h
//  drScience
//
//  Created by Matthew Parish on 11/28/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_ScreenTransitionEvent_h
#define drScience_ScreenTransitionEvent_h

#include "utils/Constants.h"

#include "events/Event.h"
#include "events/EventTypes.h"
#include "screen/Screen.h"

class ScreenTransitionEvent : public Event
{
public:
	ScreenTransitionEvent(EventType type) : Event(type) { }
	~ScreenTransitionEvent() { }

	const GameScreen& getNextScreen() { return _nextScreen; }
    const GameScreen& getPrevScreen() { return _prevScreen; }
	void setNextScreen(GameScreen nextScreen) { _nextScreen = nextScreen; }
    void setPrevScreen(GameScreen prevScreen) { _prevScreen = prevScreen; }

protected:
    
private:
	GameScreen _nextScreen;
    GameScreen _prevScreen;
};

#endif
