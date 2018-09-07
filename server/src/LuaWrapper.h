#ifndef _LUA_WRAPPER_
#define _LUA_WRAPPER_
#include "GameUtil.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include <vector>
#include <string>
#include <assert.h>

#define CPP_POINTER "__ptr"

static size_t Lua_GetSize(lua_State* L, int pos)
{
    if (lua_istable(L, pos))
    {
        return lua_rawlen(L, pos);
    }
    return 0;
}

static void Lua_Remove(lua_State* L, int n)
{
    if (n > 0)
    {
        lua_pop(L, n);
    }
    else if (n < 0)
    {
        int top = lua_gettop(L);
        lua_pop(L, top);
    }
}

static int Lua_CreateRef(lua_State* L)
{
    if (lua_isfunction(L, -1))
    {
        return luaL_ref(L, LUA_REGISTRYINDEX);
    }
    return 0;
}

static void Lua_DeleteRef(lua_State* L,int ref)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
    luaL_unref(L, LUA_REGISTRYINDEX, ref);
}

static int Lua_CreateRef(lua_State* L, Ref* ref, const char* nname)
{
    lua_newtable(L);
    lua_getglobal(L, "package");
    lua_getfield(L, -1,"loaded");
    if (lua_getfield(L, -1, nname) != LUA_TTABLE)
    {
        log("can not find the table : %s",nname);
        lua_pop(L, 2);
        return 0;
    }
    lua_remove(L, -2);
    lua_remove(L, -2);
    lua_setmetatable(L, -2);
    ref->m_luaID = luaL_ref(L, LUA_REGISTRYINDEX);
    return 1;
}

/*****************************************************************/

static void Lua_Pack(lua_State * L, int value)
{
    lua_pushinteger(L, value);
}
static void Lua_Pack(lua_State* L, size_t value)
{
    lua_pushinteger(L, value);
}
static void Lua_Pack(lua_State* L, float value)
{
    lua_pushnumber(L, value);
}
static void Lua_Pack(lua_State* L, double value)
{
    lua_pushnumber(L, value);
}
static void Lua_Pack(lua_State* L, const char* value)
{
    lua_pushstring(L, value);
}
static void Lua_Pack(lua_State* L, const std::string& value)
{
    lua_pushlstring(L, value.c_str(), value.length());
}

template<class T>
static void Lua_Pack(lua_State* L, const std::vector<T>& value)
{
    lua_newtable(L);
    for (size_t i = 0; i < value.size(); ++i)
    {
        Lua_Pack(L, value[i]);
        lua_rawseti(L,-2,i+1);
    }
}

template<typename T1, typename T2, typename... Args>
static void Lua_Pack(lua_State* L, const T1& value1, const T2& value2, Args&... args)
{
    Lua_Pack(L, value1);
    Lua_Pack(L, value2, args...);
}

/*******************************************************/
    
static int Lua_Unpack(lua_State* L, int& value)
{
    if (lua_isnumber(L, -1))
    {
        value = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        return 1;
    }
    return 0;
}

static int Lua_Unpack(lua_State* L, size_t& value)
{
    if (lua_isnumber(L, -1))
    {
        value = (size_t)lua_tointeger(L, -1);
        lua_pop(L, 1);
        return 1;
    }
    return 0;
}

static int Lua_Unpack(lua_State* L, float& value)
{
    if (lua_isnumber(L, -1))
    {
        value = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
        return 1;
    }
    return 0;
}
static int Lua_Unpack(lua_State* L, double& value)
{
    if (lua_isnumber(L, -1))
    {
        value = lua_tonumber(L, -1);
        lua_pop(L, 1);
        return 1;
    }
    return 0;
}

static int Lua_Unpack(lua_State* L, const char* value)
{
    if (lua_isstring(L, -1))
    {
        value = lua_tostring(L, -1);
        lua_pop(L,1);
        return 1;
    }
    return 0;
}

static int Lua_Unpack(lua_State* L, std::string& value)
{
    if (lua_isstring(L, -1))
    {
        const char* str = lua_tostring(L, -1);
        value.append(str);
        lua_pop(L, 1);
        return 1;
    }
    return 0;
}

template<class T>
static void Lua_Unpack(lua_State* L, std::vector<T>& value)
{
    T t;
    if (!lua_istable(L, -1))
        return 0;
    size_t len = Lua_GetSize(L, -1);
    for (size_t i = 1; i <= len; ++i)
    {
        lua_rawgeti(L, -1, i);
        if (Lua_Unpack(L, t))
            value.push_back(t);
        else
            lua_pop(L, 1);
    }
    lua_pop(L, 1);
    return 1;
}

template<typename T1, typename T2, typename... Args>
static void Lua_Unpack(lua_State* L, T1& value1, T2& value2, Args&... args)
{
    Lua_Unpack(L, value2, args...);
    Lua_Unpack(L, value1);
}
/*******************************************/

static int Lua_GetInt(lua_State* L, int index)
{
    return lua_isnumber(L, index) ? lua_tointeger(L, index) : 0;
}

static unsigned Lua_GetUnsign(lua_State* L, int index)
{
    return lua_isnumber(L, index) ? lua_tointeger(L, index) : 0;
}

static float Lua_GetFloat(lua_State* L, int index)
{
    return lua_isnumber(L, index) ? lua_tonumber(L, index) : 0;
}

static double Lua_GetDouble(lua_State* L, int index)
{
    return lua_isnumber(L, index) ? lua_tonumber(L, index) : 0;
}

static const char* Lua_GetString(lua_State* L, int index)
{
    return lua_isstring(L, index) ? lua_tostring(L, index) : "";
}

template<typename T>
static T* Lua_GetPointer(lua_State* L, int index)
{
    T* t = NULL;
    if (lua_istable(L, index) && lua_getfield(L, index, CPP_POINTER) == LUA_TLIGHTUSERDATA)
    {
        t = (T*)lua_touserdata(L, index);
    }
    return t;
}

/****************************************************/

static int Lua_SetInt(lua_State* L, int value)
{
    lua_pushinteger(L, value);
    return 1;
}

static int Lua_SetUnsigned(lua_State* L, unsigned value)
{
    lua_pushinteger(L, value);
    return 1;
}

static int Lua_SetFloat(lua_State* L, float value)
{
    lua_pushnumber(L, value);
    return 1;
}

static int Lua_SetDouble(lua_State* L, double value)
{
    lua_pushnumber(L, value);
    return 1;
}

static int Lua_SetString(lua_State* L, const char* value)
{
    lua_pushstring(L, value);
    return 1;
}

static int Lua_CreateObj(lua_State* L, const char* moudle)
{
    return 1;
}


static int Lua_SetRef(lua_State* L, const char* nname, Ref* ptr)
{
    int luaID = ptr->m_luaID;
    if (lua_rawgeti(L, LUA_REGISTRYINDEX, luaID) == LUA_TTABLE)
    {
        return 1;
    }
    else
    {
        return 1;
    }
}

/*******************************************/

static bool Lua_DoFile(lua_State* L, const char* filename,lua_CFunction fn)
{
    if (fn)
    {
        lua_pushcfunction(L, fn);
    }
    if (luaL_loadfile(L, filename))
    {
        const char* err = lua_tostring(L, -1);
        log("load file %s failed, err:%s",filename,err);
        lua_pop(L, fn ? 2 : 1);
        return false;
    }
    if(lua_pcall(L, 0, LUA_MULTRET, -2))
    {
        const char* err = lua_tostring(L, -1);
        printf("execute file %s failed , err: %s\n",filename, err);
        lua_pop(L, fn ? 2 : 1);
        return false;
    }
    return true;
}

static void Lua_Split(const char* str,  char c, std::vector<std::string>& result)
{
    const char* pre = str;
    while (*str != 0)
    {
        while (*str && *str != c) ++str;
        result.push_back(std::string(pre,str));
        pre = *str ? ++str : str;
    }
}

/*****************************************************************/

static int Lua_GetField(lua_State* L, int index, const char* key) 
{
    if (lua_istable(L, index))
    {
        lua_pushstring(L, key);
        lua_rawget(L, index);
        return 1;
    }
    return 0;
}

static int Lua_GetField(lua_State* L, int index, int i)
{
    if (lua_istable(L, index))
    {
        lua_rawgeti(L, index, i);
        return 1;
    }
    return 0;
}

template<typename T1,typename T2,typename... Args>
static int Lua_GetField(lua_State* L, int index, T1& t1, T2& t2, Args& ... args)
{
    int ret = Lua_GetField(L, index, t1) + Lua_GetField(L, index, t2, args...);
    return ret;
}

/**********************************************************/
static int Lua_TopField(lua_State* L, const char* key)
{
    if (lua_istable(L, -1))
    {
        lua_pushstring(L, key);
        lua_rawget(L, -1);
        return 1;
    }
    return 0;
}

static int Lua_TopField(lua_State* L, int i)
{
    if (lua_istable(L, -1))
    {
        lua_rawgeti(L, -1, i);
        return 1;
    }
    return 0;
}

template<typename T1,typename T2,typename... Args>
static int Lua_TopField(lua_State* L, T1& t1, T2& t2, Args... args)
{
    int ret = Lua_TopField(L, t1) + Lua_TopField(L, t2, args...);
    return ret;
}

template<typename T1, typename... Args>
static int Lua_GlobalTop(lua_State* L, const char* global,T1& t1, Args... args)
{
	lua_getglobal(L, global);
	return Lua_TopField(L, t1, args...) + 1;
}


static bool Lua_CallFunc(lua_State* L, int rt)
{
    if (!lua_isfunction(L, -1))
        return false;
    if (lua_pcall(L, 0, rt, 0) != 0)
    {
        const char* err = lua_tostring(L, -1);
        printf("call lua func failed!!!, err:%s\n", err);
        lua_pop(L, 1);
        return false;
    }
    return true;
}

template<typename T, typename... Args>
static bool Lua_CallFunc(lua_State* L,int rt, const T& t, const Args&... args)
{
    const int len = sizeof...(args) + 1;
    if (!lua_isfunction(L, -1))
        return false;
    
    Lua_Pack(L, t, args...) ;
    if (lua_pcall(L, len, rt, 0) != 0 )
    {
        const char* err = lua_tostring(L, -1);
        printf("call lua func failed!!!, err:%s\n",err);
        lua_pop(L, 1);
        return false;
    }
    return true;
}

static bool Lua_CallRef(lua_State* L, int ref, int rt)
{
    lua_rawseti(L, LUA_REGISTRYINDEX, ref);
    return Lua_CallFunc(L, rt);
}

template<typename T, typename... Args>
static bool Lua_CallRef(lua_State* L, int ref, int rt, const T& t, const Args&... args)
{
    lua_rawseti(L, LUA_REGISTRYINDEX, ref);
    return Lua_CallFunc(L, rt, t, args...);
}


#endif