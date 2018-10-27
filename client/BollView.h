#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class BollView : public Node
{
public:
    static BollView* create(PhysicsWorld* world);
    bool init(PhysicsWorld* world);
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
private:
    BollView();
    ~BollView();
private:
    
    float m_raduis;
};