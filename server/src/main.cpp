#include <iostream>
#include "NetworkManager.h"
#include "EventDispatcher.h"
#include "Ref.h"
#include "GameLua.h"
#include "GameUtil.h"
#include "log.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

int main()
{
    if (!GameLua::getInstance()->init())
    {
        g_log("gamelua int failed!!!\n");
        return 1;
    }
	//GameLua::getInstance()->luaMain();

    bool ret = NetworkManager::getInstance()->init(6666);
    if (!ret)
    {
        g_log("network init failed!!!\n");
        return 1;
    }
    g_log("network init secceed\n");

   // GameLua::getInstance()->luaMain();
    g_log("lua main exe finish");
    NetworkManager::getInstance()->dispatch();

    return 0;
}