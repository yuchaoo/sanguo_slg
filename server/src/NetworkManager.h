#ifndef _NET_WORK_MANAGER_
#define _NET_WORK_MANAGER_

#include "event2/util.h"
#include "event_struct.h"

#include<vector>
#include <unordered_map>

struct event_base;
struct evconnlistener;
struct sockaddr_in;
class Connection;

struct NetObserver
{
        virtual void onCmdNotify(const char* data,size_t size) {}
        virtual void onConnectionCreated(Connection* conn) {}
        virtual void onConnectionRemoved(Connection* conn) {}
        virtual void onUpdateNotify(float dt) {}
};

class NetworkManager
{
public:
    static NetworkManager* getInstance();
    void addObserver(NetObserver* observer);
    void removeObserver(NetObserver* observer);
    void setLuaUpdateRefId(int ref);
    void setLuaCreateConnRefId(int ref);
    bool init(int port);
    void listener(struct evconnlistener* listener, evutil_socket_t, struct sockaddr* addr);
    void addConnection(struct bufferevent* be);
    void removeConnection(struct bufferevent* be);
    void dispatch();
    void update();
public:

private:
    NetworkManager();
    ~NetworkManager();
private:
    evconnlistener * m_listener;
    event_base* m_eventBase;
    struct event* m_timer;
    struct timeval m_timeval;
    int m_luaUpdateRefId;
    int m_luaAddConnRefId;
    sockaddr_in* m_listenAddr;
    std::vector<NetObserver*> m_observers;
    std::vector<Connection*> m_connects;
};

#endif // _NET_WORK_MANAGER_
