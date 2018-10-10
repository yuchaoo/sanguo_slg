#ifndef _LUA_BIND_
#define _LUA_BIND_
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "CCRef.h"
#include "2d/CCActionInterval.h"

using namespace cocos2d;

#define LUA_CPP_POINTER_STR "__cptr"
#define LUA_CC "cc"

struct luaL_Enum
{
	const char* key;
	int value;
};

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
	log("\n/***********************trace back start****************************/");
	log(str.c_str());
	log("/***********************trace back ended****************************/");
	return 0;
}

static int Lua_Gc(lua_State* L)
{
	lua_pushstring(L, LUA_CPP_POINTER_STR);
	lua_rawget(L, -2);
	if (!lua_isnil(L, -1))
	{
		Ref* ref = (Ref*) lua_touserdata(L, -1);
		if (ref)
		{
			if (ref->_luaID > 0)
			{
				luaL_unref(L, LUA_REGISTRYINDEX, ref->_luaID);
			}
			if (ref->_callbackID > 0)
			{
				luaL_unref(L, LUA_REGISTRYINDEX, ref->_callbackID);
			}
			delete ref;
		}
		lua_pop(L, 1);
		lua_pushstring(L, LUA_CPP_POINTER_STR);
		lua_pushnil(L);
		lua_rawset(L, -3);
	}
	return 0;
}

static int Lua_SetMetatable(lua_State* L, int index, const char* module)
{
	if (!module) return 0;
	lua_getglobal(L, LUA_CC);
	lua_pushstring(L, module);
	lua_rawget(L, -2);
	if (lua_istable(L, -1))
	{
		lua_setmetatable(L, index > 0 ? index : index - 2);
		lua_pop(L, 1);
	}
	else
	{
		log("can not find the super : %s", module);
		lua_pop(L, 2);
	}
	return 0;
}

static int Lua_CreateRef(lua_State* L, const char* module, cocos2d::Ref* ref)
{
	lua_newtable(L);
	lua_pushstring(L, LUA_CPP_POINTER_STR);
	lua_pushlightuserdata(L, (void*)ref);
	lua_rawset(L, -3);

	lua_pushvalue(L, -1);
	ref->_luaID = luaL_ref(L, LUA_REGISTRYINDEX);

	Lua_SetMetatable(L, -1, module);
	return 1;
}

static int Lua_DeleteRef(lua_State* L, cocos2d::Ref* ref)
{
	if (ref->_luaID)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, ref->_luaID);
		lua_pushstring(L, LUA_CPP_POINTER_STR);
		lua_pushnil(L);
		lua_rawset(L, -3);
		luaL_unref(L, LUA_REGISTRYINDEX, ref->_luaID);
		ref->_luaID = 0;
		lua_pop(L, 1);
	}
	if(ref->_callbackID > 0)
	{
		luaL_unref(L, LUA_REGISTRYINDEX, ref->_callbackID);
		ref->_callbackID = 0;
	}
	return 0;
}

static int Lua_CreateModule(lua_State* L, luaL_Reg* reg)
{
	lua_getglobal(L, LUA_CC);
	luaL_setfuncs(L, reg, 0);
	lua_pop(L, 1);
	return 0;
}

static int Lua_CreateModule(lua_State* L, const char* module, const char* super, luaL_Reg* reg)
{
	lua_getglobal(L,LUA_CC);

	luaL_newlib(L, reg);
	lua_pushstring(L, "__name");
	lua_pushstring(L, module);
	lua_rawset(L, -3);

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_rawset(L, -3);

	lua_pushstring(L, "__gc");
	lua_pushvalue(L, -2);
	lua_rawset(L, -3);

	lua_pushstring(L, module);
	lua_pushvalue(L, -2);
	lua_rawset(L, -4);
	
	std::string str = std::string(LUA_CC) + "." + module;
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "loaded");
	lua_pushstring(L, str.c_str());
	lua_pushvalue(L, -4);
	lua_rawset(L, -3);

	Lua_SetMetatable(L, -3, super);
	lua_pop(L, 4);
	return 0;
}

static int Lua_CreateModule(lua_State* L, const char* module, const char* super, luaL_Reg* reg,Ref* singleton)
{
	Lua_CreateModule(L, module, super, reg);
	lua_getglobal(L, LUA_CC);
	lua_pushstring(L, module);
	lua_rawget(L, -3);

	lua_pushstring(L, LUA_CPP_POINTER_STR);
	lua_pushlightuserdata(L, (void*)singleton);
	lua_rawset(L, -3);

	lua_pushvalue(L, -1);
	singleton->_luaID = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_pop(L, 2);
	return 2;
}

static int Lua_AddConstVariable(lua_State* L, const char* moudle, luaL_Enum* em)
{
	int n = 1;
	lua_getglobal(L, LUA_CC);
	if (moudle)
	{
		lua_pushstring(L, moudle);
		lua_rawget(L, -2);
		n++;
	}

	while (em->key && em->value)
	{
		lua_pushstring(L, em->key);
		lua_pushinteger(L, em->value);
		lua_rawset(L, -3);
		++em;
	}
	lua_pop(L, n);
	return 0;
}

static bool Lua_isBinded(lua_State* L, cocos2d::Ref* ref)
{
	return ref && ref->_luaID > 0;
}

/***********************************************************/
template<class T>
static T* Lua_GetPointer(lua_State* L,int index)
{
	if (lua_istable(L, index))
	{
		lua_pushstring(L, LUA_CPP_POINTER_STR);
		lua_rawget(L, index);
		return (T*)lua_touserdata(L, -1);
	}
	return NULL;
}

static bool Lua_GetBool(lua_State* L, int index)
{
	if (lua_isboolean(L, index))
		return lua_toboolean(L, index);
	return !lua_isnil(L, index);
}

static unsigned char Lua_GetChar(lua_State* L, int index)
{
	if (lua_isnumber(L, index))
		return (unsigned char)lua_tointeger(L, index);
	return 0;
}

static int Lua_GetInt(lua_State* L, int index)
{
	if (lua_isnumber(L, index))
	{
		return lua_tointeger(L, index);
	}
	return 0;
}
static unsigned short Lua_GetShort(lua_State* L, int index)
{
	if (lua_isnumber(L, index))
	{
		return (unsigned short)lua_tointeger(L, index);
	}
	return 0;
}

static unsigned Lua_GetUnsigned(lua_State* L, int index)
{
	if (lua_isnumber(L, index))
	{
		return lua_tointeger(L, index);
	}
	return 0;
}

static float Lua_GetFloat(lua_State* L, int index)
{
	if (lua_isnumber(L, index))
	{
		return (float)lua_tonumber(L, index);
	}
	return 0;
}
static double Lua_GetDouble(lua_State* L, int index)
{
	if (lua_isnumber(L, index))
	{
		return (double)lua_tonumber(L, index);
	}
	return 0;
}

static const char* Lua_GetString(lua_State* L, int index)
{
	if (lua_isstring(L, index))
	{
		return lua_tostring(L, index);
	}
	return "";
}
static cocos2d::Vec2 Lua_GetVec2(lua_State* L, int index)
{
	cocos2d::Vec2 v(0,0);
	if (lua_isnumber(L, index) && lua_isnumber(L, index + 1))
	{
		v.x = lua_tonumber(L, index);
		v.y = lua_tonumber(L, index + 1);
	}
	return v;
}
static cocos2d::Vec3 Lua_GetVec3(lua_State* L, int index)
{
	Vec3 v;
	if (lua_istable(L, index))
	{
		lua_pushstring(L, "x");
		lua_rawget(L, index > 0 ? index : index - 1);
		v.x = lua_tonumber(L, -1);
		lua_pushstring(L, "y");
		lua_rawget(L, index > 0 ? index : index - 2);
		v.y = lua_tonumber(L, -1);
		lua_pushstring(L, "z");
		lua_rawget(L, index > 0 ? index : index - 3);
		v.z = lua_tonumber(L,-1);
		lua_pop(L, 3);
	}
	return v;
}

static cocos2d::Size Lua_GetSize(lua_State* L, int index)
{
	cocos2d::Size s(0,0);
	if (lua_isnumber(L, index) && lua_isnumber(L, index + 1))
	{
		s.width = lua_tonumber(L, index);
		s.height = lua_tonumber(L, index + 1);
	}
	return s;
}
static cocos2d::Size Lua_GetTSize(lua_State* L, int index)
{
	cocos2d::Size s(0, 0);
	if (lua_istable(L, index))
	{
		lua_pushstring(L, "width");
		lua_rawget(L, -2);
		s.width = lua_tonumber(L, -1);
		lua_pushstring(L, "height");
		lua_rawget(L, -3);
		s.height = lua_tonumber(L, -1);
		lua_pop(L, 2);
	}
	return s;
}
static cocos2d::Rect Lua_GetRect(lua_State* L, int index)
{
	cocos2d::Rect rt;
	if (lua_istable(L, -1))
	{
		lua_pushstring(L, "x");
		lua_rawget(L, -2);
		rt.origin.x = lua_tonumber(L, -1);
		lua_pushstring(L, "y");
		lua_rawget(L, -3);
		rt.origin.y = lua_tonumber(L, -1);
		lua_pushstring(L, "width");
		lua_rawget(L, -4);
		rt.size.width = lua_tonumber(L, -1);
		lua_pushstring(L, "height");
		lua_rawget(L, -5);
		rt.size.height = lua_tonumber(L, -1);
	}
	return rt;
}
static cocos2d::Color3B Lua_GetColor3B(lua_State* L, int index)
{
	cocos2d::Color3B color;
	if (lua_istable(L, index))
	{
		lua_pushstring(L, "r");
		lua_rawget(L, -2);
		color.r = lua_tonumber(L, -1);
		lua_pushstring(L, "g");
		lua_rawget(L, -3);
		color.g = lua_tonumber(L, -1);
		lua_pushstring(L, "b");
		lua_rawget(L, -4);
		color.b = lua_tonumber(L, -1);
	}
	return color;
}
static cocos2d::Color4B Lua_GetColor4B(lua_State* L, int index)
{
	cocos2d::Color4B color;
	if (lua_istable(L, index))
	{
		lua_pushstring(L, "r");
		lua_rawget(L, -2);
		color.r = lua_tonumber(L, -1);
		lua_pushstring(L, "g");
		lua_rawget(L, -3);
		color.g = lua_tonumber(L, -1);
		lua_pushstring(L, "b");
		lua_rawget(L, -4);
		color.b = lua_tonumber(L, -1);
		lua_pushstring(L, "a");
		lua_rawget(L, -5);
		color.a = lua_tonumber(L, -1);
	}
	return color;
}

static BlendFunc Lua_GetBlendFunc(lua_State* L, int index)
{
	BlendFunc func;
	if (lua_istable(L, index))
	{
		lua_pushstring(L, "src");
		lua_rawget(L, -2);
		func.src = lua_tointeger(L, -1);
		lua_pushstring(L, "dst");
		lua_rawget(L, -3);
		func.dst = lua_tointeger(L, -1);
		lua_pop(L, 2);
	}
	return func;
}

static ccBezierConfig Lua_GetBezierConfig(lua_State* L, int index)
{
	ccBezierConfig config;
	if (lua_istable(L, index))
	{
		lua_pushstring(L, "x");
		lua_rawget(L, index);
		config.endPosition.x = lua_tonumber(L, -1);
		lua_pushstring(L, "y");
		lua_rawget(L, index);
		config.endPosition.y = lua_tonumber(L, -1);
		lua_pushstring(L, "x1");
		lua_rawget(L, index);
		config.controlPoint_1.x = lua_tonumber(L, -1);
		lua_pushstring(L, "y1");
		lua_rawget(L, index);
		config.controlPoint_1.y = lua_tonumber(L, -1);
		lua_pushstring(L, "x2");
		lua_rawget(L, index);
		config.controlPoint_2.x = lua_tonumber(L, -1);
		lua_pushstring(L, "y2");
		lua_rawget(L, index);
		config.controlPoint_2.y = lua_tonumber(L, -1);
		lua_pop(L, 6);
		return config;
	}
	log("ccBezierConfig arg:%d is not table",index);
	return config;
}

static FontDefinition Lua_GetFontDefinition(lua_State* L, int index)
{
	FontDefinition def;
	if (lua_istable(L, index))
	{
		lua_pushstring(L,"fontName");
		lua_rawget(L, index);
		def._fontName = lua_tostring(L, -1);
		
		lua_pushstring(L, "fontSize");
		lua_rawget(L, index > 0 ? index : index - 1);
		def._fontSize = lua_isnil(L,-1) ? 10 : lua_tointeger(L, -1);

		lua_pushstring(L, "alignment");
		lua_rawget(L, index > 0 ? index : index - 2);
		def._alignment = lua_isnil(L, -1) ? TextHAlignment::CENTER : (TextHAlignment)lua_tointeger(L, -1);

		lua_pushstring(L, "vertAlignment");
		lua_rawget(L, index > 0 ? index : index - 3);
		def._vertAlignment = lua_isnil(L, -1) ? TextVAlignment::TOP : (TextVAlignment)lua_tointeger(L, -1);

		lua_pushstring(L, "dimensions");
		lua_rawget(L, index > 0 ? index : index - 4);
		def._dimensions = Lua_GetTSize(L, -1);

		lua_pushstring(L, "fontFillColor");
		lua_rawget(L, index > 0 ? index : index - 5);
		def._fontFillColor = Lua_GetColor3B(L, -1);

		lua_pushstring(L, "fontAlpha");
		lua_rawget(L, index > 0 ? index : index - 6);
		def._fontAlpha = (GLubyte) lua_tointeger(L, -1);

		lua_pushstring(L, "shadow");
		lua_rawget(L, index > 0 ? index : index - 7);
		if (!lua_isnil(L, -1))
		{
			lua_pushstring(L, "shadowEnabled");
			lua_rawget(L, -2);
			def._shadow._shadowEnabled = lua_toboolean(L, -1);

			lua_pushstring(L, "shadowOffset");
			lua_rawget(L, -3);
			def._shadow._shadowOffset = Lua_GetTSize(L, -1);

			lua_pushstring(L, "shadowBlur");
			lua_rawget(L, -4);
			def._shadow._shadowBlur = lua_tonumber(L, -1);

			lua_pushstring(L, "shadowOpacity");
			lua_rawget(L, -5);
			def._shadow._shadowOpacity = lua_tonumber(L, -1);
			lua_pop(L, 4);
		}
		lua_pushstring(L, "stroke");
		lua_rawget(L, index > 0 ? index : index - 8);
		if (!lua_isnil(L, -1))
		{
			lua_pushstring(L, "strokeEnabled");
			lua_rawget(L, -2);
			def._stroke._strokeEnabled = lua_toboolean(L, -1);
			
			lua_pushstring(L, "strokeColor");
			lua_rawget(L, -3);
			def._stroke._strokeColor = Lua_GetColor3B(L, -1);
			
			lua_pushstring(L, "strokeAlpha");
			lua_rawget(L, -4);
			def._stroke._strokeAlpha = lua_toboolean(L, -1);

			lua_pushstring(L, "strokeSize");
			lua_rawget(L, -5);
			def._stroke._strokeSize = lua_tonumber(L, -1);
			lua_pop(L, 4);
		}
		lua_pushstring(L, "enableWrap");
		lua_rawget(L, index > 0 ? index : index - 9);
		def._enableWrap = lua_toboolean(L, -1);

		lua_pushstring(L, "overflow");
		lua_rawget(L, index > 0 ? index : index - 10);
		def._overflow = lua_tonumber(L, -1);
		lua_pop(L, 10);
	}
	return def;
}

template<typename T>
static cocos2d::Vector<T*> Lua_GetRefVector(lua_State* L,int index)
{
	cocos2d::Vector<T*> value;
	if (lua_istable(L, index))
	{
		int len = lua_rawlen(L, index);
		for (int i = 1; i <= len; ++i)
		{
			lua_rawgeti(L, index, i);
			T* t = (T*)lua_touserdata(L, -1);
			if (t)
			{
				value.pushBack(t);
			}
		}
	}
	return value;
}

/*************************************************/
static int Lua_SetPointer(lua_State* L, Ref* ref, const char* module)
{
	if (ref->_luaID > 0)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, ref->_luaID);
		return 1;
	}
	else
	{
		return Lua_CreateRef(L, module, ref);
	}
}

static int Lua_SetBool(lua_State* L, bool value)
{
	lua_pushboolean(L, value);
	return 1;
}
static int Lua_SetChar(lua_State* L, unsigned char value)
{
	lua_pushinteger(L, value);
	return 1;
}

static int Lua_SetInt(lua_State* L, int value)
{
	lua_pushinteger(L, value);
	return 1;
}
static int Lua_SetShort(lua_State* L, unsigned short value)
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
static int Lua_SetString(lua_State* L, const std::string& value)
{
	lua_pushlstring(L, value.c_str(), value.length());
	return 1;
}
static int Lua_SetVec2(lua_State* L, const cocos2d::Vec2& value)
{
	lua_pushnumber(L, value.y);
	lua_pushnumber(L, value.x);
	return 2;
}
static int Lua_SetVec3(lua_State* L, const cocos2d::Vec3& value)
{
	lua_newtable(L);
	lua_pushstring(L, "x");
	lua_pushnumber(L, value.x);
	lua_rawset(L, -3);
	lua_pushstring(L, "y");
	lua_pushnumber(L, value.y);
	lua_rawset(L, -3);
	lua_pushstring(L, "z");
	lua_pushnumber(L, value.z);
	lua_rawset(L, -3);
	return 1;
}
static int Lua_SetSize(lua_State* L, const cocos2d::Size& value)
{
	lua_pushnumber(L, value.height);
	lua_pushnumber(L, value.width);
	return 2;
}

static int Lua_SetRef(lua_State* L, const cocos2d::Ref* ref)
{
	if (ref->_luaID > 0)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, ref->_luaID);
		return 1;
	}
	log("ref is not binded");
	assert(false);
	return 1;
}

static int Lua_SetRect(lua_State* L, const cocos2d::Rect& rt)
{
	lua_newtable(L);
	lua_pushstring(L, "x");
	lua_rawset(L, -2);
	lua_pushstring(L, "y");
	lua_rawset(L, -3);
	lua_pushstring(L, "width");
	lua_rawset(L, -4);
	lua_pushstring(L, "height");
	lua_rawset(L, -5);
	return 1;
}
static int Lua_SetColor3B(lua_State* L, const cocos2d::Color3B& color)
{
	lua_newtable(L);
	lua_pushstring(L,"r");
	lua_pushnumber(L, color.r);
	lua_rawset(L, -3);
	lua_pushstring(L, "g");
	lua_pushnumber(L, color.g);
	lua_rawset(L, -3);
	lua_pushstring(L, "b");
	lua_pushnumber(L, color.b);
	return 1;
}
static int Lua_SetColor4B(lua_State* L, const cocos2d::Color4B& color)
{
	lua_newtable(L);
	lua_pushstring(L, "r");
	lua_pushnumber(L, color.r);
	lua_rawset(L, -3);
	lua_pushstring(L, "g");
	lua_pushnumber(L, color.g);
	lua_rawset(L, -3);
	lua_pushstring(L, "b");
	lua_pushnumber(L, color.b);
	lua_pushstring(L, "a");
	lua_pushnumber(L, color.a);
	return 1;
}

static int Lua_SetTouch(lua_State* L, const cocos2d::Touch& touch)
{
	lua_newtable(L);
	lua_pushstring(L, "id");
	lua_pushinteger(L, touch.getID());
	lua_rawset(L, -3);
	cocos2d::Vec2 pos =  touch.getLocation();
	lua_pushstring(L, "x");
	lua_pushnumber(L, pos.x);
	lua_rawset(L, -3);
	lua_pushstring(L, "y");
	lua_pushnumber(L, pos.y);
	lua_rawset(L, -3);
	return 1;
}

static int Lua_SetTouches(lua_State* L, const std::vector<cocos2d::Touch*>& touches)
{
	lua_newtable(L);
	for (size_t i = 0; i < touches.size(); ++i)
	{
		Lua_SetTouch(L, *touches.at(i));
		lua_rawseti(L, -1, i + 1);
	}
	return 1;
}

template<typename T>
static int Lua_SetRefVector(lua_State* L, const cocos2d::Vector<T*>& value)
{
	lua_newtable(L);
	for (auto i = 0; i < value.size(); ++i)
	{
		const cocos2d::Ref* ref = dynamic_cast<const cocos2d::Ref*>(value.at(i));
		assert(ref);
		lua_pushinteger(L, ref->_luaID);
		lua_rawget(L, LUA_REGISTRYINDEX);
		if (!lua_isnil(L, -1))
		{
			lua_rawseti(L, -2, i);
		}
		lua_pop(L, 1);
	}

	return 1;
}

static int Lua_SetBlendFunc(lua_State* L, const BlendFunc& func)
{
	lua_newtable(L);
	lua_pushstring(L, "src");
	lua_pushinteger(L, func.src);
	lua_rawset(L, -3);
	lua_pushstring(L, "dst");
	lua_pushinteger(L, func.dst);
	lua_rawset(L, -3);
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
		lua_rawseti(L, -2, i + 1);
	}
}

template<typename T1, typename T2, typename... Args>
static void Lua_Pack(lua_State* L, const T1& value1, const T2& value2, Args&... args)
{
	Lua_Pack(L, value1);
	Lua_Pack(L, value2, args...);
}

/********************************************************************/

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
		lua_pop(L, 1);
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

/**************************************************************/

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
static bool Lua_CallFunc(lua_State* L, int rt, const T& t, const Args&... args)
{
	int n = lua_gettop(L);
	const int len = sizeof...(args)+1;
	if (!lua_isfunction(L, -1))
	{
		return false;
	}

	Lua_Pack(L, t, args...);
	if (lua_pcall(L, len, rt, 0) != 0)
	{
		const char* err = lua_tostring(L, -1);
		printf("call lua func failed!!!, err:%s\n", err);
		lua_pop(L, 1);
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