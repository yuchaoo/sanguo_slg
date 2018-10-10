/****************************************************************************
 Copyright (c) 2012      cocos2d-x.org
 Copyright (c) 2013-2017 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "cocos2d.h"
#include "LuaEngine.h"
#include "LuaBind.h"
NS_CC_BEGIN

LuaEngine* LuaEngine::_defaultEngine = nullptr;

LuaEngine* LuaEngine::getInstance(void)
{
	static LuaEngine engine;
	return &engine;
}

LuaEngine::~LuaEngine(void)
{
    CC_SAFE_RELEASE(_stack);
    _defaultEngine = nullptr;
}

bool LuaEngine::init(void)
{
    _stack = LuaStack::create();
    _stack->retain();
    return true;
}

void LuaEngine::addSearchPath(const char* path)
{
    _stack->addSearchPath(path);
}

void LuaEngine::addLuaLoader(lua_CFunction func)
{
    _stack->addLuaLoader(func);
}

void LuaEngine::removeScriptObjectByObject(Ref* pObj)
{

}

void LuaEngine::removeScriptHandler(int nHandler)
{
}

int LuaEngine::executeString(const char *codes)
{
    int ret = _stack->executeString(codes);
    _stack->clean();
    return ret;
}

int LuaEngine::executeScriptFile(const char* filename)
{
    int ret = _stack->executeScriptFile(filename);
    _stack->clean();
    return ret;
}

int LuaEngine::executeGlobalFunction(const char* functionName)
{
    int ret = _stack->executeGlobalFunction(functionName);
    _stack->clean();
    return ret;
}

int LuaEngine::executeNodeEvent(Node* pNode, int nAction)
{
    return 0;
}

int LuaEngine::executeMenuItemEvent(MenuItem* pMenuItem)
{
    return 0;
}

int LuaEngine::executeNotificationEvent(__NotificationCenter* pNotificationCenter, const char* pszName)
{
	return 0;
}

int LuaEngine::executeCallFuncActionEvent(CallFunc* pAction, Ref* pTarget/* = NULL*/)
{
    return 0;
}

int LuaEngine::executeSchedule(int nHandler, float dt, Node* pNode/* = NULL*/)
{
	return 0;
}

int LuaEngine::executeLayerTouchEvent(Layer* pLayer, int eventType, Touch *pTouch)
{
    return 0;
}

int LuaEngine::executeLayerTouchesEvent(Layer* pLayer, int eventType, __Set *pTouches)
{
    return 0;
}

int LuaEngine::executeLayerKeypadEvent(Layer* pLayer, int eventType)
{
    return 0;
}

int LuaEngine::executeAccelerometerEvent(Layer* pLayer, Acceleration* pAccelerationValue)
{
    return 0;
}

int LuaEngine::executeEvent(int nHandler, const char* pEventName, Ref* pEventSource /* = NULL*/, const char* pEventSourceClassName /* = NULL*/)
{
	return 0;
}

bool LuaEngine::handleAssert(const char *msg)
{
    bool ret = _stack->handleAssert(msg);
    _stack->clean();
    return ret;
}

int LuaEngine::reallocateScriptHandler(int nHandler)
{    
	return 0;
}

bool LuaEngine::parseConfig(ConfigType type, const std::string& str)
{
	return true;
}

int LuaEngine::sendEvent(ScriptEvent* evt)
{
    if (NULL == evt)
        return 0;
    
    switch (evt->type)
    {
        case kNodeEvent:
            {
               return handleNodeEvent(evt->data);
            }
            break;
        case kMenuClickedEvent:
            {
                return handleMenuClickedEvent(evt->data);
            }
            break;
        case kCallFuncEvent:
            {
                return handleCallFuncActionEvent(evt->data);
            }
            break;
        case kScheduleEvent:
            {
                return handleScheduler(evt->data);
            }
            break;
        case kTouchEvent:
            {
                return handleTouchEvent(evt->data);
            }
            break;
        case kTouchesEvent:
            {
                return handleTouchesEvent(evt->data);
            }
            break;
        case kKeypadEvent:
            {
                return handleKeypadEvent(evt->data);
            }
            break;
        case kAccelerometerEvent:
            {
                return handleAccelerometerEvent(evt->data);
            }
            break;
        case kCommonEvent:
            {
                return handleCommonEvent(evt->data);
            }
            break;
        case kControlEvent:
            {
                return handlerControlEvent(evt->data);
            }
            break;
        default:
            break;
    }
    
    return 0;
}

bool LuaEngine::getTableFunc(int refId, const char* func)
{
	if (lua_rawgeti(_stack->_state, LUA_REGISTRYINDEX, refId) != LUA_TTABLE)
		return false;
	if (lua_getfield(_stack->_state, -1, func) != LUA_TFUNCTION)
		return false;
	lua_insert(_stack->_state, -2);
	return true;
}

bool LuaEngine::getModuleFunc(const char* module, const char* func)
{
	lua_pushstring(_stack->_state, module);
	lua_rawget(_stack->_state, LUA_REGISTRYINDEX);
	lua_getfield(_stack->_state, -1, func);
	return lua_isfunction(_stack->_state, -1);
}

bool LuaEngine::callLuaFunc(int argnum, int retnum)
{
	if (lua_isfunction(_stack->_state, -argnum - 1))
	{
		return false;
	}
	int ret = lua_pcall(_stack->_state, argnum, retnum, 0);
	if (ret != LUA_OK)
	{
		const char* msg = lua_tostring(_stack->_state, -1);
		CCLOG("call lua func failed!!!, msg:%s", msg);
		return false;
	}
	return true;
}

void LuaEngine::recoveryStack(int oldtop)
{
	int top = lua_gettop(_stack->_state);
	if (top > oldtop)
	{
		lua_pop(_stack->_state, top - oldtop);
	}
}

int LuaEngine::handleNodeOnEnter(Ref* ref)
{
	if (ref == NULL || ref->_luaID == 0)
		return 0;
	int old = lua_gettop(_stack->_state);
	if (!getTableFunc(ref->_luaID, "onEnter"))
	{
		recoveryStack(old);
		return 0;
	}
	if (!callLuaFunc(1, 0))
	{
		recoveryStack(old);
		return 0;
	}
	return 0;
}

int LuaEngine::handleNodeOnExit(Ref* ref)
{
	if (ref == NULL || ref->_luaID == 0)
		return 0;
	int old = lua_gettop(_stack->_state);
	if (!getTableFunc(ref->_luaID, "onExit"))
	{
		recoveryStack(old);
		return 0;
	}
	if (!callLuaFunc(1, 0))
	{
		recoveryStack(old);
		return 0;
	}
	return 0;
}

int LuaEngine::handleOnTouchBegin(TouchScriptData* touchData)
{
	if (touchData == NULL || touchData->nativeObject == NULL)
		return 0;
	Ref* ref = (Ref*)touchData->nativeObject;
	if (ref->_luaID == 0)
		return 0;

	int oldtop = lua_gettop(_stack->_state);
	Touch* touch = touchData->touch;
	if (!getTableFunc(ref->_luaID, "onTouchBegan"))
	{
		recoveryStack(oldtop);
		return 0;
	}
	Lua_SetTouch(_stack->_state, *touch);
	if (!callLuaFunc(2, 0))
	{
		recoveryStack(oldtop);
		return 0;
	}
	return 0;
}

int LuaEngine::handleOnTouchMoved(TouchScriptData* touchData)
{
	if (touchData == NULL || touchData->nativeObject == NULL)
		return 0;
	Ref* ref = (Ref*)touchData->nativeObject;
	if (ref->_luaID == 0)
		return 0;

	int oldtop = lua_gettop(_stack->_state);
	Touch* touch = touchData->touch;
	if (!getTableFunc(ref->_luaID, "onTouchMoved"))
	{
		recoveryStack(oldtop);
		return 0;
	}
	Lua_SetTouch(_stack->_state, *touch);
	if (!callLuaFunc(2, 0))
	{
		recoveryStack(oldtop);
		return 0;
	}
	return 0;
}

int LuaEngine::handleOnTouchEnded(TouchScriptData* touchData)
{
	if (touchData == NULL || touchData->nativeObject == NULL)
		return 0;
	Ref* ref = (Ref*)touchData->nativeObject;
	if (ref->_luaID == 0)
		return 0;

	int oldtop = lua_gettop(_stack->_state);
	Touch* touch = touchData->touch;
	if (!getTableFunc(ref->_luaID, "onTouchEnded"))
	{
		recoveryStack(oldtop);
		return 0;
	}
	Lua_SetTouch(_stack->_state, *touch);
	if (!callLuaFunc(2, 0))
	{
		recoveryStack(oldtop);
		return 0;
	}
	return 0;
}

int LuaEngine::handleOnTouchCancel(TouchScriptData* touchData)
{
	if (touchData == NULL || touchData->nativeObject == NULL )
		return 0;
	Ref* ref = (Ref*)touchData->nativeObject;
	if (ref->_luaID == 0)
		return 0;

	int oldtop = lua_gettop(_stack->_state);
	Touch* touch = touchData->touch;
	if (!getTableFunc(ref->_luaID, "onTouchCancel"))
	{
		recoveryStack(oldtop);
		return 0;
	}
	Lua_SetTouch(_stack->_state, *touch);
	if (!callLuaFunc(2, 0))
	{
		recoveryStack(oldtop);
		return 0;
	}
	return 0;
}

int LuaEngine::handleOnTouchesBegan(TouchesScriptData* touchesData)
{
	if (touchesData == NULL || touchesData->nativeObject == NULL)
		return 0;
	Ref * ref = (Ref*) touchesData->nativeObject;
	if (ref->_luaID == 0)
		return 0;
	int oldtop = lua_gettop(_stack->_state);
	if (!getTableFunc(ref->_luaID, "onTouchesBegan"))
	{
		recoveryStack(oldtop);
		return 0;
	}
	Lua_SetTouches(_stack->_state, touchesData->touches);
	if (!callLuaFunc(2, 0))
	{
		recoveryStack(oldtop);
		return 0;
	}
	return 0;
}

int LuaEngine::handleOnTouchesMoved(TouchesScriptData* touchesData)
{
	if (touchesData == NULL || touchesData->nativeObject == NULL)
		return 0;
	Ref * ref = (Ref*)touchesData->nativeObject;
	if (ref->_luaID == 0)
		return 0;
	int oldtop = lua_gettop(_stack->_state);
	if (!getTableFunc(ref->_luaID, "onTouchesMoved"))
	{
		recoveryStack(oldtop);
		return 0;
	}
	Lua_SetTouches(_stack->_state, touchesData->touches);
	if (!callLuaFunc(2, 0))
	{
		recoveryStack(oldtop);
		return 0;
	}
	return 0;
}

int LuaEngine::handleOnTouchesEnded(TouchesScriptData* touchesData)
{
	if (touchesData == NULL || touchesData->nativeObject == NULL)
		return 0;
	Ref * ref = (Ref*)touchesData->nativeObject;
	if (ref->_luaID == 0)
		return 0;
	int oldtop = lua_gettop(_stack->_state);
	if (!getTableFunc(ref->_luaID, "onTouchesEnded"))
	{
		recoveryStack(oldtop);
		return 0;
	}
	Lua_SetTouches(_stack->_state, touchesData->touches);
	if (!callLuaFunc(2, 0))
	{
		recoveryStack(oldtop);
		return 0;
	}
	return 0;
}

int LuaEngine::handleOnTouchesCancel(TouchesScriptData* touchesData)
{
	if (touchesData == NULL || touchesData->nativeObject == NULL)
		return 0;
	Ref * ref = (Ref*)touchesData->nativeObject;
	if (ref->_luaID == 0)
		return 0;
	int oldtop = lua_gettop(_stack->_state);
	if (!getTableFunc(ref->_luaID, "onTouchesCancel"))
	{
		recoveryStack(oldtop);
		return 0;
	}
	Lua_SetTouches(_stack->_state, touchesData->touches);
	if (!callLuaFunc(2, 0))
	{
		recoveryStack(oldtop);
		return 0;
	}
	return 0;
}

int LuaEngine::handleNodeEvent(void* data)
{
    if (NULL == data)
        return 0;
    
    BasicScriptData* basicScriptData = (BasicScriptData*)data;
    if (NULL == basicScriptData->nativeObject || NULL == basicScriptData->value)
        return 0;
    
    int action = *((int*)(basicScriptData->value));
    switch (action)
    {
        case kNodeOnEnter:
			handleNodeOnEnter((Ref*)basicScriptData->nativeObject);
            break;
            
        case kNodeOnExit:
			handleNodeOnExit((Ref*)basicScriptData->nativeObject);
            break;
            
        case kNodeOnEnterTransitionDidFinish:
            break;
            
        case kNodeOnExitTransitionDidStart:
            break;
            
        case kNodeOnCleanup:
            break;
            
        default:
            return 0;
    }
    return 0;
}

int LuaEngine::handleMenuClickedEvent(void* data)
{
    return 0;
}

int LuaEngine::handleCallFuncActionEvent(void* data)
{
	return 0;
} 

int LuaEngine::handleScheduler(void* data)
{
    return 0;
}

int LuaEngine::handleKeypadEvent(void* data)
{
	return 0;
}

int LuaEngine::handleAccelerometerEvent(void* data)
{
	return 0;
}

int LuaEngine::handleCommonEvent(void* data)
{
	return 0;
}

int LuaEngine::handleTouchEvent(void* data)
{
    if (NULL == data)
        return 0;
    
    TouchScriptData* touchScriptData = static_cast<TouchScriptData*>(data);
    if (NULL == touchScriptData->nativeObject || NULL == touchScriptData->touch)
        return 0;
    
    switch (touchScriptData->actionType)
    {
        case EventTouch::EventCode::BEGAN:
			handleOnTouchBegin(touchScriptData);
            break;
            
        case EventTouch::EventCode::MOVED:
			handleOnTouchMoved(touchScriptData);
            break;
            
        case EventTouch::EventCode::ENDED:
			handleOnTouchEnded(touchScriptData);
            break;
            
        case EventTouch::EventCode::CANCELLED:
			handleOnTouchCancel(touchScriptData);
            break;

		default:
			return 0;
    }
    return 0;
}

int LuaEngine::handleTouchesEvent(void* data)
{
    if (NULL == data)
        return 0;
    
    TouchesScriptData* touchesScriptData = static_cast<TouchesScriptData*>(data);
    if (NULL == touchesScriptData->nativeObject || touchesScriptData->touches.size() == 0)
        return 0;
    
    switch (touchesScriptData->actionType)
    {
        case EventTouch::EventCode::BEGAN:
			handleOnTouchesBegan(touchesScriptData);
            break;
            
        case EventTouch::EventCode::MOVED:
			handleOnTouchesMoved(touchesScriptData);
            break;
            
        case EventTouch::EventCode::ENDED:
			handleOnTouchesEnded(touchesScriptData);
            break;
            
        case EventTouch::EventCode::CANCELLED:
			handleOnTouchesCancel(touchesScriptData);
            break;
            
        default:
            return 0;
    }
	return 0;
}

int LuaEngine::handlerControlEvent(void* data)
{
    return 0;    
}

int LuaEngine::handleEventAcc(void* data)
{
    return 0;
}

int LuaEngine::handleEventKeyboard(ScriptHandlerMgr::HandlerType type, void* data)
{
    return 0;
}

int LuaEngine::handleEventTouch(ScriptHandlerMgr::HandlerType type, void* data)
{   
    return 0;
}

int LuaEngine::handleEventTouches(ScriptHandlerMgr::HandlerType type,void* data)
{
    return 0;
}

int LuaEngine::handleEventMouse(ScriptHandlerMgr::HandlerType type, void* data)
{
    return 0;
}

int LuaEngine::handleEvenCustom(void* data)
{
    return 0;
}

int LuaEngine::handleEvent(ScriptHandlerMgr::HandlerType type,void* data)
{
    switch (type)
    {
        case ScriptHandlerMgr::HandlerType::SCROLLVIEW_SCROLL:
        case ScriptHandlerMgr::HandlerType::SCROLLVIEW_ZOOM:
        case ScriptHandlerMgr::HandlerType::TABLECELL_TOUCHED:
        case ScriptHandlerMgr::HandlerType::TABLECELL_HIGHLIGHT:
        case ScriptHandlerMgr::HandlerType::TABLECELL_UNHIGHLIGHT:
        case ScriptHandlerMgr::HandlerType::TABLECELL_WILL_RECYCLE:
            {
                return handleTableViewEvent(type, data);
            }
            break;
        case ScriptHandlerMgr::HandlerType::ASSETSMANAGER_PROGRESS:
        case ScriptHandlerMgr::HandlerType::ASSETSMANAGER_ERROR:
        case ScriptHandlerMgr::HandlerType::ASSETSMANAGER_SUCCESS:
            {
                return handleAssetsManagerEvent(type, data);
            }
            break;
        case ScriptHandlerMgr::HandlerType::ARMATURE_EVENT:
            {
                return handleArmatureWrapper(type, data);
            }
            break;
        case ScriptHandlerMgr::HandlerType::EVENT_ACC:
            {
                return handleEventAcc(data);
            }
            break;
        case ScriptHandlerMgr::HandlerType::EVENT_KEYBOARD_PRESSED:
        case ScriptHandlerMgr::HandlerType::EVENT_KEYBOARD_RELEASED:
            {
                return handleEventKeyboard(type,data);
            }
            break;
        case ScriptHandlerMgr::HandlerType::EVENT_CUSTIOM:
            {
                return handleEvenCustom(data);
            }
            break;
        case ScriptHandlerMgr::HandlerType::EVENT_TOUCH_BEGAN:
        case ScriptHandlerMgr::HandlerType::EVENT_TOUCH_MOVED:
        case ScriptHandlerMgr::HandlerType::EVENT_TOUCH_ENDED:
        case ScriptHandlerMgr::HandlerType::EVENT_TOUCH_CANCELLED:
            {
                return handleEventTouch(type, data);
            }
            break;
        case ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_BEGAN:
        case ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_MOVED:
        case ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_ENDED:
        case ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_CANCELLED:
            {
                return handleEventTouches(type, data);
            }
            break;
        case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_DOWN:
        case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_UP:
        case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_MOVE:
        case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_SCROLL:
            {
                return handleEventMouse(type, data);
            }
            break;
        default:
            break;
    }
    
    return 0;
}

int LuaEngine::handleEvent(ScriptHandlerMgr::HandlerType type, void* data, int numResults, const std::function<void(lua_State*,int)>& func)
{
    switch (type)
    {
        case ScriptHandlerMgr::HandlerType::TABLECELL_SIZE_FOR_INDEX:
        case ScriptHandlerMgr::HandlerType::TABLECELL_AT_INDEX:
        case ScriptHandlerMgr::HandlerType::TABLEVIEW_NUMS_OF_CELLS:
            {
                return handleTableViewEvent(type, data, numResults,func);
            }
            break;
        default:
            break;
    }
    
    return 0;
}

int LuaEngine::handleTableViewEvent(ScriptHandlerMgr::HandlerType type,void* data)
{
    return 0;
}

int LuaEngine::handleTableViewEvent(ScriptHandlerMgr::HandlerType handlerType,void* data, int numResults, const std::function<void(lua_State*,int)>& func)
{
    return 0;
}

int LuaEngine::handleAssetsManagerEvent(ScriptHandlerMgr::HandlerType type,void* data)
{
    return 0;
}

int LuaEngine::handleArmatureWrapper(ScriptHandlerMgr::HandlerType type,void* data)
{
    return 0;
}

int LuaEngine::reload(const char* moduleFileName)
{
    return _stack->reload(moduleFileName);
}

NS_CC_END
