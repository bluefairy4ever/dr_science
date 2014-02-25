//
//  BattleSystem.cpp
//

#include "systems/battle/BattleSystem.h"
#include "systems/Subsystem.h"
#include "systems/battle/BattleObject.h"
#include "systems/battle/EffectObject.h"
#include "systems/battle/BattleBlackboard.h"
#include "systems/EventManager.h"
#include "events/ScreenTransitionEvent.h"
#include "events/ValueEvent.h"
#include "events/AudioEvent.h"
#include <time.h>
#include <sstream>
using namespace sf;

typedef vector<BattleObj> battleObjList;

BattleSystem* BattleSystem::_instance = NULL;

/**
 *  Get an instance of this EventManager
 *  as there can only be one.
 */
BattleSystem* BattleSystem::get()
{
	if (!_instance)
		_instance = new BattleSystem();
	return _instance;
}


/** 
 *  Do any initialization here that could break in the constructor.
 */
void BattleSystem::initialize()
{	
	//BattleBlackboard::get()->getBattleObjList().clear();
	BattleBlackboard::get()->setObjIDCount(10000);
	BattleBlackboard::get()->setEffIDCount(10000);

	Font font;
	font.loadFromFile("images/tahomabold.ttf");
	_font = font;
	_fontSize = 12;

	Text debugStr("Testing", _font, _fontSize);
	debugStr.setPosition(220, 204);
	_allyStatStr = debugStr;

	Text menuStr("Attack\nDefend\nSkill", _font, _fontSize);
	menuStr.setPosition(80, 204);
	_menuStr = menuStr;

	_menuVisible		= 0;
	_menuSelection		= 0;
	_enemySelection		= 0;
	_cursorState		= 0;
	_enemyMaxPos		= 2;
	_menuSelectionMax	= 2;
	_keypressed			= false;
	_zoomState			= -1;
	_zoomSpeed			= 1;
	_zoomMax			= 32;
	_zoomX				= 240;
	_zoomY				= 190;
	_zoom				= 0;
	_skillTimer			= 0;
	_skillTimerMax		= 200;
	_currentCharVisible = 0;
	_comboSuccessCount	= 0;
	_comboSuccessCountMax = 0;
	_flashVisible		= 0;
	_battleComplete		= false;
	
	srand ( time(NULL) );
	setupBattle();

	// ATB bar
	RectangleShape allyATBBar;
	allyATBBar.setSize(Vector2f(50,6));
	allyATBBar.setOutlineColor(Color(255,255,255,255));
	allyATBBar.setFillColor(Color(255,255,255,255));
	allyATBBar.setOutlineThickness(1.0);
	allyATBBar.setPosition(350, 210);
	_allyATBBar = allyATBBar;

	// ATB bar outline
	RectangleShape allyATBBarBkg;
	allyATBBarBkg.setSize(Vector2f(50,6));
	allyATBBarBkg.setOutlineColor(Color(255,255,255,255));
	allyATBBarBkg.setFillColor(Color::Transparent);
	allyATBBarBkg.setOutlineThickness(1.0);
	allyATBBarBkg.setPosition(350, 210);
	_allyATBBarBkg = allyATBBarBkg;

	// Skill bar
	RectangleShape allyMPBar;
	allyMPBar.setSize(Vector2f(100,6));
	allyMPBar.setOutlineColor(Color(255,255,255,255));
	allyMPBar.setFillColor(Color(255,255,255,255));
	allyMPBar.setOutlineThickness(1.0);
	allyMPBar.setPosition(300, 223);
	_allyMPBar = allyMPBar;

	// Skill bar outline
	RectangleShape allyMPBarBkg;
	allyMPBarBkg.setSize(Vector2f(100,6));
	allyMPBarBkg.setOutlineColor(Color(255,255,255,200));
	allyMPBarBkg.setFillColor(Color::Transparent);
	allyMPBarBkg.setOutlineThickness(1.0);
	allyMPBarBkg.setPosition(300, 223);
	_allyMPBarBkg = allyMPBarBkg;

	// Skill timer
	RectangleShape skillTimerBarBkg;
	skillTimerBarBkg.setSize(Vector2f(600,600));
	skillTimerBarBkg.setOutlineColor(Color(255,255,255,200));
	skillTimerBarBkg.setFillColor(Color(255,255,255,200));
	skillTimerBarBkg.setPosition(-100,-100);
	_skillTimerBarBkg = skillTimerBarBkg;

	// Flash
	RectangleShape flash;
	flash.setSize(Vector2f(600,600));
	flash.setOutlineColor(Color(255,255,255,255));
	flash.setFillColor(Color(255,255,255,255));
	flash.setPosition(-100,-100);
	_flash = flash;

    View view(sf::FloatRect(0,0,VIEW_WIDTH,VIEW_HEIGHT));
	_view = view;
	_wait = 0;
	_timer = -1;
	_attackState = 0;



	// Background
	Texture backgroundTex;
	backgroundTex.loadFromFile("images/dungeon6.png");
	_backgroundTex = backgroundTex;

	Sprite bkgSpr(_backgroundTex);
	bkgSpr.setPosition(0,0);
	bkgSpr.setTextureRect(IntRect(0, 0, 480, 272));
	_backgroundSpr = bkgSpr;

	// Cursor
	Texture cursorTex;
	cursorTex.loadFromFile("images/FF7Cursor.png");
	_cursorTex = cursorTex;

	Sprite cursorSpr(_cursorTex);
	cursorSpr.setPosition(40, 200);
	cursorSpr.setScale(0.5, 0.5);
	_cursorSpr = cursorSpr;


	// Initialize everyone
	battleObjList list = BattleBlackboard::get()->getBattleObjList();
	for (battleObjList::iterator i = list.begin(); i != list.end(); ++i)
	{
		(*i)->initialize();
	}
}

/** 
 *  Update all of the objects in the game.
 *  @param elapsed - the amount of seconds elapsed since the last
 *     time update was called.
 */
void BattleSystem::update(float elapsed)
{
	// Check if action queue is empty, otherwise perform the stuff in it
	if (_battleActionQueue.size() > 0)
	{
		// Set wait flag. Queue will not release wait until empty.
		_wait = 1;

		//cout << "Attacking: " << _battleActionQueue.front().attackerID << endl;
		//cout << _battleActionQueue.front().finished << endl;
		
		if (_battleActionQueue.front().finished == 0)
		{
			if (_battleActionQueue.front().type == ATTACK)
			{
				executeAttack(_battleActionQueue.front().attackerID, _battleActionQueue.front().targetID);
			}
			else if (_battleActionQueue.front().type == PLAYERMODE)
			{
				_menuVisible = 1;

				// CURSOR INPUT
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && _keypressed == false)
				{
					_keypressed = true;
					
					AudioEvent* sound = new AudioEvent(EVENT_AUDIO_EFFECT);
					sound->setEffect(CURSOR_SOUND);
					EventPtr aEvent(sound);
					EventManager::get()->dispatchImmediate(aEvent);

					if (_cursorState == 0)
					{
						if (_menuSelection == 0)
						{
							// Go to enemy selection
							_cursorState = 1;
							cursorCheckValidEnemy(-1);
						}
						else if (_menuSelection == 1)
						{
							BattleAction action;
							action.targetID = BattleBlackboard::get()->getBattleObj("ally")->getID();
							action.type = DEFEND;
							addBattleAction(action);
							_menuVisible = 0;
							_battleActionQueue.front().finished = 1;
							_cursorState = 0;
						}
						else if (_menuSelection == 2)
						{
							// Ally SP must be = maxSP
							BattleObj ally = BattleBlackboard::get()->getBattleObj("ally");
							if (ally->getMP() == ally->getMaxMP())
							{
								_cursorState = 1;
								cursorCheckValidEnemy(-1);
							}
							else
							{
								// Error sound
							}
						}
					}
					else if (_cursorState == 1)
					{
						if (_menuSelection == 0)
						{
							// Select enemy to attack
							BattleAction action;
							action.attackerID = _battleActionQueue.front().attackerID;
							action.targetID = _enemyPos[_enemySelection];
							action.type = ATTACK;
							addBattleAction(action);
							_menuVisible = 0;
							_battleActionQueue.front().finished = 1;
							_cursorState = 0;
						}
						else if (_menuSelection == 2)
						{
							// Select enemy to attack
							BattleAction action;
							action.attackerID = _battleActionQueue.front().attackerID;
							action.targetID = _enemyPos[_enemySelection];
							action.type = SKILL;
							addBattleAction(action);
							_menuVisible = 0;
							_battleActionQueue.front().finished = 1;
							_cursorState = 0;	
						}
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && _keypressed == false)
				{
					_keypressed = true;

					if (_cursorState == 1)
					{			
						AudioEvent* sound = new AudioEvent(EVENT_AUDIO_EFFECT);
						sound->setEffect(CANCEL_SOUND);
						EventPtr aEvent(sound);
						EventManager::get()->dispatchImmediate(aEvent);

						_cursorState = 0;
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && _keypressed == false)
				{
					_keypressed = true;

					AudioEvent* sound = new AudioEvent(EVENT_AUDIO_EFFECT);
					sound->setEffect(CURSOR_SOUND);
					EventPtr aEvent(sound);
					EventManager::get()->dispatchImmediate(aEvent);

					if (_cursorState == 0)
					{
						if (_menuSelection > 0)
						{
							_menuSelection -= 1;
						}
						else
						{
							_menuSelection = _menuSelectionMax;
						}
					}
					else if (_cursorState == 1)
					{
						cursorCheckValidEnemy(0);
					}
				}
				
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && _keypressed == false)
				{
					_keypressed = true;
					
					AudioEvent* sound = new AudioEvent(EVENT_AUDIO_EFFECT);
					sound->setEffect(CURSOR_SOUND);
					EventPtr aEvent(sound);
					EventManager::get()->dispatchImmediate(aEvent);

					if (_cursorState == 0)
					{
						if (_menuSelection < _menuSelectionMax)
						{
							_menuSelection += 1;
						}
						else
						{
							_menuSelection = 0;
						}
					}
					else if (_cursorState == 1)
					{
						cursorCheckValidEnemy(1);
					}
				}
			}
			else if (_battleActionQueue.front().type == DEFEND)
			{
				executeDefend(_battleActionQueue.front().targetID);
			}
			else if (_battleActionQueue.front().type == SKILL)
			{
				executeSkill(_battleActionQueue.front().attackerID, _battleActionQueue.front().targetID);
			}
		}
		else
		{
			_battleActionQueue.pop();
		}
		
	}
	else
	{
		_wait = 0;
	}

	//Debug zoom
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		if (_zoomState == -1)
		{
			_zoomState = 0;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
	{
		if (_zoomState == -1)
		{
			_zoomState = 1;
		}
	}*/

	// Do some view zooming, SFML's zoom method sucks
	if (_zoomState != -1)
	{            
		float x = _zoomX;
		float y = _zoomY;

		//cout << "zoomX: " << _zoomX << endl;
		//cout << "zoomY: " << _zoomY << endl;

		if (_zoomState == 0)
		{	
			if (_zoom < _zoomMax)
			{
				_zoom += 1;
				_view.setSize(_view.getSize().x - 64/9, _view.getSize().y - 36/9);
				centerView(x, y, 0);
			}
			else
			{
				_zoom = _zoomMax;
				_zoomState = -1;
			}
		}
		else if (_zoomState == 1)
		{
			if (_zoom > 0)
			{
				_zoom -= 1;
				_view.setSize(_view.getSize().x + 64/9, _view.getSize().y + 36/9);
				centerView(x, y, 1);
			}
			else
			{
				_zoom = 0;
				_view.setSize(VIEW_WIDTH, VIEW_HEIGHT);
				_zoomState = -1;
				_view.setCenter(VIEW_WIDTH/2, VIEW_HEIGHT/2);
			}	
		}
	}


	// Increment everyone's ATB
	battleObjList list = BattleBlackboard::get()->getBattleObjList();
	for (battleObjList::iterator i = list.begin(); i != list.end(); ++i)
	{
		if (_wait == 0)
		{
			(*i)->addATB();
		}
	}

	// Updating strings and menus
	if (BattleBlackboard::get()->allyCheck())
	{
		BattleObj someone = BattleBlackboard::get()->getBattleObj("ally");
		float hp = someone->getHP();
		float hpMax = someone->getMaxHP();
		float atb = someone->getATB();
		int mp = someone->getMP();
		_allyStatStr.setString(" HP: " + ftoa(hp) + " / " + ftoa(hpMax) + "\n       SKILL");
		_allyATBBar.setSize(Vector2f(atb,6));
		_allyMPBar.setSize(Vector2f(mp*25,6));
	}

	_skillTimerBarBkg.setFillColor(Color(255,242,0,_skillTimer));

	if (_cursorState == 0)
	{
		switch(_menuSelection)
		{
		case 0:
			_cursorSpr.setPosition(40, 208);
			break;
		case 1:
			_cursorSpr.setPosition(40, 222);
			break;
		case 2:
			_cursorSpr.setPosition(40, 236);
			break;
		default:
			_cursorSpr.setPosition(40, 206);
		}
	}
	else if (_cursorState == 1)
	{
		BattleObj selectedEnemy = BattleBlackboard::get()->getBattleObjByID(_enemyPos[_enemySelection]);
		Vector2f sprTemp = selectedEnemy->getSprCenter();
		int tempX = selectedEnemy->getX() + sprTemp.x;
		int tempY = selectedEnemy->getY() + sprTemp.y;
		_cursorSpr.setPosition(tempX - 50, tempY);
	}

	// Update battle objects
	for (battleObjList::iterator i = list.begin(); i != list.end(); ++i)
	{
		(*i)->update(elapsed);
	}
    
	
	// Update effect objects
	effectObjList efflist = BattleBlackboard::get()->getEffectObjList();
	for (effectObjList::iterator i = efflist.begin(); i != efflist.end(); ++i)
	{
		(*i)->update(elapsed);
	}

	// Check for key release
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !sf::Keyboard::isKeyPressed(sf::Keyboard::X) && _keypressed == true)
	{
		_keypressed = false;
	}
	
}

/**
 *  Render everything in the game world.
 */
void BattleSystem::render(sf::RenderWindow &window)
{
	// Render background
	window.draw(_backgroundSpr);

	// Render all battle objects
	battleObjList list = BattleBlackboard::get()->getBattleObjList();

	for (battleObjList::iterator i = list.begin(); i != list.end(); i++)
	{
		(*i)->render(window);
	}

	// Render all effect objects
	effectObjList efflist = BattleBlackboard::get()->getEffectObjList();

	for (effectObjList::iterator i = efflist.begin(); i != efflist.end(); i++)
	{
		(*i)->render(window);
	}

	// Render overlays and text	
	window.setView(_view);
	
	window.draw(_allyStatStr);
	window.draw(_allyATBBarBkg);
	window.draw(_allyATBBar);
	window.draw(_allyMPBarBkg);
	window.draw(_allyMPBar);

	window.draw(_skillTimerBarBkg);

	if (_flashVisible == 1)
	{
		window.draw(_flash);
		_flashVisible = 0;
	}

	if (_currentCharVisible == 1)
	{
		Text skillStr(_currentChar, _font, _fontSize + 60);
		skillStr.setPosition(_view.getCenter().x - 48, _view.getCenter().y - 48);
		window.draw(skillStr);
	}

	if (_menuVisible == 1)
	{
		window.draw(_menuStr);
		window.draw(_cursorSpr);
	}
}

/**
 *  Finish everything, so clear it out in preparation for
 *  a new level or quitting the game.
 */
void BattleSystem::finish()
{
	// Clean up my blackboard
	BattleBlackboard::get()->cleanUp();

	// Clean up my other stuff
	_enemyPos.clear();
	while (!_battleActionQueue.empty())
	{
		_battleActionQueue.pop();
	}
	_combo.clear();

	if (_battleComplete == false)
	{
		// Reset me
		_battleComplete = true;
		cout << "Battle finished" << endl;
	}
}

int BattleSystem::cursorCheckValidEnemy(int direction)
{
	if (direction == 0)
	{
		if (_enemySelection > 0)
		{
			_enemySelection -= 1;
		}
		else
		{
			_enemySelection = _enemyMaxPos;
		}

		int origPos = _enemySelection;

		while (true) // Loop through enemies
		{
			
			BattleObj selectedEnemy = BattleBlackboard::get()->getBattleObjByID(_enemyPos[_enemySelection]);
			
			
			if (selectedEnemy->isDead())
			{
				if (_enemySelection > 0)
				{
					_enemySelection -= 1;
				}
				else
				{
					_enemySelection = _enemyMaxPos;
				}
			}
			else
			{
				return 1;
			}

			if (_enemySelection == origPos) // Back to where we were before, couldn't find a valid enemy
			{
				_cursorState = 0;
				return 0;
			}
		}
		_cursorState = 0;
		return 0;
	}
	else if (direction == 1 || direction == -1)
	{
		if (direction == 1)
		{
			if (_enemySelection < _enemyMaxPos)
			{
				_enemySelection += 1;
			}
			else
			{
				_enemySelection = 0;
			}
		}

		int origPos = _enemySelection;

		while (true)
		{
			BattleObj selectedEnemy = BattleBlackboard::get()->getBattleObjByID(_enemyPos[_enemySelection]);
			if (selectedEnemy->isDead())
			{
				if (_enemySelection < _enemyMaxPos)
				{
					_enemySelection += 1;
				}
				else
				{
					_enemySelection = 0;
				}
			}
			else
			{
				return 1;
			}

			if (_enemySelection == origPos) // Back to where we were before, couldn't find a valid enemy
			{
				_cursorState = 0;
				return 0;
			}
		}

		_cursorState = 0;
		return 0;	
	}
}

sf::String BattleSystem::ftoa(float num)
{
	std::ostringstream ss;
	ss << num;
	std::string s(ss.str());

	sf::String str = s;

	return str;
}


void BattleSystem::executeAttack(int attackerID, int targetID)
{
	BattleObj attacker = BattleBlackboard::get()->getBattleObjByID(attackerID);
	BattleObj target = BattleBlackboard::get()->getBattleObjByID(targetID);

	if (_attackState == 0) // Init attack
	{
		cout << attacker->getObjName() << " is attacking " << target->getObjName() << endl;

		// Save attacker's position
		_prevPos.x = attacker->getX();
		_prevPos.y = attacker->getY();

		_timer = 40;
		_attackState = 1;
	}

	if (_attackState == 1) // Move to position
	{
		if (_timer > 0)
		{
			_timer -= 1;
		}
		else
		{
			if (attacker->getType() == 0) // Monsters will just attack the ally
			{
				// Move attacker next to target
				float attackPosX = target->getX() - 32;
				float attackPosY = target->getY();
				attacker->setPosition(attackPosX, attackPosY);
			}
			else if (attacker->getType() == 1)
			{
				Vector2f sprTemp = target->getSprCenter();
				int tempX = target->getX() + sprTemp.x;
				int tempY = target->getY() + sprTemp.y;
				attacker->setPosition(tempX + 32, tempY);	
			}
				
			_timer = 40;
			_attackState = 2;
		}
	}

	if (_attackState == 2) // Attack
	{
		if (_timer > 0)
		{
			_timer -= 1;
		}
		else
		{
			attacker->attack(targetID);
			_timer = 40;
			_attackState = 3;

			// Show a hit effect
			EffectObj hitEffect(new EffectObject);
			Vector2f targetCenter = target->getSprCenter();
			int tempX = target->getX() + targetCenter.x;
			int tempY = target->getY() + targetCenter.y;
			hitEffect->setPosition(tempX, tempY);
			hitEffect->setType(1);
			hitEffect->createSprite(0);
			BattleBlackboard::get()->insertEffectObj(hitEffect);

		}
	}
		
	if (_attackState == 3) // Return to position
	{
			
		if (_timer > 0)
		{
			_timer -= 1;
		}
		else
		{
			// Move attacker back to original position
			std::map<std::string, float> propMap = BattleBlackboard::get()->getProperties();
			float savedX = _prevPos.x;
			float savedY = _prevPos.y;
			attacker->setPosition(savedX, savedY);
			_attackState = 4;
		}
	}

	if (_attackState == 4) // Done attacking
	{
		// Set action as finished
		cout << attacker->getObjName() << " finished attacking " << target->getObjName() << endl;
		attacker->setATB(0.0);
		_attackState = 0;
		_timer = -1;
		_battleActionQueue.front().finished = 1;

		// Check if battle is complete
		if (BattleBlackboard::get()->enemyCheck())
		{
			finish();
		}
	}
}


void BattleSystem::executeDefend(int targetID)
{
	BattleObj target = BattleBlackboard::get()->getBattleObjByID(targetID);
	target->defend();
	cout << target->getObjName() << " is defending " << endl;

	AudioEvent* sound = new AudioEvent(EVENT_AUDIO_EFFECT);
	sound->setEffect(DEFEND_SOUND);
	EventPtr aEvent(sound);
	EventManager::get()->dispatchImmediate(aEvent);


	// Set action as finished
	target->setATB(0.0);
	_battleActionQueue.front().finished = 1;
}


void BattleSystem::executeSkill(int attackerID, int targetID)
{
	BattleObj attacker = BattleBlackboard::get()->getBattleObjByID(attackerID);
	BattleObj target = BattleBlackboard::get()->getBattleObjByID(targetID);

	if (_attackState == 0) // Init attack
	{
		cout << attacker->getObjName() << " is attacking (with skill) " << target->getObjName() << endl;

		int pattern = rand() % 2;
		setupCombo(pattern);
		attacker->setUsingSkill(true);
		_comboSuccessCount	= 0;
		_comboSuccessCountMax = _combo.size();

		// Save attacker's position
		_prevPos.x = attacker->getX();
		_prevPos.y = attacker->getY();

		_skillTimer = 0;

		_timer = 40;
		_attackState = 1;
	}

	if (_attackState == 1) // Move to position
	{
		if (_timer > 0)
		{
			if (_skillTimer < _skillTimerMax)
			{
				_skillTimer += 10;
			}
			_timer -= 1;
		}
		else
		{
			if (attacker->getType() == 0) // Monsters will just attack the ally
			{
				// Move attacker next to target
				float attackPosX = target->getX() - 32;
				float attackPosY = target->getY();
				attacker->setPosition(attackPosX, attackPosY);
			}
			else if (attacker->getType() == 1)
			{
				Vector2f sprTemp = target->getSprCenter();
				int tempX = target->getX() + sprTemp.x;
				int tempY = target->getY() + sprTemp.y;
				attacker->setPosition(tempX + 32, tempY);
				
				// Zoom in
				_zoom = 0;
				_zoomX = tempX;
				_zoomY = tempY;
				_zoomState = 0;

			}
				
			_timer = 250;
			_attackState = 2;
		}
	}

	if (_attackState == 2) // Attack
	{
		if (_timer > 0)
		{
			_timer -= 1;

			if (_skillTimer > 0)
			{
				if (_currentCharVisible == 0)
				{
					_currentCharVisible = 1;
				}

				_skillTimer -= 1;
				
				// Perform combo
				if (!_combo.empty())
				{
					_currentChar = _combo.back();
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && _keypressed == false && target->getHP() > 0)
				{
					_keypressed = true;

					if (_currentChar == 'Z')
					{
						_flashVisible = 1;

						EffectObj hitEffect(new EffectObject);
						Vector2f targetCenter = target->getSprCenter();
						int tempX = target->getX() + targetCenter.x;
						int tempY = target->getY() + targetCenter.y;
						hitEffect->setPosition(tempX, tempY);
						hitEffect->setType(1);
						hitEffect->createSprite(0);
						BattleBlackboard::get()->insertEffectObj(hitEffect);
					
						attacker->attack(targetID);
						_comboSuccessCount += 1;

						if (!_combo.empty())
						{
							_combo.pop_back();
						}
					}
					else
					{
						_skillTimer -= 10;
					}
				}
				
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && _keypressed == false && target->getHP() > 0)
				{
					_keypressed = true;
					
					if (_currentChar == 'X')
					{
						_flashVisible = 1;

						EffectObj hitEffect(new EffectObject);
						Vector2f targetCenter = target->getSprCenter();
						int tempX = target->getX() + targetCenter.x;
						int tempY = target->getY() + targetCenter.y;
						hitEffect->setPosition(tempX, tempY);
						hitEffect->setType(1);
						hitEffect->createSprite(0);
						BattleBlackboard::get()->insertEffectObj(hitEffect);
					
						attacker->attack(targetID);
						_comboSuccessCount += 1;

						if (!_combo.empty())
						{
							_combo.pop_back();
						}
					}
					else
					{
						_skillTimer -= 10;
					}
				}

				if (target->isDead())
				{
					_comboSuccessCount = _comboSuccessCountMax;
				}
			}
		}
		else
		{
			_currentCharVisible = 0;
			_timer = 40;
			_attackState = 3;
		}
	}
		
	if (_attackState == 3) // Return to position
	{
			
		if (_timer > 0)
		{
			_timer -= 1;
		}
		else
		{
			// Zoom out
			_zoomState = 1;

			// Move attacker back to original position
			std::map<std::string, float> propMap = BattleBlackboard::get()->getProperties();
			float savedX = _prevPos.x;
			float savedY = _prevPos.y;
			attacker->setPosition(savedX, savedY);
			_attackState = 4;
		}
	}

	if (_attackState == 4) // Done attacking
	{
		// Set action as finished
		if (_comboSuccessCount == _comboSuccessCountMax)
		{
			attacker->setHP(attacker->getHP() + 25);

			if (attacker->getHP() > attacker->getMaxHP())
			{
				attacker->setHP(attacker->getMaxHP());
			}
		}
		cout << attacker->getObjName() << " finished attacking (with skill) " << target->getObjName() << endl;
		attacker->setUsingSkill(false);
		attacker->setATB(0.0);
		attacker->setMP(0);
		_attackState = 0;
		_skillTimer = 0;
		_timer = -1;
		_battleActionQueue.front().finished = 1;

		// Check if battle is complete
		if (BattleBlackboard::get()->enemyCheck())
		{
			finish();
		}
	}
}



void BattleSystem::addBattleAction(BattleAction action)
{
	_battleActionQueue.push(action);
	cout << "action added: " << action.attackerID << ", " << action.targetID << ", " << action.type << ", " << action.finished << endl;
}



void BattleSystem::centerView(float x, float y, int type)
{
	if (type == 0)
	{
		if (x < _view.getCenter().x)
		{
			_view.move(-2, 0);
		}

		if (x > _view.getCenter().x)
		{
			_view.move(2, 0);
		}

		if (y < _view.getCenter().y)
		{
			_view.move(0, -2);
		}

		if (y > _view.getCenter().y)
		{
			_view.move(0, 2);
		}
	}
	else if (type == 1)
	{
		if (_view.getCenter().x > VIEW_WIDTH/2)
		{
			_view.move(-2, 0);
		}
		else if (_view.getCenter().x < VIEW_WIDTH/2)
		{
			_view.move(2, 0);
		}

		if (_view.getCenter().y > VIEW_HEIGHT/2)
		{
			_view.move(0, -2);
		}
		else if (_view.getCenter().y < VIEW_HEIGHT/2)
		{
			_view.move(0, 2);
		}
	}
}


void BattleSystem::setupCombo(int pattern)
{
	_combo.clear();
	switch (pattern)
	{
	case 0:
		_combo.push_back('Z');
		_combo.push_back('X');
		_combo.push_back('Z');
		_combo.push_back('Z');
		_combo.push_back('Z');
		_combo.push_back('X');
		break;
	case 1:
		_combo.push_back('Z');
		_combo.push_back('Z');
		_combo.push_back('X');
		_combo.push_back('Z');
		_combo.push_back('X');
		_combo.push_back('X');
	case 2:
		_combo.push_back('X');
		_combo.push_back('Z');
		_combo.push_back('Z');
		_combo.push_back('X');
		_combo.push_back('Z');
		_combo.push_back('X');
	}
}



void BattleSystem::setupBattle()
{
	//int allyExists = 0;
	int enemies = 0;
	int enemiesMax = 1 + rand() % 3;

	BattleObj ally1(new BattleObject);

	// Make some test battle objects
	/*if (BattleBlackboard::get()->allyCheck())
	{
		allyExists = 1;
	}
	
	if (allyExists == 1)
	{
		ally1 = BattleBlackboard::get()->getBattleObj("ally"); // Reuse ally if he exists
	}*/

	ally1->setObjName("ally");
	ally1->setDepth(5);
	ally1->setType(1);
	Texture allyTex;
	allyTex.loadFromFile("images/drScience.png");
	Sprite allySpr(allyTex);
	ally1->setPosition(320, 100);
	ally1->setSprite(allySpr);
	ally1->setTexture(allyTex);
	ally1->setSPD(60);
	ally1->setATK(100);

	sf::Vector2i temp = BattleBlackboard::get()->getSavedHP();
	if (temp.x != 0 && temp.y != 0)
	{
		ally1->setHP(temp.x);
		ally1->setMaxHP(temp.y);
	}

	BattleBlackboard::get()->insertBattleObj(ally1);

	if (enemies < enemiesMax)
	{
		enemies += 1;
		BattleObj enemy1(new BattleObject);
		enemy1->setObjName("monster1");
		enemy1->setType(0);
		Texture enemyTex1;
		enemyTex1.loadFromFile("images/Zombie1.png");
		Sprite enemySpr1(enemyTex1);
		enemy1->setPosition(70, 34);
		enemy1->setSprite(enemySpr1);
		enemy1->setTexture(enemyTex1);
		enemy1->setSPD(4);
		int ene1id = BattleBlackboard::get()->insertBattleObj(enemy1);
		_enemyPos.push_back(ene1id);
		_enemyMaxPos = 0;
	}

	if (enemies < enemiesMax)
	{
		enemies += 1;
		BattleObj enemy2(new BattleObject);
		enemy2->setObjName("monster2");
		enemy2->setType(0); 
		Texture enemyTex2;
		enemyTex2.loadFromFile("images/Zombie2.png");
		enemy2->setPosition(100, 100);
		Sprite enemySpr2(enemyTex2);
		enemy2->setSprite(enemySpr2);
		enemy2->setTexture(enemyTex2);
		enemy2->setSPD(10);
		int ene2id = BattleBlackboard::get()->insertBattleObj(enemy2);
		_enemyPos.push_back(ene2id);
		_enemyMaxPos = 1;
	}

	if (enemies < enemiesMax)
	{
		enemies += 1;
		BattleObj enemy3(new BattleObject);
		enemy3->setObjName("monster3");
		enemy3->setType(0);
		Texture enemyTex3;
		enemyTex3.loadFromFile("images/Zombie3.png");
		enemy3->setPosition(70, 140);
		Sprite enemySpr3(enemyTex3);
		enemy3->setSprite(enemySpr3);
		enemy3->setTexture(enemyTex3);
		enemy3->setSPD(8);
		int ene3id = BattleBlackboard::get()->insertBattleObj(enemy3);
		_enemyPos.push_back(ene3id);
		_enemyMaxPos = 2;
	}





	
}