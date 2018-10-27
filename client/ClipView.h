#pragma once

#include "B2View.h"

class ClipView
    :public B2View
{
public:
    static ClipView* create(b2World* world,float worldx,float worldy);
    virtual bool init(float worldx,float worldy);
private:
    ClipView(b2World* world);
    ~ClipView();
private:
    
};