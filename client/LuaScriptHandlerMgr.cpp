/****************************************************************************
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
#include "LuaScriptHandlerMgr.h"
#include <map>
#include <string>

#include "LuaStack.h"
#include "LuaEngine.h"

using namespace cocos2d;

NS_CC_BEGIN

ScheduleHandlerDelegate* ScheduleHandlerDelegate::create()
{
    ScheduleHandlerDelegate *ret = new (std::nothrow) ScheduleHandlerDelegate();
    if (nullptr != ret )
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

void ScheduleHandlerDelegate::scheduleFunc(float elapse)
{

}

void ScheduleHandlerDelegate::update(float elapse)
{

}


LuaCallFunc * LuaCallFunc::create(const std::function<void(void* ,Node*)>& func)
{
    auto ret = new (std::nothrow) LuaCallFunc();
    
    if (ret && ret->initWithFunction(func) ) {
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void LuaCallFunc::execute()
{
    if (_functionLua)
    {
        _functionLua((void*)this,_target);
    }
    else
    {
        CallFuncN::execute();
    }
}

bool LuaCallFunc::initWithFunction(const std::function<void (void*, Node*)> &func)
{
    _functionLua = func;
    return true;
}

LuaCallFunc* LuaCallFunc::clone() const
{
    int handler =  ScriptHandlerMgr::getInstance()->getObjectHandler((void*)this, ScriptHandlerMgr::HandlerType::CALLFUNC);
    
    if (0 == handler)
        return nullptr;
    
    auto ret = new (std::nothrow) LuaCallFunc();
    
    if( _functionLua )
        ret->initWithFunction(_functionLua);
    
    ret->autorelease();

    int newscriptHandler = cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->reallocateScriptHandler(handler);
    
    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)ret, newscriptHandler, ScriptHandlerMgr::HandlerType::CALLFUNC);
        
    return ret;
}

ScriptHandlerMgr* ScriptHandlerMgr::_scriptHandlerMgr = NULL;

ScriptHandlerMgr::ScriptHandlerMgr()
{
    
}

ScriptHandlerMgr::~ScriptHandlerMgr()
{
}

ScriptHandlerMgr* ScriptHandlerMgr::getInstance()
{
    if (nullptr == _scriptHandlerMgr)
    {
        _scriptHandlerMgr = new (std::nothrow) ScriptHandlerMgr();
        _scriptHandlerMgr->init();
    }
    return _scriptHandlerMgr;
}

void ScriptHandlerMgr::destroyInstance()
{
    CC_SAFE_DELETE(_scriptHandlerMgr);
}

void ScriptHandlerMgr::init()
{
    _mapObjectHandlers.clear();
}

void ScriptHandlerMgr::addObjectHandler(void* object,int handler,ScriptHandlerMgr::HandlerType handlerType)
{
    if (nullptr == object)
        return;
    
    //may be not need
    removeObjectHandler(object,handlerType);
    
    auto iter = _mapObjectHandlers.find(object);
    VecHandlerPairs vecHandlers;
    vecHandlers.clear();
    if (_mapObjectHandlers.end() != iter)
        vecHandlers = iter->second;
    
    HandlerPair eventHanler = std::make_pair(handlerType, handler);
    vecHandlers.push_back(eventHanler);
    _mapObjectHandlers[object] = vecHandlers;
}
void ScriptHandlerMgr::removeObjectHandler(void* object,ScriptHandlerMgr::HandlerType handlerType)
{
    if (nullptr == object || _mapObjectHandlers.empty())
        return;
    
    auto iterMap = _mapObjectHandlers.find(object);
    if (_mapObjectHandlers.end() == iterMap)
        return;
    
    if (iterMap->second.empty())
        return;
    
    auto iterVec = iterMap->second.begin();
    for (; iterVec != iterMap->second.end(); ++iterVec)
    {
        if (iterVec->first == handlerType)
        {
            LuaEngine::getInstance()->removeScriptHandler(iterVec->second);
            iterMap->second.erase(iterVec);
            break;
        }
    }
}

int  ScriptHandlerMgr::getObjectHandler(void* object,ScriptHandlerMgr::HandlerType handlerType)
{
    if (nullptr == object ||   _mapObjectHandlers.empty() )
        return 0;
    
    auto iter = _mapObjectHandlers.find(object);
    if (_mapObjectHandlers.end() != iter)
        for (auto &handlerPair : iter->second)
            if (handlerPair.first == handlerType)
                return handlerPair.second;
    
    return 0;
}

void ScriptHandlerMgr::removeObjectAllHandlers(void* object)
{
    if (nullptr == object || _mapObjectHandlers.empty())
        return;
    
    auto iter = _mapObjectHandlers.find(object);
    if (_mapObjectHandlers.end() != iter)
    {
        if (!iter->second.empty())
        {
            for (auto &handlerPair : iter->second)
                LuaEngine::getInstance()->removeScriptHandler(handlerPair.second);
            
            (iter->second).clear();
        }
        _mapObjectHandlers.erase(iter);
    }
}

ScriptHandlerMgr::HandlerType ScriptHandlerMgr::addCustomHandler(void* object, int handler)
{
    assert(nullptr != object);
    
    auto iter = _mapObjectHandlers.find(object);
    VecHandlerPairs vecHandlers;
    vecHandlers.clear();
    HandlerType handlerType = HandlerType::EVENT_CUSTOM_BEGAN;
    
    if (_mapObjectHandlers.end() != iter)
    {
        vecHandlers = iter->second;
        if (!vecHandlers.empty())
            handlerType = static_cast<HandlerType>((int)vecHandlers.back().first + 1);
    }
    assert(handlerType <= HandlerType::EVENT_CUSTOM_ENDED);
    
    HandlerPair eventHanler = std::make_pair(handlerType, handler);
    vecHandlers.push_back(eventHanler);
    _mapObjectHandlers[object] = vecHandlers;
    
    return handlerType;
}

NS_CC_END

