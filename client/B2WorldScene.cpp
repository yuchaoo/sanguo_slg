#include "B2WorldScene.h"
#include "Util.h"
#include "GLES-Render.h"
#include "B2View.h"

B2WorldScene* B2WorldScene::create(float gravity)
{
    B2WorldScene* scene = new B2WorldScene(gravity);
    scene->autorelease();
    scene->init();
    return scene;
}

B2WorldScene::B2WorldScene(float gravity)
    :m_world(NULL)
    ,m_draw(NULL)
{
    m_world = new b2World(B2VEC(0, -gravity));
    m_draw = new GLESDebugDraw(PX_TM);
    m_world->SetDebugDraw(m_draw);
    m_world->SetContactListener(this);
}

B2WorldScene::~B2WorldScene()
{
    removeAllBodys();
    delete m_world;
    delete m_draw;
    m_draw = NULL;
    m_world = NULL;
}

bool B2WorldScene::init() 
{
    Scene::init();
    return true;
}

void B2WorldScene::onEnter()
{
    Scene::onEnter();
    this->scheduleUpdate();
}

void B2WorldScene::onExit()
{
    this->unscheduleUpdate();
    Scene::onExit();
}

void B2WorldScene::setDebugDraw(bool isDraw)
{
    if (isDraw)
    {
        m_draw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit);
    }
    else
    {
        m_draw->SetFlags(0);
    }
}

void B2WorldScene::removeAllBodys()
{
    b2Body* body = m_world->GetBodyList();
    while (body && m_world->GetBodyCount() > 0)
    {
        b2Body* t = body;
        body = body->GetNext();
        B2View* view = (B2View*)t->GetUserData();
        if (view)
        {
            view->removeBody();
        }
    }
}

void B2WorldScene::BeginContact(b2Contact* contact)
{

}

void B2WorldScene::EndContact(b2Contact* contact)
{

}

void B2WorldScene::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{

}

void B2WorldScene::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}

void B2WorldScene::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    Scene::draw(renderer, transform, flags);
    m_customCmd.init(_globalZOrder, transform, flags);
    m_customCmd.func = CC_CALLBACK_0(B2WorldScene::onDraw, this, transform, flags);
    renderer->addCommand(&m_customCmd);
}

void B2WorldScene::onDraw(const Mat4& transform, uint32_t flags)
{
    Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when setting matrix stack");
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

    GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
    m_world->DrawDebugData();
    CHECK_GL_ERROR_DEBUG();

    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void B2WorldScene::update(float dt)
{
    m_world->Step(dt,8,3);
    b2Body* body = m_world->GetBodyList();
    while (body)
    {
        B2View* view = (B2View*) body->GetUserData();
        if (view)
        {
            view->update(dt);
        }
        body = body->GetNext();
    }
}