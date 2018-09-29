#pragma once
#include<string>
#include<share.h>
#include<memory>
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class GameLua
{
public:
    static GameLua* getInstance();
    lua_State* getLuaState();
    bool init();
    void addLuaPath(const char* dirpath);
    void setLuaLoader(lua_CFunction fn, int index);
	void setLuaFunc();
    bool luaMain(const char* luafile);
    void clear();
private:

    GameLua();
    ~GameLua();
private:
    lua_State* m_L;
    std::string m_luapath;
};