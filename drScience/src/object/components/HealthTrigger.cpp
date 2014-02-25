//
//  HealthTrigger.cpp
//  Bloody Hell
//
//  Created by Wesley Kerr on 9/20/12.
//  Modified by Matthew Parish on 10/16/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "graphics/Animation.h"

#include "events/Event.h"
#include "events/EventTypes.h"
#include "events/ContactEvent.h"
#include "events/ValueEvent.h"
#include "events/AudioEvent.h"

#include "object/GameObject.h"
#include "object/PhysicsObject.h"
#include "object/components/Component.h"
#include "object/components/HealthTrigger.h"
#include "object/components/SpriteComponent.h"

#include "systems/EventManager.h"
#include "boost/shared_ptr.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <cmath>

using sf::Vector2f;
using std::cout;
using std::endl;
using std::max;

HealthTrigger::HealthTrigger(GameObjectPtr parent) :
	Component(parent), _hasSprite(false)
{

}

HealthTrigger::~HealthTrigger()
{

}

/**
 *  This animation can only come from a single texture object
 *  which is given as a parameter
 *  @param imgFile - a link to the image we will load that contains
 *      all of the potential animations.
 */
int HealthTrigger::initialize(const string& imgFile, int effect)
{
	SpriteComponent* sprite = new SpriteComponent(_parent);
    //  If HealthTrigger has no sprite, don't load it
    if (imgFile != "") {
        _hasSprite = true;
	    int result = sprite->initialize(imgFile);
	    if (result != SUCCESS)
		    return result;
	    _sprite.reset(sprite);
    }

	_delay = 0;
	_active = true;
    _effect = effect;

	EventManager::get()->registerForEvents(EVENT_CONTACT_BEGIN, _parent.get()->getId(), EventListenerPtr(this));
	return SUCCESS;
}

    
/**
 *  Update the animation component, moving to new animations as necessary
 *  @param elapsed - the time that has elapsed since the last update.
 */
void HealthTrigger::update(float elapsed)
{
	_delay = max(0.0f, (_delay - elapsed));
	_active = _delay == 0;
	if (_hasSprite) _sprite.get()->update(elapsed);
}

/**
 *  Render the active animation onto the screen.
 *  @param window - the screen to render to.
 */
void HealthTrigger::render(sf::RenderWindow& window)
{
	if (_active && _hasSprite) {
		_sprite.get()->render(window);
	}
}

/**
 *  onEvent is called when something collides with this trigger
 *  @param anEvent - the event we received
 */
void HealthTrigger::onEvent(const EventPtr& anEvent)
{
	if (!_active)
		return;

	EventType type = anEvent.get()->getEventType();
	if (type == EVENT_CONTACT_BEGIN) { 
		boost::shared_ptr<ContactEvent> evt = boost::static_pointer_cast<ContactEvent>(anEvent);
		// here you can test to see if the collision needs to be 
		// processed by this health trigger.  Things like bullets
		// should not trigger the health.
		PhysicsObject* objA = evt.get()->getObjectA();
		PhysicsObject* objB = evt.get()->getObjectB();

		PhysicsObject* recepient = objA;
		if (objA->getId() == _parent.get()->getId()) { 
			recepient = objB;
		}
        
		// Generate a HealthEvent for the recipient of this collision
		ValueEvent* value = new ValueEvent(EVENT_HEALTH_MODIFICATION);
		value->setValue(_effect);
		value->addRecipient(recepient->getId());
		EventManager::get()->dispatchImmediate(EventPtr(value));

        // Generate an AudioEvent to play healthpack sound
        AudioEvent* sound = new AudioEvent(EVENT_AUDIO_EFFECT);
        sound->setEffect(HEALTHPACK_SOUND);
        EventManager::get()->dispatchImmediate(EventPtr(sound));

		_delay = 10.0f;
		_active = false;
	}
}