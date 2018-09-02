#include "Connection.h"
#include "NetworkManager.h"
#include "event.h"
#include "listener.h"
#include "bufferevent.h"
#include "evutil.h"
#include "GameUtil.h"
#include "GameLua.h"
#include "LuaWrapper.h"

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

Connection::Connection(struct bufferevent* be, lua_State* L)
    :m_be(be)
    ,m_l(L)
    ,m_writeCount(0)
    ,m_curWriteProto(NULL)
    ,m_luaConnectedRefId(0)
    , m_luaRecvRefId(0)
    , m_luaCloseRefId(0)
    , m_luaFinishRefId(0)
    , m_luaWTimeoutRefId(0)
    , m_luaRTimeoutRefId(0)
{

}

Connection::~Connection()
{
}

bool Connection::init()
{
    bufferevent_setcb(m_be, readbe, writebe, eventsbe, this);
    bufferevent_enable(m_be, EV_READ | EV_WRITE);
    return true;
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
    log("connected...\n");
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

    int cmd = pro.getCmd();
    const char* data = pro.getContent();
    int len = pro.getContentLen();

    Lua_CallRef(m_l, m_luaRecvRefId, 0,cmd, data, len);
}

void Connection::finish()
{
    log("the connection fnish");
    Lua_CallRef(m_l, m_luaFinishRefId, 0);
    this->clear();
}

void Connection::close()
{
    log("the conncetion close");
    Lua_CallRef(m_l, m_luaCloseRefId, 0);
    this->clear();
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
    NetworkManager::getInstance()->removeConnection(m_be);
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