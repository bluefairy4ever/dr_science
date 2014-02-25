//
//  BattleScreen.cpp
//

#include "utils/Constants.h"
#include "screen/battle/BattleScreen.h"
#include "systems/EventManager.h"
#include "systems/battle/BattleBlackboard.h"
#include "events/ScreenTransitionEvent.h"
#include <SFML/Window.hpp>

/**
 *  Constructor for the BattleScreen
 *  @param myId - the id of this screen
 */
BattleScreen::BattleScreen(GameScreen myId) : 
    _id(myId), _activeScreen(myId)
{
    // Register for Screen Transition events    
    EventManager::get()->registerForAll(EVENT_SCREEN_TRANSITION, EventListenerPtr(this));
}

/**
 *  Destructor
 */
BattleScreen::~BattleScreen()
{
    
}

/**
 *  initialize all of the things that we will be rendering
 *  to the screen.
 */
int BattleScreen::initialize()
{
	_system = BattleSystem::get();
	//_system->initialize();

	// Only place I could initialize this
	BattleBlackboard::get()->saveHP(0,0);

    return SUCCESS;
}

/**
 *  This method updates the state of the game.
 *  @param elapsed - the amount of time elapsed since the last
       time we were called.
 */
void BattleScreen::update(float elapsed) 
{
	// Check if the battle's done
	if (_system->battleComplete())
	{
        ScreenTransitionEvent* st = new ScreenTransitionEvent(EVENT_SCREEN_TRANSITION);
        st->setNextScreen(GAMEPLAY);
        st->setPrevScreen(_activeScreen);
        EventPtr tEvent(st);
        EventManager::get()->dispatchImmediate(tEvent);	
	}

	_system->update(elapsed);
}

/**
 *  render handles updating the state of this screen as well
 *  as rendering it to the screen, so it's misnamed.
 *  @param App - the application that gave rise to this screen
 *  @param delta - the amount of time that has elapsed since we last rendered
 */
int BattleScreen::render(sf::RenderWindow &window)
{    
	_system->render(window);
    //drawGrid(app);

    return _activeScreen;
}

void BattleScreen::onEvent(const EventPtr& anEvent)
{
    EventType type = anEvent.get()->getEventType();
    if ( type == EVENT_SCREEN_TRANSITION ) {
        boost::shared_ptr<ScreenTransitionEvent> evt = boost::static_pointer_cast<ScreenTransitionEvent>(anEvent);       
        _activeScreen = evt->getNextScreen();
    }
}

void BattleScreen::keyPressed(sf::Keyboard::Key key)
{
    /*if ( key == Keyboard::S ) {
        ScreenTransitionEvent* st = new ScreenTransitionEvent(EVENT_SCREEN_TRANSITION);
        st->setNextScreen(GAMEPLAY);
        st->setPrevScreen(_activeScreen);
        EventPtr tEvent(st);
        EventManager::get()->dispatchImmediate(tEvent);
    } else if ( key == Keyboard::D ) {
        ScreenTransitionEvent* st = new ScreenTransitionEvent(EVENT_SCREEN_TRANSITION);
        st->setNextScreen(GAME_OVER);
        st->setPrevScreen(MENU);
        EventPtr tEvent(st);
        EventManager::get()->dispatchImmediate(tEvent);
    } else*/ if ( key == Keyboard::Escape ) {
        ScreenTransitionEvent* st = new ScreenTransitionEvent(EVENT_SCREEN_TRANSITION);
        st->setNextScreen(MENU);
        st->setPrevScreen(_activeScreen);
        EventPtr tEvent(st);
        EventManager::get()->dispatchImmediate(tEvent);
    }
}

void BattleScreen::keyReleased(sf::Keyboard::Key key)
{
}

//  Test function that draws a grid on the screen
void BattleScreen::drawGrid(sf::RenderWindow &window)
{
    sf::Vector2u size = window.getSize();
    sf::RectangleShape rect = sf::RectangleShape();
    rect.setOutlineThickness(0.25f);
    for (int i = 0; i < size.x; i+= 100) {
        rect.setPosition(i,0);
        rect.setSize(sf::Vector2f(1,size.y));
        window.draw(rect);
        
        rect.setPosition(0,i);
        rect.setSize(sf::Vector2f(size.x, 1));
        window.draw(rect);
    }
}