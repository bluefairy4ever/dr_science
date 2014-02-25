
#include "events/ContactEvent.h"

ContactEvent::ContactEvent(const EventType& type, PhysicsObject* objA, PhysicsObject* objB) : 
	Event(type),
	_objA(objA),
	_objB(objB)
{

}

PhysicsObject* ContactEvent::getObjectA() 
{
	return _objA;
}

PhysicsObject* ContactEvent::getObjectB()
{
	return _objB;
}

