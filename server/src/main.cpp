#include <iostream>
#include "NetworkManager.h"
#include "EventDispatcher.h"
#include "Ref.h"
#include "GameLua.h"
#include "GameUtil.h"

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
        log("gamelua int failed!!!\n");
        return 1;
    }
	GameLua::getInstance()->luaMain();

    /*bool ret = NetworkManager::getInstance()->init(6666);
    if (!ret)
    {
        log("network init failed!!!\n");
        return 1;
    }
    log("network init secceed\n");

    GameLua::getInstance()->luaMain();
    log("lua main exe finish");
    NetworkManager::getInstance()->dispatch();*/

    return 0;
}