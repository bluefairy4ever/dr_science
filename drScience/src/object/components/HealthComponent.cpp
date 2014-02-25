//
//  HealthComponent.cpp
//  drScience
//
//  Created by Matthew Parish 10/17/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "events/Event.h"
#include "events/EventTypes.h"
#include "events/ValueEvent.h"
#include "events/DeathEvent.h"
#include "events/ScreenTransitionEvent.h"

#include "object/GameObject.h"
#include "object/PhysicsObject.h"
#include "object/components/Component.h"
#include "object/components/SpriteComponent.h"
#include "object/components/HealthComponent.h"
#include "object/components/ComponentTypes.h"

#include "utils/Constants.h"

#include "systems/Blackboard.h"
#include "systems/EventManager.h"

#include "boost/shared_ptr.hpp"

#include <iostream>

using std::cout;
using std::endl;

HealthComponent::HealthComponent(GameObjectPtr parent) :
    Component(parent), _lowHealth(false), _health(100.0f)
{
    EventManager::get()->registerForAll(EVENT_HEALTH_MODIFICATION, EventListenerPtr(this));
}

HealthComponent::~HealthComponent()
{
}
    
int HealthComponent::initialize(const string& healthImgFile, const string& lowHealthImgFile)
{
    //  Initialize sprite's position over the top of the parent object
	if (!_healthTexture.loadFromFile(healthImgFile) && !_lowHealthTexture.loadFromFile(lowHealthImgFile))
        return LOAD_ERROR;
    
	_sprite = sf::Sprite(_healthTexture);

    return SUCCESS;
}

void HealthComponent::update(float elapsed)
{
    _sprite.setPosition(_parent->getPosition().x - 16.0f, _parent->getPosition().y - 36.0f);
    _sprite.setScale(_health / 100.0f, 1.0f); 
}

void HealthComponent::render(sf::RenderWindow &window)
{
    // Need to be able to get to sprite's position elements, but they are not accessible
    // by this class...
    window.draw(_sprite);
}

void HealthComponent::onEvent(const EventPtr& anEvent)
{
    cout << "Health modification event received\n";
    boost::shared_ptr<ValueEvent> evt = boost::static_pointer_cast<ValueEvent>(anEvent);

    if(_health < 100.0f - evt.get()->getValue()) {
        _health += evt.get()->getValue();
        /*if (_health <= 20.0f && !_lowHealth) {
            _lowHealth = true;
            _sprite.setTexture(_lowHealthTexture);
        }
        else {
            _lowHealth = false;
            _sprite.setTexture(_healthTexture);
        }*/   

        if (_health <= 0.0f) {
            // Generate a DeathEvent for the recipient of this collision
		    DeathEvent* death = new DeathEvent(EVENT_DEATH);
		    death->setObjectID(_parent->getId());
		    EventManager::get()->dispatchImmediate(EventPtr(death));

            // Switch to "game over" screen
            ScreenTransitionEvent* st = new ScreenTransitionEvent(EVENT_SCREEN_TRANSITION);
            st->setNextScreen(GAME_OVER);
            EventManager::get()->dispatchImmediate(EventPtr(st));
        }
    }
    else
        _health = 100.0f; 
    
    cout << "Current Health: " << _health << endl;    
}