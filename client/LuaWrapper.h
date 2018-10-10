#ifndef _LUA_WRAPPER_
#define _LUA_WRAPPER_

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include <vector>
#include <string>
#include <assert.h>
#include <iostream>
#include <sstream>

using namespace std;

namespace LuaWrapper
{
#define CPP_PTR "__ptr"
#define CPP_TABLE "cc"
    static size_t getLength(lua_State* L, int pos)
    {
        if (lua_istable(L, pos))
        {
            return lua_rawlen(L, pos);
        }
        return 0;
    }

    static void removeField(lua_State* L, int n)
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

    static int refFunc(lua_State* L)
    {
        if (lua_isfunction(L, -1))
        {
            return luaL_ref(L, LUA_REGISTRYINDEX);
        }
        return 0;
    }

    static void unrefFunc(lua_State* L,int ref,bool bRemove)
    {
        lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
        if (bRemove)
        {
            luaL_unref(L, LUA_REGISTRYINDEX, ref);
        }
    }

	static void clear(lua_State* L)
	{
		int top = lua_gettop(L);
		if (top > 0)
		{
			lua_pop(L, top);
		}
	}

    /*****************************************************************/

    static void pack(lua_State * L, const int& value)
    {
        lua_pushinteger(L, value);
    }
	static void pack(lua_State* L, const int* value)
	{
		lua_pushinteger(L, *value);
	}
    static void pack(lua_State* L, size_t value)
    {
        lua_pushinteger(L, value);
    }
	static void pack(lua_State* L, const size_t* value)
	{
		lua_pushinteger(L, *value);
	}
    static void pack(lua_State* L, float value)
    {
        lua_pushnumber(L, value);
    }
	static void pack(lua_State* L, const float* value)
	{
		lua_pushnumber(L, *value);
	}
    static void pack(lua_State* L, double value)
    {
        lua_pushnumber(L, value);
    }
	static void pack(lua_State* L, const double* value)
	{
		lua_pushnumber(L, *value);
	}
    static void pack(lua_State* L, const char* value)
    {
        lua_pushstring(L, value);
    }
	static void pack(lua_State* L, char* value)
	{
		lua_pushstring(L, value);
	}
    static void pack(lua_State* L, const std::string& value)
    {
        lua_pushlstring(L, value.c_str(), value.length());
    }
	static void pack(lua_State* L, const std::string* value)
	{
		lua_pushlstring(L, value->c_str(), value->length());
	}
#define PACK_NUMBER(key,val) \
	lua_pushstring(L, key); \
	lua_pushnumber(L, val); \
	lua_rawset(L, -3);

	static void pack(lua_State* L, const cocos2d::Size& size)
	{
		lua_newtable(L);
		PACK_NUMBER("width", size.width);
		PACK_NUMBER("height", size.height);
	}
	static void pack(lua_State* L, const cocos2d::Vec2& v)
	{
		lua_newtable(L);
		PACK_NUMBER("x", v.x);
		PACK_NUMBER("y", v.y);
	}

	static void pack(lua_State* L, const cocos2d::Color3B& c)
	{
		lua_newtable(L);
		PACK_NUMBER("r", c.r);
		PACK_NUMBER("g", c.g);
		PACK_NUMBER("b", c.b);
	}
	static void pack(lua_State* L, const cocos2d::Color4B& c)
	{
		lua_newtable(L);
		PACK_NUMBER("r", c.r);
		PACK_NUMBER("g", c.g);
		PACK_NUMBER("b", c.b);
		PACK_NUMBER("a", c.a);
	}

	static void pack(lua_State* L, const cocos2d::Color4F& c)
	{
		lua_newtable(L);
		PACK_NUMBER("r", c.r);
		PACK_NUMBER("g", c.g);
		PACK_NUMBER("b", c.b);
		PACK_NUMBER("a", c.a);
	}

    template<class T>
    static void pack(lua_State* L, const std::vector<T>& value)
    {
        lua_newtable(L);
        for (size_t i = 0; i < value.size(); ++i)
        {
            lua_pushinteger(L, i+1);
            pack(L, value[i]);
            lua_rawset(L, -3);
        }
    }

	static void pack(lua_State* L)
	{
		assert(false);
	}

	template<typename T>
	static void pack(lua_State* L, const T*& value)
	{
		lua_pushlightuserdata(L, value);
	}

    template<typename T, typename... Args>
    static void pack(lua_State* L, const T& value, const Args&... args)
    {
        pack(L, value);
        pack(L, args...);
    }
	
    /*******************************************************/
    
    static void unpack(lua_State* L, int& value)
    {
        if (lua_isinteger(L, -1))
        {
            value = (int)lua_tointeger(L, -1);
            lua_pop(L, 1);
        }
    }
    static void unpack(lua_State* L, size_t& value)
    {
        if (lua_isinteger(L, -1))
        {
            value = (size_t)lua_tointeger(L, -1);
            lua_pop(L, 1);
        }
    }
    static void unpack(lua_State* L, float& value)
    {
        if (lua_isnumber(L, -1))
        {
            value = (float)lua_tonumber(L, -1);
            lua_pop(L, 1);
        }
    }
    static void unpack(lua_State* L, double& value)
    {
        if (lua_isnumber(L, -1))
        {
            value = lua_tonumber(L, -1);
            lua_pop(L, 1);
        }
    }
    static void unpack(lua_State* L, std::string& value)
    {
        if (lua_isstring(L, -1))
        {
            const char* str = lua_tostring(L, -1);
            value.append(str);
            lua_pop(L, 1);
        }
    }

	static void unpack(lua_State* L, const char*& value)
	{
		if (lua_isstring(L, -1))
		{
			value = lua_tostring(L, -1);
			lua_pop(L, 1);
		}
	}

	static void unpack(lua_State* L, char* & value)
	{
		if (lua_isstring(L, -1))
		{
			value = const_cast<char*>(lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}
	static void unpack(lua_State* L, bool & value)
	{
		if (lua_isboolean(L, -1))
		{
			value = lua_toboolean(L, -1) != 0;
			lua_pop(L, 1);
		}
	}

#define UNPACK_NUMBER(key,val) \
	lua_pushstring(L, key); \
	lua_rawget(L, -2); \
	if (lua_isnumber(L, -1)) val = lua_tonumber(L, -1); \
	lua_pop(L, 1);

	static void unpack(lua_State* L, cocos2d::Size& size)
	{
		if (lua_istable(L, -1))
		{
			UNPACK_NUMBER("width", size.width);
			UNPACK_NUMBER("height", size.height);
		}
	}
	static void unpack(lua_State* L, cocos2d::Vec2& v)
	{
		if (lua_istable(L, -1))
		{
			UNPACK_NUMBER("x", v.x);
			UNPACK_NUMBER("y", v.y);
		}
	}


	static void unpack(lua_State* L, cocos2d::Color3B& c)
	{
		if (lua_istable(L, -1))
		{
			UNPACK_NUMBER("r", c.r);
			UNPACK_NUMBER("g", c.g);
			UNPACK_NUMBER("b", c.b);
		}
	}
	static void unpack(lua_State* L, cocos2d::Color4B& c)
	{
		if (lua_istable(L, -1))
		{
			UNPACK_NUMBER("r", c.r);
			UNPACK_NUMBER("g", c.g);
			UNPACK_NUMBER("b", c.b);
			UNPACK_NUMBER("a", c.a);
		}
	}
	static void unpack(lua_State* L, cocos2d::Color4F& c)
	{
		if (lua_istable(L, -1))
		{
			UNPACK_NUMBER("r", c.r);
			UNPACK_NUMBER("g", c.g);
			UNPACK_NUMBER("b", c.b);
			UNPACK_NUMBER("a", c.a);
		}
	}

	template<typename T>
	static void unpack(lua_State* L, const T* value)
	{
		if (lua_isuserdata(L, -1))
		{
			value = (T*)lua_touserdata(L, -1);
		}
		else if (lua_islightuserdata(L, -1))
		{
			value = (T*)lua_islightuserdata(L, -1);
		}
	}

    template<class T>
    static void unpack(lua_State* L, std::vector<T>& value)
    {
        if (lua_istable(L, -1))
        {
            size_t len = LuaWrapper::getLength(L, -1);
            for (size_t i = 1; i <= len; ++i)
            {
                T t;
                lua_rawgeti(L, -1, i);
                unpack(L, t);
                value.push_back(t);
            }
            lua_pop(L, 1);
        }
    }

    static void unpack(lua_State* L){}

    template<typename T, typename... Args>
    static void unpack(lua_State* L, T& value, Args&... args)
    {
        unpack(L, args...);
        unpack(L, value);
    }
    /*******************************************/

    static bool loadLuaFile(lua_State* L, const char* filename)
    {
        if (luaL_dofile(L, filename))
        {
            const char* err = lua_tostring(L, -1);
            printf("loadl file %s failed , err: %s\n",filename,err);
            return false;
        }
        return true;
    }

    /********************************************************/
    static bool loadGlobalTable(lua_State* L, const char* tableName)
    {
        if (tableName)
        {
            lua_getglobal(L, tableName);
            return lua_istable(L, -1);
        }
        return false;
    }

    template<typename T>
    static int loadFieldWithIndex(lua_State* L, int pos , const T& t)
    {
        if (lua_istable(L, pos))
        {
            pack(L, t);
            lua_rawget(L, pos-1);
            return 1;
        }
        return 0;
    }

    template<typename T, typename... Args>
    static int loadFieldWithIndex(lua_State* L, int pos, const T& t, Args... args)
    {
        int count = loadFieldWithIndex(L, pos, t);
        if (count > 0)
        {
            return loadFieldWithIndex(L, pos-1, args...) + count;
        }
        return count;
    }

    /************************************************************/
    template<typename T>
    static int loadField(lua_State* L, const T& t)
    {
        if (lua_istable(L,-1))
        {
            pack(L, t);
            lua_rawget(L, -2);
            return 1;
        }
        return 0;
    }

    template<typename T, typename... Args>
    static int loadField(lua_State* L, const T& t, Args... args)
    {
        int count = loadField(L, t);
        if (count > 0)
        {
            return loadField(L, args...) + count;
        }
        return count;
    }

    /************************************************************/

    static void split(const char* str, std::vector<std::string>& result)
    {
        const char* pre = str;
        while (*str)
        {
            while (*str && *str != '.') ++str;
            result.push_back(std::string(pre,str));
			*str == '.' ? ++str : str;
            pre = str;
        }
    }

    static lua_CFunction getLuaFunc(lua_State* L, const char* funcName)
    {
        std::vector<std::string> keys;
        split(funcName, keys);
        for (size_t i = 0; i < keys.size(); ++i)
        {
            if (i == 0)
            {
                lua_getglobal(L, keys[i].c_str());
            }
            else
            {
                lua_pushstring(L, keys[i].c_str());
                lua_rawget(L, -2);
                assert(lua_istable(L, -1));
            }
        }
        assert(lua_isfunction(L, -1));
        return lua_tocfunction(L, -1);
    }

    static int errFunc(lua_State* L)
    {
        return 0;
    }

    template<typename... Args>
    static bool callLuaFunc(lua_State* L, int ref, int rt , bool bRemove, const Args&... args)
    {
        const int len = sizeof...(args);
        unrefFunc(L, ref, bRemove);
        pack(L, args...);
        if (lua_pcall(L, len, rt, 0))
        {
            const char* err = lua_tostring(L, -1);
            printf("call lua func failed!!!, err:%s\n", err);
            return false;
        }
        return true;
    }

    template<typename... Args>
    static bool call(lua_State* L,int rt, const Args&... args)
    {
        if (lua_isfunction(L, -1))
        {
            const int len = sizeof...(args);
            pack(L, args...);
            if (lua_pcall(L, len, rt, 0))
            {
                const char* err = lua_tostring(L, -1);
                printf("call lua func failed!!!, err:%s\n",err);
                return false;
            }
            return true;
        }
        return false;
    }

    template<typename T>
    static void getCallResult(lua_State* L, T& value)
    {
        unpack(L, value);
    }

    template<typename T , typename... Args>
    static void getCallResult(lua_State* L, T& value, Args... args)
    {
        getCallResult(L, args...);
        unpack(L, value);
    }

	/********************************************/

	template<typename... Args>
	struct Tuple;

	template<>
	struct Tuple<> 
	{
		void pack(lua_State* L) {}
		void unpack(lua_State* L) {}
	};

	template<typename T, typename... Args>
	struct Tuple<T, Args...> : Tuple<Args...>
	{
		using ValueType = typename std::remove_reference<T>::type;
		ValueType value;
		Tuple() {}
		Tuple(const T& t, const Args&... args) : Tuple<Args...>(args...)
		{
			value = t;
		}
		void pack(lua_State* L)
		{
			LuaWrapper::pack(L, value);
			Tuple<Args...>::pack(L);
		}
		void unpack(lua_State* L)
		{
			Tuple<Args...>::unpack(L);
			LuaWrapper::unpack(L, value);
		}
	};

	template<size_t N, typename... Args>
	struct GetType;

	template<>
	struct GetType<0> {};

	template<typename T, typename... Args>
	struct GetType<1, T, Args...>
	{
		using TupleType = Tuple<T, Args...>;
		using ValueType = T;
	};

	template<size_t N, typename T, typename... Args>
	struct GetType<N, T, Args...> : GetType<N - 1, Args...> {};


	template<size_t N, typename... Args>
	typename GetType<N, Args...>::ValueType Get(const Tuple<Args...>& tp)
	{
		const GetType<N, Args...>::TupleType* p = static_cast<const GetType<N, Args...>::TupleType*>(&tp);
		return p->value;
	}

	template<size_t... N>
	struct IndexList
	{
		using NextType = IndexList<N..., sizeof...(N) + 1>;
		void print()
		{
			printf("N = %d\n",(int)sizeof...(N));
		}
	};


	template<typename... Args>
	struct ArgsList;

	template<>
	struct ArgsList<>
	{
		using IndexType = IndexList<>;
	};

	template<typename T,typename... Args>
	struct ArgsList<T,Args...>
	{
		using IndexType = typename ArgsList<Args...>::IndexType::NextType;
	};


	/*****************************************************/
	std::unordered_map<std::string, std::string> m_luaRegistMoudle;
	static int getMoudle(lua_State* L, const char* moudle)
	{
		lua_pushstring(L, moudle);
		lua_rawget(L, LUA_REGISTRYINDEX);
		if (lua_isnil(L, -1))
		{
			lua_pop(L, 1);
			return 0;
		}
		return 1;
	}
	
	template<typename T>
	static void createModule(lua_State* L, const char* moudle, const T*)
	{
		using Type = std::remove_pointer<std::remove_reference<std::remove_const<T>::type>::type>::type;
		const char* name = typeid(Type).name();
		if (!getMoudle(L, moudle))
		{
			lua_pushstring(L, name);
			lua_newtable(L);
			lua_pushstring(L, moudle);
			lua_pushvalue(L, -2);
			lua_rawset(L, LUA_REGISTRYINDEX);
			lua_rawset(L, LUA_REGISTRYINDEX);
		}
	}
	/***************************************************/

	template<typename T>
	static void createTable(lua_State* L, const char* module , const T* t)
	{
		if (getMoudle(L, module))
		{
			lua_newtable(L);
			lua_pushstring(L, CPP_PTR);
			lua_pushlightuserdata(L, (void*) const_cast<T*>(t));
			lua_rawset(L, -3);

			lua_pushstring(L, "__index");
			lua_pushvalue(L, -2);
			lua_rawset(L, -3);

			lua_pushstring(L, "__newindex");
			lua_pushvalue(L, -2);
			lua_rawset(L, -3);

			lua_pushvalue(L, -2);
			lua_setmetatable(L, -2);
		}
		else
		{
			assert(false);
			printf("%s is not registed\n", module);
		}
	}

	template<typename T>
	static void pack(lua_State* L, const T* t)
	{
		using TypeValue = std::remove_reference<std::remove_const<T>::type>::type;
		createTable(L, typeid(TypeValue).name(), t);
	}

	/***************************************************/
	template<typename RT, typename T, typename F, typename TP , size_t... N>
	static int callClassFunc(lua_State* L, T* t, F func, const TP& tp, void*, IndexList<N...>)
	{
		(t->*func)(Get<N>(tp)...);
		return 0;
	}

	template<typename RT, typename T, typename F, typename TP, size_t... N>
	static int callClassFunc(lua_State* L, T* t, F func, const TP& tp, RT*, IndexList<N...>)
	{
		RT rt = (t->*func)(Get<N>(tp)...);
		pack(L, rt);
		return 1;
	}

	template<typename T, typename RT, typename... Args>
	static int luaFunction(lua_State* L)
	{
		if (std::is_pointer<T>::value)
		{
			printf("T should be a class");
			return 0;
		}

		if (lua_gettop(L) != sizeof...(Args) + 1)
		{
			printf("args num is wrong\n");
			return 0;
		}

		if (!lua_istable(L, 1))
		{
			printf("the self is wrong\n");
			return 0;
		}

		lua_pushstring(L, CPP_PTR);
		lua_rawget(L, 1);

		if (!lua_isuserdata(L, -1))
		{
			printf("the obj ptr is null");
			return 0;
		}
		typedef Tuple<Args...> TP;
		typedef RT(T::*CF)(Args...);

		TP tp;
		CF fn = NULL;
		T* t = NULL;

		t = (T*)lua_touserdata(L, -1);
		lua_pop(L, 1);

		tp.unpack(L);

		lua_Integer intptr = lua_tointeger(L, lua_upvalueindex(1));
		memcpy(&fn, &intptr, sizeof(fn));

		return callClassFunc<RT, T, CF, TP>(L, t, fn, tp, (RT*)NULL, ArgsList<Args...>::IndexType());
	}

	template<typename RT, typename T,typename... Args>
	static void registerClassFunc(lua_State* L, const char* moudle, const char* name, RT (T::*func)(Args...))
	{
		if (getMoudle(L, moudle))
		{
			lua_CFunction luafunc = luaFunction<std::remove_const<std::remove_reference<T>::type>::type,  std::remove_const<std::remove_reference<RT>::type>::type, Args...>;
			lua_pushstring(L, name);

			lua_Integer ptrInt = 0;
			memcpy(&ptrInt, (void*)(&func), sizeof(lua_Integer));

			lua_pushinteger(L, ptrInt);
			lua_pushcclosure(L, luafunc, 1);
			lua_rawset(L, -3);
		}
		clear(L);
	}

	template<typename RT, typename T, typename... Args>
	static void registerClassFunc(lua_State* L, const char* moudle, const char* name, RT (T::*func)(Args...) const  )
	{
		RT(T::*f)(Args...);
		f = (RT(T::*)(Args...))(func);
		registerClassFunc(L, moudle, name, f);
	}

	template<typename T,typename... Args>
	static void registerClassMutiReturnFunc(lua_State* L, const char* moudle, const char* name, void(T::*func)(Args...))
	{
		if (getMoudle(L, moudle))
		{
			//lua_CFunction luafunc = luaFunction<std::remove_const<std::remove_reference<T>::type>::type, void, Args...>;
			lua_pushstring(L, name);

			lua_Integer ptrInt = 0;
			memcpy(&ptrInt, (void*)(&func), sizeof(lua_Integer));

			lua_pushinteger(L, ptrInt);
			lua_pushcclosure(L, luafunc, 1);
			lua_rawset(L, -3);
		}
		clear(L);
	}

	/**************************************************************/

	template<typename RT, typename F, typename TP, size_t... N>
	static int callStaticFunc(lua_State* L, F func, const TP& tp, void*, IndexList<N...>)
	{
		(*func)(Get<N>(tp)...);
		return 0;
	}

	template<typename RT, typename F, typename TP, size_t... N>
	static int callStaticFunc(lua_State* L, F func, const TP& tp, RT*, IndexList<N...>)
	{
		RT rt = (*func)(Get<N>(tp)...);
		pack(L, rt);
		return 1;
	}

	template<typename RT, typename... Args>
	static int luaStaticFunction(lua_State* L)
	{
		if (lua_gettop(L) != sizeof...(Args))
		{
			printf("args num is wrong\n");
			return 0;
		}

		typedef Tuple<Args...> TP;
		typedef RT(*CF)(Args...);

		TP tp;
		CF fn = NULL;

		tp.unpack(L);

		lua_Integer intptr = lua_tointeger(L, lua_upvalueindex(1));
		memcpy(&fn, &intptr, sizeof(fn));

		return callStaticFunc<RT, CF, TP>(L,fn, tp, (RT*)NULL, ArgsList<Args...>::IndexType());
	}

	template<typename RT, typename... Args>
	static void registerStaticFunc(lua_State* L, const char* module, const char* name, RT(*func)(Args...))
	{
		if (getMoudle(L, module))
		{
			lua_CFunction luafunc = luaStaticFunction<RT, Args...>;
			lua_pushstring(L, name);

			lua_Integer ptrInt = 0;
			memcpy(&ptrInt, (void*)(&func), sizeof(lua_Integer));

			lua_pushinteger(L, ptrInt);
			lua_pushcclosure(L, luafunc, 1);
			lua_rawset(L, -3);
		}
		clear(L);
	}

	template<typename T,typename... Args>
	static int creator(lua_State* L)
	{
		Tuple<Args...> tp;
		tp.unpack(L);
		return callStaticFunc(&T::create, tp, T*, ArgsList<Args...>::IndexType());
	}


	
}

#endif