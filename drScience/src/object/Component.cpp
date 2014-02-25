//
//  Component.cpp
//  Bloody Hell
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "object/Component.h"
#include "object/GameObject.h"

Component::Component(GameObjectPtr parent)
{
    _parent.reset(parent.get());
}

Component::~Component()
{
    
}

/**
 * Override this method if the component is
 * responsible for drawing anything....
 */
void Component::render(sf::RenderWindow &window)
{
    // by default do nothing...
}