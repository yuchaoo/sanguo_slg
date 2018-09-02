#include "NetworkManager.h"

#include<string.h>
#include<stdio.h>
#include<chrono>
#include <assert.h>

#include "event.h"
#include "listener.h"
#include "bufferevent.h"
#include "evutil.h"
#include "GameUtil.h"
#include "Connection.h"
#include "GameLua.h"
#include "LuaWrapper.h"

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
    fprintf(stderr, "[%s] %s\n", severity_str, msg); 
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
    m_luaUpdateRefId = ref;
}

bool NetworkManager::init(int port)
{
    memset(m_listenAddr,0,sizeof(sockaddr_in));
    m_listenAddr->sin_family = AF_INET;
    m_listenAddr->sin_port = htons(port);
    
    m_eventBase = event_base_new();
    if(!m_eventBase)
    {
        printf("Could not initialize libevent\n");
        return false;
    }

    event_set_log_callback(&eventLog);

    m_listener = evconnlistener_new_bind(m_eventBase,evlistener,(void*)m_eventBase, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,1,(struct sockaddr*)m_listenAddr ,sizeof(sockaddr_in));
    if(!m_listener)
    {
        int errCode = EVUTIL_SOCKET_ERROR();
        const char* reason = evutil_socket_error_to_string(errCode);
        event_base_free(m_eventBase);
        printf("Could not create a listener, msg : %s\n",reason);
        return false;
    }

    evutil_gettimeofday(&m_timeval, NULL);

    struct timeval tv = {1,0};
    m_timer = event_new(m_eventBase,-1, EV_PERSIST, evtimer,this);
    event_add(m_timer, &tv);
    
    return true;
}

void NetworkManager::listener(struct evconnlistener* listener, evutil_socket_t fd,struct sockaddr* addr)
{
    struct bufferevent* be = bufferevent_socket_new(m_eventBase,fd, BEV_OPT_CLOSE_ON_FREE);
    if(!be)
    {
        printf("Could not create a bufferevent\n");
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

void NetworkManager::dispatch()
{
    if(m_eventBase && m_listener)
    {
        printf("network dispatch...\n");
        event_base_dispatch(m_eventBase);
        evconnlistener_free(m_listener);
        event_base_free(m_eventBase);
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
	log("update escape = %f", escape);
}
