#include "Ref.h"
#include "LuaWrapper.h"
#include "GameLua.h"
int Ref::kRefCount = 0;

Ref::Ref()
    :m_luaID(0)
    ,m_refCount(1)
    ,m_refId(++kRefCount)
{
}

Ref::~Ref()
{
	if (m_luaID > 0)
	{
        lua_State* L = GameLua::getInstance()->getLuaState();
        Lua_DeleteRef(L, this);
	}
}

int Ref::retain()
{
    return ++m_refCount;
}

int Ref::release()
{
    if (--m_refCount == 0)
    {
        delete this;
    }
    return m_refCount;
}
