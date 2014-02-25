//
//  GameplayScreen.cpp
//  drScience
//
//  Created by Wesley Kerr on 9/16/12.
//  Modified by Matthew Parish on 10/15/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "utils/Constants.h"
#include "screen/GameplayScreen.h"

#include "events/ActionEvent.h"
#include "events/PlayAnimationEvent.h"
#include "events/RoomTransitionEvent.h"
#include "events/ScreenTransitionEvent.h"
#include "systems/battle/BattleSystem.h"

#include "systems/Blackboard.h"
#include "systems/EventManager.h"
#include <SFML/Window.hpp>

/**
 *  Constructor for the GameplayScreen
 *  @param myId - the id of this screen
 */
GameplayScreen::GameplayScreen(GameScreen myId) : 
    _id(myId), _activeScreen(myId), _battleChance(0)
{
    _eventMap.insert(pair<Keyboard::Key, boost::shared_ptr<ToggleEvent>>(Keyboard::Up, boost::shared_ptr<ToggleEvent>(new ToggleEvent(EVENT_MOVEMENT_FORWARD))));
	_eventMap.insert(pair<Keyboard::Key, boost::shared_ptr<ToggleEvent>>(Keyboard::Down, boost::shared_ptr<ToggleEvent>(new ToggleEvent(EVENT_MOVEMENT_BACKWARD))));
	_eventMap.insert(pair<Keyboard::Key, boost::shared_ptr<ToggleEvent>>(Keyboard::Left, boost::shared_ptr<ToggleEvent>(new ToggleEvent(EVENT_MOVEMENT_LEFT))));
	_eventMap.insert(pair<Keyboard::Key, boost::shared_ptr<ToggleEvent>>(Keyboard::Right, boost::shared_ptr<ToggleEvent>(new ToggleEvent(EVENT_MOVEMENT_RIGHT))));        
    
    EventManager::get()->registerForAll(EVENT_SCREEN_TRANSITION, EventListenerPtr(this));
}

/**
 *  Destructor
 */
GameplayScreen::~GameplayScreen()
{
    
}

/**
 *  initialize all of the things that we will be rendering
 *  to the screen.
 */
int GameplayScreen::initialize()
{
	_system = new GameplaySystem();
	_system->initialize();
	_system->loadLevel("world.tmx", "levels/");
    
	// Generate a RoomTransitionEvent to set the first room to the LAB
	RoomTransitionEvent* transition = new RoomTransitionEvent(EVENT_ROOM_TRANSITION);
	transition->setNextRoom(LAB);
	EventManager::get()->dispatch(EventPtr(transition));

    return SUCCESS;
}

/**
 *  This method updates the state of the game.
 *  @param elapsed - the amount of time elapsed since the last
       time we were called.
 */
void GameplayScreen::update(float elapsed) 
{
	_system->update(elapsed);
    
    // Give the player 10 sec of acclimation time
    if ( sf::Time(_clock.getElapsedTime()).asSeconds() > 10.0f ) {
        // Random battle counter update
	    if (_battleChance == 5) {
		    _battleChance = 0;
			BattleSystem::get()->initialize();
            ScreenTransitionEvent* st = new ScreenTransitionEvent(EVENT_SCREEN_TRANSITION);
            st->setNextScreen(BATTLE);
            st->setPrevScreen(_activeScreen);
            EventManager::get()->dispatchImmediate(EventPtr(st));
		    cout << "battle" << endl;
	    }
    }
	
	_battleChance = rand() % 700 + 1;

	// cout << _battleChance<< endl;

}

/**
 *  render handles updating the state of this screen as well
 *  as rendering it to the screen, so it's misnamed.
 *  @param App - the application that gave rise to this screen
 *  @param delta - the amount of time that has elapsed since we last rendered
 */
int GameplayScreen::render(sf::RenderWindow &app)
{
	_system->render(app);

	return _activeScreen;
}

/**
 *  handle key presses.
 *  @param key - the key that was pressed
 */
void GameplayScreen::keyPressed(sf::Keyboard::Key key) 
{
	// check for movement key presses
	map< Keyboard::Key,boost::shared_ptr<ToggleEvent> >::iterator iter;
	iter = _eventMap.find(key);
	if (iter != _eventMap.end()) {
		boost::shared_ptr<ToggleEvent> evt = iter->second;
		evt->setStatus(true);
		evt->clearRecipients();
		evt->addRecipient(Blackboard::get()->getControlled());
		EventManager::get()->dispatch(evt);
		return;
	}

    /*if ( key == Keyboard::S ) { // System Transition Key
		BattleSystem::get()->initialize();
        ScreenTransitionEvent* st = new ScreenTransitionEvent(EVENT_SCREEN_TRANSITION);
        st->setNextScreen(BATTLE);
        st->setPrevScreen(_activeScreen);
        EventManager::get()->dispatchImmediate(EventPtr(st));

    } else*/ if ( key == Keyboard::Z ) { // Action key
        boost::shared_ptr<ActionEvent> action = boost::shared_ptr<ActionEvent>(new ActionEvent(EVENT_CONTROL_ACTION));
        action->addRecipient(Blackboard::get()->getControlled());
        EventManager::get()->dispatch(action);
    } else if ( key == Keyboard::Escape ) { // Start/Pause key
        ScreenTransitionEvent* st = new ScreenTransitionEvent(EVENT_SCREEN_TRANSITION);
        st->setNextScreen(MENU);
        st->setPrevScreen(_activeScreen);
        EventManager::get()->dispatchImmediate(EventPtr(st));
    }
}

/**
 *   handle key releases.
 *   @param key - the key that was released.
 */
void GameplayScreen::keyReleased(sf::Keyboard::Key key) 
{
	// check for movement key releases
	map< Keyboard::Key,boost::shared_ptr<ToggleEvent> >::iterator iter;
	iter = _eventMap.find(key);
	if (iter != _eventMap.end()) {
		boost::shared_ptr<ToggleEvent> evt = iter->second;
		evt->setStatus(false);
		evt->clearRecipients();
		evt->addRecipient(Blackboard::get()->getControlled());
		EventManager::get()->dispatch(evt);
		return;
	}
}

void GameplayScreen::onEvent(const EventPtr& anEvent)
{
    EventType type = anEvent.get()->getEventType();
    if ( type == EVENT_SCREEN_TRANSITION ) {
        boost::shared_ptr<ScreenTransitionEvent> evt = boost::static_pointer_cast<ScreenTransitionEvent>(anEvent);       
        _activeScreen = evt->getNextScreen();
        _clock.restart();
    }
}

void GameplayScreen::drawGrid(sf::RenderWindow &app)
{
    sf::Vector2u size = app.getSize();
    sf::RectangleShape rect = sf::RectangleShape();
    rect.setOutlineThickness(0.25f);
    for (int i = 0; i < size.x; i+= 100) {
        rect.setPosition(i,0);
        rect.setSize(sf::Vector2f(1,size.y));
        app.draw(rect);
        
        rect.setPosition(0,i);
        rect.setSize(sf::Vector2f(size.x, 1));
        app.draw(rect);
    }
}