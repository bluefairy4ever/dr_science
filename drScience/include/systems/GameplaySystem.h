//
//  GameplaySystem.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_GameplaySystem_h
#define drScience_GameplaySystem_h

#include "utils/Constants.h"
#include "utils/TmxLoader.h"

#include "events/EventListener.h"

#include "object/Rooms.h"

#include "systems/PhysicsSubsystem.h"
#include "systems/SpawnSubsystem.h"
#include "systems/AudioSubsystem.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <map>

using std::map;
using std::string;

class Subsystem;

class GameplaySystem : public EventListener
{
public:
    GameplaySystem();
	~GameplaySystem();

	void loadLevel(const string& level, const string& baseDir);

	void initialize();
	void update(float elapsed);
	void render(sf::RenderWindow &window);
	void finish();

    virtual void onEvent(const EventPtr& anEvent);

protected:
    
private:    
	sf::View _view;    
    bool _mobileCamera[2]; // current state of camera (mobile or not; 0 corresponds to x, 1 to y)
    RoomsEnum _currentRoom;
    sf::Rect<float> _mobileBounds; // current camera boundaries

    sf::Music _music;
    
	tmx::TileMap _map;

	PhysicsSubsystem _physics;
	SpawnSubsystem  _spawn;
    AudioSubsystem _audio;
    
};

#endif
