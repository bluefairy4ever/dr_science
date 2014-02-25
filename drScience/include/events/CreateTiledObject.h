//
//  Event.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_CreateTiledObject_h
#define drScience_CreateTiledObject_h

#include "utils/Constants.h"

#include "events/Event.h"
#include "events/EventTypes.h"

#include "utils/TmxLoader.h"

#include "Box2D/Box2D.h"

using tmx::Object;

class CreateTiledObject : public Event
{
public:
	CreateTiledObject(const EventType& type, b2World* world, const Object& object);
	~CreateTiledObject();

	const Object& getObject() { return _object; }
	b2World* getWorld() { return _world; }

protected:
    
private:
	b2World* _world;
	const Object& _object;
};

#endif
