#include "Connection.h"
#include "NetworkManager.h"
#include "event.h"
#include "listener.h"
#include "bufferevent.h"
#include "evutil.h"
#include "GameUtil.h"
#include "GameLua.h"
#include "LuaWrapper.h"
#include "Log.h"

int Connection::s_maxRewriteCount = 3;
int ProtoData::s_serilId = 0;

void readbe(struct bufferevent* be, void* data)
{
    Connection* client = (Connection*)data;
    client->read();
}

void writebe(struct bufferevent* be, void* data)
{
    Connection* client = (Connection*)data;
    client->write();
}

void eventsbe(struct bufferevent *be, short events, void *data)
{
    Connection* client = (Connection*)(data);
    client->events(events);
}

/**********************************************************************/

ProtoData::ProtoData()
    :m_cmd(NULL)
    , m_content(NULL)
    , m_data(NULL)
    , m_contentLen(NULL)
    , m_serilId(NULL)
    , m_ptm(NULL)
{
}

ProtoData::~ProtoData()
{
    if (m_data)
    {
        delete[] m_data;
    }
}

void ProtoData::pack(int cmd, const char* data, int len)
{
    m_data = new char[HEAD_LEN + len];
    m_serilId = (int*)(m_data);
    m_cmd = (int*)(m_data + sizeof(int));
    m_contentLen = (int*)(m_data + sizeof(int) * 2);
    m_content = m_data + sizeof(int) * 3;
    *m_cmd = htons(cmd);
    *m_serilId = htons(++s_serilId);
    *m_contentLen = htons(len);
    memcpy(m_content, data, len);
}

char* ProtoData::alloc(size_t size)
{
    assert(size >= HEAD_LEN);
    m_data = new char[size];
    m_serilId = (int*)(m_data);
    m_cmd = (int*)(m_data + sizeof(int));
    m_contentLen = (int*)(m_data + sizeof(int) * 2);
    m_content = m_data + sizeof(int) * 3;
    memset(m_data, 0, size);
    return m_data;
}

void ProtoData::setHeader(const char* data)
{
    int len = ntohl(*(int*)(data + sizeof(int) * 2)) + HEAD_LEN;
    m_data = new char[len];
    m_serilId = (int*)(m_data);
    m_cmd = (int*)(m_data + sizeof(int));
    m_contentLen = (int*)(m_data + sizeof(int) * 2);
    m_content = m_data + sizeof(int) * 3;
    m_ptm = m_content;
    memset(m_data, 0, len);
    memcpy(m_data, data, HEAD_LEN);
}

void ProtoData::addContent(const char* data, int len)
{
    assert(m_ptm + len <= m_data + getDataLen());
    memcpy(m_ptm, data, len);
    m_ptm += len;
}

int ProtoData::getSerialId() const
{
    return ntohl(*m_serilId);
}

int ProtoData::getCmd() const
{
    return ntohl(*m_cmd);
}

const char* ProtoData::getContent() const
{
    return m_content;
}

int ProtoData::getContentLen() const
{
    return ntohl(*m_contentLen);
}

const char* ProtoData::getData() const
{
    return m_data;
}

int ProtoData::getDataLen() const
{
    return ntohl(*m_contentLen) + HEAD_LEN;
}

/*************************************************************/

Connection::Connection(struct bufferevent* be, event_base* base)
    :m_be(be)
    ,m_writeCount(0)
    ,m_curWriteProto(NULL)
    ,m_luaConnectedRefId(0)
    , m_luaRecvRefId(0)
    , m_luaCloseRefId(0)
    , m_luaFinishRefId(0)
    , m_luaWTimeoutRefId(0)
    , m_luaRTimeoutRefId(0)
    , m_lastSerialId(-1)
    , m_base(base)
{
    m_l = GameLua::getInstance()->getLuaState();
}

Connection::Connection(event_base* base)
    :m_be(NULL)
    ,m_writeCount(0)
    ,m_curWriteProto(NULL)
    ,m_luaConnectedRefId(0)
    , m_luaRecvRefId(0)
    , m_luaCloseRefId(0)
    , m_luaFinishRefId(0)
    , m_luaWTimeoutRefId(0)
    , m_luaRTimeoutRefId(0)
    , m_lastSerialId(-1)
    , m_base(base)
{
    m_l = GameLua::getInstance()->getLuaState();
    m_be = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
}

Connection::~Connection()
{
    clear();
}

bool Connection::init()
{
    evutil_socket_t fd = bufferevent_getfd(m_be);
    if (fd > 0)
    {
        evutil_make_listen_socket_reuseable(fd);
        evutil_make_socket_nonblocking(fd);
    }
    bufferevent_setcb(m_be, readbe, writebe, eventsbe, this);
    int ret = bufferevent_enable(m_be, EV_READ | EV_WRITE);
    if (ret == -1)
    {
        g_log("the bufferevent is disable");
        return false;
    }
    return true;
}

void Connection::connect(const char* ip, int port)
{
    struct sockaddr_in serversock;
    memset(&serversock, 0, sizeof(serversock));
#ifdef _WIN32 
    inet_pton(AF_INET, ip,&serversock.sin_addr);
#else
    serversock.sin_addr.s_addr = inet_addr(ip);
#endif
    serversock.sin_port = htons(port);
    serversock.sin_family = AF_INET;
    if (!m_be)
    {
        m_be = bufferevent_socket_new(m_base, -1, BEV_OPT_CLOSE_ON_FREE);
        bufferevent_setcb(m_be, readbe, writebe, eventsbe, this);
        bufferevent_enable(m_be, EV_READ | EV_WRITE);
    }
    int ret = bufferevent_socket_connect(m_be, (struct sockaddr*)&serversock, sizeof(serversock));
    if (ret == -1)
    {
        g_log("connect failed!!!\n");
    }
}

void Connection::send(int cmd, const char* data, size_t len)
{
    ProtoData* pro = new ProtoData;
    pro->pack(cmd, data, len);
    m_prolist.push_back(pro);
    if(!m_curWriteProto)
        this->write();
}

void Connection::connected()
{
    g_log("connected...\n");
    evutil_socket_t fd = bufferevent_getfd(m_be);
    evutil_make_listen_socket_reuseable(fd);
    evutil_make_socket_nonblocking(fd);
    Lua_CallRef(m_l, m_luaConnectedRefId, 0);
}

void Connection::write()
{
    if (m_prolist.size() > 0)
    {
        ProtoData* nextPro = m_prolist.front();
        m_prolist.pop_front();
        m_writeCount = (nextPro == m_curWriteProto) ? m_writeCount + 1 : 0;
        m_curWriteProto = nextPro;
        bufferevent_write(m_be, m_curWriteProto->getData(), m_curWriteProto->getDataLen());
    }
}

void Connection::read()
{
    struct evbuffer* input = bufferevent_get_input(m_be);
    size_t size = evbuffer_get_length(input);
    if (size <= 0)
        return;

    ProtoData pro;
    char* buff = pro.alloc(size);
    bufferevent_read(m_be, buff, size);

    int serialId = pro.getSerialId();
    if (serialId > m_lastSerialId)
    {
        int cmd = pro.getCmd();
        const char* data = pro.getContent();
        int len = pro.getContentLen();
        Lua_CallRef(m_l, m_luaRecvRefId, 0, cmd, data, len);
        m_lastSerialId = serialId;
    }
}

void Connection::finish()
{
    g_log("the connection fnish");
    Lua_CallRef(m_l, m_luaFinishRefId, 0);
    NetworkManager::getInstance()->removeConnection(m_be);
}

void Connection::close()
{
    g_log("the conncetion close");
    Lua_CallRef(m_l, m_luaCloseRefId, 0);
    NetworkManager::getInstance()->removeConnection(m_be);
}

void Connection::wtmout()
{
    evbuffer* buffer = bufferevent_get_output(m_be);
    evbuffer_unfreeze(buffer, 1);
    evbuffer_drain(buffer, evbuffer_get_length(buffer));
    bufferevent_enable(m_be, EV_WRITE);
    if (m_writeCount < s_maxRewriteCount)
    {
        m_prolist.push_front(m_curWriteProto);
        this->write();
        Lua_CallRef(m_l, m_luaWTimeoutRefId, 0, m_writeCount, s_maxRewriteCount);
    }
    else
    {
        this->close();
    }
}

void Connection::rtmout()
{
    evbuffer* buffer = bufferevent_get_output(m_be);
    evbuffer_unfreeze(buffer, 1);
    evbuffer_drain(buffer, evbuffer_get_length(buffer));
    bufferevent_enable(m_be, EV_READ);
}

void Connection::events(int events)
{
    if (events & BEV_EVENT_CONNECTED)
    {
        this->connected();
    }
    else if (events & BEV_EVENT_EOF)
    {
        this->finish();
    }
    else if (events & BEV_EVENT_ERROR)
    {
        this->close();
    }
    else if (events & BEV_EVENT_TIMEOUT)
    {
        if (events & BEV_EVENT_WRITING)
            this->wtmout();
        if (events & BEV_EVENT_WRITING)
            this->rtmout();
    }
}

void Connection::clear()
{
    if (m_be)
    {
        bufferevent_free(m_be);
        m_be = NULL;
    }
    m_curWriteProto = NULL;
    m_writeCount = 0;
}

void Connection::update(float dt)
{
    if (m_prolist.size() > 0)
    {
        this->write();
    }
}
/***************************************/

int lua_connection_init(lua_State* L)
{
    Connection* connect = Lua_GetPointer<Connection>(L, 1);
    if (!connect) return 0;
    return Lua_SetBool(L, connect->init());
}

int lua_connection_send(lua_State* L)
{
    Connection* connect = Lua_GetPointer<Connection>(L, 1);
    if (!connect) return 0;
    int cmd = Lua_GetInt(L, 2);
    size_t len = 0;
    const char* msg = lua_tolstring(L, 3, &len);

    if (cmd > 0 && len > 0)
        connect->send(cmd, msg, len);
    else
        g_log("send cmd:%d failed", cmd);
    return 0;
}

int lua_connection_setMaxRewriteCount(lua_State* L)
{
    Connection* connect = Lua_GetPointer<Connection>(L, 1);
    if (!connect) return 0;
    int count = Lua_GetInt(L, 2);
    connect->setMaxRewriteCount(count);
    return 0;
}
int lua_connection_setLuaConnectedRefId(lua_State* L)
{
    Connection* connect = Lua_GetPointer<Connection>(L, 1);
    if (!connect) return 0;
    if (lua_isfunction(L, 2))
    {
        int luaID = luaL_ref(L, LUA_REGISTRYINDEX);
        connect->setLuaConnectedRefId(luaID);
    }
    else
    {
        g_log("set lua connected handler failed");
    }
    return 0;
}
int lua_connection_setLuaRecvRefId(lua_State* L)
{
    Connection* connect = Lua_GetPointer<Connection>(L, 1);
    if (!connect) return 0;
    if (lua_isfunction(L, 2))
    {
        int luaID = luaL_ref(L, LUA_REGISTRYINDEX);
        connect->setLuaRecvRefId(luaID);
    }
    else
    {
        g_log("set lua recv handler failed");
    }
    return 0;
}
int lua_connection_setLuaCloseRefId(lua_State* L)
{
    Connection* connect = Lua_GetPointer<Connection>(L, 1);
    if (!connect) return 0;
    if (lua_isfunction(L, 2))
    {
        int luaID = luaL_ref(L, LUA_REGISTRYINDEX);
        connect->setLuaCloseRefId(luaID);
    }
    else
    {
        g_log("set lua close handler failed");
    }
    return 0;
}
int lua_connection_setLuaFinishRefId(lua_State* L)
{
    Connection* connect = Lua_GetPointer<Connection>(L, 1);
    if (!connect) return 0;
    if (lua_isfunction(L, 2))
    {
        int luaID = luaL_ref(L, LUA_REGISTRYINDEX);
        connect->setLuaFinishRefId(luaID);
    }
    else
    {
        g_log("set lua finish handler failed");
    }
    return 0;
}
int lua_connection_setLuaWTimeoutRefId(lua_State* L)
{
    Connection* connect = Lua_GetPointer<Connection>(L, 1);
    if (!connect) return 0;
    if (lua_isfunction(L, 2))
    {
        int luaID = luaL_ref(L, LUA_REGISTRYINDEX);
        connect->setLuaWTimeoutRefId(luaID);
    }
    else
    {
        g_log("set lua wtimeout handler failed");
    }
    return 0;
}
int lua_connection_connect(lua_State* L)
{
    Connection* connect = Lua_GetPointer<Connection>(L, 1);
    if (!connect) return 0;
    const char* ip = Lua_GetString(L, 2);
    int port = Lua_GetInt(L, 3);
    connect->connect(ip, port);
    return 0;
}

int lua_open_connection_module(lua_State* L)
{
    luaL_Reg reg[] = {
        {"init",lua_connection_init },
        {"send",lua_connection_send },
        {"connect",lua_connection_connect},
        {"setMaxRewriteCount",lua_connection_setMaxRewriteCount },
        {"setLuaConnectedHandler",lua_connection_setLuaConnectedRefId },
        {"setLuaRecvHandler",lua_connection_setLuaRecvRefId },
        {"setLuaCloseHandler",lua_connection_setLuaCloseRefId },
        {"setLuaFinishHandler",lua_connection_setLuaFinishRefId },
        {"setLuaTimeoutHandler",lua_connection_setLuaWTimeoutRefId },
        {NULL,NULL}
    };
    return Lua_CreateModule(L, "Connection", reg, NULL);
}