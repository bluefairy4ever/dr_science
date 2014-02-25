
#include "events/CreateTiledObject.h"

/**
 *  CreateTiledObject is an event that gets sent to who ever is responsible
 *  for creating the actual objects.
 *  @param type - the type of event.
 *  @param obj - the object that we need to convert from Tiled to a Physics object
 */
CreateTiledObject::CreateTiledObject(const EventType& type, b2World* world, const Object& obj) : 
	Event(type),
	_object(obj),
	_world(world)
{

}

CreateTiledObject::~CreateTiledObject()
{

}