#pragma once
#include "cocos2d.h"
#include <vector>
using namespace cocos2d;

class Shape : public Node
{
public:
    static Shape* create();
    bool init();
    bool addPoint(const Vec2& v);
    int checkPoint(const Vec2& v);
    Vec2 getLastPoint();
    bool isFinish() { return m_isFinish; };
private:
    Shape();
    ~Shape();
private:
    std::vector<Vec2> m_points;
    DrawNode* m_drawNode;
    bool m_isFinish;
};

class EditShapeScene : public Scene
{
public:
    static EditShapeScene* create();
    virtual bool init();
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
    virtual void onKeyRelease(EventKeyboard::KeyCode code, Event* event);
    virtual void onMouseMove(EventMouse* event);
    virtual void onMouseDown(EventMouse* event);
    virtual void onMouseUp(EventMouse* event);
    virtual void onMouseScroll(EventMouse* event);
private:
    EditShapeScene();
    ~EditShapeScene();
private:
    DrawNode* m_drawNode;
    DrawNode* m_moveDraw;
    EventKeyboard::KeyCode m_pressCode;
    std::vector<Shape*> m_shapes;
    Shape* m_curShape;
    Vec2 m_curPoint;
};