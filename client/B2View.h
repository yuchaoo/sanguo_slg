#pragma once
#include "cocos2d.h"
#include "Box2D/Box2D.h"
using namespace cocos2d;

class B2View
    :public Node
{
public:
    B2View(b2World* world);
    ~B2View();
    virtual void onEnter();
    virtual void onExit();
    void setBody(b2Body* body);
    b2Body* getBody();
    void removeBody();
    void update(float dt);
protected:
    b2World* m_world;
    b2Body* m_body;
};