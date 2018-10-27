#include "ClipView.h"
#include "Util.h"

ClipView* ClipView::create(b2World* world, float worldx, float worldy)
{
    ClipView* view = new ClipView(world);
    view->autorelease();
    view->init(worldx, worldy);
    return view;
}

ClipView::ClipView(b2World* world)
    :B2View(world)
{

}

ClipView::~ClipView()
{

}

bool ClipView::init(float worldx, float worldy)
{
    Size size1(40, 10), size2(10,120);
    Vec2 v1(worldx - size1.width / 2, worldy), v2(worldx + size1.width/2, worldy);

    b2BodyDef bd;
    bd.position.Set(CB(worldx), CB(worldy));

    b2PolygonShape shape;
    shape.SetAsBox(CB(size1.width / 2), CB(size1.height / 2));
    
    b2FixtureDef fd;
    fd.shape = &shape;

    b2Body* body = m_world->CreateBody(&bd);
    body->CreateFixture(&fd);

    b2BodyDef lbd;
    lbd.position.Set(CB(v1.x), CB(v1.y));

    b2PolygonShape shape1;
    shape1.SetAsBox(CB(size2.width / 2), CB(size2.height / 2) , B2VEC(0, -size2.height / 4), 0);// , -b2_pi / 6);
    fd.shape = &shape1;

    b2Body* leftBody = m_world->CreateBody(&lbd);
    leftBody->CreateFixture(&fd);

    Vec2 p1 = v1 + Vec2(0, -size2.height / 4);
    leftBody->SetTransform(CBV(p1), -b2_pi / 2);

    setBody(body);
    return true;
}

