
#include "graphics/Animation.h"

#include "events/Event.h"
#include "events/EventTypes.h"
#include "events/PlayAnimationEvent.h"

#include "object/GameObject.h"
#include "object/Component.h"
#include "object/AnimationComponent.h"

#include "systems/EventManager.h"
#include "boost/shared_ptr.hpp"

AnimationComponent::AnimationComponent(GameObjectPtr parent) :
	Component(parent)
{

}

AnimationComponent::~AnimationComponent()
{

}

/**
 *  This animation can only come from a single texture object
 *  which is given as a parameter
 *  @param imgFile - a link to the image we will load that contains
 *      all of the potential animations.
 */
int AnimationComponent::initialize(const string& imgFile)
{
    bool success = _texture.loadFromFile(imgFile);
	if (!success)
        return LOAD_ERROR;

	_animations = map<string, AnimationParamsPtr>();
	_animation.reset( new Animation(_texture, 80, 80) );

	EventManager::get()->registerForEvents(EVENT_ANIMATION_START, _parent.get()->getId(), EventListenerPtr(this));
	return SUCCESS;
}

/** 
 *  Add an amiation to this component.
 *  @param name - the name to give to the animation
 *  @param start - the start frame for this animation
 *  @param end - the end frame of the animation
 *  @param type - the looping type of the animation.
 */
void AnimationComponent::addAnimation(const string& name, const int& start, const int& end, const float& speed, const Animation::PlayType& type)
{
	AnimationParamsPtr params( new AnimationParams );
	params.get()->start = start;
	params.get()->end = end;
	params.get()->speed = speed;
	params.get()->type = type;

	printf("%s %d %d\n", name.c_str(), params, _animations);
	_animations.insert( make_pair(name, params) );

	if (_activeParams.get() == NULL) { 
		_activeParams = params;
		
		_animation.get()->setPlayType(type);
		_animation.get()->setLoopSpeed(speed);

		printf("Play: %d %d\n", start, end);
		_animation.get()->play(start, end);

	}    
}
    
/**
 *  Update the animation component, moving to new animations as necessary
 *  @param elapsed - the time that has elapsed since the last update.
 */
void AnimationComponent::update(float elapsed)
{
	_animation.get()->update(elapsed);
}

/**
 *  Render the active animation onto the screen.
 *  @param window - the screen to render to.
 */
void AnimationComponent::render(sf::RenderWindow& window)
{
	window.draw(*_animation.get());
}

/**
 *  onEvent is called when we receive play animation events
 *  @param anEvent - the event we received
 */
void AnimationComponent::onEvent(const EventPtr& anEvent)
{
	printf("Received -- %d\n", anEvent.get()->getEventType());

	EventType type = anEvent.get()->getEventType();
	if (type == EVENT_ANIMATION_START) { 
		boost::shared_ptr<PlayAnimationEvent> evt = boost::static_pointer_cast<PlayAnimationEvent>(anEvent);
		map<string, AnimationParamsPtr>::iterator iter;
		iter = _animations.find(evt.get()->getName());

		AnimationParamsPtr params = iter->second;
		_activeParams = params;
		
		_animation.get()->stop();
		_animation.get()->setPlayType(params->type);
		_animation.get()->setLoopSpeed(params->speed);

		_animation.get()->play(params->start, params->end);
	}
}