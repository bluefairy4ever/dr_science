#include "systems/EventManager.h"
#include "events/ValueEvent.h"
#include "events/AudioEvent.h"

#include "systems\battle\BattleObject.h"
#include "systems\battle\BattleBlackboard.h"
#include "systems\battle\BattleSystem.h"
#include "utils\constants.h"
#include <string>
#include <sstream>
#include <time.h>
using namespace std;

BattleObject::BattleObject()
{

	// Default values
	_x				= 16;
	_y				= 16;
	_HP				= 100;
	_maxHP			= 100;
	_MP				= 4;
	_maxMP			= 4;
	_ATK			= 10;
	_DEF			= 2;
	_SPD			= 20;

	_ATB			= 0;
	_maxATB			= 50;
	_objName		= "";
	_visible		= 1;
	_blinkTimer		= -1;
	_blinkInterval	= 5;
	_alpha			= 255;
	_depth			= 0;

	_dead = false;
	_defending = false;
	_usingSkill = false;
}

BattleObject::~BattleObject()
{
	
}

void BattleObject::initialize()
{
	_sprite.setPosition(_x, _y);
	_sprite.setTexture(_texture);
	cout << "BattleObj(" << _x << "," << _y << ")" << endl;
}

void BattleObject::addATB()
{
	if (_ATB < _maxATB)
	{
		// SPEED CALCULATION FORMULA HERE
		float randomModifier = (rand() % 100); 
		_ATB += ((_SPD + randomModifier) / 1000);
		//cout << _objName << "(ATB): " << _ATB << endl;
	}
	else
	{
		_ATB = _maxATB;
		_defending = false;

		if (_type == 0) // if monster, attack ally for now
		{
			//attack();
			// Queue an attack

			BattleAction action;
			action.attackerID = _id;
			action.targetID = BattleBlackboard::get()->getBattleObj("ally")->getID();
			action.type = ATTACK;
			BattleSystem::get()->addBattleAction(action);
		}
		else if (_type == 1) // if ally, switch to player mode
		{
			BattleAction action;
			action.type = PLAYERMODE;
			action.attackerID = _id;
			BattleSystem::get()->addBattleAction(action);
		}
	}
}

void BattleObject::setType(int type)
{
	_type = type;
}

void BattleObject::setSprite(sf::Sprite sprite)
{
	_sprite = sprite;	
}

void BattleObject::setTexture(sf::Texture tex)
{
	_texture = tex;
}

void BattleObject::update(float elapsed)
{
	// Update sprite position to my position
	_sprite.setPosition(_x, _y);
	
	// Update blink timer if > -1
	if (_blinkTimer != -1)
	{
		if (_blinkTimer > 0)
		{
			_blinkTimer -= 1;
		}
		else
		{
			// Blink every something steps
			_blinkTimer = _blinkInterval; 
			if (_visible == 0)
			{
				_visible = 1;
			}
			else
			{
				_visible = 0;
			}
		}
	}
	else
	{
		_visible = 1; // Inactive timer means the object should be visible
	}

	// Update blink timer stopper
	if (_blinkTimerStopper > 0)
	{
		_blinkTimerStopper -= 1;
	}
	else
	{
		_blinkTimerStopper = 0;
		_blinkTimer = -1;
	}

	// Fade away if HP <= 0
	if (_HP <= 0)
	{
		if (_dead == false)
		{
			if (_objName != "ally")
			{
				AudioEvent* sound = new AudioEvent(EVENT_AUDIO_EFFECT);
				sound->setEffect(ENEMY_DEATH_SOUND);
				EventPtr aEvent(sound);
				EventManager::get()->dispatchImmediate(aEvent);
			}

			_dead = true;
			//die();
		}

		if (_alpha > 0)
		{
			_alpha -= 20;
		}
		else
		{
			_alpha = 0;
			_visible = 0;
			_ATB = 0;
		}
	}

	if (_defending == true)
	{
		_sprite.setColor(sf::Color(0, 0, 255, _alpha));	
	}
	else
	{
		_sprite.setColor(sf::Color(255, 255, 255, _alpha));			
	}

}

void BattleObject::render(sf::RenderWindow &window)
{
	// Draw my sprite
	if (_visible == 1)
	{
		window.draw(_sprite);
	}
}

// Only updates the ally in case the player has picked up a health pack
void BattleObject::onEvent(const EventPtr& anEvent)
{
   /* if ( _objName == "ally" ) {
        boost::shared_ptr<ValueEvent> evt = boost::static_pointer_cast<ValueEvent>(anEvent);

        if(_HP < 100.0f - evt.get()->getValue()) {
            _HP += evt.get()->getValue();
        } else {
            _HP = 100.0f; 
        }
    }*/
}

void BattleObject::setPosition(float x, float y)
{
	_x = x;
	_y = y;
}

void BattleObject::setObjName(string newName)
{
	_objName = newName;

}

string BattleObject::getObjName()
{
	return _objName;
}

void BattleObject::setBlinkTimer(float interval, float duration)
{
	// Use -1 duration for infinite loop
	_blinkTimer		= interval;
	_blinkInterval	= interval;
	
	if (duration != -1)
	{
		_blinkTimerStopper	= duration;
	}
}

void BattleObject::setSPD(float SPD)
{
	_SPD = SPD;
}

float BattleObject::getHP()
{
	return _HP;
}

void BattleObject::setHP(float newHP)
{
	_HP = newHP;
}

float BattleObject::getMaxHP()
{
	return _maxHP;
}

void BattleObject::setMaxHP(float newMaxHP)
{
	_maxHP = newMaxHP;
}

float BattleObject::getATB()
{
	return _ATB;
}

void BattleObject::setATB(float newATB)
{
	_ATB = newATB;
}

float BattleObject::getX()
{
	return _x;
}

float BattleObject::getY()
{
	return _y;
}

void BattleObject::attack(int targetID)
{
	BattleObj target = BattleBlackboard::get()->getBattleObjByID(targetID);

	// DAMAGE CALCULATION HERE, do random damage for now
	int damage = _ATK + (rand() % 5) - target->getDEF();

	if (_usingSkill == true) // Make multi-hit skill fair
	{
		damage = damage / 4;
	}
	
	if (target->isDefending())
	{
		damage = damage / 2;
	}

	float hpDiff = target->getHP() - damage;
    if ( target->getObjName() == "ally" )
    {
        // Create and send a health modification event to update
        // the player's health
        ValueEvent* value = new ValueEvent(EVENT_HEALTH_MODIFICATION);
	    value->setValue(float(0-damage));
	    EventPtr vEvent(value);
	    EventManager::get()->dispatchImmediate(vEvent);        
        cout << "Health modification event created\n";
    }
    else 
    {
        // Update enemy health
	    target->setHP(hpDiff);
    }

	target->setHP(hpDiff);
	
	if (target->getObjName() == "ally")
	{
		BattleBlackboard::get()->saveHP(target->getHP(),target->getMaxHP());
	}

	target->setBlinkTimer(3, 50);
    
	// Make a damage indicator effect
	EffectObj dmgInd(new EffectObject);
	dmgInd->setType(0);
	sf::Vector2f sprTemp = target->getSprCenter();
	int tempX = target->getX() + sprTemp.x;
	int tempY = target->getY();
	dmgInd->setPosition(tempX, tempY);
	dmgInd->setStr(ftoa(damage));
	dmgInd->setTimer(60);
	BattleBlackboard::get()->insertEffectObj(dmgInd);
	
    // Create and send an audio event to play attack sound
    AudioEvent* sound = new AudioEvent(EVENT_AUDIO_EFFECT);
    
	if (target->getObjName() == "ally")
	{
		sound->setEffect(ENEMY_ATTACK_SOUND);
	}
	else
	{
		sound->setEffect(ATTACK_SOUND);
	}
	EventPtr aEvent(sound);
	EventManager::get()->dispatchImmediate(aEvent);    

	// Add skill point/MP
	if (_MP < _maxMP)
	{
		_MP += 1;
	}
}


void BattleObject::defend()
{
	_defending = true;
}


string BattleObject::ftoa(float num)
{
	std::ostringstream ss;
	ss << num;
	std::string s(ss.str());

	sf::String str = s;

	return str;
}


sf::Vector2f BattleObject::getSprCenter()
{
	sf::Vector2f temp;
	float centerX = _texture.getSize().x / 2.0;
	float centerY = _texture.getSize().y / 2.0;
	temp.x = centerX;
	temp.y = centerY;
	return temp;
}


void BattleObject::die()
{
	_visible = 0;
	BattleBlackboard::get()->removeBattleObjByID(_id); // Kill yourself
}