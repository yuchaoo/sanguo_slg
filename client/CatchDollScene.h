#pragma once

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "B2WorldScene.h"
using namespace cocos2d;

class CatchDollScene : public B2WorldScene
{
public:
    static CatchDollScene* create();
    virtual bool init();
private:
    CatchDollScene();
    ~CatchDollScene();
private:

};