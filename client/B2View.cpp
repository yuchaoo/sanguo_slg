#include "B2View.h"
#include "Util.h"

B2View::B2View(b2World* world)
    :m_world(world)
    ,m_body(NULL)
{

}

B2View::~B2View()
{
    if (m_body)
    {
        m_world->DestroyBody(m_body);
        m_body = NULL;
    }
    m_world = NULL;
}

void B2View::onEnter() 
{
    Node::onEnter();
    if (m_body)
    {
        m_body->SetAwake(true);
    }
}

void B2View::onExit()
{
    Node::onExit();
    if (m_body)
    {
        m_body->SetActive(false);
    }
}

void B2View::setBody(b2Body* body)
{
    m_body = body;
    m_body->SetSleepingAllowed(true);
    m_body->SetUserData(this);
}

b2Body* B2View::getBody() 
{
    return m_body;
}

void B2View::removeBody()
{
    m_world->DestroyBody(m_body);
    m_body = NULL;
}

void B2View::update(float dt)
{
    if (m_body && m_body->GetType() == b2_dynamicBody && m_body->IsActive() && m_body->IsAwake())
    {
        if (!m_body->IsFixedRotation())
        {
            float angle = m_body->GetAngle();
            this->setRotation(-CC_RADIANS_TO_DEGREES(angle));
        }
        const b2Vec2& pos = m_body->GetPosition();
        Vec2 p = getParent()->convertToNodeSpace(BCV(pos));
        this->setPosition(p);
    }
}