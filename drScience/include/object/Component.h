//
//  Component.h
//  Bloody Hell
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Bloody_Hell_Component_h
#define Bloody_Hell_Component_h

#include "utils/Constants.h"

#include <SFML/Graphics.hpp>

class GameObject;

class Component
{
public:
    Component(GameObjectPtr parent);
    ~Component();
    
    virtual void update(float elapsed)=0;
    virtual void render(sf::RenderWindow &window);
    
protected:
    GameObjectPtr _parent;
    
private:

};

#endif
