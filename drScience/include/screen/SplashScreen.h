//
//  SplashScreen.h
//  ClassDemo
//
//  Created by Wesley Kerr on 9/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ClassDemo_SplashScreen_h
#define ClassDemo_SplashScreen_h

#include <string>

using std::string;

#include "Screen.h"
#include <SFML/Graphics.hpp>

class SplashScreen : public Screen
{
public:
    SplashScreen(const string& imgFile, GameScreen myId, GameScreen nextScreen);
    ~SplashScreen();

    virtual int initialize();
	virtual void update(float elapsed);
    virtual int render(sf::RenderWindow &app);

protected:
    
private:
    GameScreen _id;
    GameScreen _nextScreen;
    
    float _elapsed;
    float _time;
    
    string _imgFile;
    
    sf::Texture _texture;
    sf::Sprite _sprite;
};

#endif
