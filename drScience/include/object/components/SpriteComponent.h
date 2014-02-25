//
//  AnimationComponent.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_SpriteComponent_h
#define drScience_SpriteComponent_h

#include <map>
#include <string>

using std::map;
using std::pair;
using std::string;

#include "utils/Constants.h"

#include "object/components/Component.h"
#include <SFML/Graphics.hpp>

class GameObject;

class SpriteComponent : public Component
{
public:
    SpriteComponent(GameObjectPtr parent);
    ~SpriteComponent();
    
    int initialize(const string& imgFile);

    void update(float elapsed);
    void render(sf::RenderWindow &window);

protected:
    
private:
	 sf::Texture _texture;
	 sf::Sprite _sprite;
};


#endif
