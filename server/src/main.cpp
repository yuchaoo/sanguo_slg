#include <iostream>
#include "NetworkManager.h"
#include "EventDispatcher.h"
#include "Ref.h"
#include "GameLua.h"
#include "GameUtil.h"
#include "log.h"
#include "FileSystem.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        g_log("arg is not enough");
        return 0;
    }
    const char* workpath = argv[1];
    FileSystem::getInstance()->setWorkspace(workpath);

    if (!GameLua::getInstance()->init())
    {
        g_log("gamelua int failed!!!\n");
        return 1;
    }
  
    GameLua::getInstance()->luaMain();
    return 0;
}