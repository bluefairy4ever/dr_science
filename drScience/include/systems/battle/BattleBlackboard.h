
#ifndef drScience_BattleBlackboard_h
#define drScience_BattleBlackboard_h

#include "utils/Constants.h"
#include "BattleSystem.h"
#include "SFML/Graphics.hpp"
#include <vector>
#include <string>
#include <map>

using namespace std;


typedef vector<BattleObj> battleObjList;
typedef vector<EffectObj> effectObjList;
typedef vector<BattleAction> actionQueue;


class BattleBlackboard
{
public:
	static BattleBlackboard* get();
	
	battleObjList	getBattleObjList();
	effectObjList	getEffectObjList();
	int				insertBattleObj(BattleObj obj);
	int				insertEffectObj(EffectObj obj);
	void			removeBattleObj(BattleObj obj);
	void			removeEffectObjByID(int ID);
	void			removeBattleObjByID(int ID);
	BattleObj		getBattleObj(string objName);
	BattleObj		getBattleObjByID(int ID);
	void			setObjIDCount(int ID) {_objIDCount = ID;};
	void			setEffIDCount(int ID) {_effIDCount = ID;};
	void			addAction(BattleAction act);
	void			cleanUp();
	void			saveHP(int hp, int hpmax) {_savedHP = hp, _savedMaxHP = hpmax;};
	
	sf::Vector2i	getSavedHP();

	bool			allyCheck();
	bool			enemyCheck(); // Check for any non-dead enemies

	std::map<std::string, float>		getProperties() {return _properties;};

private:
	BattleBlackboard() {};
	BattleBlackboard(BattleBlackboard const&) {};
	BattleBlackboard& operator=(BattleBlackboard const&) {};

	static BattleBlackboard* _instance;
	int					_objIDCount;				// ID counter for objects
	int					_effIDCount;				// ID counter for effects
    battleObjList		_battleObjList;			// List of battle objects
    effectObjList		_effectObjList;			// List of effect objects
	actionQueue			_actionQueue;			// Queue of actions
	std::map<std::string, float>	_properties;				// Store any kind of property here

	int					_savedHP;
	int					_savedMaxHP;
};

#endif
