//
//  Screen.h
//  drScience
//
//  Created by Wesley Kerr on 9/16/12.
//  Modified by Matthew Parish on 10/15/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ClassDemo_Screen_h
#define ClassDemo_Screen_h

#include <SFML/Graphics.hpp>

enum GameScreen {
    QUIT=-1,
    INTRO_SPLASH=0,
    GAME_OVER,
    YOU_WIN,
    GAMEPLAY,
    BATTLE,
	MENU
};

class Screen
{
public:
    virtual int render(sf::RenderWindow &app) = 0;
	virtual void update(float delta)=0;
    virtual int initialize() = 0;
        
	virtual void keyPressed(sf::Keyboard::Key key) {}
	virtual void keyReleased(sf::Keyboard::Key key) {}
};

#endif
