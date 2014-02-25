
#include "systems/PhysicsSubsystem.h"
#include "ResourcePath.hpp"

#include "events/Event.h"
#include "events/EventTypes.h"
#include "events/ContactEvent.h"

#include "object/GameObject.h"
#include "object/PhysicsObject.h"

#include "systems/EventManager.h"

#include <Box2D/Box2D.h>
#include <iostream>

using std::cout;
using std::endl;
using std::make_pair;

/**
 *  Create a new PhysicsSubsystem which initializes the
 *  physics world and currently the gravity is set to zero
 */
PhysicsSubsystem::PhysicsSubsystem() 
{
}

/**
 *  Clean every thing up in the destructor.
 */
PhysicsSubsystem::~PhysicsSubsystem()
{

}

/**
 *  initialize the physics world.  
 */
int PhysicsSubsystem::initialize()
{
	_world = new b2World(b2Vec2(0,0));
	_world->SetContactListener(this);

	return SUCCESS;
}

/**
 *  Update the physics of the world.
 *  @param elapsed -- the amount of time that elapsed since the last update.
 */
void PhysicsSubsystem::update(float elapsed)
{
	_world->Step(elapsed, 9, 3);
}

/** 
 *  Render any physics things that you are interested in.
 *  @param window -- the window that we render on
 */
void PhysicsSubsystem::render(sf::RenderWindow &window)
{

}

/**
 *  clean up the physics world.
 */
void PhysicsSubsystem::finish()
{
	delete _world;
	_world = NULL;
}

/**
 *  BeginContact is called everytime that a contact begins
 *  @param contact - the contact that is beginning.
 */
void PhysicsSubsystem::BeginContact(b2Contact* contact)
{
	b2Fixture* fA = contact->GetFixtureA();
	b2Fixture* fB = contact->GetFixtureB();

	PhysicsObject* objA = (PhysicsObject*) fA->GetUserData();
	PhysicsObject* objB = (PhysicsObject*) fB->GetUserData();

	cout << "CollisionBegin "<< objA->getName() << " " << objB->getName() << endl;

	EventPtr evt(new ContactEvent(EVENT_CONTACT_BEGIN, objA, objB));
	evt->addRecipient(objA->getId());
	evt->addRecipient(objB->getId());
	EventManager::get()->dispatch(evt);
}

/**
 *  EndContact is called everytime that a contact ends
 *  @param contact - the contact that is ending.
 */
void PhysicsSubsystem::EndContact(b2Contact* contact)
{
	b2Fixture* fA = contact->GetFixtureA();
	b2Fixture* fB = contact->GetFixtureB();

	PhysicsObject* objA = (PhysicsObject*) fA->GetUserData();
	PhysicsObject* objB = (PhysicsObject*) fB->GetUserData();

	cout << "CollisionEnd "<< objA->getName() << " " << objB->getName() << endl;

	EventPtr evt(new ContactEvent(EVENT_CONTACT_END, objA, objB));
	evt->addRecipient(objA->getId());
	evt->addRecipient(objB->getId());
	EventManager::get()->dispatch(evt);
}

/**
 *  PreSolve - called before in case you want to make modifications to the
 *  contact that is going to occur.
 *  @param contact - the contact that is going to happen
 *  @param oldManifold - the manifold that is changing
 */
void PhysicsSubsystem::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{

}

/** 
 *  PostSolve - called after solve.
 *  @param contact - the contact that is happening
 *  @param impulse - the contact impulse that is being applied.
 */
void PhysicsSubsystem::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}

