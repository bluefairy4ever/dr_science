//
//  AudioSubsystem.cpp
//  drScience
//
//  Created by Matthew Parish on 12/05/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "systems/AudioSubsystem.h"
#include "systems/EventManager.h"
#include "events/AudioEvent.h"
#include "events/ToggleEvent.h"

AudioSubsystem::AudioSubsystem() : _status(true), _theme(NO_THEME)
{
}

AudioSubsystem::~AudioSubsystem()
{
}

int AudioSubsystem::initialize()
{
    // Register for audio events
    EventManager::get()->registerForAll(EVENT_AUDIO_EFFECT, EventListenerPtr(this));
    EventManager::get()->registerForAll(EVENT_AUDIO_THEME, EventListenerPtr(this));
    EventManager::get()->registerForAll(EVENT_AUDIO_STATUS, EventListenerPtr(this));

    // Open and initialize themes
    _themes[MENU_THEME].openFromFile(MENU_THEME_PATH);
    _themes[GAME_THEME].openFromFile(GAME_THEME_PATH);
    _themes[BATTLE_THEME].openFromFile(BATTLE_THEME_PATH);

    for( int i=0; i<THEMES_TOTAL; i++ ) {
        _themes[i].setLoop(true);
        _themes[i].setVolume(70.0f);
    }

    // Open and initialize effects
    _effects[ATTACK_SOUND].openFromFile(ATTACK_SOUND_PATH);
	_effects[ENEMY_ATTACK_SOUND].openFromFile(ENEMY_ATTACK_SOUND_PATH);
	_effects[ENEMY_DEATH_SOUND].openFromFile(ENEMY_DEATH_SOUND_PATH);
    _effects[HEALTHPACK_SOUND].openFromFile(HEALTHPACK_SOUND_PATH);
	_effects[CURSOR_SOUND].openFromFile(CURSOR_SOUND_PATH);
	_effects[CANCEL_SOUND].openFromFile(CANCEL_SOUND_PATH);
    _effects[DEFEND_SOUND].openFromFile(DEFEND_SOUND_PATH);

    return SUCCESS;
}

void AudioSubsystem::update(float elapsed)
{
}

void AudioSubsystem::render(sf::RenderWindow &window)
{
}

void AudioSubsystem::finish()
{
}

void AudioSubsystem::setTheme(AudioThemes theme)
{
    if ( _theme != BATTLE_THEME ) { // Battle theme has to restart, so only pause if !Battle theme
        _themes[_theme].pause();     // Pause currently playing theme
    } else {
        _themes[_theme].stop();     // Stop 
    }
    _theme = theme;             // Set new theme
    if ( _theme != NO_THEME ) {     // Don't play a theme if set to NO_THEME
        _themes[_theme].play();     // Play new theme
    }
}

void AudioSubsystem::onEvent(const EventPtr& anEvent)
{
    EventType type = anEvent.get()->getEventType();

    // Set system to on or off
    if ( type == EVENT_AUDIO_STATUS ) {
        _status = !_status;
        if ( _status ) {
            _themes[_theme].play();
        } else {
            _themes[_theme].pause();
        }
        /*boost::shared_ptr<ToggleEvent> sEvt = boost::static_pointer_cast<ToggleEvent>(anEvent);
        setStatus(sEvt->getStatus());*/
    } else {
        // Only process audio events if system is on
        if ( _status ) {
            if ( type == EVENT_AUDIO_THEME ) {
                boost::shared_ptr<AudioEvent> aEvt = boost::static_pointer_cast<AudioEvent>(anEvent);
                setTheme(aEvt->getTheme());   // Set new theme
            } else if ( type == EVENT_AUDIO_EFFECT ) {
                boost::shared_ptr<AudioEvent> aEvt = boost::static_pointer_cast<AudioEvent>(anEvent);
                _effects[aEvt->getEffect()].play();  // Play effect
            } else {
                return;
            }
        }
    }
}
