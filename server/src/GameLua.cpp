#include "GameLua.h"
#include "LuaWrapper.h"
#include "GameUtil.h"
#include "NetworkManager.h"
#include "Connection.h"
#include "Log.h"
#include "FileSystem.h"
#include <fstream>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

extern int luaopen_protobuf_c(lua_State *L);
extern "C" int luaopen_lpeg(lua_State *L);

/***************************************************************/
GameLua* GameLua::getInstance()
{
    static GameLua lua;
    return &lua;
}

GameLua::GameLua()
    :m_L(NULL)
{

}

GameLua::~GameLua()
{
    if (m_L)
        lua_close(m_L);
}

bool GameLua::init()
{
    m_L = luaL_newstate();
    if (!m_L)
    {
        printf("create lua_state failed!!!\n");
        return false;
    }
    lua_newtable(m_L);
    lua_setglobal(m_L,LUA_CPP);

    luaL_openlibs(m_L);
    luaL_requiref(m_L, "lpeg", luaopen_lpeg, 0);
    luaL_requiref(m_L, "protobuf.c", luaopen_protobuf_c, 0);

    setLuaLoader(Lua_Loader, 2);
    const string & scriptsdir = FileSystem::getInstance()->getScriptsDirectory();
    addLuaPath(scriptsdir.c_str());

	setLuaFunc();
    lua_open_network_module(m_L);
    lua_open_connection_module(m_L);

    return true;
}

lua_State* GameLua::getLuaState()
{
    return m_L;
}

bool GameLua::luaMain(const char* luafile)
{
    FileSystem* fs = FileSystem::getInstance();
    size_t size = 0;

    string path = fs->getLuaFilePath(luafile);
    int ret = luaL_loadfile(m_L, path.c_str());
    if (ret)
    {
        const char* err = lua_tostring(m_L, -1);
        g_log("load main.lua failed, err:%s", err);
        lua_pop(m_L, 1);
        return false;
    }
    
    ret = lua_pcall(m_L, 0, LUA_MULTRET, 0);
    if (ret)
    {
        const char* err = lua_tostring(m_L, -1);
        g_log("execute file failed , err: %s\n", err);
        lua_pop(m_L, 1);
        return false;
    }
    g_log("-------------lua main----------");
    return true;
}

void GameLua::setLuaLoader(lua_CFunction fn, int index)
{
    lua_getglobal(m_L, LUA_LOADLIBNAME);
    lua_getfield(m_L, -1, "searchers");
    int len = lua_rawlen(m_L, -1);
    for (int i = len; i >= index; --i)
    {
        lua_rawgeti(m_L, -1, i);
        lua_rawseti(m_L, -2, i + 1);
    }
    lua_pushcfunction(m_L, fn);
    lua_rawseti(m_L, -2, index);
    lua_pop(m_L, 2);
}

void GameLua::addLuaPath(const char* dirpath)
{
    lua_getglobal(m_L, "_G");
    lua_getfield(m_L, -1, "package");
    lua_getfield(m_L, -1, "path");

    const char* path = lua_tostring(m_L, -1);
    std::string str = std::string(dirpath) + "?.lua;" + std::string(dirpath) + "?;" + path;

    lua_pushstring(m_L, str.c_str());
    lua_setfield(m_L, -3, "path");
    lua_pop(m_L, 3);

    g_log("modify lua path:");
    std::vector<std::string> paths;
    Lua_Split(str.c_str(), ';', paths);
    for (size_t i = 0; i < paths.size(); ++i)
    {
        g_log(paths[i].c_str());
    }
}

void GameLua::setLuaFunc()
{
	luaL_Reg reg[] = {
		{"cppclass",Lua_Class},
		{"print",Lua_Print},
        {"logError",Lua_Error},
		{NULL,NULL}
	};
	lua_getglobal(m_L, "_G");
	luaL_setfuncs(m_L, reg, 0);
	lua_pop(m_L, 1);
}

void GameLua::clear()
{
    lua_close(m_L);
    m_L = NULL;
}

