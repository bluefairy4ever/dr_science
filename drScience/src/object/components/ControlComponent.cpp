
#include "graphics/Animation.h"

#include "events/Event.h"
#include "events/EventTypes.h"
#include "events/ToggleEvent.h"
#include "events/ActionEvent.h"
#include "events/RoomTransitionEvent.h"

#include "object/GameObject.h"
#include "object/PhysicsObject.h"
#include "object/components/Component.h"
#include "object/components/ControlComponent.h"

#include "utils/Constants.h"

#include "systems/Blackboard.h"
#include "systems/EventManager.h"

#include "boost/shared_ptr.hpp"

#include <iostream>

using std::cout;
using std::endl;

ControlComponent::ControlComponent(GameObjectPtr parent) :
	Component(parent),
	_controlStates(NULL),
	_body(NULL)
{

}

ControlComponent::~ControlComponent()
{
//	printf("Deleting ControlComponent from %s\n", _parent.get()->getName().c_str());
	if (_controlStates != NULL) { 
		delete [] _controlStates;
		_controlStates = NULL;
	}
}

/**
 *  This animation can only come from a single texture object
 *  which is given as a parameter
 *  @param imgFile - a link to the image we will load that contains
 *      all of the potential animations.
 */
int ControlComponent::initialize()
{
	_controlStates = new bool[4];
	_controlStates[0] = false;  // Forward
	_controlStates[1] = false;  // Backward
	_controlStates[2] = false;  // Left
	_controlStates[3] = false;  // Right

	Blackboard::get()->setControlled(_parent);
	
	boost::shared_ptr<PhysicsObject> physicsObj = boost::static_pointer_cast<PhysicsObject>(_parent);
	_body = physicsObj.get()->getBody();
    _body->SetFixedRotation(true);

	EventManager::get()->registerForEvents(EVENT_MOVEMENT_FORWARD, _parent.get()->getId(), EventListenerPtr(this));
	EventManager::get()->registerForEvents(EVENT_MOVEMENT_BACKWARD, _parent.get()->getId(), EventListenerPtr(this));
	EventManager::get()->registerForEvents(EVENT_MOVEMENT_LEFT, _parent.get()->getId(), EventListenerPtr(this));
    EventManager::get()->registerForEvents(EVENT_MOVEMENT_RIGHT, _parent.get()->getId(), EventListenerPtr(this));
    EventManager::get()->registerForEvents(EVENT_ROOM_TRANSITION, _parent.get()->getId(), EventListenerPtr(this));
    EventManager::get()->registerForEvents(EVENT_CONTROL_ACTION, _parent.get()->getId(), EventListenerPtr(this));
    EventManager::get()->registerForAll(EVENT_SCREEN_TRANSITION, EventListenerPtr(this));

    cout << _body->GetPosition().x << " " << _body->GetPosition().y << endl;
	return SUCCESS;
}
    
/**
 *  Update the control component, moving the physics body as necessary
 *  @param elapsed - the time that has elapsed since the last update.
 */
void ControlComponent::update(float elapsed)
{
//	cout << "Update Control Component " << endl;
	updateFB(elapsed);
	updateLR(elapsed);    
}

/**
 *  Render the active animation onto the screen.
 *  @param window - the screen to render to.
 */
void ControlComponent::render(sf::RenderWindow& window)
{
}

/** 
 *  Update the body when we receive a forward or backward
 *  message.
 *  @param elapsed - the amount of time that has elapsed
 */
void ControlComponent::updateFB(float elapsed)
{
    b2Vec2 velocity = b2Vec2(_body->GetLinearVelocity().x, 0);

    if ( !_controlStates[0] || !_controlStates[1] ) {
	    if (_controlStates[0]) {
            velocity.y = -MAX_SPEED;
	    } else if (_controlStates[1]) {
            velocity.y = MAX_SPEED;
	    }
    }

//	cout << "Mass: " << mass << " Angle: " << angle << endl;

//	 cout << "Direction: " << direction.x << "," << direction.y << " Position: " << position.x << "," << position.y << endl;

    _body->SetLinearVelocity(velocity);
	_body->SetAwake(true);
}

/**
 *  Update the entity in case it is moving left or right.
 *  @param elapsed
 */
void ControlComponent::updateLR(float elapsed)
{	
    b2Vec2 velocity = b2Vec2(0, _body->GetLinearVelocity().y);

    if ( !_controlStates[2] || !_controlStates[3] ) {
	    if (_controlStates[2]) { 
            velocity.x = -MAX_SPEED;
	    } else if (_controlStates[3]) { 
            velocity.x = MAX_SPEED;
	    }
    }

//	cout << "Mass: " << mass << " Angle: " << angle << endl;
    
//	 cout << "Direction: " << direction.x << "," << direction.y << " Position: " << position.x << "," << position.y << endl;
	
    _body->SetLinearVelocity(velocity);
    _body->SetAwake(true);
}

/**
 *  onEvent is called when we receive play animation events
 *  @param anEvent - the event we received
 */
void ControlComponent::onEvent(const EventPtr& anEvent)
{
	EventType type = anEvent.get()->getEventType();

    if (type >= EVENT_MOVEMENT_FORWARD && type <= EVENT_MOVEMENT_RIGHT) {
	    boost::shared_ptr<ToggleEvent> evt = boost::static_pointer_cast<ToggleEvent>(anEvent);

	    switch (type) { 
	    case EVENT_MOVEMENT_FORWARD:
		    _controlStates[0] = evt.get()->getStatus();
		    break;
	    case EVENT_MOVEMENT_BACKWARD:
		    _controlStates[1] = evt.get()->getStatus();
		    break;
	    case EVENT_MOVEMENT_LEFT:
		    _controlStates[2] = evt.get()->getStatus();
		    break;
	    case EVENT_MOVEMENT_RIGHT:
		    _controlStates[3] = evt.get()->getStatus();
		    break;
	    }
    } else if ( type == EVENT_CONTROL_ACTION ) {
        cout << "Action key pressed\n";
    } else if (type == EVENT_ROOM_TRANSITION) {
        // Relocate player to new position upon room transition
	    boost::shared_ptr<RoomTransitionEvent> evt = boost::static_pointer_cast<RoomTransitionEvent>(anEvent);

        b2Vec2 nextPos = b2Vec2(evt->getNextPosition().x, evt->getNextPosition().y);

        _body->SetTransform(nextPos, _body->GetAngle());
        _parent->setPosition(nextPos.x, nextPos.y);
    } else if ( type == EVENT_SCREEN_TRANSITION ) {
        // When random battle is initiated, set all controls to false
        for( int i=0; i < 4; i++ ) {
            _controlStates[i] = false;
        }
    }
}