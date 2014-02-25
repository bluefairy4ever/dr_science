//
//  GameObject.cpp
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "object/GameObject.h"
#include "object/components/Component.h"

/**
 * Construct a new GameObject with the name
 * and id
 * @param id
 * @param name
 */
GameObject::GameObject(int id, string name) : 
    _id(id), 
    _name(name),
	_components()
{ 

}

GameObject::~GameObject()
{

}

/**
 * Add a component to this GameObject
 * 
 * TODO: update the order of components after we add
 * one so that things that need to update or render first
 * are higher in the list of components
 * @param c
 */
void GameObject::addComponent(ComponentPtr c) 
{ 
    _components.push_back(c);
}

/**
 * Sets the position of the object to the given coordinates
 * @param x
 * @param y
 */
void GameObject::setPosition(float x, float y) { 
    _position.x = x;
    _position.y = y;
}

/**
 * Sets the dimensions of the object to the given width & height
 * @param width
 * @param height
 */
void GameObject::setDimensions(float width, float height) {
    _dimensions.x = width;
    _dimensions.y = height;
    cout << "Object dimensions: " << _dimensions.x << "," << _dimensions.y << std::endl;
}

/**
 * Set the rotation of the GameObject
 * @param rotation
 */
void GameObject::setRotation(float rotation) { 
    _rotation = rotation;
}


/**
 * Update each of the components of this GameObject
 * @param elapsed
 */
void GameObject::update(float elapsed) 
{ 
    vector<ComponentPtr>::iterator iter;
    for (iter = _components.begin(); iter < _components.end(); ++iter) {
        (*iter).get()->update(elapsed);
    }
}

/**
 * Render each of the components of this GameObject
 */
void GameObject::render(sf::RenderWindow &window) 
{
    vector<ComponentPtr>::iterator iter;
    for (iter = _components.begin(); iter < _components.end(); ++iter) { 
        (*iter).get()->render(window);
    }
}

