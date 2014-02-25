
#include "ResourcePath.hpp"

#include "events/Event.h"
#include "events/EventTypes.h"
#include "events/AudioEvent.h"
#include "events/ToggleEvent.h"
#include "events/CreateTiledObject.h"
#include "events/RoomTransitionEvent.h"
#include "events/ScreenTransitionEvent.h"
#include "events/DeathEvent.h"
#include "events/ContactEvent.h"

#include "object/GameObject.h"
#include "object/PhysicsObject.h"
#include "object/components/AnimationComponent.h"

#include "systems/GameplaySystem.h"
#include "systems/PhysicsSubsystem.h"
#include "systems/SpawnSubsystem.h"
#include "systems/Subsystem.h"

#include "systems/EventManager.h"
#include "systems/Blackboard.h"

#include "utils/TmxLoader.h"

using std::make_pair;
using tmx::Object;

GameplaySystem::GameplaySystem() :
   _spawn(), _physics(), _audio(), _view()
{
}

GameplaySystem::~GameplaySystem()
{

}

/** 
 *  Do any initialization here that could break in the constructor.
 */
void GameplaySystem::initialize()
{

	_physics.initialize();
	_spawn.initialize();
    _audio.initialize();

    _audio.setTheme(MENU_THEME);

    _view.setSize(VIEW_WIDTH, VIEW_HEIGHT);
    _mobileCamera[0] = _mobileCamera[1] = true;

    EventManager::get()->registerForAll(EVENT_ROOM_TRANSITION, EventListenerPtr(this));
    EventManager::get()->registerForAll(EVENT_SCREEN_TRANSITION, EventListenerPtr(this));  
    EventManager::get()->registerForAll(EVENT_DEATH, EventListenerPtr(this));    
}

/** 
 *  Update all of the objects in the game.
 *  @param elapsed - the amount of seconds elapsed since the last
 *     time update was called.
 */
void GameplaySystem::update(float elapsed)
{
	_spawn.update(elapsed);
	_physics.update(elapsed);

	EventManager::get()->update(elapsed);

	ObjectList list = Blackboard::get()->getObjects();
	ObjectList::iterator it;
	for (it = list.begin(); it != list.end(); ++it) { 
		(*it)->update(elapsed);
	}

    // Code for rooms as correct size with camera following player:
    Vector2f playerPos = Blackboard::get()->getControlled()->getPosition();
    GameObjectPtr room = Blackboard::get()->getRoom(_currentRoom);
    Vector2f viewPos = _view.getCenter();

    if ( _mobileCamera[0] ) {
        // If player's x is within camera's mobile bounds
        if (playerPos.x > _mobileBounds.left && playerPos.x < _mobileBounds.width) {
            viewPos.x = playerPos.x;
        } else {
            // If player is in top half of room
            if ( playerPos.x < room->getPosition().x + (room->getDimensions().x / 2.0f) ) {
                viewPos.x = room->getPosition().x + _view.getSize().x / 2.0f;
            } else {
                viewPos.x = room->getPosition().x + room->getDimensions().x - _view.getSize().x / 2.0f;
            }
        }
    }
    if ( _mobileCamera[1] ) {
        // If player's y is within camera's mobile bounds
        if (playerPos.y > _mobileBounds.top && playerPos.y < _mobileBounds.height) {
            viewPos.y = playerPos.y;
        } else {
            // If player is in left half of room
            if ( playerPos.y < room->getPosition().y + (room->getDimensions().y / 2.0f) ) {
                viewPos.y = room->getPosition().y + _view.getSize().y / 2.0f;
            } else {
                viewPos.y = room->getPosition().y + room->getDimensions().y - _view.getSize().y / 2.0f;
            }
        }
    }
    _view.setCenter(int(viewPos.x), int(viewPos.y));
}

/**
 *  Render everything in the game world.
 */
void GameplaySystem::render(sf::RenderWindow &window)
{    
    window.setView(_view);
	_map.Draw(window);

	ObjectList list = Blackboard::get()->getObjects();
	ObjectList::iterator it;
	for (it = list.begin(); it != list.end(); ++it) { 
		(*it)->render(window);
	}

	_physics.render(window);
	_spawn.render(window);
    _audio.render(window);
}

/**
 *  Finish everything, so clear it out in preparation for
 *  a new level or quitting the game.
 */
void GameplaySystem::finish()
{
    
}

/**
 *  onEvent is called when we receive events
 *  @param anEvent - the event we received
 */
void GameplaySystem::onEvent(const EventPtr& anEvent)
{
    EventType type = anEvent.get()->getEventType();
    if ( type == EVENT_SCREEN_TRANSITION ) {
        boost::shared_ptr<ScreenTransitionEvent> evt = boost::static_pointer_cast<ScreenTransitionEvent>(anEvent);
        AudioEvent* audio = new AudioEvent(EVENT_AUDIO_THEME);

        // Set theme based on next screen
        switch (evt->getNextScreen()) {
        case MENU:
            audio->setTheme(MENU_THEME);
            break;
        case GAMEPLAY:
            audio->setTheme(GAME_THEME);
            break;
        case BATTLE:
            audio->setTheme(BATTLE_THEME);
            break;
        default:
            audio->setTheme(NO_THEME);
            break;
        }
        
        // Don't dispatch an audio event if the game is exiting
        if ( evt->getNextScreen() != QUIT ) {
            EventManager::get()->dispatchImmediate(EventPtr(audio));
        }

    } else if ( type == EVENT_ROOM_TRANSITION ) {
        boost::shared_ptr<RoomTransitionEvent> evt = boost::static_pointer_cast<RoomTransitionEvent>(anEvent);
        
        // Set the window to the new room
        GameObjectPtr nextRoom = Blackboard::get()->getRoom(evt.get()->getNextRoom());
                
        // Code for rooms as correct size with camera following player:
        cout << "Next room position: "
            << nextRoom->getPosition().x << " " << nextRoom->getPosition().y << endl;
        _map.SetDrawingBounds(sf::Rect<float>(nextRoom->getPosition().x + 1,
            nextRoom->getPosition().y + 1, nextRoom->getDimensions().x - 1, nextRoom->getDimensions().y - 1));            

        // Check for rooms that are smaller than the view size
        if (nextRoom->getDimensions().x <= _view.getSize().x) {
            _mobileCamera[0] = false;
            _view.setCenter(int(nextRoom->getPosition().x + (nextRoom->getDimensions().x / 2.0f)),
                int(_view.getCenter().y));
        } else {
            _mobileCamera[0] = true;
        }
        if (nextRoom->getDimensions().y <= _view.getSize().y) {
            _mobileCamera[1] = false;
            _view.setCenter(int(_view.getCenter().x),
                int(nextRoom->getPosition().y + (nextRoom->getDimensions().y / 2.0f)));
        } else {
            _mobileCamera[1] = true;
        }

        // Create the mobile boundaries of the camera
        _mobileBounds = sf::Rect<float>(nextRoom->getPosition().x + _view.getSize().x / 2.0f,
        nextRoom->getPosition().y + _view.getSize().y / 2.0f,
        nextRoom->getPosition().x + nextRoom->getDimensions().x - _view.getSize().x / 2.0f,
        nextRoom->getPosition().y + nextRoom->getDimensions().y - _view.getSize().y / 2.0f);

        _currentRoom = evt.get()->getNextRoom();
    } else if ( type == EVENT_CONTACT_BEGIN ) {
        boost::shared_ptr<ContactEvent> evt = boost::static_pointer_cast<ContactEvent>(anEvent);
        if ( evt.get()->getObjectA()->getId() == Blackboard::get()->getBattery()->getId()
            || evt.get()->getObjectB()->getId() == Blackboard::get()->getBattery()->getId() )
        {
            // Switch to "game over" screen
            ScreenTransitionEvent* st = new ScreenTransitionEvent(EVENT_SCREEN_TRANSITION);
            st->setNextScreen(YOU_WIN);
            st->setPrevScreen(MENU);
            EventManager::get()->dispatchImmediate(EventPtr(st));

        }
    } else if( type == EVENT_DEATH ) {
        boost::shared_ptr<DeathEvent> evt = boost::static_pointer_cast<DeathEvent>(anEvent);

        if( evt->getObjectID() == Blackboard::get()->getControlled()->getId() ) {
            cout << "Game over\n";            
        }
    }
}

/**
 *  Load the specified level into the game and 
 *  initialize what needs to be initialized
 *  @param level - the file containing the level information.
 */
void GameplaySystem::loadLevel(const string& level, const string& baseDir)
{
    // Dispatch LoadStart event
	EventManager::get()->dispatchImmediate( EventPtr( new Event(EVENT_LEVEL_LOAD_START) ) );
		
	// for each of the objects in the level file ...
	//    dispach events to have them created.
	// 	  CreateGameObject event = new CreateGameObject(obj);
	//    EventManager::get().dispatchImmediate(event);

	_map.LoadFromFile(level, baseDir);

	vector<Object> objects = _map.GetObjects(); 
	vector<Object>::iterator iter;
	for (iter = objects.begin(); iter != objects.end(); ++iter) {
		EventPtr evt = EventPtr(new CreateTiledObject(EVENT_CREATE_TILED_OBJECT, _physics.getWorld(), *iter));
		EventManager::get()->dispatchImmediate( evt );
	}

    // Dispatch LoadEnd event
	EventManager::get()->dispatchImmediate( EventPtr( new Event(EVENT_LEVEL_LOAD_END) ) );
    
    EventManager::get()->registerForEvents(EVENT_CONTACT_BEGIN, Blackboard::get()->getBattery()->getId(), EventListenerPtr(this));
}

