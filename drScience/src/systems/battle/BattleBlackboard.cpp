#include "systems/battle/BattleBlackboard.h"
#include "systems/battle/BattleObject.h"

struct compareBattleObjDepth
{
    inline bool operator() (const BattleObj& lhs, const BattleObj& rhs)
    {
        return (lhs->getDepth() < rhs->getDepth());
    }
};

BattleBlackboard* BattleBlackboard::_instance = NULL;

/**
 *  Get an instance of this EventManager
 *  as there can only be one.
 */
BattleBlackboard* BattleBlackboard::get()
{
	if (!_instance)
		_instance = new BattleBlackboard();
	return _instance;
}


int BattleBlackboard::insertBattleObj(BattleObj obj)
{
	_objIDCount += 1;
	obj->setID(_objIDCount);
	_battleObjList.push_back(obj);
	sort(_battleObjList.begin(), _battleObjList.end(), compareBattleObjDepth()); // Sort by depth
	return obj->getID();
}

int BattleBlackboard::insertEffectObj(EffectObj obj)
{
	_effIDCount += 1;
	obj->setID(_effIDCount);
	_effectObjList.push_back(obj);
	return obj->getID();
}


void BattleBlackboard::removeBattleObj(BattleObj obj)
{
	// Iterate through object list and remove the object
	for (battleObjList::iterator i = _battleObjList.begin(); i != _battleObjList.end(); i++)
	{
		if (*i == obj)
		{
			_battleObjList.erase(i);
			break;
		}
	}
}

void BattleBlackboard::removeEffectObjByID(int ID)
{
	// Iterate through object list and remove the object
	for (effectObjList::iterator i = _effectObjList.begin(); i != _effectObjList.end(); i++)
	{
		if ((*i)->getID() == ID)
		{
			_effectObjList.erase(i);
			sort(_effectObjList.begin(), _effectObjList.end());
			return;
		}
	}
}

void BattleBlackboard::removeBattleObjByID(int ID)
{
	// Iterate through object list and remove the object
	for (battleObjList::iterator i = _battleObjList.begin(); i != _battleObjList.end(); i++)
	{
		if ((*i)->getID() == ID)
		{
			_battleObjList.erase(i);
			sort(_battleObjList.begin(), _battleObjList.end());
			return;
		}
	}
}

BattleObj BattleBlackboard::getBattleObj(string objName)
{
	// Iterate through object list and search for the object, return it if found
	for (battleObjList::iterator i = _battleObjList.begin(); i != _battleObjList.end(); i++)
	{
		if ((*i)->getObjName() == objName)
		{
			return (*i);
		}
	}	
}

BattleObj BattleBlackboard::getBattleObjByID(int ID)
{
	// Iterate through object list and search for the object, return it if found
	for (battleObjList::iterator i = _battleObjList.begin(); i != _battleObjList.end(); i++)
	{
		if ((*i)->getID() == ID)
		{
			return (*i);
		}
	}	
}

battleObjList BattleBlackboard::getBattleObjList()
{
	return _battleObjList;
}

effectObjList BattleBlackboard::getEffectObjList()
{
	return _effectObjList;
}


bool BattleBlackboard::allyCheck()
{
	// Iterate through object list and search for the object, return it if found
	for (battleObjList::iterator i = _battleObjList.begin(); i != _battleObjList.end(); i++)
	{
		if ((*i)->getObjName() == "ally")
		{
			return true;
		}
	}	
	return false;
}

bool BattleBlackboard::enemyCheck()
{
	for (battleObjList::iterator i = _battleObjList.begin(); i != _battleObjList.end(); i++)
	{
		if ((*i)->isDead() == false && (*i)->getObjName() != "ally")
		{
			return false;
		}
	}	
	return true;
}


void BattleBlackboard::cleanUp()
{
	//BattleObj savedAlly = getBattleObj("ally");
	_objIDCount = 0;				
	_effIDCount = 0;				
    _battleObjList.clear();
	//_battleObjList.push_back(savedAlly);
	_effectObjList.clear();			
	_actionQueue.clear();				
}


sf::Vector2i BattleBlackboard::getSavedHP()
{
	sf::Vector2i temp;
	temp.x = _savedHP;
	temp.y = _savedMaxHP;
	return temp;
}