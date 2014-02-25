//
//  BattleSystem.h
//

#ifndef drScience_BattleSystem_h
#define drScience_BattleSystem_h

#include "utils/Constants.h"
#include "systems/battle/BattleObject.h"
#include "systems/battle/EffectObject.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <queue>

using sf::Vector2f;

class Subsystem;
typedef boost::shared_ptr<BattleObject> BattleObj;
typedef boost::shared_ptr<EffectObject> EffectObj;

enum actionTypes {ATTACK, DEFEND, PLAYERMODE, SKILL};

struct BattleAction
{
	actionTypes type;
	int attackerID;
	int targetID;
	int finished;

	BattleAction()
	{
		finished = 0;
	}
};


class BattleSystem
{
public:
	static BattleSystem* get();

	void executeAttack(int attackerID, int targetID);
	void executeDefend(int targetID);
	void executeSkill(int attackerID, int targetID);
	void setWait(int wait) {_wait = wait;};

	sf::String ftoa(float num); // Convert float to string

	int cursorCheckValidEnemy(int direction); //-1 = any, 0 = up, 1 = down
	void centerView(float x, float y, int type); // type 0 = move towards xy, 1 = reset
	void zoomEffect(float x, float y);
	void setupCombo(int pattern);
	void setupBattle();

	bool battleComplete() {return _battleComplete;};

    //  These functions should not be removed
	void initialize();
	void update(float elapsed);
	void render(sf::RenderWindow &window);
	void finish();

	// Queue methods
	void addBattleAction(BattleAction action);
    
private:
	BattleSystem() {};
	BattleSystem(BattleSystem const&) {};
	BattleSystem& operator=(BattleSystem const&) {};

	static BattleSystem*	_instance;

	vector<int>				_enemyPos;				// Enemy positions, used for cursor selection
	int						_enemyMaxPos;			// Maximum enemy positions
	sf::Text				_allyStatStr;
	sf::Text				_menuStr;
	sf::RectangleShape		_allyATBBar;
	sf::RectangleShape		_allyATBBarBkg;
	sf::RectangleShape		_allyMPBar;
	sf::RectangleShape		_allyMPBarBkg;
	sf::RectangleShape		_skillTimerBarBkg;
	sf::RectangleShape		_flash;

	sf::View				_view;
	int						_wait;					//Pauses ATB timer
	queue<BattleAction>		_battleActionQueue;
	int						_timer;
	int						_attackState;			//-1 = idle, 0 = init, 1 = moving, 2 = attacking, 3 = returning, 4 = done
	Vector2f				_prevPos;

	int						_menuVisible;
	int						_enemySelection;		// EnemyPos selection
	int						_menuSelection;
	int						_menuSelectionMax;
	int						_cursorState;			// 0 = on menu, 1 = selecting enemy

	sf::Texture				_backgroundTex;
	sf::Sprite				_backgroundSpr;
	sf::Texture				_cursorTex;
	sf::Sprite				_cursorSpr;
	sf::Font				_font;
	int						_fontSize;

	bool					_keypressed;

	int						_skillTimer;
	int						_skillTimerMax;
	int						_skillTimerVisible;
	int						_flashVisible;
	
	int						_zoomState;				// Zoom the view; -1 = idle, 0 = zooming in, 1 = zooming out
	float					_zoomSpeed;
	float					_zoom;
	float					_zoomMax;				// Max zoom

	int						_zoomX;
	int						_zoomY;

	vector<char>			_combo;
	int						_comboSuccessCount;
	int						_comboSuccessCountMax;
	char					_currentChar;


	int						_currentCharVisible;

	bool					_battleComplete;
};

#endif
