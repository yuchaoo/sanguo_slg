#include "BollView.h"

BollView* BollView::create(PhysicsWorld* world)
{
    BollView* view = new BollView();
    view->init(world);
    view->autorelease();
    return view;
}

BollView::BollView()
    : m_raduis(20)
{

}

BollView::~BollView()
{

}

bool BollView::init(PhysicsWorld* world)
{
    Sprite* sprite = Sprite::create("CloseSelected.png");
    this->addChild(sprite);

    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = std::bind(&BollView::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
    listener->onTouchMoved = std::bind(&BollView::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
    listener->onTouchEnded = std::bind(&BollView::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
    return true;
}

bool BollView::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}
void BollView::onTouchMoved(Touch* touch, Event* event)
{

}
void BollView::onTouchEnded(Touch* touch, Event* event)
{

}