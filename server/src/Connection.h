#ifndef _CLIENT_EVENT_
#define _CLIENT_EVENT_
#include <unordered_map>
#include "GameLua.h"
#include "Ref.h"

struct bufferevent;
#define HEAD_LEN 12

struct ProtoData
{
    static int s_serilId;
    int* m_serilId, *m_cmd, *m_contentLen;
    char * m_data, *m_content, *m_ptm;
    ProtoData();
    ~ProtoData();

    void pack(int cmd, const char* data, int len);
    char* alloc(size_t size);

    void setHeader(const char* data);
    void addContent(const char* data, int len);

    int getSerialId() const;
    int getCmd() const;
    const char* getContent() const;
    int getContentLen() const;
    const char* getData() const;
    int getDataLen() const;
};

class Connection
    :public Ref
{
public:
    Connection(struct bufferevent* be, lua_State* L);
    ~Connection();
    bool init();
    void send(int cmd, const char* data, size_t len);
    void connected();
    void write();
    void read();
    void events(int et);
    void close();
    void finish();
    void wtmout();
    void rtmout();
    void clear();
    void update(float dt);
public:
    static void setMaxRewriteCount(int count) { s_maxRewriteCount = count; }
    void setLuaConnectedRefId(int ref) { m_luaConnectedRefId = ref; }
    void setLuaRecvRefId(int ref) { m_luaRecvRefId = ref; }
    void setLuaCloseRefId(int ref) { m_luaCloseRefId = ref; }
    void setLuaFinishRefId(int ref) { m_luaFinishRefId = ref; }
    void setLuaWTimeoutRefId(int ref) { m_luaWTimeoutRefId = ref; }
    void setLuaRTimeoutRefId(int ref) { m_luaRTimeoutRefId = ref; }
    const struct bufferevent* getBufferEvent() const { return m_be; }
private:
    static int s_maxRewriteCount;
    struct bufferevent* m_be;
    int m_writeCount;
    int m_lastSerialId;

    lua_State* m_l;
    int m_luaConnectedRefId;
    int m_luaRecvRefId;
    int m_luaCloseRefId;
    int m_luaFinishRefId;
    int m_luaWTimeoutRefId;
    int m_luaRTimeoutRefId;

    ProtoData* m_curWriteProto;
    std::list<ProtoData*> m_prolist;
};

int lua_open_connection_module(lua_State* L);

#endif