//
//  HealthTrigger.h
//  Bloody Hell
//
//  Created by Wesley Kerr on 9/20/12.
//  Modified by Matthew Parish on 10/16/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef BloodyHell_HealthTrigger_h
#define BloodyHell_HealthTrigger_h

#include <string>

using std::string;

#include "utils/Constants.h"
#include "events/EventListener.h"

#include "object/components/Component.h"
#include "graphics/Animation.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class GameObject;

class HealthTrigger : public Component, public EventListener
{
public:
    HealthTrigger(GameObjectPtr parent);
    ~HealthTrigger();
    
	int initialize(const string& imgFile, int effect);
    void update(float elapsed);
    void render(sf::RenderWindow &window);

	virtual void onEvent(const EventPtr& anEvent);
    
protected:

private:
	ComponentPtr _sprite;
    bool _hasSprite;

	float _delay;
	bool _active;
    int _effect;
};


#endif
