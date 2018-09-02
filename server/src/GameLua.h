#pragma once
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
    bool luaMain();
    
    void clear();
private:
    GameLua();
    ~GameLua();
private:
    lua_State* m_L;
};