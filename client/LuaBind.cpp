#include "cocos2d.h"
#include "LuaBind.h"
#include "LuaWrapper.h"
#include "CCDirector.h"
using namespace cocos2d;

/*******************Director*************************/

int lua_cocos2dx_director_getRunningScene(lua_State* L)
{
	Director* ptr = Lua_GetPointer<Director>(L, 1);
	if (!ptr) return 0;
	return Lua_SetRef(L, ptr->getRunningScene());
}

int lua_cocos2dx_director_getAnimationInterval(lua_State* L)
{
	Director* ptr = Lua_GetPointer<Director>(L, 1);
	if (!ptr) return 0;
	return Lua_SetFloat(L,ptr->getAnimationInterval());
}

int lua_cocos2dx_director_setAnimationInterval(lua_State* L)
{
	Director* ptr = Lua_GetPointer<Director>(L, 1);
	if (!ptr) return 0;
	ptr->setAnimationInterval(Lua_GetFloat(L, 2));
	return 0;
}

int lua_cocos2dx_director_isDisplayStats(lua_State* L)
{
	Director* ptr = Lua_GetPointer<Director>(L,1);
	if (!ptr) return 0;
	return Lua_SetBool(L,ptr->isDisplayStats());
}

int lua_cocos2dx_director_setDisplayStats(lua_State* L)
{
	Director* ptr = Lua_GetPointer<Director>(L, 1);
	if (!ptr) return 0;
	ptr->setDisplayStats(Lua_GetBool(L, 2));
	return 0;
}

int lua_cocos2dx_director_getSecondsPerFrame(lua_State* L)
{
	Director* ptr = Lua_GetPointer<Director>(L, 1);
	if (!ptr) return 0;
	return Lua_SetFloat(L, ptr->getSecondsPerFrame());
}

int lua_cocos2dx_director_getOpenGLView(lua_State* L)
{
	Director* ptr = Lua_GetPointer<Director>(L, 1);
	if (!ptr) return 0;
	return Lua_SetRef(L, ptr->getOpenGLView());
}

int lua_cocos2dx_director_setOpenGLView(lua_State* L)
{
	Director* ptr = Lua_GetPointer<Director>(L, 1);
	if (!ptr) return 0;
	ptr->setOpenGLView(Lua_GetPointer<GLView>(L, 2));
	return 0;
}

int lua_cocos2dx_director_getTextureCache(lua_State* L)
{
	Director* ptr = Lua_GetPointer<Director>(L, 1);
	if (!ptr) return 0;
	return Lua_SetRef(L, ptr->getTextureCache());
}

int lua_open_cocos2dx_director_module(lua_State* L)
{
	luaL_Reg reg[] = {
		{"getRunningScene",lua_cocos2dx_director_getRunningScene },
		{"getAnimationInterval",lua_cocos2dx_director_getAnimationInterval },
		{"setAnimationInterval",lua_cocos2dx_director_setAnimationInterval },
		{"isDisplayStats",lua_cocos2dx_director_isDisplayStats },
		{"setDisplayStats",lua_cocos2dx_director_setDisplayStats },
		{"getSecondsPerFrame",lua_cocos2dx_director_getSecondsPerFrame },
		{"getOpenGLView",lua_cocos2dx_director_getOpenGLView },
		{"setOpenGLView",lua_cocos2dx_director_setOpenGLView },
		{"getTextureCache",lua_cocos2dx_director_getTextureCache },
		{NULL,NULL}
	};
	return Lua_CreateModule(L, "Director", NULL, reg, Director::getInstance());
}

/*******************Ref***************************/
int lua_cocos2dx_ref_retain(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->retain();
	return 0;
}
int lua_cocos2dx_ref_release(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->release();
	return 0;
}
int lua_cocos2dx_ref_isNull(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	return Lua_SetBool(L,ptr == NULL);
}
int lua_open_cocos2dx_ref_module(lua_State* L)
{
	luaL_Reg reg[] = {
		{ "retain", lua_cocos2dx_ref_retain },
		{ "release", lua_cocos2dx_ref_release },
		{ "isNull", lua_cocos2dx_ref_isNull },
		{NULL,NULL}
	};
	Lua_CreateModule(L, "Ref", NULL, reg);
	return 1;
}
/*******************Node***********************/
int lua_cocos2dx_node_setLocalZOrder(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	ptr->setLocalZOrder(Lua_GetInt(L, 2));
	return 0;
}
int lua_cocos2dx_node_getLocalZOrder(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetInt(L, ptr->getLocalZOrder());
}
int lua_cocos2dx_node_setScaleX(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->setScaleX(Lua_GetFloat(L,2));
	return 0;
}
int lua_cocos2dx_node_getScaleX(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	return Lua_SetFloat(L, ptr->getScaleX());
}
int lua_cocos2dx_node_setScaleY(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->setScaleY(Lua_GetFloat(L, 2));
	return 0;
}
int lua_cocos2dx_node_getScaleY(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetFloat(L, ptr->getScaleY());
}
int lua_cocos2dx_node_setScale(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->setScale(Lua_GetFloat(L, 2));
	return 0;
}
int lua_cocos2dx_node_getScale(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetFloat(L, ptr->getScale());
}
int lua_cocos2dx_node_setPosition(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	ptr->setPosition(Lua_GetVec2(L, 2));
	return 0;
}
int lua_cocos2dx_node_getPosition(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetVec2(L,ptr->getPosition());
}
int lua_cocos2dx_node_setPositionX(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->setPositionX(Lua_GetFloat(L,2));
	return 0;
}
int lua_cocos2dx_node_getPositionX(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetFloat(L, ptr->getPositionX());
}
int lua_cocos2dx_node_setPositionY(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->setPositionY(Lua_GetFloat(L, 2));
	return 0;
}
int lua_cocos2dx_node_getPositionY(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetFloat(L, ptr->getPositionY());
}
int lua_cocos2dx_node_setSkewX(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->setSkewX(Lua_GetFloat(L, 2));
	return 0;
}
int lua_cocos2dx_node_getSkewX(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetFloat(L, ptr->getSkewX());
}
int lua_cocos2dx_node_setSkewY(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->setSkewY(Lua_GetFloat(L, 2));
	return 0;
}
int lua_coco2dx_node_getSkewY(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetFloat(L, ptr->getSkewY());
}
int lua_cocos2dx_node_setAnchorPoint(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->setAnchorPoint(Lua_GetVec2(L, 2));
	return 0;
}
int lua_cocos2dx_node_getAnchorPoint(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetVec2(L, ptr->getAnchorPoint());
}
int lua_cocos2dx_node_setContentSize(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->setContentSize(Lua_GetSize(L, 2));
	return 0;
}
int lua_cocos2dx_node_getContentSize(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetSize(L, ptr->getContentSize());
}
int lua_cocos2dx_node_setVisible(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->setVisible(Lua_GetBool(L, 2));
	return 0;
}
int lua_cocos2dx_node_isVisible(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetBool(L, ptr->isVisible());
}
int lua_cocos2dx_node_setRotation(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->setRotation(Lua_GetFloat(L, 2));
	return 0;
}
int lua_cocos2dx_node_getRotation(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetFloat(L, ptr->getRotation());
}
int lua_cocos2dx_node_setIgnoreAnchorPointForPosition(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->setIgnoreAnchorPointForPosition(Lua_GetBool(L, 2));
	return 0;
}
int lua_cocos2dx_node_isIgnoreAnchorPointForPosition(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetFloat(L, ptr->isIgnoreAnchorPointForPosition());
}

int lua_cocos2dx_node_addChild(lua_State* L)
{
	Node* node = Lua_GetPointer<Node>(L, 1);
	Node* child = Lua_GetPointer<Node>(L, 2);
	if (!node || !child) return 0;
	int n = lua_gettop(L);
	if (n == 2)
		node->addChild(child);
	else if (n == 3)
	{
		int zorder = Lua_GetInt(L, 3);
		node->addChild(child, zorder);
	}
	else if (n == 4)
	{
		int zorder = Lua_GetInt(L, 3);
		if (lua_isstring(L, 4))
			node->addChild(child, zorder, lua_tostring(L, 4));
		else if (lua_isnumber(L, 4))
			node->addChild(child, zorder, lua_tointeger(L, 4));
		else
		{
			assert(false);
		}
	}
	return 0;
}

int lua_cocos2dx_node_getChildByTag(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	Node* child = ptr->getChildByTag(Lua_GetInt(L, 2));
	if (!child) return 0;
	lua_pushinteger(L, child->_luaID);
	lua_rawget(L, LUA_REGISTRYINDEX);
	return 1;
}

int lua_cocos2dx_node_getChildByName(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	Node* child = ptr->getChildByName(Lua_GetString(L, 2));
	if (!child) return 0;
	lua_pushinteger(L, child->_luaID);
	lua_rawget(L, LUA_REGISTRYINDEX);
	return 1;
}

int lua_cocos2dx_node_getChildren(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	const cocos2d::Vector<cocos2d::Node*>& children = ptr->getChildren();
	return Lua_SetRefVector(L, children);
}
int lua_cocos2dx_node_getChildrenCount(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetInt(L, ptr->getChildrenCount());
}
int lua_cocos2dx_node_getParent(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	Node* parent = ptr->getParent();
	if (!parent) return 0;
	lua_rawgeti(L, LUA_REGISTRYINDEX,parent->_luaID);
	return 1;
}
int lua_cocos2dx_node_removeFromParent(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->removeFromParent();
	return 0;
}
int lua_cocos2dx_node_removeChild(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	Node* child = Lua_GetPointer<Node>(L,2);
	if (!child) return 0;
	ptr->removeChild(child);
	return 0;
}
int lua_cocos2dx_node_removeChildByTag(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	ptr->removeChildByTag(Lua_GetInt(L, 2));
	return 0;
}
int lua_cocos2dx_node_removeChildByName(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	ptr->removeChildByName(Lua_GetString(L, 2));
	return 0;
}
int lua_cocos2dx_node_removeAllChildren(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	ptr->removeAllChildren();
	return 0;
}
int lua_cocos2dx_node_getTag(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	return Lua_SetInt(L,ptr->getTag());
}
int lua_cocos2dx_node_setTag(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (ptr) return 0;
	ptr->setTag(Lua_GetInt(L, 2));
	return 0;
}
int lua_cocos2dx_node_getName(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	return Lua_SetString(L,ptr->getName());
}
int lua_cocos2dx_node_setName(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	ptr->setName(Lua_GetString(L, 2));
	return 0;
}
int lua_cocos2dx_node_getGLProgram(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	Ref* ref = ptr->getGLProgram();
	if (!ref) return 0;
	lua_pushinteger(L, ref->_luaID);
	lua_rawget(L, LUA_REGISTRYINDEX);
	return 1;
}
int lua_cocos2dx_node_setGLProgram(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	GLProgram* glp = Lua_GetPointer<GLProgram>(L,2);
	if (!glp) return 0;
	ptr->setGLProgram(glp);
	return 0;
}
int lua_cocos2dx_node_getGLProgramState(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	return Lua_SetRef(L, ptr->getGLProgramState());
}
int lua_cocos2dx_node_setGLProgramState(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	cocos2d::GLProgramState* state = Lua_GetPointer<cocos2d::GLProgramState>(L, 2);
	if (!ptr || !state) return 0;
	ptr->setGLProgramState(state);
	return 0;
}
int lua_cocos2dx_node_isRunning(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetBool(L, ptr->isRunning());
}
int lua_cocos2dx_node_onEnter(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	ptr->onEnter();
	return 0;
}
int lua_cocos2dx_node_onExit(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	ptr->onExit();
	return 0;
}
int lua_cocos2dx_node_getScene(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	return Lua_SetRef(L, ptr->getScene());
}
int lua_cocos2dx_node_getBoundingBox(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	return Lua_SetRect(L, ptr->getBoundingBox());
}
int lua_cocos2dx_node_runAction(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	Action* action = Lua_GetPointer<Action>(L, 2);
	if (!ptr || !action) return 0;
	ptr->runAction(action);
	return 0;
}
int lua_cocos2dx_node_stopAllActions(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	ptr->stopAllActions();
	return 0;
}
int lua_cocos2dx_node_stopAction(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	Action* action = Lua_GetPointer<Action>(L, 2);
	if (!ptr || !action) return 0;
	ptr->stopAction(action);
	return 0;
}
int lua_cocos2dx_node_stopActionByTag(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	ptr->stopActionByTag(Lua_GetInt(L, 2));
	return 0;
}
int lua_cocos2dx_node_getActionByTag(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	return Lua_SetRef(L,ptr->getActionByTag(Lua_GetInt(L, 2)));
}
int lua_cocos2dx_node_scheduleUpdate(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L, 1);
	if (!ptr) return 0;
	ptr->scheduleUpdate();
	return 0;
}
int lua_cocos2dx_node_unscheduleUpdate(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	ptr->unscheduleUpdate();
	return 0;
}
int lua_cocos2dx_node_convertToNodeSpace(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	return Lua_SetVec2(L,ptr->convertToNodeSpace(std::move(Lua_GetVec2(L, 2))));
}
int lua_cocos2dx_node_convertToWorldSpace(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	return Lua_SetVec2(L, ptr->convertToWorldSpace(std::move(Lua_GetVec2(L, 2))));
}
int lua_cocos2dx_node_convertTouchToNodeSpace(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	Touch* touch = Lua_GetPointer<Touch>(L, 2);
	if (!ptr || !touch) return 0;
	return Lua_SetVec2(L, ptr->convertTouchToNodeSpace(touch));
}
int lua_cocos2dx_node_getOpacity(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	return Lua_SetInt(L,ptr->getOpacity());
}
int lua_cocos2dx_node_setOpacity(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	ptr->setOpacity(Lua_GetInt(L, 2));
	return 0;
}
int lua_cocos2dx_node_isCascadeOpacityEnabled(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	return Lua_SetBool(L, ptr->isCascadeOpacityEnabled());
}
int lua_cocos2dx_node_setCascadeOpacityEnabled(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	ptr->setCascadeOpacityEnabled(Lua_GetBool(L, 2));
	return 0;
}
int lua_cocos2dx_node_getColor(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	return Lua_SetColor3B(L, ptr->getColor());
}
int lua_cocos2dx_node_isCascadeColorEnabled(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	return Lua_SetBool(L, ptr->isCascadeColorEnabled());
}
int lua_cocos2dx_node_setCascadeColorEnabled(lua_State* L)
{
	Node* ptr = Lua_GetPointer<Node>(L,1);
	if (!ptr) return 0;
	ptr->setCascadeColorEnabled(Lua_GetBool(L, 2));
	return 0;
}
int lua_cocos2dx_node_create(lua_State* L)
{
	Node* node = Node::create();
	return Lua_CreateRef(L, "Node", node);
}

int lua_open_cocos2dx_node_module(lua_State* L)
{
	luaL_Reg reg[] = {
		{ "setLocalZOrder", lua_cocos2dx_node_setLocalZOrder },
		{ "getLocalZOrder", lua_cocos2dx_node_getLocalZOrder },
		{ "setScaleX", lua_cocos2dx_node_setScaleX },
		{ "getScaleX", lua_cocos2dx_node_getScaleX },
		{ "setScaleY", lua_cocos2dx_node_setScaleY },
		{ "getScaleY", lua_cocos2dx_node_getScaleY },
		{ "setScale", lua_cocos2dx_node_setScale },
		{ "getScale", lua_cocos2dx_node_getScale },
		{ "setPosition", lua_cocos2dx_node_setPosition },
		{ "getPosition", lua_cocos2dx_node_getPosition },
		{ "setPositionX", lua_cocos2dx_node_setPositionX },
		{ "getPositionX", lua_cocos2dx_node_getPositionX },
		{ "setPositionY", lua_cocos2dx_node_setPositionY },
		{ "getPositionY", lua_cocos2dx_node_getPositionY },
		{ "setSkewX", lua_cocos2dx_node_setSkewX },
		{ "getSkewX", lua_cocos2dx_node_getSkewX },
		{ "setSkewY", lua_cocos2dx_node_setSkewY },
		{ "getSkewY", lua_coco2dx_node_getSkewY },
		{ "setAnchorPoint", lua_cocos2dx_node_setAnchorPoint },
		{ "getAnchorPoint", lua_cocos2dx_node_getAnchorPoint },
		{ "setContentSize", lua_cocos2dx_node_setContentSize },
		{ "getContentSize", lua_cocos2dx_node_getContentSize },
		{ "setVisible", lua_cocos2dx_node_setVisible },
		{ "isVisible", lua_cocos2dx_node_isVisible },
		{ "setRotation", lua_cocos2dx_node_setRotation },
		{ "getRotation", lua_cocos2dx_node_getRotation },
		{ "setIgnoreAnchorPointForPosition", lua_cocos2dx_node_setIgnoreAnchorPointForPosition },
		{ "isIgnoreAnchorPointForPosition", lua_cocos2dx_node_isIgnoreAnchorPointForPosition },
		{ "addChild", lua_cocos2dx_node_addChild },
		{ "getChildByTag", lua_cocos2dx_node_getChildByTag },
		{ "getChildByName", lua_cocos2dx_node_getChildByName },
		{ "getChildren", lua_cocos2dx_node_getChildren },
		{ "getChildrenCount", lua_cocos2dx_node_getChildrenCount },
		{ "getParent", lua_cocos2dx_node_getParent },
		{ "removeFromParent", lua_cocos2dx_node_removeFromParent },
		{ "removeChild", lua_cocos2dx_node_removeChild },
		{ "removeChildByTag", lua_cocos2dx_node_removeChildByTag },
		{ "removeChildByName", lua_cocos2dx_node_removeChildByName },
		{ "removeAllChildren", lua_cocos2dx_node_removeAllChildren },
		{ "getTag", lua_cocos2dx_node_getTag },
		{ "setTag", lua_cocos2dx_node_setTag },
		{ "getName", lua_cocos2dx_node_getName },
		{ "setName", lua_cocos2dx_node_setName },
		{ "getGLProgram", lua_cocos2dx_node_getGLProgram },
		{ "setGLProgram", lua_cocos2dx_node_setGLProgram },
		{ "getGLProgramState", lua_cocos2dx_node_getGLProgramState },
		{ "setGLProgramState", lua_cocos2dx_node_setGLProgramState },
		{ "isRunning", lua_cocos2dx_node_isRunning },
		{ "onEnter", lua_cocos2dx_node_onEnter },
		{ "onExit", lua_cocos2dx_node_onExit },
		{ "getScene", lua_cocos2dx_node_getScene },
		{ "getBoundingBox", lua_cocos2dx_node_getBoundingBox },
		{ "runAction", lua_cocos2dx_node_runAction },
		{ "stopAllActions", lua_cocos2dx_node_stopAllActions },
		{ "stopAction", lua_cocos2dx_node_stopAction },
		{ "stopActionByTag", lua_cocos2dx_node_stopActionByTag },
		{ "getActionByTag", lua_cocos2dx_node_getActionByTag },
		{ "scheduleUpdate", lua_cocos2dx_node_scheduleUpdate },
		{ "unscheduleUpdate", lua_cocos2dx_node_unscheduleUpdate },
		{ "convertToNodeSpace", lua_cocos2dx_node_convertToNodeSpace },
		{ "convertToWorldSpace", lua_cocos2dx_node_convertToWorldSpace },
		{ "convertTouchToNodeSpace", lua_cocos2dx_node_convertTouchToNodeSpace },
		{ "getOpacity", lua_cocos2dx_node_getOpacity },
		{ "setOpacity", lua_cocos2dx_node_setOpacity },
		{ "isCascadeOpacityEnabled", lua_cocos2dx_node_isCascadeOpacityEnabled },
		{ "setCascadeOpacityEnabled", lua_cocos2dx_node_setCascadeOpacityEnabled },
		{ "getColor", lua_cocos2dx_node_getColor },
		{ "isCascadeColorEnabled", lua_cocos2dx_node_isCascadeColorEnabled },
		{ "setCascadeColorEnabled", lua_cocos2dx_node_setCascadeColorEnabled },
		{ "create", lua_cocos2dx_node_create },
		{NULL,NULL}
	};
	Lua_CreateModule(L, "Node", "Ref", reg);
	return 0;
}

/***************************Texture2D*******************************/

int lua_cocos2dx_Texture2D_setDefaultAlphaPixelFormat(lua_State* L)
{
	Texture2D::setDefaultAlphaPixelFormat( (Texture2D::PixelFormat) Lua_GetInt(L, 2));
	return 0;
}
int lua_cocos2dx_Texture2D_getDefaultAlphaPixelFormat(lua_State* L)
{
	return Lua_SetInt(L,(int) Texture2D::getDefaultAlphaPixelFormat());
}
int lua_cocos2dx_Texture2D_getDescription(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	return Lua_SetString(L, texture->getDescription());
}
int lua_cocos2dx_Texture2D_releaseGLTexture(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	texture->releaseGLTexture();
	return 0;
}
int lua_cocos2dx_Texture2D_drawAtPoint(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	texture->drawAtPoint(Lua_GetVec2(L, 2));
	return 0;
}
int lua_cocos2dx_Texture2D_drawInRect(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	texture->drawInRect(Lua_GetRect(L, 2));
	return 0;
}
int lua_cocos2dx_Texture2D_initWithImage(lua_State* L)
{
	int top = lua_gettop(L);
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	Image* image = Lua_GetPointer<Image>(L, 2);
	if (!image) return 0;
	if (top == 2)
	{
		texture->initWithImage(image);
	}
	else if (top == 3)
	{
		texture->initWithImage(image, (Texture2D::PixelFormat) Lua_GetInt(L, 3));
	}
	return 0;
}
int lua_cocos2dx_Texture2D_initWithString(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	const char* text = Lua_GetString(L, 2);
	if (lua_istable(L, 3))
	{
		texture->initWithString(text, Lua_GetFontDefinition(L, 3));
		return Lua_CreateRef(L, "Texture2D", texture);
	}
	else if(lua_isstring(L,3))
	{
		const char* fontName = Lua_GetString(L, 3);
		float fontSize = Lua_GetFloat(L, 4);
		Size dimensions(0, 0);
		if (lua_istable(L,5))
		{
			dimensions = Lua_GetTSize(L, 5);
		}
		TextHAlignment hAlignment = TextHAlignment::CENTER;
		if (lua_isnumber(L, 6))
		{
			hAlignment = (TextHAlignment)Lua_GetInt(L, 6);
		}
		TextVAlignment vAlignment = TextVAlignment::TOP;
		if (lua_isnumber(L, 7))
		{
			vAlignment = (TextVAlignment)Lua_GetInt(L, 7);
		}
		bool enableWrap = true;
		if (lua_isboolean(L,8))
		{
			enableWrap = Lua_GetBool(L, 8);
		}
		int overflow = 0;
		if (lua_isnumber(L, 9))
		{
			overflow = Lua_GetInt(L, 9);
		}
		texture->initWithString(text, fontName, fontSize, dimensions, hAlignment, vAlignment, enableWrap, overflow);
		return Lua_CreateRef(L, "Texture2D", texture);
	}
	return 0;
}

int lua_cocos2dx_Texture2D_getStringForFormat(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	return Lua_SetString(L, texture->getStringForFormat());
}

int lua_cocos2dx_Texture2D_getBitsPerPixelForFormat(lua_State* L)
{
	Texture2D * texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	if (lua_isnumber(L, 2))
	{
		return Lua_SetInt(L, texture->getBitsPerPixelForFormat((Texture2D::PixelFormat) Lua_GetInt(L, 2)));
	}
	else
	{
		return Lua_SetInt(L, texture->getBitsPerPixelForFormat());
	}
}
int lua_cocos2dx_Texture2D_getContentSizeInPixels(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	return Lua_SetSize(L, texture->getContentSizeInPixels());
}
int lua_cocos2dx_Texture2D_hasPremultipliedAlpha(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	return Lua_SetBool(L, texture->hasPremultipliedAlpha());
}
int lua_cocos2dx_Texture2D_hasMipmaps(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	return Lua_SetBool(L, texture->hasMipmaps());
}
int lua_cocos2dx_Texture2D_getPixelFormat(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	return Lua_SetInt(L, (int)texture->getPixelFormat());
}
int lua_cocos2dx_Texture2D_getPixelsWide(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	return Lua_SetInt(L,texture->getPixelsWide());
}
int lua_cocos2dx_Texture2D_getPixelsHigh(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	return Lua_SetInt(L, texture->getPixelsHigh());
}
int lua_cocos2dx_Texture2D_getContentSize(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	return Lua_SetSize(L, texture->getContentSize());
}
int lua_cocos2dx_Texture2D_setGLProgram(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	GLProgram* pro = Lua_GetPointer<GLProgram>(L, 2);
	if (!pro)
	{
		log("setGLProgram failed!!!");
		return 0;
	}
	texture->setGLProgram(pro);
	return 0;
}
int lua_cocos2dx_Texture2D_getPath(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	return Lua_SetString(L, texture->getPath().c_str());
}
int lua_cocos2dx_Texture2D_setAlphaTexture(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	Texture2D* alpha = Lua_GetPointer<Texture2D>(L, 2);
	if (!alpha) return 0;
	texture->setAlphaTexture(alpha);
	return 0;
}
int lua_cocos2dx_Texture2D_getAlphaTexture(lua_State* L)
{
	Texture2D* texture = Lua_GetPointer<Texture2D>(L, 1);
	if (!texture) return 0;
	return Lua_SetPointer(L, texture->getAlphaTexture(),"Texture2D");
}
int lua_open_cocos2dx_Texture2D_module(lua_State* L)
{
	luaL_Reg reg[] = {
		{"setDefaultAlphaPixelFormat",lua_cocos2dx_Texture2D_setDefaultAlphaPixelFormat },
		{"getDefaultAlphaPixelFormat",lua_cocos2dx_Texture2D_getDefaultAlphaPixelFormat },
		{"getDescription",lua_cocos2dx_Texture2D_getDescription },
		{"releaseGLTexture",lua_cocos2dx_Texture2D_releaseGLTexture },
		{"drawAtPoint",lua_cocos2dx_Texture2D_drawAtPoint },
		{"drawInRect",lua_cocos2dx_Texture2D_drawInRect },
		{"initWithImage",lua_cocos2dx_Texture2D_initWithImage },
		{"initWithString",lua_cocos2dx_Texture2D_initWithString },
		{"getStringForFormat",lua_cocos2dx_Texture2D_getStringForFormat },
		{"getBitsPerPixelForFormat",lua_cocos2dx_Texture2D_getBitsPerPixelForFormat },
		{"getContentSizeInPixels",lua_cocos2dx_Texture2D_getContentSizeInPixels },
		{"hasPremultipliedAlpha",lua_cocos2dx_Texture2D_hasPremultipliedAlpha },
		{"hasMipmaps",lua_cocos2dx_Texture2D_hasMipmaps },
		{"getPixelFormat",lua_cocos2dx_Texture2D_getPixelFormat },
		{"getPixelsWide",lua_cocos2dx_Texture2D_getPixelsWide },
		{"getPixelsHigh",lua_cocos2dx_Texture2D_getPixelsHigh },
		{"getContentSize",lua_cocos2dx_Texture2D_getContentSize },
		{"setGLProgram",lua_cocos2dx_Texture2D_setGLProgram },
		{"getPath",lua_cocos2dx_Texture2D_getPath },
		{"setAlphaTexture",lua_cocos2dx_Texture2D_setAlphaTexture },
		{"getAlphaTexture",lua_cocos2dx_Texture2D_getAlphaTexture },
		{NULL,NULL}
	};
	
	lua_getglobal(L, LUA_CC);
	lua_newtable(L);
	const char* keys[] = {"AUTO","BGRA8888","RGBA8888","RGB888","RGB565","A8","I8","AI88","RGBA4444","RGB5A1","PVRTC4","PVRTC4A","PVRTC2","PVRTC2A","ETC","S3TC_DXT1","S3TC_DXT3","S3TC_DXT5","ATC_RGB","ATC_EXPLICIT_ALPHA","ATC_INTERPOLATED_ALPHA","DEFAULT","NONE"};
	const int values[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,0,-1};
	for (int i = 0; i < sizeof(values) / sizeof(int); ++i)
	{
		lua_pushstring(L, keys[i]);
		lua_pushinteger(L, values[i]);
		lua_rawset(L, -3);
	}
	lua_pushstring(L, "PixelFormat");
	lua_rawset(L, -3);
	lua_pop(L, 1);
	return Lua_CreateModule(L, "Texture2D", "Ref", reg);
}

/***************************Sprite**********************************/

int lua_cocos2dx_sprite_updateTransform(lua_State* L)
{
	Sprite* sprite = (Sprite*)Lua_GetPointer<Sprite*>(L,1);
	if (!sprite) return 0;
	sprite->updateTransform();
	return 0;
}

int lua_cocos2dx_sprite_setTexture(lua_State* L)
{
	Sprite* sprite = (Sprite*)Lua_GetPointer<Sprite*>(L, 1);
	if (lua_isstring(L, 2))
	{
		sprite->setTexture(Lua_GetString(L, 2));
	}
	else if (lua_istable(L, 2))
	{
		Texture2D* texture = (Texture2D*)Lua_GetPointer<Texture2D>(L, 2);
		if (!sprite || !texture) return 0;
		sprite->setTexture(texture);
	}
	return 0;
}

int lua_cocos2dx_sprite_getTexture(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	Texture2D* texture = sprite->getTexture();
	if (!texture) return 0;
	if (!Lua_isBinded(L,texture))
	{
		Lua_CreateRef(L, "Texture2D", texture);
	}
	return Lua_SetRef(L, sprite->getTexture());
}

int lua_cocos2dx_sprite_getBatchNode(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	SpriteBatchNode* batchNode = sprite->getBatchNode();
	if (!batchNode) return 0;
	if (!Lua_isBinded(L,batchNode))
	{
		Lua_CreateRef(L, "SpriteBatchNode", batchNode);
	}
	return Lua_SetRef(L, batchNode);
}

int lua_cocos2dx_sprite_setBatchNode(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L,1);
	if (!sprite) return 0;
	SpriteBatchNode* batchNode = Lua_GetPointer<SpriteBatchNode>(L,2);
	if (!batchNode) return 0;
	sprite->setBatchNode(batchNode);
	return 0;
}

int lua_cocos2dx_sprite_setTextureRect(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L,1);
	if (!sprite) return 0;
	if (lua_isboolean(L, 2) && lua_istable(L,3))
	{
		sprite->setTextureRect(Lua_GetRect(L, 2), Lua_GetBool(L, 3), Lua_GetSize(L, 3));
	}
	else
	{
		sprite->setTextureRect(Lua_GetRect(L, 2));
	}
	return 0;
}

int lua_cocos2dx_sprite_setCenterRectNormalized(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	sprite->setCenterRectNormalized(Lua_GetRect(L, 2));
	return 0;
}

int lua_cocos2dx_sprite_getCenterRectNormalized(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	Lua_SetRect(L, sprite->getCenterRectNormalized());
	return 0;
}

int lua_cocos2dx_sprite_setCenterRect(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	sprite->setCenterRect(Lua_GetRect(L, 2));
	return 0;
}

int lua_cocos2dx_sprite_getCenterRect(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	return Lua_SetRect(L, sprite->getCenterRect());
}

int lua_cocos2dx_sprite_setSpriteFrame(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	if (lua_isstring(L, 2))
	{
		sprite->setSpriteFrame(Lua_GetString(L,2));
	}
	else if (lua_istable(L, 2))
	{
		sprite->setSpriteFrame(Lua_GetPointer<SpriteFrame>(L, 2));
	}
	return 0;
}

int lua_cocos2dx_sprite_getSpriteFrame(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	SpriteFrame* frame = sprite->getSpriteFrame();
	if (!frame) return 0;
	if (!Lua_isBinded(L,frame))
	{
		Lua_CreateRef(L, "SpriteFrame", frame);
	}
	return Lua_SetRef(L, frame);
}

int lua_cocos2dx_sprite_setDisplayFrameWithAnimationName(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	sprite->setDisplayFrameWithAnimationName(Lua_GetString(L, 2), Lua_GetInt(L, 3));
	return 0;
}

int lua_cocos2dx_sprite_getTextureRect(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	return Lua_SetRect(L, sprite->getTextureRect());
}

int lua_cocos2dx_sprite_getTextureAtlas(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L,1);
	if (!sprite) return 0;
	TextureAtlas* atlas = sprite->getTextureAtlas();
	if (!atlas) return 0;
	if (!Lua_isBinded(L, atlas))
	{
		Lua_CreateRef(L, "TextureAtlas", atlas);
	}
	return Lua_SetRef(L, atlas);
}

int lua_cocos2dx_sprite_setTextureAtlas(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L,1);
	if (!sprite) return 0;
	TextureAtlas* atlas = Lua_GetPointer<TextureAtlas>(L,2);
	if (!atlas) return 0;
	sprite->setTextureAtlas(atlas);
	return 0;
}

int lua_cocos2dx_sprite_getOffsetPosition(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L,1);
	if (!sprite) return 0;
	return Lua_SetVec2(L, sprite->getOffsetPosition());
}

int lua_cocos2dx_sprite_isFlippedX(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L,1);
	if (!sprite) return 0;
	return Lua_SetBool(L, sprite->isFlippedX());
}

int lua_cocos2dx_sprite_setFlippedX(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	sprite->setFlippedX(Lua_GetBool(L, 2));
	return 0;
}

int lua_cocos2dx_sprite_isFlippedY(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	return Lua_SetBool(L,sprite->isFlippedY());
}

int lua_cocos2dx_sprite_setFlippedY(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	sprite->setFlippedY(Lua_GetBool(L, 2));
	return 0;
}

int lua_cocos2dx_sprite_setBlendFunc(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	sprite->setBlendFunc(Lua_GetBlendFunc(L, 2));
	return 0;
}

int lua_cocos2dx_sprite_getBlendFunc(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	return Lua_SetBlendFunc(L, sprite->getBlendFunc());
}

int lua_cocos2dx_sprite_getResourceName(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 1);
	if (!sprite) return 0;
	return Lua_SetString(L, sprite->getResourceName());
}

int lua_cocos2dx_sprite_create(lua_State* L)
{
	Sprite* sprite = NULL;
	if (lua_isnil(L, 2))
	{
		sprite = Sprite::create();
	}
	else if (lua_isstring(L, 2))
	{
		if (lua_isnil(L, 3))
		{
			sprite = Sprite::create(Lua_GetString(L, 2));
		}
		else if (lua_istable(L,3))
		{
			sprite = Sprite::create(Lua_GetString(L, 2),Lua_GetRect(L,3));
		}
	}
	return Lua_CreateRef(L, "Sprite", sprite);
}

int lua_open_cocos2dx_sprite_module(lua_State* L)
{
	luaL_Reg reg[] = {
		{ "updateTransform", lua_cocos2dx_sprite_updateTransform },
		{ "setTexture", lua_cocos2dx_sprite_setTexture },
		{ "getTexture", lua_cocos2dx_sprite_getTexture },
		{ "getBatchNode", lua_cocos2dx_sprite_getBatchNode },
		{ "setBatchNode", lua_cocos2dx_sprite_setBatchNode },
		{ "setTextureRect", lua_cocos2dx_sprite_setTextureRect },
		{ "setCenterRectNormalized", lua_cocos2dx_sprite_setCenterRectNormalized },
		{ "getCenterRectNormalized", lua_cocos2dx_sprite_getCenterRectNormalized },
		{ "setCenterRect", lua_cocos2dx_sprite_setCenterRect },
		{ "getCenterRect", lua_cocos2dx_sprite_getCenterRect },
		{ "setSpriteFrame", lua_cocos2dx_sprite_setSpriteFrame },
		{ "getSpriteFrame", lua_cocos2dx_sprite_getSpriteFrame },
		{ "setDisplayFrameWithAnimationName", lua_cocos2dx_sprite_setDisplayFrameWithAnimationName },
		{ "getTextureRect", lua_cocos2dx_sprite_getTextureRect },
		{ "getTextureAtlas", lua_cocos2dx_sprite_getTextureAtlas },
		{ "setTextureAtlas", lua_cocos2dx_sprite_setTextureAtlas },
		{ "getOffsetPosition", lua_cocos2dx_sprite_getOffsetPosition },
		{ "isFlippedX", lua_cocos2dx_sprite_isFlippedX },
		{ "setFlippedX", lua_cocos2dx_sprite_setFlippedX },
		{ "isFlippedY", lua_cocos2dx_sprite_isFlippedY },
		{ "setFlippedY", lua_cocos2dx_sprite_setFlippedY },
		{ "setBlendFunc", lua_cocos2dx_sprite_setBlendFunc },
		{ "getBlendFunc", lua_cocos2dx_sprite_getBlendFunc },
		{ "getResourceName", lua_cocos2dx_sprite_getResourceName },
		{ "create", lua_cocos2dx_sprite_create },
		{NULL,NULL}
	};
	Lua_CreateModule(L, "Sprite", "Node", reg);
	return 0;
}

/************************Scene*************************/

int lua_cocos2dx_scene_getDescription(lua_State* L)
{
	Scene* scene = Lua_GetPointer<Scene>(L, 1);
	if (!scene) return 0;
	return Lua_SetString(L,scene->getDescription());
}

int lua_cocos2dx_scene_removeAllChildren(lua_State* L)
{
	Scene* scene = Lua_GetPointer<Scene>(L,1);
	if (!scene) return 0;
	scene->removeAllChildren();
	return 0;
}
int lua_cocos2dx_scene_init(lua_State* L)
{
	Scene* scene = Lua_GetPointer<Scene>(L,1);
	if (scene) 
		scene->init();
	return 0;
}
int lua_cocos2dx_scene_initWithSize(lua_State* L)
{
	Scene* scene = Lua_GetPointer<Scene>(L,1);
	if (!scene) return 0;
	scene->initWithSize(Lua_GetSize(L, 2));
	return 0;
}
int lua_cocos2dx_scene_setCameraOrderDirty(lua_State* L)
{
	Scene* scene = Lua_GetPointer<Scene>(L,1);
	if (scene)
		scene->setCameraOrderDirty();
	return 0;
}
int lua_cocos2dx_scene_create(lua_State* L)
{
	Scene* scene = NULL;
	int top = lua_gettop(L);
	if (top == 1)
	{
		scene = Scene::create();
	}
	else if (top == 3)
	{
		scene = Scene::createWithSize(Lua_GetSize(L, 2));
	}
	return Lua_CreateRef(L, "Scene",scene);
}

int lua_cocos2dx_scene_createWithPhysics(lua_State* L)
{
	Scene* scene = Scene::createWithPhysics();
	return Lua_CreateRef(L, "Scene", scene);
}

int lua_open_cocos2dx_scene_module(lua_State* L)
{
	luaL_Reg reg[] = {
		{ "getDescription", lua_cocos2dx_scene_getDescription },
		{ "removeAllChildren", lua_cocos2dx_scene_removeAllChildren },
		{ "init", lua_cocos2dx_scene_init },
		{ "initWithSize", lua_cocos2dx_scene_initWithSize },
		{ "setCameraOrderDirty", lua_cocos2dx_scene_setCameraOrderDirty },
		{ "create", lua_cocos2dx_scene_create },
		{ "createWithPhysics", lua_cocos2dx_scene_createWithPhysics },
	};
	return Lua_CreateModule(L, "Scene", "Node", reg);
}

/*************************Action**************************/

int lua_cocos2dx_action_description(lua_State* L)
{
	Action* action = Lua_GetPointer<Action>(L, 1);
	if (!action) return 0;
	return Lua_SetString(L,action->description());
}
int lua_cocos2dx_action_clone(lua_State* L)
{
	Action* action = Lua_GetPointer<Action>(L, 1);
	if (!action) return 0;
	Action* ac = action->clone();
	if (!ac) return 0;
	return Lua_CreateRef(L, "Action", ac);
}
int lua_cocos2dx_action_reverse(lua_State* L)
{
	Action* action = Lua_GetPointer<Action>(L, 1);
	if (!action) return 0;
	Action* ac = action->reverse();
	if (!ac) return 0;
	return Lua_CreateRef(L, "Action", ac);
}
int lua_cocos2dx_action_isDone(lua_State* L)
{
	Action* action = Lua_GetPointer<Action>(L,1);
	if (!action) return 0;
	return Lua_SetBool(L, action->isDone());
}
int lua_cocos2dx_action_startWithTarget(lua_State* L)
{
	Action* action = Lua_GetPointer<Action>(L,1);
	if (!action) return 0;
	Node* node = Lua_GetPointer<Node>(L, 2);
	if (!node) return 0;
	action->startWithTarget(node);
	return 0;
}
int lua_cocos2dx_action_stop(lua_State* L)
{
	Action* action = Lua_GetPointer<Action>(L,1);
	if (!action) return 0;
	action->stop();
	return 0;
}
int lua_cocos2dx_action_getTarget(lua_State* L)
{
	Action* action = Lua_GetPointer<Action>(L, 1);
	if (!action) return 0;
	Node* node = action->getTarget();
	if (!node) return 0;
	return Lua_SetRef(L, node);
}
int lua_cocos2dx_action_setTarget(lua_State* L)
{
	Action* action = Lua_GetPointer<Action>(L,1);
	if (!action) return 0;
	Node* node = Lua_GetPointer<Node>(L, 2);
	if (!node) return 0;
	action->setTarget(node);
	return 0;
}
int lua_cocos2dx_action_getTag(lua_State* L)
{
	Action* action = Lua_GetPointer<Action>(L, 1);
	if (!action) return 0;
	return Lua_SetInt(L, action->getTag());
}
int lua_cocos2dx_action_setTag(lua_State* L)
{
	Action* action = Lua_GetPointer<Action>(L, 1);
	if (!action) return 0;
	action->setTag(Lua_GetInt(L, 2));
	return 0;
}

int lua_open_cocos2dx_action_module(lua_State* L)
{
	luaL_Reg reg[] = {
		{ "description",lua_cocos2dx_action_description },
		{ "clone",lua_cocos2dx_action_clone },
		{ "reverse",lua_cocos2dx_action_reverse },
		{ "isDone",lua_cocos2dx_action_isDone },
		{ "startWithTarget",lua_cocos2dx_action_startWithTarget },
		{ "stop",lua_cocos2dx_action_stop },
		{ "getTarget",lua_cocos2dx_action_getTarget },
		{ "setTarget",lua_cocos2dx_action_setTarget },
		{ "getTag",lua_cocos2dx_action_getTag },
		{ "setTag",lua_cocos2dx_action_setTag }
	};
	return Lua_CreateModule(L, "Action", "Ref", reg);
}

/*************************Animation***********************/
int lua_open_cocos2dx_animation_create(lua_State* L)
{
	cocos2d::Vector<SpriteFrame*> frames = Lua_GetRefVector<SpriteFrame>(L, 2);
	Animation* animation = Animation::createWithSpriteFrames(Lua_GetRefVector<SpriteFrame>(L, 2), Lua_GetFloat(L, 3), Lua_GetInt(L, 4));
	return Lua_CreateRef(L, "Animation", animation);
}

int lua_open_cocos2dx_animation_addSpriteFrame(lua_State* L)
{
	Animation* animation = Lua_GetPointer<Animation>(L, 1);
	if (!animation) return 0;
	SpriteFrame* frame = Lua_GetPointer<SpriteFrame>(L,2);
	if (frame)
	{
		animation->addSpriteFrame(frame);
	}
	return 0;
}
int lua_open_cocos2dx_animation_addSpriteFrameWithFile(lua_State* L)
{
	Animation* animation = Lua_GetPointer<Animation>(L, 1);
	if (!animation) return 0;
	animation->addSpriteFrameWithFile(Lua_GetString(L, 2));
	return 0;
}
int lua_open_cocos2dx_animation_getTotalDelayUnits(lua_State* L)
{
	Animation* animation = Lua_GetPointer<Animation>(L, 1);
	if (!animation) return 0;
	return Lua_SetFloat(L, animation->getTotalDelayUnits());
}
int lua_open_cocos2dx_animation_setDelayPerUnit(lua_State* L)
{
	Animation* animation = Lua_GetPointer<Animation>(L, 1);
	if (!animation) return 0;
	animation->setDelayPerUnit(Lua_GetFloat(L, 2));
	return 0;
}
int lua_open_cocos2dx_animation_getDelayPerUnit(lua_State* L)
{
	Animation* animation = Lua_GetPointer<Animation>(L, 1);
	if (!animation) return 0;
	return Lua_SetFloat(L, animation->getDelayPerUnit());
}
int lua_open_cocos2dx_animation_getDuration(lua_State* L)
{
	Animation* animation = Lua_GetPointer<Animation>(L, 1);
	if (!animation) return 0;
	return Lua_SetFloat(L, animation->getDuration());
}
int lua_open_cocos2dx_animation_getRestoreOriginalFrame(lua_State* L)
{
	Animation* animation = Lua_GetPointer<Animation>(L, 1);
	if (!animation) return 0;
	return Lua_SetBool(L, animation->getRestoreOriginalFrame());
}
int lua_open_cocos2dx_animation_setRestoreOriginalFrame(lua_State* L)
{
	Animation* animation = Lua_GetPointer<Animation>(L, 1);
	if (!animation) return 0;
	animation->setRestoreOriginalFrame(Lua_GetBool(L, 2));
	return 0;
}
int lua_open_cocos2dx_animation_getLoops(lua_State* L)
{
	Animation* animation = Lua_GetPointer<Animation>(L, 1);
	if (!animation) return 0;
	return Lua_SetInt(L, animation->getLoops());
}
int lua_open_cocos2dx_animation_setLoops(lua_State* L)
{
	Animation * animation = Lua_GetPointer<Animation>(L, 1);
	if (!animation) return 0;
	animation->setLoops(Lua_GetInt(L, 2));
	return 0;
}
int lua_open_cocos2dx_animation_clone(lua_State* L)
{
	Animation* animation = Lua_GetPointer<Animation>(L, 1);
	if (!animation) return 0;
	Animation* clone = animation->clone();
	return Lua_CreateRef(L, "Animation", clone);
}
int lua_open_cocos2dx_animation_module(lua_State* L)
{
	luaL_Reg reg[] = {
		{"create",lua_open_cocos2dx_animation_create },
		{"addSpriteFrame",lua_open_cocos2dx_animation_addSpriteFrame },
		{"addSpriteFrameWithFile",lua_open_cocos2dx_animation_addSpriteFrameWithFile },
		{"getTotalDelayUnits",lua_open_cocos2dx_animation_getTotalDelayUnits },
		{"setDelayPerUnit",lua_open_cocos2dx_animation_setDelayPerUnit },
		{"getDelayPerUnit",lua_open_cocos2dx_animation_getDelayPerUnit },
		{"getDuration",lua_open_cocos2dx_animation_getDuration },
		{"getRestoreOriginalFrame",lua_open_cocos2dx_animation_getRestoreOriginalFrame },
		{"setRestoreOriginalFrame",lua_open_cocos2dx_animation_setRestoreOriginalFrame },
		{"getLoops",lua_open_cocos2dx_animation_getLoops },
		{"setLoops",lua_open_cocos2dx_animation_setLoops },
		{"clone",lua_open_cocos2dx_animation_clone },
	};
	return Lua_CreateModule(L, "Animation", "Ref", reg);
}
/****************************Animate**********************/

int lua_open_cocos2dx_Animate_create(lua_State* L)
{
	Animation* animation = Lua_GetPointer<Animation>(L, 2);
	if (!animation) return 0;
	Animate* animate = Animate::create(animation);
	return Lua_CreateRef(L, "Animate", animate);
}
int lua_cocos2dx_Animate_setAnimation(lua_State* L)
{
	Animate* animate = Lua_GetPointer<Animate>(L, 1);
	Animation* animation = Lua_GetPointer<Animation>(L, 2);
	if (!animate || !animation) return 0;
	animate->setAnimation(animation);
	return 0;
}
int lua_cocos2dx_Animate_getAnimation(lua_State* L)
{
	Animate* animate = Lua_GetPointer<Animate>(L, 1);
	if (!animate) return 0;
	return Lua_SetPointer(L, animate->getAnimation(), "Animation");
}
int lua_cocos2dx_Animate_getCurrentFrameIndex(lua_State* L)
{
	Animate* animate = Lua_GetPointer<Animate>(L, 1);
	if (!animate) return 0;
	return Lua_SetInt(L, animate->getCurrentFrameIndex());
}
int lua_open_cocos2dx_Animate_module(lua_State* L)
{
	luaL_Reg reg[] = {
		{"create",lua_open_cocos2dx_Animate_create },
		{"setAnimation",lua_cocos2dx_Animate_setAnimation },
		{"getAnimation",lua_cocos2dx_Animate_getAnimation },
		{"getCurrentFrameIndex",lua_cocos2dx_Animate_getCurrentFrameIndex },
		{NULL,NULL}
	};
	return Lua_CreateModule(L, "Animate", "Action", reg);
}

/*********************************************************/
int lua_cocos2dx_action_show(lua_State* L)
{
	auto action = Show::create();
	return Lua_CreateRef(L, "Action", action);
}
int lua_cocos2dx_action_hide(lua_State* L)
{
	auto action = Hide::create();
	return Lua_CreateRef(L, "Action", action);
}
int lua_cocos2dx_action_togglevisibility(lua_State* L)
{
	auto action = ToggleVisibility::create();
	return Lua_CreateRef(L, "Action", action);
}
int lua_cocos2dx_action_remove(lua_State* L)
{
	auto action = RemoveSelf::create(true);
	return Lua_CreateRef(L, "Action", action);
}
int lua_cocos2dx_action_flipX(lua_State* L)
{
	auto action = FlipX::create(Lua_GetBool(L,1));
	return Lua_CreateRef(L, "Action", action);
}
int lua_cocos2dx_action_flipY(lua_State* L)
{
	auto action = FlipY::create(Lua_GetBool(L, 1));
	return Lua_CreateRef(L, "Action", action);
}
int lua_cocos2dx_action_place(lua_State* L)
{
	auto action = Place::create(Lua_GetVec2(L, 1));
	return Lua_CreateRef(L, "Action", action);
}
int lua_cocos2dx_action_callFunc(lua_State* L)
{
	if (!lua_isfunction(L, 1))
	{
		log("action callfunc : arg1 is not a lua func");
		assert(false);
		return 0;
	}
	int ref = luaL_ref(L, 1);
	CallFunc* action = NULL;
	action = CallFunc::create([=]() {
		Lua_CallRef(L, ref, 0);
		luaL_unref(L, LUA_REGISTRYINDEX, ref);
		action->_callbackID = 0;
	});
	action->_callbackID = ref;
	return Lua_CreateRef(L, "Action", action);
}
int lua_cocos2dx_action_sequence(lua_State* L)
{
	int top = lua_gettop(L);
	Vector<FiniteTimeAction*> v;
	for (int i = 1; i <= top; ++i)
	{
		FiniteTimeAction* action = Lua_GetPointer<FiniteTimeAction>(L, i);
		if (action)
		{
			v.pushBack(action);
		}
		else
		{
			log("sequence create action:%d is nil",i);
		}
	}
	Sequence * seq = Sequence::create(v);
	return Lua_CreateRef(L, "Action", seq);
}
int lua_cocos2dx_action_repeat(lua_State* L)
{
	FiniteTimeAction* action = Lua_GetPointer<FiniteTimeAction>(L, 1);
	if (!action) return 0;
	int times = Lua_GetInt(L, 2);
	Repeat * repeat = Repeat::create(action, times);
	return Lua_CreateRef(L, "Action", repeat);
}
int lua_cocos2dx_action_repeatForever(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	RepeatForever* repeat = RepeatForever::create(action);
	return Lua_CreateRef(L, "Action", repeat);
}
int lua_cocos2dx_action_spawn(lua_State* L)
{
	int top = lua_gettop(L);
	Vector<FiniteTimeAction*> v;
	for (int i = 1; i <= top; ++i)
	{
		FiniteTimeAction* action = Lua_GetPointer<FiniteTimeAction>(L, i);
		if (action)
		{
			v.pushBack(action);
		}
		else
		{
			log("spawn create , action:%d is nil",i);
		}
	}
	Spawn* spawn = Spawn::create(v);
	return Lua_CreateRef(L, "Action", spawn);
}
int lua_cocos2dx_action_rotateTo(lua_State* L)
{
	int top = lua_gettop(L);
	if (top == 3)
	{
		RotateTo* rotate = RotateTo::create(Lua_GetFloat(L, 1), Lua_GetFloat(L, 2), Lua_GetFloat(L, 3));
		return Lua_CreateRef(L, "Action", rotate);
	}
	else if (top == 2)
	{
		RotateTo* rotate = RotateTo::create(Lua_GetFloat(L, 1), Lua_GetFloat(L, 2));
		return Lua_CreateRef(L, "Action", rotate);
	}
	log("rotateTo create arg is not enough");
	return 0;
}
int lua_cocos2dx_action_rotateBy(lua_State* L)
{
	int top = lua_gettop(L);
	if (top == 3)
	{
		RotateBy* rotate = RotateBy::create(Lua_GetFloat(L, 1), Lua_GetFloat(L, 2), Lua_GetFloat(L, 3));
		return Lua_CreateRef(L, "Action", rotate);
	}
	else if (top == 2)
	{
		RotateBy* rotate = RotateBy::create(Lua_GetFloat(L, 1), Lua_GetFloat(L, 2));
		return Lua_CreateRef(L, "Action", rotate);
	}
	log("rotateBy create arg is not enough");
	return 0;
}
int lua_cocos2dx_action_moveBy(lua_State* L)
{
	MoveBy* move = MoveBy::create(Lua_GetFloat(L, 1), Lua_GetVec2(L, 2));
	return Lua_CreateRef(L, "Action", move);
}
int lua_cocos2dx_action_moveTo(lua_State* L)
{
	MoveTo* move = MoveTo::create(Lua_GetFloat(L, 1), Lua_GetVec2(L, 2));
	return Lua_CreateRef(L, "Action", move);
}
int lua_cocos2dx_action_skewTo(lua_State* L)
{
	SkewTo* skew = SkewTo::create(Lua_GetFloat(L, 1), Lua_GetFloat(L, 2), Lua_GetFloat(L, 3));
	return Lua_CreateRef(L, "Action", skew);
}
int lua_cocos2dx_action_skewBy(lua_State* L)
{
	SkewBy* skew = SkewBy::create(Lua_GetFloat(L, 1), Lua_GetFloat(L, 2), Lua_GetFloat(L, 3));
	return Lua_CreateRef(L, "Action", skew);
}
int lua_cocos2dx_action_resizeTo(lua_State* L)
{
	ResizeTo* resize = ResizeTo::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2));
	return Lua_CreateRef(L, "Action", resize);
}
int lua_cocos2dx_action_resizeBy(lua_State* L)
{
	ResizeBy* resize = ResizeBy::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2));
	return Lua_CreateRef(L, "Action", resize);
}
int lua_cocos2dx_action_jumpBy(lua_State* L)
{
	JumpBy* jump = JumpBy::create(Lua_GetFloat(L, 1), Lua_GetVec2(L, 2), Lua_GetFloat(L, 4), Lua_GetInt(L, 5));
	return Lua_CreateRef(L, "Action", jump);
}
int lua_cocos2dx_action_jumpTo(lua_State* L)
{
	JumpTo* jump = JumpTo::create(Lua_GetFloat(L, 1), Lua_GetVec2(L, 2), Lua_GetFloat(L, 4), Lua_GetInt(L, 5));
	return Lua_CreateRef(L, "Action", jump);
}
int lua_cocos2dx_action_bezierBy(lua_State* L)
{
	BezierBy* bezier = BezierBy::create(Lua_GetFloat(L, 1), Lua_GetBezierConfig(L, 2));
	return Lua_CreateRef(L, "Action", bezier);
}
int lua_cocos2dx_action_bezierTo(lua_State* L)
{
	BezierTo* bezier = BezierTo::create(Lua_GetFloat(L, 1), Lua_GetBezierConfig(L, 2));
	return Lua_CreateRef(L, "Action", bezier);
}
int lua_cocos2dx_action_scaleTo(lua_State* L)
{
	int top = lua_gettop(L);
	if (top == 2)
	{
		ScaleTo* scale = ScaleTo::create(Lua_GetFloat(L, 1), Lua_GetFloat(L, 2));
		return Lua_CreateRef(L, "Action", scale);
	}
	else if (top == 3)
	{
		ScaleTo* scale = ScaleTo::create(Lua_GetFloat(L, 1), Lua_GetFloat(L, 2), Lua_GetFloat(L, 3));
		return Lua_CreateRef(L, "Action", scale);
	}
	log("scaleTo create ,the arg is not enough");
	return 0;
}
int lua_cocos2dx_action_scaleBy(lua_State* L)
{
	int top = lua_gettop(L);
	if (top == 2)
	{
		ScaleBy* scale = ScaleBy::create(Lua_GetFloat(L, 1), Lua_GetFloat(L, 2));
		return Lua_CreateRef(L, "Action", scale);
	}
	else if (top == 3)
	{
		ScaleBy* scale = ScaleBy::create(Lua_GetFloat(L, 1), Lua_GetFloat(L, 2), Lua_GetFloat(L, 3));
		return Lua_CreateRef(L, "Action", scale);
	}
	log("scaleBy create , the arg is not enough");
	return 0;
}
int lua_cocos2dx_action_blink(lua_State* L)
{
	Blink* blink = Blink::create(Lua_GetFloat(L, 1), Lua_GetInt(L, 2));
	return Lua_CreateRef(L,"Action",blink);
}
int lua_cocos2dx_action_fadeTo(lua_State* L)
{
	FadeTo* fade = FadeTo::create(Lua_GetFloat(L, 1), (GLubyte)Lua_GetInt(L, 2));
	return Lua_CreateRef(L, "Action", fade);
}
int lua_cocos2dx_action_fadeIn(lua_State* L)
{
	FadeIn* fade = FadeIn::create(Lua_GetFloat(L, 1));
	return Lua_CreateRef(L, "Action", fade);
}
int lua_cocos2dx_action_fadeOut(lua_State* L)
{
	FadeOut* fade = FadeOut::create(Lua_GetFloat(L, 1));
	return Lua_CreateRef(L, "Action", fade);
}
int lua_cocos2dx_action_tintTo(lua_State* L)
{
	TintTo* tint = TintTo::create(Lua_GetFloat(L, 1), Lua_GetChar(L, 2), Lua_GetChar(L, 3), Lua_GetChar(L, 4));
	return Lua_CreateRef(L, "Action", tint);
}
int lua_cocos2dx_action_tintBy(lua_State* L)
{
	TintBy* tint = TintBy::create(Lua_GetFloat(L, 1), Lua_GetShort(L, 2), Lua_GetShort(L, 3), Lua_GetShort(L, 4));
	return Lua_CreateRef(L, "Action", tint);
}
int lua_cocos2dx_action_delayTime(lua_State* L)
{
	DelayTime* delay = DelayTime::create(Lua_GetFloat(L, 1));
	return Lua_CreateRef(L, "Action", delay);
}
int lua_cocosdx_action_reverseTime(lua_State* L)
{
	FiniteTimeAction* action = Lua_GetPointer<FiniteTimeAction>(L, 1);
	if (!action) return 0;
	ReverseTime* reverse = ReverseTime::create(action);
	return Lua_CreateRef(L, "Action", reverse);
}
int lua_cocos2dx_action_animate(lua_State* L)
{
	Animation* action = Lua_GetPointer<Animation>(L, 1);
	if (!action) return 0;
	Animate* animate = Animate::create(action);
	return Lua_CreateRef(L, "Animate", animate);
}
int lua_cocos2dx_action_ProgressTo(lua_State* L)
{
	ProgressTo* to = ProgressTo::create(Lua_GetFloat(L, 1), Lua_GetFloat(L, 2));
	return Lua_CreateRef(L, "Action", to);
}
int lua_cocos2dx_action_ProgressFromTo(lua_State* L)
{
	ProgressFromTo* fromto = ProgressFromTo::create(Lua_GetFloat(L, 1), Lua_GetFloat(L, 2), Lua_GetFloat(L, 3));
	return Lua_CreateRef(L, "Action", fromto);
}

int lua_open_cocos2dx_actions_module(lua_State* L)
{
	luaL_Reg reg[] = {
		{"Show",lua_cocos2dx_action_show },
		{"Hide",lua_cocos2dx_action_hide },
		{"Togglevisibility",lua_cocos2dx_action_togglevisibility },
		{"Remove",lua_cocos2dx_action_remove },
		{"FlipX",lua_cocos2dx_action_flipX },
		{"FlipY",lua_cocos2dx_action_flipY },
		{"Place",lua_cocos2dx_action_place },
		{"CallFunc",lua_cocos2dx_action_callFunc },
		{"Sequence",lua_cocos2dx_action_sequence },
		{"Repeat",lua_cocos2dx_action_repeat },
		{"RepeatForever",lua_cocos2dx_action_repeatForever },
		{"Spawn",lua_cocos2dx_action_spawn },
		{"RotateTo",lua_cocos2dx_action_rotateTo },
		{"RotateBy",lua_cocos2dx_action_rotateBy },
		{"MoveBy",lua_cocos2dx_action_moveBy },
		{"MoveTo",lua_cocos2dx_action_moveTo },
		{"SkewTo",lua_cocos2dx_action_skewTo },
		{"SkewBy",lua_cocos2dx_action_skewBy },
		{"ResizeTo",lua_cocos2dx_action_resizeTo },
		{"ResizeBy",lua_cocos2dx_action_resizeBy },
		{"JumpBy",lua_cocos2dx_action_jumpBy },
		{"JumpTo",lua_cocos2dx_action_jumpTo },
		{"BezierBy",lua_cocos2dx_action_bezierBy },
		{"BezierTo",lua_cocos2dx_action_bezierTo },
		{"ScaleTo",lua_cocos2dx_action_scaleTo },
		{"ScaleBy",lua_cocos2dx_action_scaleBy },
		{"Blink",lua_cocos2dx_action_blink },
		{"FadeTo",lua_cocos2dx_action_fadeTo },
		{"FadeIn",lua_cocos2dx_action_fadeIn },
		{"FadeOut",lua_cocos2dx_action_fadeOut },
		{"TintTo",lua_cocos2dx_action_tintTo },
		{"TintBy",lua_cocos2dx_action_tintBy },
		{"DelayTime",lua_cocos2dx_action_delayTime },
		{"ReverseTime",lua_cocosdx_action_reverseTime },
		{"Animate",lua_cocos2dx_action_animate },
		{"ProgressTo",lua_cocos2dx_action_ProgressTo },
		{"ProgressFromTo",lua_cocos2dx_action_ProgressFromTo },
		{NULL,NULL}
	};
	return Lua_CreateModule(L, reg);
}
/**********************************GridBase********************************/
int lua_cocos2dx_GridBase_create(lua_State* L)
{
	GridBase* grid = NULL;
	int top = lua_gettop(L);
	if (top == 3)
	{
		grid = GridBase::create(Lua_GetSize(L, 2));
	}
	else if(top >= 4)
	{
		Texture2D* texture = Lua_GetPointer<Texture2D>(L, 4);
		if (!texture)
		{
			log("GridBase create Texture2D is null");
			return 0;
		}
		grid = GridBase::create(Lua_GetSize(L, 2), texture, Lua_GetBool(L, 5));
	}
	if (!grid)
	{
		log("GridBase create arg is wrong");
		return 0;
	}
	return Lua_CreateRef(L, "GridBase", grid);
}
int lua_cocos2dx_GridBase_initWithSize(lua_State* L)
{
	int top = lua_gettop(L);
	GridBase* grid = Lua_GetPointer<GridBase>(L, 1);
	if (!grid) return 0;
	if (top == 3)
	{
		grid->initWithSize(Lua_GetSize(L, 2));
	}
	else if (top == 4)
	{
		grid->initWithSize(Lua_GetSize(L, 2), Lua_GetRect(L, 4));
	}
	else if (top == 5)
	{
		Texture2D* texture = Lua_GetPointer<Texture2D>(L, 4);
		if (!texture)
		{
			log("GridBase initWithSize texture is null");
			return 0;
		}
		grid->initWithSize(Lua_GetSize(L, 2), texture, Lua_GetBool(L, 5));
	}
	else if (top == 6)
	{
		Texture2D* texture = Lua_GetPointer<Texture2D>(L, 4);
		if (!texture)
		{
			log("GridBase initWithSize texture is null");
			return 0;
		}
		grid->initWithSize(Lua_GetSize(L, 2), texture, Lua_GetBool(L, 5), Lua_GetRect(L, 6));
	}
	return 0;
}
int lua_cocos2dx_GridBase_isActive(lua_State* L)
{
	GridBase* action = Lua_GetPointer<GridBase>(L, 1);
	if (!action) return 0;
	return Lua_SetBool(L, action->isActive());
}
int lua_cocos2dx_GridBase_setActive(lua_State* L)
{
	GridBase* action = Lua_GetPointer<GridBase>(L, 1);
	if (!action) return 0;
	action->setActive(Lua_GetBool(L, 2));
	return 0;
}
int lua_cocos2dx_GridBase_getReuseGrid(lua_State* L)
{
	GridBase* action = Lua_GetPointer<GridBase>(L, 1);
	if (!action) return 0;
	return Lua_SetInt(L, action->getReuseGrid());
}
int lua_cocos2dx_GridBase_setReuseGrid(lua_State* L)
{
	GridBase* action = Lua_GetPointer<GridBase>(L, 1);
	if (!action) return 0;
	action->setReuseGrid(Lua_GetInt(L, 2));
	return 0;
}
int lua_coco2dx_GridBase_getGridSize(lua_State* L)
{
	GridBase* action = Lua_GetPointer<GridBase>(L, 1);
	if (!action) return 0;
	return Lua_SetSize(L,action->getGridSize());
}
int lua_cocos2dx_GridBase_setGridSize(lua_State* L)
{
	GridBase* grid = Lua_GetPointer<GridBase>(L, 1);
	if (!grid) return 0;
	grid->setGridSize(Lua_GetSize(L, 2));
	return 0;
}
int lua_cocos2dx_GridBase_getStep(lua_State* L)
{
	GridBase* grid = Lua_GetPointer<GridBase>(L, 1);
	if (!grid) return 0;
	return Lua_SetVec2(L, grid->getStep());
}
int lua_cocos2dx_GridBase_setStep(lua_State* L)
{
	GridBase* grid = Lua_GetPointer<GridBase>(L, 1);
	if (!grid) return 0;
	grid->setStep(Lua_GetVec2(L, 2));
	return 0;
}
int lua_cocos2dx_GridBase_isTextureFlipped(lua_State* L)
{
	GridBase* grid = Lua_GetPointer<GridBase>(L, 1);
	if (!grid) return 0;
	return Lua_SetBool(L,grid->isTextureFlipped());
}
int lua_cocos2dx_GridBase_setTextureFlipped(lua_State* L)
{
	GridBase* grid = Lua_GetPointer<GridBase>(L, 1);
	if (!grid) return 0;
	grid->setTextureFlipped(Lua_GetBool(L, 2));
	return 0;
}
int lua_cocos2dx_GridBase_setGridRect(lua_State* L)
{
	GridBase* grid = Lua_GetPointer<GridBase>(L, 1);
	if (!grid) return 0;
	grid->setGridRect(Lua_GetRect(L, 2));
	return 0;
}
int lua_cocos2dx_GridBase_getGridRect(lua_State* L)
{
	GridBase* grid = Lua_GetPointer<GridBase>(L, 1);
	if (!grid) return 0;
	return Lua_SetRect(L, grid->getGridRect());
}
int lua_open_GridBase_module(lua_State* L)
{
	luaL_Reg reg[] = {
		{"create",lua_cocos2dx_GridBase_create },
		{"initWithSize",lua_cocos2dx_GridBase_initWithSize },
		{"isActive",lua_cocos2dx_GridBase_isActive },
		{"setActive",lua_cocos2dx_GridBase_setActive },
		{"getReuseGrid",lua_cocos2dx_GridBase_getReuseGrid },
		{"setReuseGrid",lua_cocos2dx_GridBase_setReuseGrid },
		{"getGridSize",lua_coco2dx_GridBase_getGridSize },
		{"setGridSize",lua_cocos2dx_GridBase_setGridSize },
		{"getStep",lua_cocos2dx_GridBase_getStep },
		{"setStep",lua_cocos2dx_GridBase_setStep },
		{"isTextureFlipped",lua_cocos2dx_GridBase_isTextureFlipped },
		{"setTextureFlipped",lua_cocos2dx_GridBase_setTextureFlipped },
		{"setGridRect",lua_cocos2dx_GridBase_setGridRect },
		{"getGridRect",lua_cocos2dx_GridBase_getGridRect },
		{NULL,NULL}
	};
	return Lua_CreateModule(L, "GridBase", "Ref", reg);
}

/**********************************Grid3DAction*****************************/
int lua_cocos2dx_Grid3DAction_getGrid(lua_State* L)
{
	Grid3DAction* action = Lua_GetPointer<Grid3DAction>(L, 1);
	if (!action) return 0;
	return Lua_SetPointer(L, action->getGrid(), "GridBase");
}
int lua_cocos2dx_Grid3DAction_getVertex(lua_State* L)
{
	Grid3DAction* action = Lua_GetPointer<Grid3DAction>(L, 1);
	if (!action) return 0;
	return Lua_SetVec3(L, action->getVertex(Lua_GetVec2(L,2)));
}
int lua_cocos2dx_Grid3DAction_getOriginalVertex(lua_State* L)
{
	Grid3DAction* action = Lua_GetPointer<Grid3DAction>(L, 1);
	if (!action) return 0;
	return Lua_SetVec3(L, action->getOriginalVertex(Lua_GetVec2(L, 2)));
}
int lua_cocos2dx_Grid3DAction_setVertex(lua_State* L)
{
	Grid3DAction* action = Lua_GetPointer<Grid3DAction>(L, 1);
	if (!action) return 0;
	action->setVertex(Lua_GetVec2(L, 2), Lua_GetVec3(L, 4));
	return 0;
}
int lua_cocos2ex_Grid3DAction_getGridRect(lua_State* L)
{
	Grid3DAction* action = Lua_GetPointer<Grid3DAction>(L, 1);
	if (!action) return 0;
	return Lua_SetRect(L, action->getGridRect());
}
int lua_open_Grid3DAction_module(lua_State* L)
{
	luaL_Reg reg[] = {
		{"getGrid",lua_cocos2dx_Grid3DAction_getGrid },
		{"getVertex",lua_cocos2dx_Grid3DAction_getVertex },
		{"getOriginalVertex",lua_cocos2dx_Grid3DAction_getOriginalVertex },
		{"setVertex",lua_cocos2dx_Grid3DAction_setVertex },
		{"getGridRect",lua_cocos2ex_Grid3DAction_getGridRect },
		{NULL,NULL},
	};
	return Lua_CreateModule(L, "Grid3DAction", "Action", reg);
}

/**********************************Effect*********************************/
int lua_cocos2dx_effect_Shaky3D(lua_State* L)
{
	Shaky3D* shaky3d = Shaky3D::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2), Lua_GetInt(L, 4), Lua_GetBool(L, 5));
	return Lua_CreateRef(L, "Grid3DAction", shaky3d);
}
int lua_cocos2dx_effect_Waves3D(lua_State* L)
{
	Waves3D* waves = Waves3D::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2), Lua_GetInt(L, 4), Lua_GetFloat(L,5));
	return Lua_CreateRef(L, "Grid3DAction", waves);
}
int lua_cocos2dx_effect_FlipX3D(lua_State* L)
{
	FlipX3D* flip = FlipX3D::create(Lua_GetFloat(L, 1));
	return Lua_CreateRef(L, "Grid3DAction", flip);
}
int lua_cocos2dx_effect_FlipY3D(lua_State* L)
{
	FlipY3D* flip = FlipY3D::create(Lua_GetFloat(L, 1));
	return Lua_CreateRef(L, "Grid3DAction", flip);
}
int lua_cocos2dx_effect_Lens3D(lua_State* L)
{
	Lens3D* lens = Lens3D::create(Lua_GetFloat(L, 1), Lua_GetSize(L,2), Lua_GetVec2(L, 4), Lua_GetFloat(L, 6));
	return Lua_CreateRef(L, "Grid3DAction", lens);
}
int lua_cocos2dx_effect_Ripple3D(lua_State* L)
{
	Ripple3D* ripple = Ripple3D::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2), Lua_GetVec2(L, 4), Lua_GetFloat(L, 6), Lua_GetInt(L, 7), Lua_GetFloat(L, 8));
	return Lua_CreateRef(L, "Grid3DAction", ripple);
}
int lua_cocos2dx_effect_Liquid(lua_State* L)
{
	Liquid* liquid = Liquid::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2), Lua_GetInt(L, 4), Lua_GetFloat(L, 5));
	return Lua_CreateRef(L,"Grid3DAction", liquid);
}
int lua_cocos2dx_effect_Waves(lua_State* L)
{
	Waves* waves = Waves::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2), Lua_GetInt(L, 4), Lua_GetFloat(L, 5), Lua_GetBool(L, 6), Lua_GetBool(L, 7));
	return Lua_CreateRef(L, "Grid3DAction", waves);
}
int lua_cocos2dx_effect_Twirl(lua_State* L)
{
	Twirl* twirl = Twirl::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2), Lua_GetVec2(L, 4), Lua_GetInt(L, 5), Lua_GetFloat(L, 6));
	return Lua_CreateRef(L, "Grid3DAction", twirl);
}
int lua_cocos2dx_effect_ShakyTiles3D(lua_State* L)
{
	ShakyTiles3D* shaky = ShakyTiles3D::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2), Lua_GetInt(L, 4), Lua_GetBool(L, 5));
	return Lua_CreateRef(L, "Grid3DAction", shaky);
}
int lua_cocos2dx_effect_ShatteredTiles3D(lua_State* L)
{
	ShatteredTiles3D* shatter = ShatteredTiles3D::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2), Lua_GetInt(L, 4), Lua_GetBool(L, 5));
	return Lua_CreateRef(L, "Grid3DAction", shatter);
}
int lua_cocos2dx_effect_ShuffleTiles(lua_State* L)
{
	ShuffleTiles* shuffle = ShuffleTiles::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2), Lua_GetInt(L, 4));
	return Lua_CreateRef(L, "Grid3DAction", shuffle);
}
int lua_cocos2dx_effect_FadeOutTRTiles(lua_State* L)
{
	FadeOutTRTiles* fade = FadeOutTRTiles::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2));
	return Lua_CreateRef(L, "Grid3DAction", fade);
}
int lua_cocos2dx_effect_FadeOutBLTiles(lua_State* L)
{
	FadeOutBLTiles* fade = FadeOutBLTiles::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2));
	return Lua_CreateRef(L, "Grid3DAction", fade);
}
int lua_cocos2dx_effect_FadeOutUpTiles(lua_State* L)
{
	FadeOutUpTiles* fade = FadeOutUpTiles::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2));
	return Lua_CreateRef(L, "Grid3DAction", fade);
}
int lua_cocos2dx_effect_FadeOutDownTiles(lua_State* L)
{
	FadeOutDownTiles* fade = FadeOutDownTiles::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2));
	return Lua_CreateRef(L, "Grid3DAction", fade);
}
int lua_cocos2dx_effect_TurnOffTiles(lua_State* L)
{
	TurnOffTiles* turn = TurnOffTiles::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2));
	return Lua_CreateRef(L, "Grid3DAction", turn);
}
int lua_cocos2dx_effect_WavesTiles3D(lua_State* L)
{
	WavesTiles3D* waves = WavesTiles3D::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2), Lua_GetInt(L, 4), Lua_GetFloat(L, 5));
	return Lua_CreateRef(L, "Grid3DAction", waves);
}
int lua_cocos2dx_effect_JumpTiles3D(lua_State* L)
{
	JumpTiles3D* jump = JumpTiles3D::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2), Lua_GetInt(L, 4), Lua_GetFloat(L, 5));
	return Lua_CreateRef(L, "Grid3DAction", jump);
}
int lua_cocos2dx_effect_SplitRows(lua_State* L)
{
	SplitRows* split = SplitRows::create(Lua_GetFloat(L, 1), Lua_GetInt(L, 2));
	return Lua_CreateRef(L, "Grid3DAction", split);
}
int lua_cocos2dx_effect_SplitCols(lua_State* L)
{
	SplitCols* split = SplitCols::create(Lua_GetFloat(L, 1), Lua_GetInt(L, 2));
	return Lua_CreateRef(L, "Grid3DAction", split);
}

int lua_cocos2dx_effect_PageTurn3D(lua_State* L)
{
	PageTurn3D* pageTurn = PageTurn3D::create(Lua_GetFloat(L, 1), Lua_GetSize(L, 2));
	return Lua_CreateRef(L,"Grid3DAction",pageTurn);
}

int lua_open_effect_module(lua_State* L)
{
	luaL_Reg reg[] = {
		{"Shaky3D",lua_cocos2dx_effect_Shaky3D },
		{"Waves3D",lua_cocos2dx_effect_Waves3D },
		{"FlipX3D",lua_cocos2dx_effect_FlipX3D },
		{"FlipY3D",lua_cocos2dx_effect_FlipY3D },
		{"Lens3D",lua_cocos2dx_effect_Lens3D },
		{"Ripple3D",lua_cocos2dx_effect_Ripple3D },
		{"Liquid",lua_cocos2dx_effect_Liquid },
		{"Waves",lua_cocos2dx_effect_Waves },
		{"Twirl",lua_cocos2dx_effect_Twirl },
		{"ShakyTiles3D",lua_cocos2dx_effect_ShakyTiles3D },
		{"ShatteredTiles3D",lua_cocos2dx_effect_ShatteredTiles3D },
		{"ShuffleTiles",lua_cocos2dx_effect_ShuffleTiles },
		{"FadeOutTRTiles",lua_cocos2dx_effect_FadeOutTRTiles },
		{"FadeOutBLTiles",lua_cocos2dx_effect_FadeOutBLTiles },
		{"FadeOutUpTiles",lua_cocos2dx_effect_FadeOutUpTiles },
		{"FadeOutDownTiles",lua_cocos2dx_effect_FadeOutDownTiles },
		{"TurnOffTiles",lua_cocos2dx_effect_TurnOffTiles },
		{"WavesTiles3D",lua_cocos2dx_effect_WavesTiles3D },
		{"JumpTiles3D",lua_cocos2dx_effect_JumpTiles3D },
		{"SplitRows",lua_cocos2dx_effect_SplitRows },
		{"SplitCols",lua_cocos2dx_effect_SplitCols },
		{"PageTurn3D",lua_cocos2dx_effect_PageTurn3D },
		{NULL,NULL},
	};
	return Lua_CreateModule(L, reg);
}

/*******************************ActionEase*******************************/
int lua_cocos2dx_actionease_EaseIn(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseIn::create(action, Lua_GetFloat(L, 2));
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseOut::create(action, Lua_GetFloat(L, 2));
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseInOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseInOut::create(action, Lua_GetFloat(L, 2));
	return Lua_CreateRef(L,"Action", ease);
}
int lua_cocos2dx_actionease_EaseExponentialIn(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseExponentialIn::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseExponentialOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseExponentialOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseExponentialInOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseExponentialInOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseSineIn(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseSineIn::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseSineOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseSineOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseSineInOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseSineInOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseElasticIn(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseElasticIn::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseElasticOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseElasticOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseElasticInOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseElasticInOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseBounceIn(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseBounceIn::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseBounceOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseBounceOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseBounceInOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseBounceInOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseBackIn(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseBackIn::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseBackOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseBackOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseBackInOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseBackInOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}

int lua_cocos2dx_actionease_EaseBezierAction(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseBezierAction::create(action);
	return Lua_CreateRef(L, "Action", ease);
}

int lua_cocos2dx_actionease_EaseQuadraticActionIn(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseQuadraticActionIn::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseQuadraticActionOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseQuadraticActionOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseQuadraticActionInOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseQuadraticActionInOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseQuarticActionIn(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseQuarticActionIn::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseQuarticActionOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseQuarticActionOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseQuarticActionInOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseQuarticActionInOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseQuinticActionIn(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseQuinticActionIn::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseQuinticActionOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseQuinticActionOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_EaseQuinticActionInOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseQuinticActionInOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}

int lua_cocos2dx_actionease_EaseCircleActionIn(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseCircleActionIn::create(action);
	return Lua_CreateRef(L, "Action", ease);
}

int lua_cocos2dx_actionease_EaseCircleActionOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseCircleActionOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}

int lua_cocos2dx_actionease_EaseCircleActionInOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseCircleActionInOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}

int lua_cocos2dx_actionease_EaseCubicActionIn(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseCubicActionIn::create(action);
	return Lua_CreateRef(L, "Action", ease);
}

int lua_cocos2dx_actionease_EaseCubicActionOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseCubicActionOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}

int lua_cocos2dx_actionease_EaseCubicActionInOut(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = EaseCubicActionInOut::create(action);
	return Lua_CreateRef(L, "Action", ease);
}
int lua_cocos2dx_actionease_Speed(lua_State* L)
{
	ActionInterval* action = Lua_GetPointer<ActionInterval>(L, 1);
	if (!action) return 0;
	auto ease = Speed::create(action,Lua_GetFloat(L,2));
	return Lua_CreateRef(L, "Action", ease);
}
int lua_open_actionease_module(lua_State* L)
{
	luaL_Reg reg[] = {
		{"EaseIn",lua_cocos2dx_actionease_EaseIn },
		{"EaseOut",lua_cocos2dx_actionease_EaseOut },
		{"EaseInOut",lua_cocos2dx_actionease_EaseInOut },
		{"EaseExponentialIn",lua_cocos2dx_actionease_EaseExponentialIn },
		{"EaseExponentialOut",lua_cocos2dx_actionease_EaseExponentialOut },
		{"EaseExponentialInOut",lua_cocos2dx_actionease_EaseExponentialInOut },
		{"EaseSineIn",lua_cocos2dx_actionease_EaseSineIn },
		{"EaseSineOut",lua_cocos2dx_actionease_EaseSineOut },
		{"EaseSineInOut",lua_cocos2dx_actionease_EaseSineInOut },
		{"EaseElasticIn",lua_cocos2dx_actionease_EaseElasticIn },
		{"EaseElasticOut",lua_cocos2dx_actionease_EaseElasticOut },
		{"EaseElasticInOut",lua_cocos2dx_actionease_EaseElasticInOut },
		{"EaseBounceIn",lua_cocos2dx_actionease_EaseBounceIn },
		{"EaseBounceOut",lua_cocos2dx_actionease_EaseBounceOut },
		{"EaseBounceInOut",lua_cocos2dx_actionease_EaseBounceInOut },
		{"EaseBackIn",lua_cocos2dx_actionease_EaseBackIn },
		{"EaseBackOut",lua_cocos2dx_actionease_EaseBackOut },
		{"EaseBackInOut",lua_cocos2dx_actionease_EaseBackInOut },
		{"EaseBezierAction",lua_cocos2dx_actionease_EaseBezierAction },
		{"EaseQuadraticActionIn",lua_cocos2dx_actionease_EaseQuadraticActionIn },
		{"EaseQuadraticActionOut",lua_cocos2dx_actionease_EaseQuadraticActionOut },
		{"EaseQuadraticActionInOut",lua_cocos2dx_actionease_EaseQuadraticActionInOut },
		{"EaseQuarticActionIn",lua_cocos2dx_actionease_EaseQuarticActionIn },
		{"EaseQuarticActionOut",lua_cocos2dx_actionease_EaseQuarticActionOut },
		{"EaseQuarticActionInOut",lua_cocos2dx_actionease_EaseQuarticActionInOut },
		{"EaseQuinticActionIn",lua_cocos2dx_actionease_EaseQuinticActionIn },
		{"EaseQuinticActionOut",lua_cocos2dx_actionease_EaseQuinticActionOut },
		{"EaseQuinticActionInOut",lua_cocos2dx_actionease_EaseQuinticActionInOut },
		{"EaseCircleActionIn",lua_cocos2dx_actionease_EaseCircleActionIn },
		{"EaseCircleActionOut",lua_cocos2dx_actionease_EaseCircleActionOut },
		{"EaseCircleActionInOut",lua_cocos2dx_actionease_EaseCircleActionInOut },
		{"EaseCubicActionIn",lua_cocos2dx_actionease_EaseCubicActionIn },
		{"EaseCubicActionOut",lua_cocos2dx_actionease_EaseCubicActionOut },
		{"EaseCubicActionInOut",lua_cocos2dx_actionease_EaseCubicActionInOut },
		{"EaseSpeed",lua_cocos2dx_actionease_Speed },
		{NULL,NULL}
	};
	return Lua_CreateModule(L, reg);
}

/***************************ActionManager********************************/

int lua_cocos2dx_ActionManager_addAction(lua_State* L)
{
	ActionManager* mgr = Lua_GetPointer<ActionManager>(L, 1);
	if (!mgr) return 0;
	Action* action = Lua_GetPointer<Action>(L, 2);
	if (!action) return 0;
	Node* node = Lua_GetPointer<Node>(L, 3);
	if (!node) return 0;
	bool paused = Lua_GetBool(L, 4);
	mgr->addAction(action, node, paused);
	return 0;
}

int lua_cocos2dx_ActionManager_removeAllActions(lua_State* L)
{
	ActionManager* mgr = Lua_GetPointer<ActionManager>(L, 1);
	if (!mgr) return 0;
	mgr->removeAllActions();
	return 0;
}

int lua_cocos2dx_ActionManager_removeAllActionsFromTarget(lua_State* L)
{
	ActionManager* mgr = Lua_GetPointer<ActionManager>(L, 1);
	if (!mgr) return 0;
	Node* target = Lua_GetPointer<Node>(L, 2);
	if (!target) return 0;
	mgr->removeAllActionsFromTarget(target);
	return 0;
}

int lua_cocos2dx_ActionManager_removeAction(lua_State* L)
{
	ActionManager* mgr = Lua_GetPointer<ActionManager>(L, 1);
	if (!mgr) return 0;
	Action* action = Lua_GetPointer<Action>(L, 2);
	if (!action) return 0;
	mgr->removeAction(action);
	return 0;
}
int lua_cocos2dx_ActionManager_removeActionByTag(lua_State* L)
{
	ActionManager* mgr = Lua_GetPointer<ActionManager>(L, 1);
	if (!mgr) return 0;
	int tag = Lua_GetInt(L, 2);
	Node* node = Lua_GetPointer<Node>(L, 3);
	if (!node) return 0;
	mgr->removeActionByTag(tag, node);
	return 0;
}
int lua_cocos2dx_ActionManager_getActionByTag(lua_State* L)
{
	ActionManager* mgr = Lua_GetPointer<ActionManager>(L, 1);
	if (!mgr) return 0;
	int tag = Lua_GetInt(L, 2);
	Node* node = Lua_GetPointer<Node>(L, 3);
	if (!node) return 0;
	Action* action = mgr->getActionByTag(tag, node);
	return Lua_SetPointer(L, action,"Action");
}
int lua_cocos2dx_ActionManager_getNumberOfRunningActionsInTarget(lua_State* L)
{
	ActionManager* mgr = Lua_GetPointer<ActionManager>(L, 1);
	if (!mgr) return 0;
	Node* target = Lua_GetPointer<Node>(L, 2);
	if (!target) return 0;
	return Lua_SetInt(L,mgr->getNumberOfRunningActionsInTarget(target));
}
int lua_cocos2dx_ActionManager_getNumberOfRunningActions(lua_State* L)
{
	ActionManager* mgr = Lua_GetPointer<ActionManager>(L, 1);
	if (!mgr) return 0;
	return Lua_SetInt(L, mgr->getNumberOfRunningActions());
}
int lua_cocos2dx_ActionManager_pauseTarget(lua_State* L)
{
	ActionManager* mgr = Lua_GetPointer<ActionManager>(L, 1);
	if (!mgr) return 0;
	Node* node = Lua_GetPointer<Node>(L, 2);
	if (!node) return 0;
	mgr->pauseTarget(node);
	return 0;
}
int lua_cocos2dx_ActionManager_resumeTarget(lua_State* L)
{
	ActionManager* mgr = Lua_GetPointer<ActionManager>(L, 1);
	if (!mgr) return 0;
	Node* node = Lua_GetPointer<Node>(L, 2);
	if (!node) return 0;
	mgr->resumeTarget(node);
	return 0;
}

int lua_cocos2dx_open_ActionManager(lua_State* L)
{
	luaL_Reg  reg[] = {
		{"addAction",lua_cocos2dx_ActionManager_addAction },
		{"removeAllActions",lua_cocos2dx_ActionManager_removeAllActions },
		{"removeAllActionsFromTarget",lua_cocos2dx_ActionManager_removeAllActionsFromTarget },
		{"removeAction",lua_cocos2dx_ActionManager_removeAction },
		{"removeActionByTag",lua_cocos2dx_ActionManager_removeActionByTag },
		{"getActionByTag",lua_cocos2dx_ActionManager_getActionByTag },
		{"getNumberOfRunningActionsInTarget",lua_cocos2dx_ActionManager_getNumberOfRunningActionsInTarget },
		{"getNumberOfRunningActions",lua_cocos2dx_ActionManager_getNumberOfRunningActions },
		{"pauseTarget",lua_cocos2dx_ActionManager_pauseTarget },
		{"resumeTarget",lua_cocos2dx_ActionManager_resumeTarget },
		{NULL,NULL}
	};
	return Lua_CreateModule(L, "ActionManager", "Ref", reg, Director::getInstance()->getActionManager());
}
/**************************ProgressTimer*******************************/
int lua_cocos2dx_ProgressTimer_create(lua_State* L)
{
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 2);
	if (!sprite) return 0;
	ProgressTimer* timer = ProgressTimer::create(sprite);
	return Lua_CreateRef(L, "ProgressTimer", timer);
}
int lua_cocos2dx_ProgressTimer_getType(lua_State* L)
{
	ProgressTimer* timer = Lua_GetPointer<ProgressTimer>(L, 1);
	if (!timer) return 0;
	return Lua_SetInt(L, (int)timer->getType());
}
int lua_cocos2dx_ProgressTimer_getPercentage(lua_State* L)
{
	ProgressTimer* timer = Lua_GetPointer<ProgressTimer>(L, 1);
	if (!timer) return 0;
	return Lua_SetFloat(L, timer->getPercentage());
}
int lua_cocos2dx_ProgressTimer_getSprite(lua_State* L)
{
	ProgressTimer* timer = Lua_GetPointer<ProgressTimer>(L, 1);
	if (!timer) return 0;
	return Lua_SetPointer(L, timer->getSprite(),"ProgressTimer");
}
int lua_cocos2dx_ProgressTimer_setPercentage(lua_State* L)
{
	ProgressTimer* timer = Lua_GetPointer<ProgressTimer>(L, 1);
	if (!timer) return 0;
	timer->setPercentage(Lua_GetFloat(L, 2));
	return 0;
}
int lua_cocos2dx_ProgressTimer_setSprite(lua_State* L)
{
	ProgressTimer* timer = Lua_GetPointer<ProgressTimer>(L, 1);
	if (!timer) return 0;
	Sprite* sprite = Lua_GetPointer<Sprite>(L, 2);
	if (!sprite) return 0;
	timer->setSprite(sprite);
	return 0;
}
int lua_cocos2dx_ProgressTimer_setType(lua_State* L)
{
	ProgressTimer* timer = Lua_GetPointer<ProgressTimer>(L, 1);
	if (!timer) return 0;
	timer->setType((ProgressTimer::Type) Lua_GetInt(L, 2));
	return 0;
}
int lua_cocos2dx_ProgressTimer_isReverseDirection(lua_State* L)
{
	ProgressTimer* timer = Lua_GetPointer<ProgressTimer>(L, 1);
	if (!timer) return 0;
	return Lua_SetBool(L, timer->isReverseDirection());
}
int lua_cocos2dx_ProgressTimer_setReverseDirection(lua_State* L)
{
	ProgressTimer* timer = Lua_GetPointer<ProgressTimer>(L, 1);
	if (!timer) return 0;
	timer->setReverseProgress(Lua_GetBool(L, 2));
	return 0;
}
int lua_cocos2dx_ProgressTimer_setMidpoint(lua_State* L)
{
	ProgressTimer* timer = Lua_GetPointer<ProgressTimer>(L, 1);
	if (!timer) return 0;
	timer->setMidpoint(Lua_GetVec2(L, 2));
	return 0;
}
int lua_cocos2dx_ProgressTimer_getMidpoint(lua_State* L)
{
	ProgressTimer* timer = Lua_GetPointer<ProgressTimer>(L, 1);
	if (!timer) return 0;
	return Lua_SetVec2(L, timer->getMidpoint());
}
int lua_cocos2dx_ProgressTimer_setBarChangeRate(lua_State* L)
{
	ProgressTimer* timer = Lua_GetPointer<ProgressTimer>(L, 1);
	if (!timer) return 0;
	timer->setBarChangeRate(Lua_GetVec2(L, 2));
	return 0;
}
int lua_cocos2dx_ProgressTimer_getBarChangeRate(lua_State* L)
{
	ProgressTimer* timer = Lua_GetPointer<ProgressTimer>(L, 1);
	if (!timer) return 0;
	return Lua_SetVec2(L, timer->getBarChangeRate());
}
int lua_cocos2dx_open_ProgressTimer(lua_State* L)
{
	luaL_Reg reg[] = {
		{"create", lua_cocos2dx_ProgressTimer_create },
		{"getType",lua_cocos2dx_ProgressTimer_getType },
		{"getPercentage",lua_cocos2dx_ProgressTimer_getPercentage },
		{"getSprite",lua_cocos2dx_ProgressTimer_getSprite },
		{"setPercentage",lua_cocos2dx_ProgressTimer_setPercentage },
		{"setSprite",lua_cocos2dx_ProgressTimer_setSprite },
		{"setType",lua_cocos2dx_ProgressTimer_setType },
		{"isReverseDirection",lua_cocos2dx_ProgressTimer_isReverseDirection },
		{"setReverseDirection",lua_cocos2dx_ProgressTimer_setReverseDirection },
		{"setMidpoint",lua_cocos2dx_ProgressTimer_setMidpoint },
		{"getMidpoint",lua_cocos2dx_ProgressTimer_getMidpoint },
		{"setBarChangeRate",lua_cocos2dx_ProgressTimer_setBarChangeRate },
		{"getBarChangeRate",lua_cocos2dx_ProgressTimer_getBarChangeRate },
		{NULL,NULL}
	};
	Lua_CreateModule(L, "ProgressTimer", "Node", reg);
	luaL_Enum em[] = {
		{"RADIAL",(int)ProgressTimer::Type::RADIAL },
		{"BAR",(int)ProgressTimer::Type::BAR},
		{NULL,NULL}
	};
	Lua_AddConstVariable(L, "ProgressTimer", em);
	return 1;
}
