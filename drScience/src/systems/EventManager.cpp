
#include "systems/EventManager.h"

using std::make_pair;

EventManager* EventManager::_instance = NULL;

/**
 *  Get an instance of this EventManager
 *  as there can only be one.
 */
EventManager* EventManager::get()
{
	if (!_instance)
		_instance = new EventManager();
	return _instance;
}

/**
 *  listen for all messages that are of the given message type.
 *  @param type - the type of messages to listen for
 *  @param callback - the pointer to the callback function
 */
void EventManager::registerForAll(const EventType& type, const EventListenerPtr& callback)
{
	map< EventType,CallbackList >::iterator it;
	it = _all.find(type);
	if (it == _all.end()) { 
		CallbackList v;
		v.push_back(callback);
		_all.insert( make_pair(type, v) );
	} else { 
		CallbackList& v = it->second;
		v.push_back(callback);
	}
}

/**
 *  Listen for all messages for a specific type and a specific
 *  recipient.  
 *  @param type - the type of messages to listen for
 *  @param objectId - the id of the recipient we are interested in
 *  @param callback - the pointer to the callback function
 */
void EventManager::registerForEvents(const EventType& type, const int& objectId, const EventListenerPtr& callback)
{
	map< EventType, map< int,CallbackList > >::iterator it;
	map< int, CallbackList >::iterator map2It;

	it = _unique.find(type);
	if (it == _unique.end()) {
		map< int, CallbackList > amap;
		CallbackList v;
		v.push_back(callback);
		
		amap.insert( make_pair(objectId, v) );
		_unique.insert( make_pair(type, amap) );
	} else { 
		map< int, CallbackList >& amap = it->second;
		map2It = amap.find(objectId);
		if (map2It == amap.end()) { 
			CallbackList v;
			v.push_back(callback);
			amap.insert( make_pair(objectId, v) );
		} else { 
			CallbackList& v = map2It->second;
			v.push_back(callback);
		}
	}

}

/**
 *  Dispatch an event by adding it to the event queue.
 *  @param anEvent - the event to dispatch at the earliest convenient opportunity
 */
void EventManager::dispatch(EventPtr anEvent)
{
	_queue.push_back(anEvent);
}
	
/**
 *  Contains the logic to send an event right now.
 *  This bypasses the queue and is routed to all of the
 *  interested parties.
 *  @param anEvent - the event to send around
 */
void EventManager::dispatchImmediate(EventPtr anEvent)
{
//	printf("Dispatch Event: %d\n", anEvent.get()->getEventType());

	map< EventType,CallbackList >::iterator it;
	it = _all.find(anEvent.get()->getEventType());

	if (it != _all.end()) { 
		CallbackList list = it->second;
		for (CallbackList::iterator iter = list.begin(); iter != list.end(); ++iter) {
			(*iter).get()->onEvent(anEvent);
		}
	}

	// Now dispatch to anyone who is listing for this
	// specific type of message.
	map< EventType, map< int, CallbackList > >::iterator iit;
	iit = _unique.find(anEvent.get()->getEventType());
	if (iit != _unique.end()) { 
		vector<int> recipients = anEvent.get()->getRecipients();
		map< int, CallbackList >& registered = iit->second;
		for (vector<int>::iterator riter = recipients.begin(); riter != recipients.end(); ++riter) { 
			map<int, CallbackList >::iterator regIter;
			regIter = registered.find(*riter);
			if (regIter != registered.end()) {
				CallbackList& list = regIter->second;
				for (CallbackList::iterator aiter = list.begin(); aiter != list.end(); ++aiter) {
					(*aiter).get()->onEvent(anEvent);
				}
			}
		}
	}	
}
	

void EventManager::update(float elapsed)
{
	vector<EventPtr> copy(_queue);
	_queue.clear();

	for (vector<EventPtr>::iterator iter = copy.begin(); iter != copy.end(); ++iter) { 
		dispatchImmediate(*iter);
	}
}
	
