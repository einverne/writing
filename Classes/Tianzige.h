#pragma once

#include "cocos2d.h"
#include "Character.h"
USING_NS_CC;
using namespace std;

class CTianzige :
    public CCSprite
{
public:
    CTianzige(void);
    ~CTianzige(void);
    virtual void draw();
    Character character;

};

