#ifndef drScience_EffectObject_h
#define drScience_EffectObject_h

#include <string>
#include <SFML\Graphics.hpp>
#include "utils\constants.h"
#include <vector>
using namespace std;



class EffectObject
{
public:
	EffectObject();
	~EffectObject();

	void			setStr(string str);
	sf::Text		getStr();

	void			setPosition(float x, float y);
	void			setSpeed(float spd);
	void			setAlpha(float alpha);
	void			setTimer(float time);
	void			setType(int type) {_type = type;};
	void			setID(int ID) {_id = ID;};
	int				getID() {return _id;};
	float			getX() {return _x;};
	float			getY() {return _y;};
	void			createSprite(int sprType);
	void			die();
	sf::Vector2f	getSprCenter();

	// ########################
	void initialize();
	void update(float elapsed);
	void render(sf::RenderWindow &window);
	void finish();


private:
	int					_id;
	int					_type;			// 0 = damage indicator, 1 = generic animation effect
	float				_x;
	float				_y;
	sf::Text			_str;
	sf::Font			_font;
	int					_fontSize;

	float				_speed;			// Movement speed
	float				_alpha;
	float				_timer;			// TTL

	int					_visible;

	// Animation stuff
	int					_spriteType;	// 0 = hit flash
	sf::Sprite			_sprite;
	sf::Texture			_texture;
	vector<sf::Texture>	_frames;		// Subimages, load by file
	float				_frameSpeed;	// Animation speed, 0 to 1
	int					_frameTimer;

};

#endif