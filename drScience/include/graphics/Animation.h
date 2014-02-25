//
//  Animation.h
//  ClassDemo
//
//  Created by Wesley Kerr on 9/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ClassDemo_Animation_h
#define ClassDemo_Animation_h


#include <SFML/Graphics.hpp>

class Animation : public sf::Sprite
{
public:
    
    enum PlayType               
    {
        NONE,
        LOOP,
        PING_PONG
    };
    
    
    Animation();
    Animation(const sf::Texture &img, int frameWidth, int frameHeight);
    
    ~Animation();
    
    sf::IntRect getFramePosition(int frame);
    
    int getFrameCount();
    
    void setPlayType(PlayType playType);
    void setFrameSize(int frameW, int frameH);
    void setFrame(int frame);
    void setLoopSpeed(float speed);
    
    void play();
    void play(int start, int end);
    void stop();

    void update(float elapsed);
    
private:
    sf::Clock _clock;
    float _speed;
    
    float _duration;
    float _nextFrame;
    
    bool _isPlaying;
    
    int _loopStart;
    int _loopEnd;

    int _currentFrame;
    int _direction;

    PlayType _playType;
    
    int _frameWidth;
    int _frameHeight;
};


#endif
