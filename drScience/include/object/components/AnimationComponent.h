//
//  AnimationComponent.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_AnimationComponent_h
#define drScience_AnimationComponent_h

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

class GameObject;

/** 
 * this struct is used to keep track of the
 * parameters for the different animations.
 */
struct AnimationParams { 
	int start;
	int end;
	float speed;
	Animation::PlayType type;
};
typedef boost::shared_ptr<AnimationParams> AnimationParamsPtr;

class AnimationComponent : public Component, public EventListener
{
public:
    AnimationComponent(GameObjectPtr parent);
    ~AnimationComponent();
    
    int initialize(const string& imgFile);
	void addAnimation(const string& name, const int& start, const int& end, const float& speed, const Animation::PlayType& type);
    
    void update(float elapsed);
    void render(sf::RenderWindow &window);

	virtual void onEvent(const EventPtr& anEvent);
    
protected:
    
private:
    map<string, AnimationParamsPtr> _animations;

	AnimationParamsPtr _activeParams;
    AnimationPtr _animation;

	 sf::Texture _texture;
};


#endif
