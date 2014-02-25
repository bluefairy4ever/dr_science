//
//  AudioEvent.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_AudioEvent_h
#define drScience_AudioEvent_h

#include "utils/Constants.h"

#include "events/Event.h"
#include "events/EventTypes.h"
#include "systems/AudioSubsystem.h"

class AudioEvent : public Event
{
public:
	AudioEvent(EventType type) : Event(type) { }
	~AudioEvent() { }

	AudioThemes getTheme() { return _theme; }
	void setTheme(AudioThemes theme) { _theme = theme; }
    AudioFX getEffect() { return _effect; }
	void setEffect(AudioFX effect) { _effect = effect; }

protected:
    
private:
    AudioThemes _theme;
    AudioFX _effect;
};

#endif
