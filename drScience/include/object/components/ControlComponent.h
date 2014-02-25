//
//  ControlComponent.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_ControlComponent_h
#define drScience_ControlComponent_h

#include <map>
#include <string>

using std::map;
using std::pair;
using std::string;

#include "utils/Constants.h"
#include "events/EventListener.h"

#include "object/components/Component.h"
#include "graphics/Animation.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class GameObject;

class ControlComponent : public Component, public EventListener
{
public:
    ControlComponent(GameObjectPtr parent);
    ~ControlComponent();
    
	int initialize();
    void update(float elapsed);
    void render(sf::RenderWindow &window);

	virtual void onEvent(const EventPtr& anEvent);
    
protected:

	void updateFB(float elapsed);
	void updateLR(float elapsed);
    
private:
	bool* _controlStates;
	b2Body* _body;
};


#endif
