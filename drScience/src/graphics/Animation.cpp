//
//  Animation.cpp
//  ClassDemo
//
//  Created by Wesley Kerr on 9/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
using std::cout;
using std::endl;

#include "graphics/Animation.h"

Animation::Animation(void) : sf::Sprite()
{
    _currentFrame = 0;
    _loopStart = 0;
    _isPlaying = false;
    _direction = 1;
        
    setLoopSpeed(1.0);
    setPlayType(NONE);
    setFrameSize(0, 0);
}

Animation::Animation(const sf::Texture &img, int frameWidth, int frameHeight) : sf::Sprite(img)
{
    _currentFrame = 0;
    _loopStart = 0;
    _isPlaying = false;
    _direction = 1;
    
    setLoopSpeed(1.0);
    setFrameSize(frameWidth, frameHeight);
}

Animation::~Animation() 
{     

}

void Animation::setPlayType(PlayType playType)
{
    _playType = playType;
}

int Animation::getFrameCount()
{
    unsigned int horizontal = (this->getTexture()->getSize().x / _frameWidth);
    unsigned int vertical = (this->getTexture()->getSize().y / _frameHeight);
    
    return horizontal * vertical;
}

sf::IntRect Animation::getFramePosition(int frame)
{
    unsigned int horizontal = (this->getTexture()->getSize().x / _frameWidth);
    unsigned int vertical = (this->getTexture()->getSize().y / _frameHeight);
    
    int tileY = frame / horizontal;
    int tileX = frame % horizontal;
    
    sf::IntRect result(
                       tileX * _frameWidth,
                       tileY * _frameHeight,
                       _frameWidth,
                       _frameHeight);
    
    return result;
}

void Animation::setFrameSize(int frameW, int frameH)
{
    _frameWidth = frameW;
    _frameHeight = frameH;
    
    setTextureRect(sf::IntRect(0, 0, frameW, frameH));
}

void Animation::setFrame(int frame) 
{ 
    _currentFrame = frame; 
}

void Animation::setLoopSpeed(float speed) 
{ 
    _speed = speed; 
}

void Animation::play() 
{ 
    play(0, getFrameCount()); 
}

void Animation::play(int start, int end)
{
    _loopStart = start;
    _loopEnd = end;
    _currentFrame = start;
    
    _isPlaying = true;
    
    _clock.restart();
    
    int frameCount = (_loopEnd + 1) - _loopStart;
    _duration = _speed / frameCount;
    _nextFrame = _duration;   
    
//    cout << "Duration " << _duration << " Next Frame " << _nextFrame << endl;
//    cout << "Current Frame " << _currentFrame << endl;
}

void Animation::stop() 
{ 
    _isPlaying = false; 
}

void Animation::update(float elapsed)
{
    if(_isPlaying)
    {
//        cout << " Elapsed " << _clock.getElapsedTime().asSeconds() << endl;
        _nextFrame -= elapsed;
        while (_nextFrame < 0 && _isPlaying)
        {
            // at this point we need to transition to the next image
            _currentFrame += _direction;
            _nextFrame += _duration;
            
//            cout << " Transition Current Frame " << _currentFrame << " NextFrame " << _nextFrame << endl;
            
            if (_currentFrame < _loopStart) 
            {
                if (_playType == NONE) { 
                    stop();
                    _currentFrame = _loopStart;
                } else if (_playType == LOOP) { 
                    _currentFrame = _loopEnd;
                } else if (_playType == PING_PONG) {
                    _currentFrame = _loopStart + 1;
                    _direction *= -1;
                }
//                cout << "   Swapping Directions (bottom) " << _direction << endl;
            }
            
            if (_currentFrame > _loopEnd)
            {
                if (_playType == NONE) { 
                    stop();
                    _currentFrame = _loopEnd;
                } else if (_playType == LOOP) { 
                    _currentFrame = _loopStart;
                } else if (_playType == PING_PONG) {
                    _currentFrame = _loopEnd - 1;
                    _direction *= -1;
                }
//                cout << "   Swapping Directions (top) " << _direction << " _loopEnd " << _loopEnd << " _currentFrame " << _currentFrame << endl;
            }
        }
        setTextureRect(getFramePosition(_currentFrame));
    }
}