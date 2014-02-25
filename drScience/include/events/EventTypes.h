//
//  EventListener.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_EventTypes_h
#define drScience_EventTypes_h

enum EventType
{
    //  !!!AS SOME FUNCTIONS RELY ON THE ORDER OF THESE EVENTS,
    //  !!!PLEASE ONLY ADD TO THE BOTTOM OF THE LIST, OR TO THE
    //  !!!BOTTOM OF THE CATEGORY

    //  Animation Events
	EVENT_ANIMATION_START,

    //  Initialization Events
	EVENT_LEVEL_LOAD_START,
	EVENT_LEVEL_LOAD_END,
	EVENT_CREATE_TILED_OBJECT,

    //  Control Events
	EVENT_MOVEMENT_FORWARD,
	EVENT_MOVEMENT_BACKWARD,
	EVENT_MOVEMENT_LEFT,
	EVENT_MOVEMENT_RIGHT,
    EVENT_CONTROL_ACTION,

    // Health Events
    EVENT_HEALTH_MODIFICATION,
    EVENT_DEATH,

    //  Physics Events
	EVENT_CONTACT_BEGIN,
	EVENT_CONTACT_END,

    //  Transition Events
	EVENT_ROOM_TRANSITION,
    EVENT_SCREEN_TRANSITION,

    // Audio Events
    EVENT_AUDIO_THEME,
    EVENT_AUDIO_EFFECT,
    EVENT_AUDIO_STATUS

};


#endif