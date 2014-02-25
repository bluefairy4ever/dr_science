
#include "events/PlayAnimationEvent.h"

PlayAnimationEvent::PlayAnimationEvent(const EventType& type, const string& name) : 
	Event(type),
	_name(name)
{

}

const string& PlayAnimationEvent::getName()
{
	return _name;
}