//
//  BattleScreen.h
//  drScience
//

#ifndef drScience_BattleScreen_h
#define drScience_BattleScreen_h

#include "screen/Screen.h"
#include "systems/battle/BattleSystem.h"

#include "events/EventListener.h"
#include "events/ToggleEvent.h"

#include <SFML/Graphics.hpp>
#include "boost/shared_ptr.hpp"

using sf::Keyboard;

class BattleScreen : public Screen, public EventListener
{
public:
    BattleScreen(GameScreen myId);
    ~BattleScreen();
    
    // These functions should not be removed
    virtual int initialize();
	virtual void update(float elapsed);
    virtual int render(sf::RenderWindow &window);
    virtual void onEvent(const EventPtr& anEvent);

	virtual void keyPressed(sf::Keyboard::Key key);
	virtual void keyReleased(sf::Keyboard::Key key);
    
protected:
    
    void drawGrid(sf::RenderWindow& window);
private:
    
    // These variables should not be removed
    GameScreen _id;
    GameScreen _activeScreen;
	BattleSystem* _system;
};

#endif
