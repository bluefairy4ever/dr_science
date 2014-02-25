//
//  GameObject.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_PhysicsObject_h
#define drScience_PhysicsObject_h

#include <string>
#include <vector>

#include "object/GameObject.h"
#include "utils/Constants.h"

#include <SFML/Graphics.hpp>

#include "Box2D/Box2D.h"

using sf::Vector2f;
using std::string;
using std::vector;

class PhysicsObject : public GameObject
{
public:
	PhysicsObject(int id, string name);
    ~PhysicsObject();
        
	void setBody(b2Body* body) { _body = body; }
	b2Body* getBody() { return _body; }
	
	virtual void update(float elapsed);

protected:
    
private:
	b2Body* _body;
};

#endif
