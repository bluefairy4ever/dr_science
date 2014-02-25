//
//  Constants.h
//  ClassDemo
//
//  Created by Wesley Kerr on 9/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ClassDemo_Constants_h
#define ClassDemo_Constants_h

#include "boost/shared_ptr.hpp"

#include <iostream>

using std::cout;

#define SUCCESS 1
#define LOAD_ERROR 200

#define WORLD_TO_BOX 0.01
#define BOX_TO_WORLD 100

#define TILE_HEIGHT 16

#define VIEW_WIDTH 480.0f
#define VIEW_HEIGHT 272.0f
#define WINDOW_WIDTH 1024.0f
#define WINDOW_HEIGHT 600.0f
#define FRAME_LIMIT 60

#define DEG_TO_RAD 0.0174532925199432957f
#define RAD_TO_DEG 57.295779513082320876f

#define MAX_ACCELERATION 2.5f
#define MAX_SPEED 1.0f

#define MAX_ANGULAR_ACCELERATION 0.08f
#define MAX_ROTATION 15

#define GAME_OVER_PATH "images/gameOver.png"
#define YOU_WIN_PATH "images/youWin.png"

#define MENU_THEME_PATH "audio/themes/MonsterPlanet.wav"
#define GAME_THEME_PATH "audio/themes/TypeYourText.wav"
#define BATTLE_THEME_PATH "audio/themes/LandOfTheGhosts.wav"

#define ATTACK_SOUND_PATH "audio/effects/attack.wav"
#define ENEMY_ATTACK_SOUND_PATH "audio/effects/pierce.wav"
#define ENEMY_DEATH_SOUND_PATH "audio/effects/EnemyDeathLong.wav"
#define DEFEND_SOUND_PATH "audio/effects/defendOn.wav"
#define HEALTHPACK_SOUND_PATH "audio/effects/Health1.wav"

#define CURSOR_SOUND_PATH "audio/effects/cursor.wav"
#define CANCEL_SOUND_PATH "audio/effects/cancel.wav"

class GameObject;
typedef boost::shared_ptr<GameObject> GameObjectPtr;

class PhysicsObject;
typedef boost::shared_ptr<PhysicsObject> PhysicsObjectPtr;

class Component; 
typedef boost::shared_ptr<Component> ComponentPtr;

class Event;
typedef boost::shared_ptr<Event> EventPtr;

class EventListener;
typedef boost::shared_ptr<EventListener> EventListenerPtr;

class Animation;
typedef boost::shared_ptr<Animation> AnimationPtr;

class Screen;
typedef boost::shared_ptr<Screen> ScreenPtr;

class Subsystem;
typedef boost::shared_ptr<Subsystem> SubsystemPtr;

#endif
