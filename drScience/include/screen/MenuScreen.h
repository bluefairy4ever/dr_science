//
//  Menu.h

#ifndef Menu_h
#define Menu_h

#include <string>

#include <vector>

#include "screen/Screen.h"
#include "events/EventListener.h"
#include "events/ToggleEvent.h"

#include <SFML/Graphics.hpp>
#include "boost/shared_ptr.hpp"

#include <string>
#include <map>

using std::map;
using std::string;
using sf::Keyboard;
using std::pair;

enum Move {UP = 0, DOWN };

enum MenuScreens {
    START=0,
    PAUSE,
    SAVE,
    LOAD,
    MUSIC,
    EXIT,
    MENUS_TOTAL
};

class MenuScreen : public Screen, public EventListener
{
public:
	MenuScreen(const string &background, GameScreen myId);
	~MenuScreen();
    virtual int render(sf::RenderWindow &app);
    virtual int initialize();
	virtual void update(float elapsed);
    virtual void onEvent(const EventPtr& anEvent);

	virtual void keyPressed(sf::Keyboard::Key key);
	virtual void keyReleased(sf::Keyboard::Key key);

	int mod (int x, int m);


	// for now, set to a bool to return to stats->active, so stats knows whether it's been chosen or not
	//bool getCommand(sf::RenderWindow &mainWindow);

private:
    
	MenuScreens screenList[MENUS_TOTAL];

	GameScreen _id;
    GameScreen _activeScreen;
    GameScreen _prevScreen;
    
	MenuScreens selectedScreen;

	int currentPos;
	int heightMove;
	static const int selectHeight = 60; //60, height of each option
	int currentScreen; // use to navigate through screenList


	bool keyTrigger;

	bool active;
	bool reset;

	float _triggerElapse;
    float _elapsed;
    float _time;
    
    string _selections;
	string _cursor;
    string _background;
    
    sf::Texture _selectTexture;
    sf::Texture _cursorTexture;
    sf::Texture _backgroundTexture;
	sf::Sprite _cursorSprite;
    sf::Sprite _selectSprite;
    sf::Sprite _backgroundSprite;
    sf::Text _select[MENUS_TOTAL];

    sf::View _view;
};

#endif
