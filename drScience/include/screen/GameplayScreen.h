//
//  GameplayScreen.h
//  drScience
//
//  Created by Wesley Kerr on 9/16/12.
//  Modified by Matthew Parish on 10/15/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ClassGameplay_GameplayScreen_h
#define ClassGameplay_GameplayScreen_h

#include "graphics/Animation.h"

#include "screen/Screen.h"
#include "systems/GameplaySystem.h"

#include "events/EventListener.h"
#include "events/ToggleEvent.h"

#include <SFML/Graphics.hpp>
#include "boost/shared_ptr.hpp"

#include <string>
#include <map>

using std::string;
using std::map;

using sf::Keyboard;

class GameplayScreen : public Screen, public EventListener
{
public:
     GameplayScreen(GameScreen myId);
    ~GameplayScreen();
    
    virtual int initialize();
	virtual void update(float elapsed);
    virtual int render(sf::RenderWindow &app);
    virtual void onEvent(const EventPtr& anEvent);

	virtual void keyPressed(sf::Keyboard::Key key);
	virtual void keyReleased(sf::Keyboard::Key key);
    
protected:    
    void drawGrid(sf::RenderWindow& app);

private:
    // Random battle variables
    int _battleChance;
    sf::Clock _clock;

    GameScreen _id;
    GameScreen _activeScreen;
	GameplaySystem* _system;
    
	map< Keyboard::Key,boost::shared_ptr<ToggleEvent> > _eventMap;
};

#endif
