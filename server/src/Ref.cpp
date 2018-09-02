#include "Ref.h"

int Ref::kRefCount = 0;

Ref::Ref()
    :m_luaID(0)
    ,m_refCount(1)
    ,m_refId(++kRefCount)
{
    RefManager::getInstance()->addRef(this);
}

Ref::~Ref()
{

}

int Ref::retain()
{
    return ++m_refCount;
}

int Ref::release()
{
    if (--m_refCount == 0)
    {
        delete this;
    }
    return m_refCount;
}

/*****************************************************/

RefManager* RefManager::getInstance()
{
    static RefManager mgr;
    return &mgr;
}

RefManager::RefManager()
{

}

RefManager::~RefManager()
{

}

void RefManager::addRef(Ref* ref) 
{
    m_refs.push_back(ref);
}

void RefManager::update(float dt)
{
    if (m_refs.size() <= 0)
        return;
    for (auto& iter = m_refs.begin(); iter != m_refs.end(); ++iter)
    {
        (*iter)->release();
    }
    m_refs.resize(0);
}