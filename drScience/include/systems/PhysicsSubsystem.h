//
//  PhysicsSubsystem.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_PhysicsSubsystem_h
#define drScience_PhysicsSubsystem_h

#include "Box2D/Box2D.h"

#include "systems/Subsystem.h"
#include "utils/Constants.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <map>

using std::map;
using std::string;

class PhysicsSubsystem : public Subsystem, public b2ContactListener
{
public:
    PhysicsSubsystem();
	~PhysicsSubsystem();

	int initialize();
	void update(float elapsed);
	void render(sf::RenderWindow &window);
	void finish();

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	b2World* getWorld() { return _world; }

protected:
    
private:
	b2World* _world;
};

#endif
