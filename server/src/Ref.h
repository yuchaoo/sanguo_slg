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