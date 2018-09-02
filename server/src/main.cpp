#include <iostream>
#include "NetworkManager.h"
#include "EventDispatcher.h"
#include "Ref.h"
#include "GameLua.h"

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
        printf("gamelua int failed!!!\n");
        return 1;
    }

    bool ret = NetworkManager::getInstance()->init(6666);
    if (!ret)
    {
        printf("network init failed!!!\n");
        return 1;
    }
    printf("network init secceed\n");

    GameLua::getInstance()->luaMain();
    NetworkManager::getInstance()->dispatch();
    return 0;
}