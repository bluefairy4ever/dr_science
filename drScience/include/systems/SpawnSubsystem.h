//
//  PhysicsSubsystem.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_SpawnSubsystem_h
#define drScience_SpawnSubsystem_h

#include "utils/Constants.h"

#include "systems/Subsystem.h"

#include "events/EventListener.h"
#include "events/CreateTiledObject.h"

#include "utils/TmxLoader.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <map>

using std::map;
using std::string;

using tmx::Object;

class SpawnSubsystem : public Subsystem, public EventListener
{
public:
    SpawnSubsystem();
	~SpawnSubsystem();

	virtual int initialize();

	virtual void update(float elapsed);
	virtual void render(sf::RenderWindow &window);

	void finish();

	virtual void onEvent(const EventPtr& anEvent);

protected:
    
private:
	void createTiledObject(const Object& tmxObj, b2World* world) const;

	int _index;

};

#endif
