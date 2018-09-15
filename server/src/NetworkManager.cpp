#include "NetworkManager.h"

#include<string.h>
#include<stdio.h>
#include<chrono>
#include <assert.h>
#include <signal.h>
#include <thread>
#include <conio.h>

#include "event.h"
#include "listener.h"
#include "bufferevent.h"
#include "evutil.h"
#include "GameUtil.h"
#include "Connection.h"
#include "GameLua.h"
#include "LuaWrapper.h"
#include "Log.h"

#ifdef _WIN32
#include<WinSock2.h>
#endif

void evlistener(struct evconnlistener* listener, evutil_socket_t fd,struct sockaddr* addr, int socklen, void* data)
{
    NetworkManager* net = (NetworkManager*)data;
    net->listener(listener,fd,addr);
}

void evtimer(int fd, short event, void* data)
{
    NetworkManager* net = (NetworkManager*)(data);
    net->update();
}

void evsignal(evutil_socket_t sig, short events, void *user_data)
{
    g_log("to exit in 2 seconds.\n");
    struct event_base *base = (struct event_base *)user_data;
    struct timeval delay = { 2, 0 };
    event_base_loopexit(base, &delay);
}
void evquit(evutil_socket_t fd, short evetns, void * data)
{
    struct event_base* base = (struct event_base*) data;
    event_base_loopbreak(base);
}

void eventLog(int severity, const char *msg)
{
    const char *severity_str;
    switch (severity)
    {
    case _EVENT_LOG_DEBUG:
        severity_str = "debug";
        break;
    case _EVENT_LOG_MSG:
        severity_str = "msg";
        break;
    case _EVENT_LOG_WARN:
        severity_str = "warn";
        break;
    case _EVENT_LOG_ERR:
        severity_str = "err";
        break;
    default:
        severity_str = "???";
        break;
    }
    g_log("[%s] %s\n", severity_str, msg);
}

/********************************************************/

NetworkManager* NetworkManager::getInstance()
{
    static NetworkManager mgr;
    return &mgr;
}

NetworkManager::NetworkManager()
:m_listener(NULL)
,m_eventBase(NULL)
,m_listenAddr(NULL)
,m_timer(NULL)
, m_luaCreateConnRefId(0)
,m_luaUpdateRefId(0)
{
    m_timeval.tv_sec = m_timeval.tv_usec = 0;
    m_listenAddr = new sockaddr_in;
#ifdef _WIN32
    WSADATA ws;
    WSAStartup(MAKEWORD(2, 2), &ws);
#endif
}

NetworkManager::~NetworkManager()
{
    delete m_listenAddr;
    WSACleanup();
    if (m_eventBase)
    {
        struct timeval delay = { 0, 0 };
        event_base_loopexit(m_eventBase, &delay);
        m_eventBase = NULL;
    }
}

void NetworkManager::addObserver(NetObserver* observer)
{
    for(auto iter = m_observers.begin(); iter != m_observers.end(); ++iter)
    {
        if((*iter) == observer)
            return;
    }
    m_observers.push_back(observer);
}

void NetworkManager::removeObserver(NetObserver* observer)
{
    for(auto iter = m_observers.begin(); iter != m_observers.end(); ++iter)
    {
        if((*iter) == observer)
        {
            m_observers.erase(iter);
            return;
        }
    }
}

void NetworkManager::setLuaUpdateRefId(int ref)
{
    if (m_luaUpdateRefId > 0)
    {
        lua_State* L = GameLua::getInstance()->getLuaState();
        luaL_unref(L, LUA_REGISTRYINDEX, m_luaUpdateRefId);
    }
    m_luaUpdateRefId = ref;
}

void NetworkManager::setLuaCreateConnRefId(int ref)
{
    if (m_luaCreateConnRefId > 0)
    {
        lua_State* L = GameLua::getInstance()->getLuaState();
        luaL_unref(L, LUA_REGISTRYINDEX, m_luaCreateConnRefId);
    }
    m_luaCreateConnRefId = ref;
}

bool NetworkManager::init(int port)
{
    memset(m_listenAddr,0,sizeof(sockaddr_in));
    m_listenAddr->sin_family = AF_INET;
    m_listenAddr->sin_port = htons(port);
    
    m_eventBase = event_base_new();
    if(!m_eventBase)
    {
        g_log("Could not initialize libevent");
        return false;
    }

    event_set_log_callback(&eventLog);

    m_listener = evconnlistener_new_bind(m_eventBase,evlistener,(void*)m_eventBase, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,1,(struct sockaddr*)m_listenAddr ,sizeof(sockaddr_in));
    if(!m_listener)
    {
        event_base_free(m_eventBase);
        g_log("Could not create a listener");
        return false;
    }

    evutil_gettimeofday(&m_timeval, NULL);

    struct timeval tv = {1,0};
    m_timer = event_new(m_eventBase,-1, EV_PERSIST, evtimer,this);
    if (!m_timer || event_add(m_timer, &tv) < 0)
    {
        evconnlistener_free(m_listener);
        event_base_free(m_eventBase);
        return false;
    }
    
    struct event *signal_event =  evsignal_new(m_eventBase, SIGINT, evsignal, m_eventBase);
    if (!signal_event || event_add(signal_event, NULL) < 0)
    {
        evconnlistener_free(m_listener);
        event_base_free(m_eventBase);
        return false;
    }
    
    //监听终端输入事件
    /*struct timeval tv1 = {5,0 };
    struct event* ev_cmd = event_new(m_eventBase, -1,  EV_PERSIST, evquit, m_eventBase);
    if (!ev_cmd || event_add(ev_cmd, &tv1) < 0)
    {
        evconnlistener_free(m_listener);
        event_base_free(m_eventBase);
        return false;
    }*/

    initGmThread();

    return true;
}

void NetworkManager::initGmThread() 
{
    std::thread thread([=](NetworkManager* mgr) {
        bool isStart = false;
        char text[1024];
        int index = 0;
        while (1)
        {
            if (_kbhit())
            {
                if (!isStart)
                {
                    g_log.setBlock(true);
                    char c = _getch();
                    isStart = true;
                    cout << "请输入你的命令:" << endl;
                    memset(text, 0, sizeof(text));
                    index = 0;
                }
                else
                {
                    cin.getline(text, sizeof(text));
                    g_log.setBlock(false);
                    isStart = false;
                    index = 0;
                    mgr->addGmCommand(text);
                }
            }
        }
    },this);
    thread.detach();
}

void NetworkManager::listener(struct evconnlistener* listener, evutil_socket_t fd,struct sockaddr* addr)
{
    struct bufferevent* be = bufferevent_socket_new(m_eventBase,fd, BEV_OPT_CLOSE_ON_FREE);
    if(!be)
    {
        g_log("Could not create a bufferevent");
        event_base_loopbreak(m_eventBase);
        return ;
    }
    addConnection(be);
}

void NetworkManager::addConnection(struct bufferevent* be)
{
    lua_State* L = GameLua::getInstance()->getLuaState();
    Connection* conn = new Connection(be, L);
    m_connects.push_back(conn);
    for (auto iter = m_observers.begin(); iter != m_observers.end(); ++iter)
    {
        (*iter)->onConnectionCreated(conn);
    }
    Lua_CallRef(L, m_luaCreateConnRefId, 0);
}

void NetworkManager::removeConnection(struct bufferevent* be)
{
    for (auto& iter = m_connects.begin(); iter != m_connects.end(); ++iter)
    {
        Connection* conn = (*iter);
        if (conn->getBufferEvent() == be)
        {
            m_connects.erase(iter);
            for (auto iter = m_observers.begin(); iter != m_observers.end(); ++iter)
            {
                (*iter)->onConnectionRemoved(conn);
            }
            break;
        }
    }
}

void NetworkManager::addGmCommand(const char* gm)
{
    m_gmmutex.lock();
    m_gmlist.push_back(gm);
    m_gmmutex.unlock();
}

void NetworkManager::handleGm()
{
    std::string gm;
    m_gmmutex.lock();
    if (m_gmlist.size() > 0)
    {
        gm = m_gmlist.front();
        m_gmlist.pop_front();
    }
    m_gmmutex.unlock();
    if (gm.empty())
    {
        return;
    }
    g_log("***************handle gm : %s********************",gm.c_str());
    if (gm == "quit")
    {
        
        event_base_loopbreak(m_eventBase);
    }
    g_log("***********************handle gm end**********************");
}

void NetworkManager::dispatch()
{
    if(m_eventBase && m_listener)
    {
        g_log("network dispatch...");
        event_base_dispatch(m_eventBase);
        event_del(m_timer);
        evconnlistener_free(m_listener);
        event_base_free(m_eventBase);
        m_eventBase = NULL;
    }
}

void NetworkManager::update()
{
    struct timeval tv, difference;
    evutil_gettimeofday(&tv, NULL);
    evutil_timersub(&tv, &m_timeval, &difference);
    double escape = difference.tv_sec + (difference.tv_usec / 1.0e6);
    m_timeval = tv;
    for (auto iter = m_connects.begin(); iter != m_connects.end(); ++iter)
    {
        (*iter)->update(escape);
    }
    for (auto& iter = m_observers.begin(); iter != m_observers.end(); ++iter)
    {
        (*iter)->onUpdateNotify(escape);
    }
    lua_State* L = GameLua::getInstance()->getLuaState();
    Lua_CallRef(L, m_luaUpdateRefId, 0, escape);

    handleGm();
	g_log("update escape = %f", escape);
}

/**************************************************************/

int lua_network_init(lua_State* L)
{
    NetworkManager* mgr = Lua_GetPointer<NetworkManager>(L,1);
	if (!mgr)
	{
		return Lua_SetBool(L,false);
	}
    return Lua_SetBool(L,mgr->init(Lua_GetInt(L,1)));
}

int lua_network_setLuaUpdateHandler(lua_State* L)
{
    NetworkManager* mgr = Lua_GetPointer<NetworkManager>(L, 1);
    if (!mgr || !lua_isfunction(L,2)) 
        return 0;
    int ref = luaL_ref(L, LUA_REGISTRYINDEX);
    mgr->setLuaUpdateRefId(ref);
    return 0;
}

int lua_network_setLuaCreateConnHandler(lua_State* L)
{
    NetworkManager* mgr = Lua_GetPointer<NetworkManager>(L, 1);
    if (!mgr || !lua_isfunction(L, 2))
        return 0;
    int ref = luaL_ref(L, LUA_REGISTRYINDEX);
    mgr->setLuaCreateConnRefId(ref);
    return 0;
}

int lua_network_dispatch(lua_State* L)
{
    NetworkManager* mgr = Lua_GetPointer<NetworkManager>(L, 1);
    if (!mgr) return 0;
    mgr->dispatch();
    return 0;
}

int lua_open_network_module(lua_State* L)
{
    luaL_Reg reg[] = {
        {"init",lua_network_init},
        {"setLuaUpdateHandler",lua_network_setLuaUpdateHandler },
        {"setLuaCreateConnHandler",lua_network_setLuaCreateConnHandler },
		{"dispatch",lua_network_dispatch },
        {NULL,NULL}
    };
    Lua_CreateModule(L, "net", reg, NULL, NetworkManager::getInstance());
    return 0;
}