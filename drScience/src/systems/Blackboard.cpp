
#include "systems/Blackboard.h"

using std::make_pair;

Blackboard* Blackboard::_instance = NULL;

/**
 *  Get an instance of this EventManager
 *  as there can only be one.
 */
Blackboard* Blackboard::get()
{
	if (!_instance)
		_instance = new Blackboard();
	return _instance;
}

GameObjectPtr Blackboard::getRoom(RoomsEnum room)
{
    return _rooms[room];
}

void Blackboard::addRoom(GameObjectPtr object)
{
    RoomsEnum room = getRoomsEnum(object.get()->getName());
    _rooms[room] = object;
}

RoomsEnum Blackboard::getRoomsEnum(const string& room)
{
    //  Return correct value of room room
    if (room == "lab") {
        return LAB;
    } else if (room == "animal") {
        return ANIMAL;
    } else if (room == "board") {
        return BOARD;
    } else if (room == "executive") {
        return EXECUTIVE;
    } else if (room == "lobby") {
        return LOBBY;
    } else if (room == "locker") {
        return LOCKER;
    } else if (room == "observation") {
        return OBSERVATION;
    } else if (room == "record") {
        return RECORD;
    } else if (room == "security") {
        return SECURITY;
    } else if (room == "trap") {
        return TRAP;
    } else {
        cout << "ERROR: " << room << " is not defined.\n";
        return LAB;
    }
};