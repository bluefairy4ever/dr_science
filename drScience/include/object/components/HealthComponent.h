//
//  HealthComponent.h
//  drScience
//
//  Created by Matthew Parish 10/17/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Bloody_Hell_HealthComponent_h
#define Bloody_Hell_HealthComponent_h

#include <map>
#include <string>

using std::map;
using std::pair;
using std::string;

#include "utils/Constants.h"
#include "events/EventListener.h"

#include "object/components/Component.h"
#include "object/components/ComponentTypes.h"
#include "object/components/SpriteComponent.h"
#include "graphics/Animation.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class GameObject;

class HealthComponent : public Component, public EventListener
{
public:
    HealthComponent(GameObjectPtr parent);
    ~HealthComponent();
    
	int initialize(const string& healthImgFile, const string& lowHealthImgFile);
    void update(float elapsed);
    void render(sf::RenderWindow &window);

	virtual void onEvent(const EventPtr& anEvent);
    
protected:
    
private:
    sf::Texture _healthTexture;
    sf::Texture _lowHealthTexture;
    sf::Sprite _sprite;

    bool _lowHealth;

	float _health;
};


#endif
