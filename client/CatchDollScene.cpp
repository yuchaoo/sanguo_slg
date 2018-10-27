#include "CatchDollScene.h"
#include "ClipView.h"

CatchDollScene* CatchDollScene::create()
{
    CatchDollScene* scene = new CatchDollScene();
    scene->init();
    scene->autorelease();
    return scene;
}
CatchDollScene::CatchDollScene()
    :B2WorldScene(1.0f)
{

}

CatchDollScene::~CatchDollScene()
{

}

bool CatchDollScene::init()
{
    setDebugDraw(true);
    Size size = Director::getInstance()->getWinSize();
    Vec2 center(size.width/2,size.height/2);
    ClipView* view = ClipView::create(m_world, center.x, center.y);
    view->setPosition(center.x, center.y);
    addChild(view);
    return true;
}