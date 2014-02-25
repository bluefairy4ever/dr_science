//
//  GameObject.cpp
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "object/PhysicsObject.h"

/**
 * Construct a new GameObject with the name
 * and id
 * @param id
 * @param name
 */
PhysicsObject::PhysicsObject(int id, string name) : 
	GameObject(id, name)
{ 

}

PhysicsObject::~PhysicsObject()
{

}


void PhysicsObject::update(float elapsed)
{
	const b2Vec2& position = _body->GetPosition();
	setPosition(position.x*BOX_TO_WORLD, position.y*BOX_TO_WORLD);

	float rotation = _body->GetAngle();
	setRotation(rotation*RAD_TO_DEG);

	GameObject::update(elapsed);
}