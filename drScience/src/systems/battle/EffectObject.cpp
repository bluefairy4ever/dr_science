#include "systems\battle\EffectObject.h"
#include "systems\battle\BattleBlackboard.h"
using namespace std;

EffectObject::EffectObject()
{
	_x				= 0;
	_y				= 0;
	_alpha			= 1;
	_timer			= 300;
	_speed			= 1;
	_visible		= 1;
	_type			= 0;
	_spriteType		= 0;
	_frameTimer		= 1;
	_frameSpeed		= 0.25;

	sf::Text		initStr("");
	initStr.setPosition(0,0);
	_str			= initStr;

	sf::Font font;
	font.loadFromFile("images/tahomabold.ttf");
	_font = font;
	_fontSize = 12;
}

EffectObject::~EffectObject()
{

}


void EffectObject::setStr(string str)
{
	sf::Text temp(str, _font, _fontSize);
	_str = temp;
}

sf::Text EffectObject::getStr()
{
	return _str;
}

void EffectObject::update(float elapsed)
{
	if (_type == 0)
	{
		_y -= _speed; // Float upward
		_str.setPosition(_x, _y);

		if (_timer > 0)
		{
			_timer -= 1;
			_visible = 1;
		}
		else
		{
			die();
		}
	}
	else if (_type == 1) // That frame vector better have something in there
	{
		// Update frame
		if (_frameTimer > 0)
		{
			_frameTimer -= _frameSpeed;
		}
		else
		{
			_frameTimer = 1; // Reset timer
			
			// Go to next frame
			if (!_frames.empty())
			{
				sf::Texture nextFrame = _frames.back();
				_texture = nextFrame;
				_sprite.setTexture(_texture);
				_frames.pop_back();
			}
			else
			{
				_visible = 0;
				//die();
			}
		}
	}
}


void EffectObject::render(sf::RenderWindow &window)
{
	if (_visible == 1)
	{
		if (_type == 0)
		{
			window.draw(_str);
		}
		else
		{
			Vector2f center = getSprCenter();
			_sprite.setPosition(_x - center.x, _y - center.y);
			window.draw(_sprite);
		}
	}
}


void EffectObject::setPosition(float x, float y)
{
	_x = x;
	_y = y;
}

void EffectObject::setAlpha(float alpha)
{
	_alpha = alpha;
}

void EffectObject::setSpeed(float spd)
{
	_speed = spd;
}

void EffectObject::setTimer(float time)
{
	_timer = time;
}


void EffectObject::die()
{
	_visible = 0;
	BattleBlackboard::get()->removeEffectObjByID(_id); // Kill yourself
}

Vector2f EffectObject::getSprCenter()
{
	sf::Vector2f temp;
	float centerX = _texture.getSize().x / 2.0;
	float centerY = _texture.getSize().y / 2.0;
	temp.x = centerX;
	temp.y = centerY;
	return temp;
}


void EffectObject::createSprite(int sprType)
{
	if (_type != 1) // Return if not an effect type
	{
		return;
	}

	switch (_spriteType)
	{
	case 0:								// Hit sprite
		sf::Texture hit0Tex;
		hit0Tex.loadFromFile("images/hit0.png");
		sf::Texture hit1Tex;
		hit1Tex.loadFromFile("images/hit1.png");
		sf::Texture hit2Tex;
		hit2Tex.loadFromFile("images/hit2.png");
		sf::Texture hit3Tex;
		hit3Tex.loadFromFile("images/hit3.png");
		
		_frames.push_back(hit3Tex);
		_frames.push_back(hit2Tex);
		_frames.push_back(hit1Tex);
		_frames.push_back(hit0Tex);
		break;
	}
}