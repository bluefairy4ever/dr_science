
#include "graphics/Animation.h"

#include "events/Event.h"
#include "events/EventTypes.h"
#include "events/PlayAnimationEvent.h"

#include "object/GameObject.h"
#include "object/components/Component.h"
#include "object/components/SpriteComponent.h"

#include "systems/EventManager.h"
#include "boost/shared_ptr.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>

using sf::Vector2f;
using std::cout;
using std::endl;

SpriteComponent::SpriteComponent(GameObjectPtr parent) :
	Component(parent)
{

}

SpriteComponent::~SpriteComponent()
{

}

/**
 *  This animation can only come from a single texture object
 *  which is given as a parameter
 *  @param imgFile - a link to the image we will load that contains
 *      all of the potential animations.
 */
int SpriteComponent::initialize(const string& imgFile)
{
    bool success = _texture.loadFromFile(imgFile);
	if (!success)
        return LOAD_ERROR;

	_sprite = sf::Sprite(_texture);
	_sprite.setOrigin(_texture.getSize().x/2, _texture.getSize().y/2);
	return SUCCESS;
}

    
/**
 *  Update the animation component, moving to new animations as necessary
 *  @param elapsed - the time that has elapsed since the last update.
 */
void SpriteComponent::update(float elapsed)
{
	const Vector2f& position = _parent->getPosition();
//	cout << " Position: " << position.x << "," << position.y << endl;
	_sprite.setPosition(_parent->getPosition().x, _parent->getPosition().y);
	_sprite.setRotation(_parent->getRotation());
}

/**
 *  Render the active animation onto the screen.
 *  @param window - the screen to render to.
 */
void SpriteComponent::render(sf::RenderWindow& window)
{
	window.draw(_sprite);
}
