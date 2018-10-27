#pragma once
#include "cocos2d.h"
#include "Box2D/Box2D.h"

using namespace cocos2d;

class B2View;

class B2WorldScene 
    : public Scene
    ,public b2ContactListener
{
public:
    static B2WorldScene* create(float gravity);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    virtual void update(float dt);
    void setDebugDraw(bool isDraw);
    void removeAllBodys();
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
private:
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
    void onDraw(const Mat4& transform, uint32_t flags);
protected:
    B2WorldScene(float gravity);
    ~B2WorldScene();
protected:
    b2World* m_world;
private:
    b2Draw* m_draw;
    CustomCommand m_customCmd;
};