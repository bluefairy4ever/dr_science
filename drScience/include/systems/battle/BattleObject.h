/**
	The game's primary parent battle object. Contains all properties
	and functions that every battle object will have.
**/

#ifndef drScience_BattleObject_h
#define drScience_BattleObject_h

#include "events/EventListener.h"

#include <string>
#include <SFML\Graphics.hpp>
#include <boost\any.hpp>
#include "utils\constants.h"
#include "systems/battle/EffectObject.h"
using namespace std;
using sf::Sprite;
using sf::Texture;

class BattleObject: public EventListener
{
typedef boost::shared_ptr<BattleObject> BattleObj;
typedef boost::shared_ptr<EffectObject> EffectObj;

public:
	BattleObject();
	~BattleObject();


	void			setType(int type);
	void			setID(int ID) {_id = ID;};
	int				getID() {return _id;};
	int				getType() {return _type;};
	void			setSprite(Sprite sprite);
	void			setTexture(Texture tex);
	void			setPosition(float x, float y);
	void			setObjName(string newName);
	string			getObjName();
	float			getX();
	float			getY();
	float			getHP();
	void			setHP(float newHP);
	float			getMaxHP();
	void			setMaxHP(float newMaxHP);
	int				getMP() {return _MP;};
	void			setMP(int newMP) {_MP = newMP;};
	float			getMaxMP() {return _maxMP;};
	void			setMaxMP(float newMP) {_maxMP = newMP;};
	float			getATB();
	void			setATB(float newATB);
	void			setSPD(float SPD);
	void			setATK(int ATK) {_ATK = ATK;};
	int				getDEF() {return _DEF;};
	void			setDEF(int DEF) {_DEF = DEF;};

	bool			isDead() {return _dead;};
	bool			isDefending() {return _defending;};
	void			setUsingSkill(bool usingSkill) {_usingSkill = usingSkill;};
	int				getDepth() {return _depth;};
	void			setDepth(int depth) {_depth = depth;};
	string			ftoa(float num);
	Sprite			getSprite() {return _sprite;};
	void			die();

	sf::Vector2f	getSprCenter();


	void		addATB();
	void		setBlinkTimer(float interval, float duration);
	void		attack(int targetID);
	void		defend();

	// ########################
	void initialize();
	void update(float elapsed);
	void render(sf::RenderWindow &window);
	void finish();
	virtual void onEvent(const EventPtr& anEvent);

private:
	int _id;		
	float _x;
	float _y;
	int _slot;
	
	string		_objName;
	int			_type; // 0 = monster/enemy, 1 = ally
	Sprite		_sprite;
	Texture		_texture;
	float		_blinkTimer;
	float		_blinkInterval;
	float		_blinkTimerStopper;
	int			_visible;
	float		_alpha;

	float		_HP;
	float		_maxHP;

	int			_MP;
	int			_maxMP;
	
	float		_ATK;
	float		_DEF;
	float		_SPD;

	float		_ATB;
	float		_maxATB;

	bool		_dead;
	bool		_defending;
	bool		_usingSkill;

	int			_depth;
	
};

#endif