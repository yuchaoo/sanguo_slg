#ifndef _EVENT_DISPATCHER_
#define _EVENT_DISPATCHER_
#include <functional>
#include<vector>
#include<string>
#include<unordered_map>
#include "NetworkManager.h"
#include "document.h"
using namespace std;
typedef std::function<void(struct bufferevent* be, rapidjson::Value& jsonValue)> ProtoCall;

class EventDispatcher : public NetObserver
{
public:
        struct ListenInfo
        {
                void* listener;
                ProtoCall callback;
        };
        static EventDispatcher* getInstance();
        void init();

        void addListener(int cmd,void* listener, ProtoCall callback);
        void removeListener(int cmd, void * listener);
        void sendMessage(struct bufferevent* be, const rapidjson::Value& json);
        virtual void onReadDataNotify(struct bufferevent* be, const char* data,size_t size);
        virtual void onCmdNotify(const char* data,size_t size) ;
private:
        std::unordered_map<int,std::vector<ListenInfo>> m_callbacks;
};

#endif // _EVENT_DISPATCHER_
