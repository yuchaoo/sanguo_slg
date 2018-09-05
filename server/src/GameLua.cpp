#include "GameLua.h"
#include "LuaWrapper.h"
#include "GameUtil.h"
#include <fstream>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

extern int luaopen_protobuf_c(lua_State *L);
extern "C" int luaopen_lpeg(lua_State *L);

int luaLoader(lua_State* L)
{
    std::string path = lua_tostring(L, -1);
    if (path.empty())
        return 0;
    size_t end = path.rfind(".lua");

    std::string tmp = path;
    if (end == std::string::npos)
        tmp = path.substr(0, end);

    size_t pos = tmp.find(".");
    while (pos != std::string::npos)
    {
        tmp.replace(pos, pos + 1, "/");
        pos = tmp.find(".", pos + 1);
    }

    std::string filepath = std::string("./scripts/") + tmp + ".lua";

    FILE* file = fopen(filepath.c_str(), "rb");
    if (!file)
    {
        log("read the file %s failed!!", filepath.c_str());
        return 0;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char* buffer = new  unsigned char[size];
    size = fread(buffer, sizeof(char), size, file);
    fclose(file);

    unsigned char* content = buffer;
    size_t contentLen = size;
    if (size >= 3)
    {
        unsigned bom = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16);
        if (bom == 0xBFBBEF || bom == 0xEFBBBF)
        {
            content += 3;
            contentLen -= 3;
        }
    }

    int ret = luaL_loadbuffer(L, (char*)content, contentLen, path.c_str());
    if (ret != 0)
    {
        const char* str = lua_tostring(L, -1);
        log("load script %s failed!!!, msg:%s", path.c_str(), str);
        lua_pop(L, 1);
        delete[] buffer;
        return 0;
    }
    delete[] buffer;
    return 1;
}

int luaTraceBack(lua_State* L)
{
    const char* msg = lua_tostring(L, -1);
    if (msg)
    {
        luaL_traceback(L, L, msg, 1);
        msg = lua_tostring(L, -1);
        log("***********************trace back start************************");
        log(msg);
        log("***********************trace back end**************************");
        lua_pop(L, 2);
    }
    return 0;
}

int luaClass(lua_State* L)
{
	int n = lua_gettop(L);
	const char* dir = lua_tostring(L, 1);
	const char* super = lua_tostring(L, 2);

	lua_getglobal(L, "package");
	lua_getfield(L, -1, "loaded");

    if (super)
    {
        if (lua_getfield(L, -1, super) != LUA_TTABLE)
        {
            log("can not create class %s , reason : can not find the super class %s", dir, super);
            return 0;
        }
        if (lua_getfield(L, -2, dir) == LUA_TTABLE)
        {
            log("%s have be a class", dir);
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
        if (lua_getfield(L, -2, dir) == LUA_TTABLE)
        {
            log("%s have be a class", dir);
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

    luaL_openlibs(m_L);
    luaopen_lpeg(m_L);
    luaopen_protobuf_c(m_L);

    addLuaPath("./script");
    setLuaLoader(luaLoader, 2);
	setLuaClass(luaClass);

    return true;
}

lua_State* GameLua::getLuaState()
{
    return m_L;
}

bool GameLua::luaMain()
{
    lua_pushcfunction(m_L, luaTraceBack);
    if (luaL_loadfile(m_L, "scripts/main.lua"))
    {
        const char* err = lua_tostring(m_L, -1);
        log("load main.lua failed, err:%s", err);
        lua_pop(m_L, 2);
        return false;
    }
    if (lua_pcall(m_L, 0, LUA_MULTRET, -2))
    {
        const char* err = lua_tostring(m_L, -1);
        printf("execute file failed , err: %s\n", err);
        lua_pop(m_L, 2);
        return false;
    }
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
    std::string str = std::string(dirpath) + "/?.lua;" + std::string(dirpath) + "/?;" + path;

    lua_pushstring(m_L, str.c_str());
    lua_setfield(m_L, -3, "path");
    lua_pop(m_L, 3);

    log("modify lua path:");
    std::vector<std::string> paths;
    Lua_Split(str.c_str(), ';', paths);
    for (size_t i = 0; i < paths.size(); ++i)
    {
        log(paths[i].c_str());
    }
}

void GameLua::setLuaClass(lua_CFunction fn)
{
	lua_pushcfunction(m_L, luaClass);
	lua_setglobal(m_L, "class");
}

void GameLua::clear()
{
    lua_close(m_L);
    m_L = NULL;
}

