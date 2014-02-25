

#include "events/Event.h"
#include "object/GameObject.h"


/**
 *  Constructor for default types of events
 *  @param type - the type of event this is.
 */
Event::Event(EventType type) : _type(type)
{

}

/**
 *  Destructor for evey event
 */
Event::~Event()
{

}

/**
 *  Return the type of this event.
 */
EventType Event::getEventType()
{
	return _type;
}

/**
 *  Add a recipient to the receiving side of
 *  this message.
 *  @param obj - the game object pointer that we want to receive the 
 *     message.
 */
void Event::addRecipient(GameObjectPtr obj) 
{
	_recipients.push_back(obj.get()->getId());
}

/**
 *  Add a recipient to the receiving side of
 *  this message.
 *  @param objId - the game object identifier that we want to receive the 
 *     message.
 */
void Event::addRecipient(int objId)
{
	_recipients.push_back(objId);
}

/**
 *  Get the recipients of this event.  Will be a list
 *  of GameObject ids
 */
vector<int> Event::getRecipients()
{
	return _recipients;
}

/**
 *  Clear out the list of recipients in order to reuse
 *  this event across multiple time steps.
 */
void Event::clearRecipients()
{
	_recipients.clear();
}