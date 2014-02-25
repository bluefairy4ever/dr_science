#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "ResourcePath.hpp"

#include "utils/Constants.h"

#include "screen/Screen.h"
#include "screen/SplashScreen.h"
#include "screen/MenuScreen.h"
#include "screen/GameplayScreen.h"
#include "screen/battle/BattleScreen.h"

#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>

#include <vector>

using std::vector;

int main (int argc, const char * argv[])
{
	// Create the main window
    sf::RenderWindow app(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Dr. Science");
    app.setVerticalSyncEnabled(true);
    app.setFramerateLimit(FRAME_LIMIT);

    sf::Clock clock;

    // The very last paramater in this call is the screen to which the splash screen will switch control,
    // So to test the Gameplay or Battle system, you simply have to change the paramater to GAMEPLAY or BATTLE...
	ScreenPtr splash(new SplashScreen(resourcePath()+"images/average-joeys-logo.png", INTRO_SPLASH, MENU));
    splash->initialize();

    ScreenPtr gameOver(new SplashScreen(resourcePath()+GAME_OVER_PATH, GAME_OVER, MENU));
    gameOver->initialize();

    ScreenPtr youWin(new SplashScreen(resourcePath()+YOU_WIN_PATH, YOU_WIN, MENU));
    youWin->initialize();
        
    ScreenPtr gameplay(new GameplayScreen(GAMEPLAY));
    gameplay->initialize();

    ScreenPtr battle(new BattleScreen(BATTLE));
    battle->initialize();
    
	ScreenPtr menu(new MenuScreen(resourcePath()+"images/menuBackground.png", MENU));
	menu->initialize();

    vector<ScreenPtr> screens;
    screens.push_back(splash);
    screens.push_back(gameOver);
    screens.push_back(youWin);
    screens.push_back(gameplay);
    screens.push_back(battle);
    screens.push_back(menu);
    int screenIndex = 0;

    // Start the game loop
    while (app.isOpen() && screenIndex >= 0)
    {
        ScreenPtr active = screens.at(screenIndex);

    	// Process events
    	sf::Event event;
    	while (app.pollEvent(event))
    	{
    		// Close window : exit
    		if (event.type == sf::Event::Closed)
    			app.close();    		

            // Handle key presses
			if (event.type == sf::Event::KeyPressed) {
				active->keyPressed(event.key.code);
			}

			if (event.type == sf::Event::KeyReleased) { 
				active->keyReleased(event.key.code);
			}
    	}
        
        float elapsed = clock.restart().asSeconds();
        
		// TODO: This game loop is wrong.  We should be updating at
		// a fixed rate and rendering as much as possible.  We will
		// fix it this week and update the code...
		active.get()->update(elapsed);

		app.clear();
		screenIndex = active.get()->render(app);
		app.display();
    }

	screens.clear();

	return EXIT_SUCCESS;
}
