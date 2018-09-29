#ifndef _LUA_WRAPPER_
#define _LUA_WRAPPER_
#include "GameUtil.h"
#include "Ref.h"
#include "Log.h"
#include "GameLua.h"
#include "FileSystem.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include <vector>
#include <string>
#include <assert.h>
#include<iostream>
#include<regex>
#include<share.h>

#define CPP_POINTER "__ptr"

#define LUA_CPP "cpp"

static int Lua_Error(lua_State* L)
{
    string str;
    int top = lua_gettop(L);
    const char* err = lua_tostring(L, -1);
    luaL_traceback(L, L, err, 0);
    int top1 = lua_gettop(L);
    for (int i = top + 1; i <= top1; ++i)
    {
        const char* e = lua_tostring(L, i);
        str += e;
        if (i != top1) str += "\n";
    }
    g_log("\n/***********************trace back start****************************/");
    g_log(str.c_str());
    g_log("/***********************trace back ended****************************/");
    return 0;
}

static int Lua_Loader(lua_State* L)
{
	std::string path = lua_tostring(L, -1);
	if (path.empty())
		return 0;

    auto fs = FileSystem::getInstance();

    size_t size = 0;
    std::string filepath = fs->getLuaFilePath(path);
    auto buffer = fs->getFileData(filepath.c_str(), size);
    if (buffer.get() == NULL)
    {
        g_log("load the lua file %s failed!!!",filepath.c_str());
        return 0;
    }
    g_log("load the lua file : %s", filepath.c_str());
	int ret = luaL_loadbuffer(L, (char*)buffer.get(), size, ("@" + filepath).c_str());
	if (ret != 0)
	{
		const char* str = lua_tostring(L, -1);
		g_log("load script %s failed!!!, msg:%s", path.c_str(), str);
		lua_pop(L, 1);
		return 0;
	}
	return 1;
}

static int Lua_Class(lua_State* L)
{
	int n = lua_gettop(L);
	const char* dir = lua_tostring(L, 1);
	const char* super = lua_tostring(L, 2);

	lua_getglobal(L, "package");
	lua_getfield(L, -1, "loaded");

	if (super)
	{
		lua_getfield(L, -1, super);
		if (lua_isnil(L, -1))
		{
			g_log("can not create class %s , reason : can not find the super class %s", dir, super);
			return 0;
		}
		lua_getfield(L, -2, dir);
		if (lua_isnil(L, -1))
		{
			g_log("%s have be a class", dir);
			return 2;
		}
		lua_pop(L, 1);

		lua_newtable(L);
		lua_pushstring(L, "__index");
		lua_pushvalue(L, -2);
		lua_rawset(L, -3);

		lua_pushstring(L, "__newindex");
		lua_pushvalue(L, -2);
		lua_rawset(L, -3);

		lua_pushvalue(L, -2);
		lua_setmetatable(L, -2);
		lua_pushvalue(L, -1);
		lua_setfield(L, -4, dir);
		return 2;
	}
	else
	{
		lua_getfield(L, -2, dir);
		if (!lua_isnil(L, -1))
		{
			g_log("%s have be a class", dir);
			return 1;
		}
		lua_pop(L, 1);
		lua_newtable(L);
		lua_pushstring(L, "__index");
		lua_pushvalue(L, -2);
		lua_rawset(L, -3);

		lua_pushstring(L, "__newindex");
		lua_pushvalue(L, -2);
		lua_rawset(L, -3);

		lua_pushvalue(L, -1);
		lua_setfield(L, -3, dir);
		return 1;
	}
}

static int Lua_Print(lua_State * L)
{
	int n = lua_gettop(L);  
	int i;
	std::string out;

	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; i++) 
	{
		const char *s;
		lua_pushvalue(L, -1); 
		lua_pushvalue(L, i);  
		lua_call(L, 1, 1);
		size_t sz;
		s = lua_tolstring(L, -1, &sz); 
		if (s == NULL)
			return luaL_error(L, LUA_QL("tostring") " must return a string to "
				LUA_QL("print"));
		if (i>1) out.append("\t");
		out.append(s, sz);
		lua_pop(L, 1);  
	}
	g_log(out.c_str());
	return 0;
}

template<typename T>
inline static std::string Lua_GetName()
{
    using TP = std::remove_const<std::remove_reference<std::remove_pointer<T>::type>::type>::type;
	const char* str = typeid(TP).name();
	std::cmatch cm;
	const char* s2 = "(class )*(\\w*::)*(\\w*)(<\\w*>)*";
	if (std::regex_match(str, cm, std::regex(s2), std::regex_constants::match_default))
	{
		return std::string("cpp.") + cm[3].str();
	}
	assert(false);
	return "";
}

static int Lua_Gc(lua_State* L)
{
    lua_pushstring(L, CPP_POINTER);
    lua_rawget(L, -2);
    if (lua_isuserdata(L, -1))
    {
        Ref* ref = (Ref*)lua_touserdata(L, -1);
        if (ref && ref->m_luaID > 0)
        {
            luaL_unref(L, LUA_REGISTRYINDEX, ref->m_luaID);
            ref->m_luaID = 0;
        }
        lua_pushstring(L, CPP_POINTER);
        lua_pushnil(L);
        lua_rawset(L, 1);
    }
    return 0;
}

static int Lua_SetMetatable(lua_State* L, int index, const char* super)
{
    if (!super) return 0;
    lua_getglobal(L, LUA_CPP);
    lua_pushstring(L, super);
    lua_rawget(L, -2);

    if (lua_istable(L, -1))
    {
        lua_setmetatable(L, index > 0 ? index : index - 2);
        lua_pop(L, 1);
    }
    else
    {
        g_log("can not find the super : %s",super);
        lua_pop(L, 2);
    }
    return 0;
}

static int Lua_CreateModule(lua_State* L, const char* nname, luaL_Reg* fn, const char* super)
{
    int t1 = lua_gettop(L);
    lua_getglobal(L, LUA_CPP);
    lua_pushstring(L, nname);
    lua_rawget(L, -2);
    if (!lua_istable(L, -1))
    {
		lua_pop(L, 1);
        luaL_newlib(L, fn);
        lua_pushstring(L, nname);
        lua_pushvalue(L, -2);
        lua_rawset(L, -4);

        lua_pushstring(L, "name");
        lua_pushstring(L, nname);
        lua_rawset(L, -3);

		lua_pushstring(L, "__index");
        lua_pushvalue(L, -2);
		lua_rawset(L, -3);

        lua_pushstring(L, "__gc");
        lua_pushcfunction(L, Lua_Gc);
        lua_rawset(L, -3);

        lua_getglobal(L, "package");
        lua_getfield(L, -1, "loaded");

        std::string s = std::string(LUA_CPP) + "." + nname;
		lua_pushstring(L, s.c_str());
        lua_pushvalue(L, -4);
		lua_rawset(L, -3);

        Lua_SetMetatable(L, -3, super);
        lua_pop(L, 4);
        return 0;
    }
    g_log("the module:%s has exist",nname);
	lua_pop(L, 2);
    return 0;
}

static int Lua_CreateModule(lua_State* L, const char* nname, luaL_Reg* fn, const char* super, Ref* singleton)
{
    int n = lua_gettop(L);
    lua_getglobal(L, LUA_CPP);
    lua_pushstring(L, nname);
    lua_rawget(L, -2);
	if (!lua_istable(L, -1))
	{
		lua_pop(L, 1);
		luaL_newlib(L, fn);
        lua_pushstring(L, nname);
        lua_pushvalue(L, -2);
        lua_rawset(L, -4);

        lua_pushstring(L, "name");
        lua_pushstring(L, nname);
        lua_rawset(L, -3);

		lua_pushstring(L, "__index");
		lua_pushvalue(L, -2);
		lua_rawset(L, -3);

        lua_pushstring(L, "__gc");
        lua_pushcfunction(L, Lua_Gc);
        lua_rawset(L, -3);

        lua_pushstring(L, CPP_POINTER);
        lua_pushlightuserdata(L, singleton);
        lua_rawset(L, -3);

        lua_pushvalue(L, -1);
        singleton->m_luaID = luaL_ref(L, LUA_REGISTRYINDEX);

        lua_getglobal(L, "package");
        lua_getfield(L, -1, "loaded");

        int n1 = lua_gettop(L);

        std::string s = std::string(LUA_CPP) + "." + nname;
		lua_pushstring(L, s.c_str());
		lua_pushvalue(L, -4);
		lua_rawset(L, -3);

        Lua_SetMetatable(L, -3, super);
        lua_pop(L, 4);
        return 0;
	}
    g_log("the module:%s has exist", nname);
	lua_pop(L, 2);
	return 0;
}

static int Lua_CreateRef(lua_State* L, const char* nname, Ref* ref)
{
    lua_newtable(L);
    Lua_SetMetatable(L, -1, nname);

    if (lua_getmetatable(L, -1))
    {
        lua_getfield(L, -1, "name");
        string s = lua_tostring(L, -1);
        lua_pop(L, 2);
    }

    lua_pushvalue(L, -1);
    ref->m_luaID = luaL_ref(L, LUA_REGISTRYINDEX);

	lua_pushstring(L, CPP_POINTER);
    lua_pushlightuserdata(L, ref);
	lua_rawset(L, -3);
    return 1;
}

static int Lua_BindRef(lua_State* L, const char* nname, Ref* ref)
{
    if (ref->m_luaID > 0)
        return 0;
    Lua_CreateRef(L, nname, ref);
    lua_pop(L, 1);
    return 0;
}

static int Lua_DeleteRef(lua_State* L, Ref* ref)
{
	if (ref->m_luaID > 0)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, ref->m_luaID);
        if (lua_istable(L, -1))
        {
            lua_pushstring(L, CPP_POINTER);
            lua_pushnil(L);
            lua_rawset(L, -3);
        }
		luaL_unref(L, LUA_REGISTRYINDEX, ref->m_luaID);
		ref->m_luaID = 0;
		lua_pop(L, 1);
	}
	return 0;
}

static void Lua_Clear(lua_State* L, int oldtop)
{
	int top = lua_gettop(L);
	if (top > oldtop)
	{
		lua_pop(L, top - oldtop);
	}
}

/*******************************************/

static int Lua_GetInt(lua_State* L, int index)
{
    return lua_isnumber(L, index) ? (int)lua_tointeger(L, index) : 0;
}

static unsigned Lua_GetUnsign(lua_State* L, int index)
{
    return lua_isnumber(L, index) ? (unsigned)lua_tointeger(L, index) : 0;
}

static float Lua_GetFloat(lua_State* L, int index)
{
    return lua_isnumber(L, index) ? (float)lua_tonumber(L, index) : 0;
}

static double Lua_GetDouble(lua_State* L, int index)
{
    return lua_isnumber(L, index) ? (double)lua_tonumber(L, index) : 0;
}

static const char* Lua_GetString(lua_State* L, int index)
{
    return lua_isstring(L, index) ? lua_tostring(L, index) : "";
}

static bool Lua_GetBool(lua_State* L, int index)
{
	return lua_isboolean(L, index) ? (bool)lua_toboolean(L, index) : false;
}

template<typename T>
static T* Lua_GetPointer(lua_State* L, int index)
{
    T* t = NULL;
    if (lua_istable(L, index))
    {
		lua_pushstring(L, CPP_POINTER);
		lua_rawget(L, index > 0 ? index : index - 1);
        t = (T*)lua_touserdata(L, -1);
		lua_pop(L, 1);
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

static int Lua_SetBool(lua_State* L, bool value)
{
	lua_pushboolean(L, value);
	return 1;
}

static int Lua_SetPointer(lua_State* L, Ref* ptr, const char* nname)
{
    if (lua_rawgeti(L, LUA_REGISTRYINDEX, ptr->m_luaID) != LUA_TNIL)
    {
        return 1;
    }
    return Lua_CreateRef(L, nname,ptr);
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

static void Lua_Pack(lua_State* L, Ref* ref)
{
    assert(ref->m_luaID > 0);
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref->m_luaID);
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

static bool Lua_DoFile(lua_State* L, const char* filename,lua_CFunction fn)
{
    if (fn)
    {
        lua_pushcfunction(L, fn);
    }
    if (luaL_loadfile(L, filename))
    {
        const char* err = lua_tostring(L, -1);
        g_log("load file %s failed, err:%s",filename,err);
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

    lua_pushcfunction(L, Lua_Error);
    lua_insert(L, -2);
    if (lua_pcall(L, 0, rt, -2) != 0)
    {
        return false;
    }
    return true;
}

template<typename T, typename... Args>
static bool Lua_CallFunc(lua_State* L,int rt, const T& t, const Args&... args)
{
	int n = lua_gettop(L);
    const int len = sizeof...(args) + 1;
	if (!lua_isfunction(L, -1))
	{
		return false;
	}   
    lua_pushcfunction(L, Lua_Error);
    lua_insert(L, -2);

    Lua_Pack(L, t, args...) ;
    if (lua_pcall(L, len, rt, -len - 2) != 0 )
    {
        return false;
    }
    return true;
}

static bool Lua_CallRef(lua_State* L, int ref, int rt)
{
	if (ref <= 0) return false;
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	if (!lua_isfunction(L, -1))
	{
		lua_pop(L, 1);
		return false;
	}
    return Lua_CallFunc(L, rt);
}

template<typename T, typename... Args>
static bool Lua_CallRef(lua_State* L, int ref, int rt, const T& t, const Args&... args)
{
	if (ref <= 0) return false;
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	if (!lua_isfunction(L, -1))
	{
		lua_pop(L, 1);
		return false;
	}
    return Lua_CallFunc(L, rt, t, args...);
}


#endif