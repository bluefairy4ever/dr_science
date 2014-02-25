//
//  GameObject.h
//  drScience
//
//  Created by Wesley Kerr on 9/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef drScience_GameObject_h
#define drScience_GameObject_h

#include <string>
#include <vector>

#include "utils/Constants.h"
#include <SFML/Graphics.hpp>

using sf::Vector2f;
using std::string;
using std::vector;

class GameObject
{
public:
    GameObject(int id, string name);
    ~GameObject();
    
    int getId() { return _id; } 
    const string& getName() { return _name; } 
    
    void addComponent(ComponentPtr c);
    
    const Vector2f& getPosition() { return _position; }
    const Vector2f& getDimensions() { return _dimensions; }
    float getRotation() { return _rotation; } 
    
    void setPosition(float x, float y);
    void setDimensions(float width, float height);
    void setRotation(float rotation);

    virtual void update(float elapsed);
    void render(sf::RenderWindow &window);
    
protected:
    
private:
    int _id;
    string _name;
    
    Vector2f _position;
    Vector2f _dimensions;
    float _rotation;
    
    vector<ComponentPtr> _components;
};

#endif
