#include "EditShapeScene.h"

Shape* Shape::create() 
{
    Shape* shape = new Shape();
    shape->autorelease();
    shape->init();
    return shape;
}

Shape::Shape()
    :m_isFinish(false)
{

}

Shape::~Shape()
{

}

bool Shape::init()
{
    m_drawNode = DrawNode::create(1.0f);
    addChild(m_drawNode);
    m_drawNode->drawLine(Vec2(0, 100), Vec2(0, 100), Color4F(1, 1, 0, 1));
    m_drawNode->drawCircle(Vec2(100, 100), 50, 10, 0, true, Color4F(1, 1, 0, 1));
    return true;
}

bool Shape::addPoint(const Vec2& v)
{
    if (m_isFinish)
    {
        return false;
    }

    int index = this->checkPoint(v);
    if (index < 0)
    {
        m_points.push_back(v);
        m_drawNode->drawDot(v, 4, Color4F(1, 0, 0, 1));
        if (m_points.size() > 1)
        {
            m_drawNode->drawLine(m_points.back(), v, Color4F(1, 0, 0, 1));
        }
        return true;
    }
    else if(index == 0)
    {
        m_isFinish = true;
        m_drawNode->drawLine(m_points.back(), v, Color4F(1, 0, 0, 1));
    }
    return false;
}

int Shape::checkPoint(const Vec2& v)
{
    for (size_t i = 0; i < m_points.size(); ++i)
    {
        const Vec2 p = m_points[i];
        if (fabs(v.x - p.x) < 4 && fabs(v.y - p.y) < 4)
            return i;
    }
    return -1;
}

Vec2 Shape::getLastPoint()
{
    if (m_points.size() > 0)
        return m_points.back();
    return Vec2(0,0);
}

/*****************************************************************/

EditShapeScene* EditShapeScene::create()
{
    EditShapeScene* scene = new EditShapeScene();
    scene->autorelease();
    scene->init();
    return scene;
}

EditShapeScene::EditShapeScene()
    :m_pressCode(EventKeyboard::KeyCode::KEY_NONE)
    , m_curShape(NULL)
{

}

EditShapeScene::~EditShapeScene()
{
 
}

bool EditShapeScene::init()
{
    Size size = Director::getInstance()->getWinSize();
    m_drawNode = DrawNode::create(1.0f);
    addChild(m_drawNode);

    m_drawNode->drawLine(Vec2(0, size.height / 2), Vec2(size.width, size.height / 2),Color4F(1,0,0,0.5));
    m_drawNode->drawLine(Vec2(size.width / 2, 0), Vec2(size.width / 2, size.height), Color4F(1, 0, 0, 0.5));
    
    float t = size.width / 2, interval = 20;
    while (t > 0)
    {
        m_drawNode->drawDot(Vec2(t, size.height / 2), 2, Color4F(1, 0, 0, 0.5));
        t -= interval;
    }
    t = size.width / 2;
    while (t < size.width)
    {
        m_drawNode->drawDot(Vec2(t, size.height / 2), 2, Color4F(1, 0, 0, 0.5));
        t += interval;
    }
    t = size.height / 2;
    while (t > 0)
    {
        m_drawNode->drawDot(Vec2(size.width / 2, t), 2, Color4F(1, 0, 0, 0.5));
        t -= interval;
    }
    t = size.height / 2;
    while (t < size.height)
    {
        m_drawNode->drawDot(Vec2(size.width / 2, t), 2, Color4F(1, 0, 0, 0.5));
        t += interval;
    }

    m_moveDraw = DrawNode::create(1);
    addChild(m_moveDraw);

    auto mouselistener = EventListenerMouse::create();
    mouselistener->onMouseMove = std::bind(&EditShapeScene::onMouseMove, this, std::placeholders::_1);
    mouselistener->onMouseDown = std::bind(&EditShapeScene::onMouseDown, this, std::placeholders::_1);
    mouselistener->onMouseScroll = std::bind(&EditShapeScene::onMouseScroll, this, std::placeholders::_1);
    mouselistener->onMouseUp = std::bind(&EditShapeScene::onMouseUp, this, std::placeholders::_1);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouselistener, this);

    auto keyboard = EventListenerKeyboard::create();
    keyboard->onKeyPressed = std::bind(&EditShapeScene::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
    keyboard->onKeyReleased = std::bind(&EditShapeScene::onKeyRelease, this, std::placeholders::_1, std::placeholders::_2);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard, this);

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = std::bind(&EditShapeScene::onTouchBegan,this,std::placeholders::_1,std::placeholders::_2);
    listener->onTouchMoved = std::bind(&EditShapeScene::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
    listener->onTouchEnded = std::bind(&EditShapeScene::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

bool EditShapeScene::onTouchBegan(Touch* touch, Event* event)
{
    if (m_pressCode == EventKeyboard::KeyCode::KEY_CTRL && m_curShape)
    {

        return true;
    }
    return false;
}

void EditShapeScene::onTouchMoved(Touch* touch, Event* event)
{

}

void EditShapeScene::onTouchEnded(Touch* touch, Event* event)
{

}

void EditShapeScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
    m_pressCode = code;
    if (m_pressCode == EventKeyboard::KeyCode::KEY_CTRL)
    {
        m_curShape = Shape::create();
        addChild(m_curShape);
    }
}

void EditShapeScene::onKeyRelease(EventKeyboard::KeyCode code, Event* event)
{
    if (code == m_pressCode)
    {
        if (m_curShape->isFinish())
        {
            m_shapes.push_back(m_curShape);
        }
        else
        {
            m_curShape->removeFromParent();
            m_curShape = NULL;
        }
        m_curPoint.setZero();
    }
}

void EditShapeScene::onMouseMove(EventMouse* event)
{
    m_moveDraw->clear();
    if (!m_curPoint.isZero())
    {
        m_moveDraw->drawLine(m_curPoint, event->getLocationInView(), Color4F(0, 1, 0, 1));
    }
}

void EditShapeScene::onMouseDown(EventMouse* event)
{
    if (m_pressCode == EventKeyboard::KeyCode::KEY_CTRL && m_curShape)
    {
        if (m_curShape->addPoint(event->getLocationInView()))
        {
            m_curPoint = event->getLocationInView();
        }
        else if (m_curShape->isFinish())
        {
            m_curPoint.setZero();
        }
    }
}

void EditShapeScene::onMouseUp(EventMouse* event)
{
    
}

void EditShapeScene::onMouseScroll(EventMouse* event)
{

}

