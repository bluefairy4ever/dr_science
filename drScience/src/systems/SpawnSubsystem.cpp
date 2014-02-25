
#include "systems/SpawnSubsystem.h"
#include "ResourcePath.hpp"

#include "events/Event.h"
#include "events/CreateTiledObject.h"
#include "events/EventTypes.h"

#include "object/PhysicsObject.h"
#include "object/Rooms.h"
#include "object/components/ComponentTypes.h"
#include "object/components/ControlComponent.h"
#include "object/components/SpriteComponent.h"
#include "object/components/HealthComponent.h"
#include "object/components/HealthTrigger.h"
#include "object/components/RoomTransitionTrigger.h"

#include "systems/Blackboard.h"
#include "systems/EventManager.h"

#include "utils/Constants.h"

#include "Box2D/Box2D.h"

#include <iostream>
#include <sstream>

using std::make_pair;
using std::stringstream;
using std::cout;
using std::endl;

/**
 *  Create a new PhysicsSubsystem which initializes the
 *  physics world and currently the gravity is set to zero
 */
SpawnSubsystem::SpawnSubsystem() : _index(0) 
{
}

/**
 *  Clean every thing up in the destructor.
 */
SpawnSubsystem::~SpawnSubsystem()
{

}

/**
 *  initialize the ability to spawn.  
 */
int SpawnSubsystem::initialize()
{
	EventManager::get()->registerForAll(EVENT_CREATE_TILED_OBJECT, EventListenerPtr(this));
	return SUCCESS;
}

/**
 *  Update the spawn subsystem
 *  @param elapsed -- the amount of time that elapsed since the last update.
 */
void SpawnSubsystem::update(float elapsed)
{
}

/** 
 *  @param window -- the window that we render on
 */
void SpawnSubsystem::render(sf::RenderWindow &window)
{
    
}

/**
 * 
 */
void SpawnSubsystem::finish()
{
}

/**
 *  onEvent is called when we receive play animation events
 *  @param anEvent - the event we received
 */
void SpawnSubsystem::onEvent(const EventPtr& anEvent)
{
	EventType type = anEvent.get()->getEventType();
	if (type == EVENT_CREATE_TILED_OBJECT) { 
		_index += 1;
		boost::shared_ptr<CreateTiledObject> evt = boost::static_pointer_cast<CreateTiledObject>(anEvent);
		createTiledObject(evt.get()->getObject(), evt.get()->getWorld());
	}
}

/**
 *  the method that actually constructs everything.
 *  @param object - the object containing the TMX information
 */
void SpawnSubsystem::createTiledObject(const Object& tmxObj, b2World* world) const
{    
    b2Vec2 position = b2Vec2(tmxObj.rect.left, tmxObj.rect.top);
	b2Vec2 dimensions = b2Vec2(tmxObj.rect.width, tmxObj.rect.height);

    string tmxObjType = tmxObj.type;

    if (tmxObjType == "ROOM") {
        GameObjectPtr obj = GameObjectPtr(new GameObject(_index, tmxObj.name));

        obj->setPosition(position.x, position.y);
        obj->setDimensions(dimensions.x, dimensions.y);

        Blackboard::get()->addRoom(obj);
    } else {
	    position *= WORLD_TO_BOX;

	    dimensions *= WORLD_TO_BOX*0.5f;

	    position += dimensions;
	    printf("Create Object %d %s\n", tmxObj.gid, tmxObj.name.c_str());
	    printf(" ... original %d %d ..  %d %d\n", tmxObj.rect.left, tmxObj.rect.top, tmxObj.rect.width, tmxObj.rect.height);
	    printf("Actual position: %f %f\n", position.x, position.y);
	    printf("Actual dimensions: %f %f\n", dimensions.x, dimensions.y);

	    PhysicsObjectPtr obj = PhysicsObjectPtr(new PhysicsObject(_index, tmxObj.name));
    
	    b2BodyDef bodyDef;
	    bodyDef.position.Set(position.x, position.y);
	    bodyDef.angle = tmxObj.GetPropertyFloat("angle", 0);
	    bodyDef.linearDamping = tmxObj.GetPropertyFloat("linearDamping", 1);
	    bodyDef.angularDamping = tmxObj.GetPropertyFloat("angularDamping", 1);
	    bodyDef.fixedRotation = false;
	    bodyDef.allowSleep = true;
	    bodyDef.bullet = false;
	    bodyDef.gravityScale = 1;

	    string objectType = tmxObj.GetPropertyString("object.type", "static");
	    if (objectType == "dynamic") { 
		    bodyDef.type = b2_dynamicBody;
		    bodyDef.active = true;
	    } else { 
		    bodyDef.type = b2_staticBody;
	    }

	    b2Body* body = world->CreateBody( &bodyDef );

	    b2FixtureDef fixtureDef;
	    fixtureDef.density = tmxObj.GetPropertyFloat("fixture.density", 1);
	    fixtureDef.friction = tmxObj.GetPropertyFloat("fixture.friction", 1);
	    fixtureDef.isSensor = tmxObj.GetPropertyString("fixture.isSensor", "false") == "true" ? true : false;
	    fixtureDef.restitution = tmxObj.GetPropertyFloat("fixture.restitution", 1);
	    string shapeType = tmxObj.GetPropertyString("shape.type", "rectangle");
	    cout << "  Constructing " << shapeType << " " << objectType << " " << fixtureDef.isSensor << endl;

	    b2Shape* shape;
	    if (shapeType == "circle") {
		    b2CircleShape* circle = new b2CircleShape;
		    circle->m_p.Set(0,0);
		    circle->m_radius = tmxObj.GetPropertyFloat("shape.radius", 1) * WORLD_TO_BOX;
		    cout << "  Circle Radius " << circle->m_radius << endl;
		    shape = circle;
	    } else if (shapeType == "rectangle") { 
		    b2PolygonShape* rectangle = new b2PolygonShape;
		    rectangle->SetAsBox(dimensions.x, dimensions.y);
		    shape = rectangle;
	    } else { 
		    cout << shapeType << " Not yet supported !!" << endl;
		    shape = NULL;
	    }

	    fixtureDef.shape = shape;
	    b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	    fixture->SetUserData(obj.get());
	
	    obj->setBody(body);

	    string componentStr = tmxObj.GetPropertyString("components", "");
	    stringstream ss(componentStr);
	    string itemStr;
	    while (std::getline(ss, itemStr, ',')) { 
		    if (itemStr == "control") { 
			    ControlComponent* cc = new ControlComponent(obj);
			    cc->initialize();
			    obj->addComponent(ComponentPtr(cc));
            } else if (itemStr == "gameelement") {
                Blackboard::get()->setBattery(GameObjectPtr(obj));
		    } else if (itemStr == "sprite") {
			    SpriteComponent* sc = new SpriteComponent(obj);
			    sc->initialize(tmxObj.GetPropertyString("sprite.filename",""));
			    obj->addComponent(ComponentPtr(sc));
            } else if (itemStr == "health") {
                HealthComponent* hc = new HealthComponent(obj);
                hc->initialize(tmxObj.GetPropertyString("healthbar.filename", ""),
                    tmxObj.GetPropertyString("healthbarlow.filename", ""));
                obj->addComponent(ComponentPtr(hc));
		    } else if (itemStr == "healthtrigger") { 
			    HealthTrigger* ht = new HealthTrigger(obj);
			    ht->initialize(tmxObj.GetPropertyString("sprite.filename", ""),
                    tmxObj.GetPropertyInt("health.effect", 0));
			    obj->addComponent(ComponentPtr(ht));
		    } else if (itemStr == "roomtransitiontrigger") {
                Vector2f nextPosition;
                nextPosition.x = tmxObj.GetPropertyFloat("door.nextx", 0.0f);
                nextPosition.y = tmxObj.GetPropertyFloat("door.nexty", 0.0f);
                nextPosition = nextPosition * float(WORLD_TO_BOX * TILE_HEIGHT);
                RoomTransitionTrigger* tT = new RoomTransitionTrigger(obj);
                tT->initialize(tT->getNextRoomEnum(tmxObj.name), nextPosition.x, nextPosition.y);
                obj->addComponent(ComponentPtr(tT));
            }
	    }

	    Blackboard::get()->addObject(obj);
    }
}

