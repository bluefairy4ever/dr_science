//
//  Subsystem.h
//  drScience
//
//  Created by Wesley Kerr on 9/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_Subsystem_h
#define drScience_Subsystem_h

#include <SFML/Graphics.hpp>

class Subsystem
{
public:
    virtual int initialize() = 0;

	virtual void update(float elapsed)=0;
    virtual void render(sf::RenderWindow &app) = 0;
};

#endif
