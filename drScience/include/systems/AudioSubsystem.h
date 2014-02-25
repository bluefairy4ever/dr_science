//
//  AudioSubsystem.h
//  drScience
//
//  Created by Matthew Parish on 12/05/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_AudioSubsystem_h
#define drScience_AudioSubsystem_h

#include "systems/Subsystem.h"
#include "events/EventListener.h"
#include "utils/Constants.h"

#include <SFML/Audio.hpp>

using sf::Music;

enum AudioThemes {
    NO_THEME,
    MENU_THEME,
    GAME_THEME,
    BATTLE_THEME,
    THEMES_TOTAL
};

enum AudioFX {
    ACTION_SOUND,
    ATTACK_SOUND,
	ENEMY_ATTACK_SOUND,
    ENEMY_DEATH_SOUND,
	CURSOR_SOUND,
	CANCEL_SOUND,
	DEFEND_SOUND,
    HEALTHPACK_SOUND,
    EFFECTS_TOTAL
};

class AudioSubsystem : public Subsystem, public EventListener
{
public:
    AudioSubsystem();
	~AudioSubsystem();

	int initialize();
	void update(float elapsed);
	void render(sf::RenderWindow &window);
	void finish();

    bool getStatus()const {return _status;}
    void setStatus(bool status) {_status = status;}

    AudioThemes getTheme()const {return _theme;}
    void setTheme(AudioThemes theme);

    virtual void onEvent(const EventPtr& anEvent);

protected:
    
private:
    Music _themes[THEMES_TOTAL];
    Music _effects[EFFECTS_TOTAL];

    AudioThemes _theme;

    bool _status;
};

#endif
