#include <iostream>
#include "screen/MenuScreen.h"
#include "utils/Constants.h"

#include "events/AudioEvent.h"
#include "events/ScreenTransitionEvent.h"
#include "systems/EventManager.h"

#include <Windows.h>
#include <iostream>

using std::cout;
using std::endl;

MenuScreen::MenuScreen(const string &background, GameScreen myId): 
_selections("images/sample-menu.png"), 
_cursor("images/FF7Cursor.png"),
_background(background),
_id(myId),
_activeScreen(myId),
_prevScreen(GAMEPLAY),
currentPos(30),
heightMove(0),
keyTrigger(false),
active(false),
_triggerElapse(0),
_elapsed(0),
_time(1.0f),
currentScreen(SAVE)
{    
    EventManager::get()->registerForAll(EVENT_SCREEN_TRANSITION, EventListenerPtr(this));
}

MenuScreen::~MenuScreen()
{
}

// current + 1 % size

int MenuScreen::mod(int x, int m) {
    return (x%m + m)%m;
}

void MenuScreen::update(float elapsed) 
{ 	
	if ((_elapsed) - _triggerElapse > .14)
//		cout << "h" << endl;
		keyTrigger = false;

	_elapsed += elapsed;
}


int MenuScreen::initialize()
{
    _select[START].setString("Play Game");
	_select[PAUSE].setString("Back to Game");
	// _select[SAVE].setString("Save");
	// _select[LOAD].setString("Load");
	_select[MUSIC].setString("Music");
	_select[EXIT].setString("Exit");

    sf::Color textColor = sf::Color(204, 255, 204);
    
    for ( int i=0; i<MENUS_TOTAL; i++ ) {
        _select[i].setColor(textColor);
    }

    _select[START].setPosition(90,currentPos);
	_select[PAUSE].setPosition(90,-50);
	// _select[SAVE].setPosition(90,currentPos+selectHeight);
	// _select[LOAD].setPosition(90,currentPos+(selectHeight*2));
	_select[MUSIC].setPosition(90,currentPos+(selectHeight));
	_select[EXIT].setPosition(90,currentPos+(selectHeight*2));
    
	cout << _selections << endl;
	_cursorTexture.loadFromFile(_cursor);
	_selectTexture.loadFromFile(_selections);

	_selectSprite.setTexture(_selectTexture);
    _selectSprite.setPosition(93, 0);
   
    _cursorSprite.setTexture(_cursorTexture);

	//currentPos += 10;

    _cursorSprite.setPosition(15,currentPos+10);

    _view = sf::View(sf::FloatRect(0,0,WINDOW_WIDTH, WINDOW_HEIGHT));

    if (!_backgroundTexture.loadFromFile(_background))
    	return LOAD_ERROR;

    _backgroundSprite = sf::Sprite(_backgroundTexture);
    
    return SUCCESS;
}

int MenuScreen::render(sf::RenderWindow &app)
{
    app.setView(_view);

    //keyTrigger = false;
    //app.draw(_selectSprite);

    for ( int i=0; i<MENUS_TOTAL; i++ ) {
        app.draw(_select[i]);
    }
    app.draw(_cursorSprite);
    
    _backgroundSprite.setPosition(app.getSize().x - _backgroundTexture.getSize().x,
        (app.getSize().y / 2.0f) - (_backgroundTexture.getSize().y / 2.0f));
    app.draw(_backgroundSprite);

    return _activeScreen;
}

void MenuScreen::onEvent(const EventPtr& anEvent)
{
    EventType type = anEvent.get()->getEventType();
    if ( type == EVENT_SCREEN_TRANSITION ) {
        boost::shared_ptr<ScreenTransitionEvent> evt = boost::static_pointer_cast<ScreenTransitionEvent>(anEvent);       
        _activeScreen = evt->getNextScreen();
        _prevScreen = evt->getPrevScreen();
        if ( _activeScreen == GAMEPLAY ) {
            _select[PAUSE].setPosition(90, 30);
            _select[START].setPosition(90,-50);
        } else if ( _activeScreen == GAME_OVER || _activeScreen == YOU_WIN ) {
            _activeScreen = _id;
            _prevScreen = GAMEPLAY;
            _select[START].setPosition(90, 30);
            _select[PAUSE].setPosition(90,-50);
        }
    }
}

void MenuScreen::keyPressed(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Up && !keyTrigger)
	{
		_triggerElapse = _elapsed;
		cout << _elapsed << endl;
		keyTrigger = true;
		cout << " Up" << endl;
		currentScreen -= 1;
		currentScreen = mod(currentScreen, MENUS_TOTAL);
		selectedScreen = screenList[currentScreen];

		heightMove = selectHeight;
		currentPos -= heightMove;
		if (currentPos <= selectHeight && currentPos > 0)
		{
			cout << " <selectHeight" << endl;
			//heightMove = 85;
			currentPos = 30;
		}
		else if (currentPos < 0)
		{
			currentPos = 30 + selectHeight*2;    // currentPos = EXIT
			cout << "> height" << currentPos << " pos and hght" << heightMove << endl;
		}

		cout << currentPos << " Up " << endl;   
		//sf::Sprite *spt = new sf::Sprite(*_cursorTexture);
		_cursorSprite.setPosition(15,currentPos+10);

        // Create and send audio event for cursor movement
        AudioEvent* sound = new AudioEvent(EVENT_AUDIO_EFFECT);
        sound->setEffect(CURSOR_SOUND);
        EventManager::get()->dispatchImmediate(EventPtr(sound));
	}

	else if (key == sf::Keyboard::Down && !keyTrigger)
	{
		_triggerElapse = _elapsed;
		keyTrigger = true;
		currentScreen += 1;
		currentScreen = mod(currentScreen, MENUS_TOTAL);
		selectedScreen = screenList[currentScreen];

	//	cout << currentScreen << " cur scren " << endl;
		heightMove = selectHeight;
		currentPos += heightMove;
		if (currentPos <= selectHeight)
		{
			cout << " <selectHeight" << endl;
			//heightMove = 85;
			currentPos = 30;
		}
		else if (currentPos > 30 + selectHeight*2)
		{
			cout << "> height" << currentPos << " pos and hght" << heightMove << endl;
		//	heightMove = 0;
			currentPos = 30;
		}
		cout << currentPos << " Down" << endl;   

		_cursorSprite.setPosition(15,currentPos+10);

        // Create and send audio event for cursor movement
        AudioEvent* sound = new AudioEvent(EVENT_AUDIO_EFFECT);
        sound->setEffect(CURSOR_SOUND);
        EventManager::get()->dispatchImmediate(EventPtr(sound));
	}    
	else if (key == sf::Keyboard::Z) {
		cout << " Select" << endl;
        // Determine which screen to go to based on position of cursor when
        // Return key is pressed
        ScreenTransitionEvent* st = new ScreenTransitionEvent(EVENT_SCREEN_TRANSITION);
        if ( currentPos == 30 ) { // START or PAUSE
            st->setNextScreen(_prevScreen);
        } else if ( currentPos == (30 + selectHeight) ) { // MUSIC
            ToggleEvent* tEvt = new ToggleEvent(EVENT_AUDIO_STATUS);
            tEvt->setStatus(false);
            EventManager::get()->dispatchImmediate(EventPtr(tEvt));
        } else if ( currentPos == (30 + (selectHeight*2) ) ) { // EXIT
            st->setNextScreen(QUIT);
        } else {
            st->setNextScreen(_activeScreen);
        }
        if ( (st->getNextScreen() != _activeScreen) && (currentPos != (30 + selectHeight) ) ) {
            st->setPrevScreen(_activeScreen);
            EventManager::get()->dispatchImmediate(EventPtr(st));
        }
    }
}

void MenuScreen::keyReleased(sf::Keyboard::Key key)
{
}