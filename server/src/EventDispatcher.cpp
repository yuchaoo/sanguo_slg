#include "EventDispatcher.h"
#include "rapidjson.h"
#include "stream.h"
#include "document.h"
#include "NetworkManager.h"
#include "stringbuffer.h"
#include "prettywriter.h"
using namespace std;
using namespace rapidjson;

EventDispatcher* EventDispatcher::getInstance()
{
        static EventDispatcher ed;
        return &ed;
}

void EventDispatcher::init()
{
        NetworkManager::getInstance()->addObserver(this);
}

void EventDispatcher::addListener(int cmd,void* listener, ProtoCall callback)
{
        ListenInfo info = {listener,callback};
        unordered_map<int,vector<ListenInfo>>::iterator iter = m_callbacks.find(cmd);
        if(iter == m_callbacks.end())
        {
                auto it = m_callbacks.insert(std::make_pair(cmd,std::vector<ListenInfo>()));
                it.first->second.push_back(info);
        }
        else
        {
                 iter->second.push_back(info);
        }
}

void EventDispatcher::removeListener(int cmd, void * listener)
{
        auto iter = m_callbacks.find(cmd);
        if(iter != m_callbacks.end())
        {
                for(auto it = iter->second.begin(); it != iter->second.end(); ++it)
                {
                        if(it->listener == listener)
                        {
                                iter->second.erase(it);
                                return;
                        }
                }
        }
}

 void EventDispatcher::sendMessage(struct bufferevent* be, const rapidjson::Value& json)
 {
         StringBuffer sb;
         PrettyWriter<StringBuffer> writer(sb);
         json.Accept(writer);
         auto iter = json.FindMember("cmd");
         if(iter != json.MemberEnd())
         {
                 printf("cmd = %d, json = %s\n",iter->value.GetInt(),sb.GetString());
                 //NetworkManager::getInstance()->write(be,sb.GetString(),sb.GetLength());
         }
 }

void EventDispatcher::onReadDataNotify(struct bufferevent* be, const char* data,size_t size)
{
        Document doc;
        doc.SetObject();

        doc.Parse(data);
        rapidjson::Value::MemberIterator iter = doc.FindMember("cmd");
        if(iter == doc.MemberEnd())
        {
                 return;
        }

        int cmd = iter->value.GetInt();
        auto it = m_callbacks.find(cmd);
        if(it != m_callbacks.end())
        {
                for(auto v = it->second.begin(); v != it->second.end(); ++v)
                {
                        v->callback(be,doc);
                }
        }
}

void EventDispatcher::onCmdNotify(const char* data,size_t size)
{

}
