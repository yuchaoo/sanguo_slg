#pragma once
#include <vector>

class Ref
{
public:
    Ref();
    ~Ref();
    int retain();
    int release();
public:
    int m_luaID;
    int m_refId;
    static int kRefCount;
private:
    int m_refCount;
};

class RefManager
{
public:
    static RefManager* getInstance();
    RefManager();
    ~RefManager();
    void addRef(Ref* ref);
    void update(float dt);
private:
    std::vector<Ref*> m_refs;
};