//
//  EventManager.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_Blackboard_h
#define drScience_Blackboard_h

#include "utils/Constants.h"

#include "object/Rooms.h"
#include "object/GameObject.h"

#include <vector>
#include <map>

using namespace std;
using std::vector;
using std::map;
using std::pair;

typedef vector<GameObjectPtr> ObjectList;

class Blackboard
{
public:

	static Blackboard* get();

	ObjectList getObjects() { return _objects; }

	void addObject(GameObjectPtr object) { _objects.push_back(object); }

	GameObjectPtr getControlled() { return _controlled; }
	void setControlled(GameObjectPtr object) { _controlled = object; }
    GameObjectPtr getBattery() { return _battery; }
    void setBattery(GameObjectPtr object) { _battery = object; }

    GameObjectPtr getRoom(RoomsEnum room);
    void addRoom(GameObjectPtr object);
	// TODO: allow objects to unregister for events....
	//   most likely occurs when an object dies or is removed from the game.
	//   also occurs every time we load a new level.

    RoomsEnum getRoomsEnum(const string& room);

protected:
    
private:
	Blackboard() {};
	Blackboard(Blackboard const&) {};
	Blackboard& operator=(Blackboard const&) {};

	static Blackboard* _instance;

	ObjectList _objects;
	GameObjectPtr _controlled;
    GameObjectPtr _battery;
    map<RoomsEnum, GameObjectPtr> _rooms;
    
};

#endif
