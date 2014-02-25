//
//  SplashScreen.cpp
//  ClassDemo
//
//  Created by Wesley Kerr on 9/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "screen/SplashScreen.h"
#include "events/ScreenTransitionEvent.h"
#include "systems/EventManager.h"
#include "utils/Constants.h"

#include <iostream>
using std::cout;
using std::endl;

/**
 *  Constructor for the splash screen.  
 *  @param imgFile - the image to be displayed on this screen
 *  @param myId - the id of this splash screen
 *  @param nextScreen - the id of the splash screen to transition to
 */
SplashScreen::SplashScreen(const string& imgFile, GameScreen myId, GameScreen nextScreen) :
    _imgFile(imgFile), 
    _id(myId), 
    _nextScreen(nextScreen),
    _elapsed(0.0f),
    _time(6.0f),
    _texture()
{   
}

/**
 *  Destructor
 */
SplashScreen::~SplashScreen()
{
}

/**
 *  Initialization should be handled inside of the program and
 *  outside of the constructor.
 */
int SplashScreen::initialize()
{
    if (!_texture.loadFromFile(_imgFile))
    	return LOAD_ERROR;

    _sprite = sf::Sprite(_texture);
    _sprite.setPosition(0.0f, 0.0f);
    
    return SUCCESS;
}

void SplashScreen::update(float elapsed) 
{ 
    _elapsed += elapsed;
}

/**
 *  render handles updating the state of this screen as well
 *  as rendering it to the screen, so it's misnamed.
 *  @param App - the application that gave rise to this screen
 *  @param delta - the amount of time that has elapsed since we last rendered
 */
int SplashScreen::render(sf::RenderWindow &app)
{
    app.setView(sf::View(sf::FloatRect(0,0,WINDOW_WIDTH, WINDOW_HEIGHT)));
    _sprite.setPosition(app.getSize().x / 2.0f - (_sprite.getTexture()->getSize().x / 2.0f),
        app.getSize().y / 2.0f - (_sprite.getTexture()->getSize().y / 2.0f));
    app.draw(_sprite);
    
    if (_elapsed > _time) {
        _elapsed = 0.0f;

        // Create screen transition event
        ScreenTransitionEvent* st = new ScreenTransitionEvent(EVENT_SCREEN_TRANSITION);
        st->setNextScreen(_nextScreen);
        st->setPrevScreen(GAMEPLAY);
        EventManager::get()->dispatchImmediate(EventPtr(st));
        return _nextScreen;
    }
    return _id;
}