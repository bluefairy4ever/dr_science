//
//  ComponentTypes.h
//  drScience
//
//  Created by Wesley Kerr on 9/16/12.
//  Modified by Matthew Parish on 10/15/2012
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_ComponentTypes_h
#define drScience_ComponentTypes_h

#include "boost/shared_ptr.hpp"

class Component; 
typedef boost::shared_ptr<Component> ComponentPtr;

class AnimationComponent;
typedef boost::shared_ptr<AnimationComponent> AnimationComponentPtr;

class ControlComponent;
typedef boost::shared_ptr<ControlComponent> ControlComponentPtr;

class SpriteComponent;
typedef boost::shared_ptr<SpriteComponent> SpriteComponentPtr;

class DoorComponent;
typedef boost::shared_ptr<DoorComponent> DoorComponentPtr;

#endif
