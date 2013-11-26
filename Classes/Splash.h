#ifndef __SPLASH_H__
#define __SPLASH_H__

#include "cocos2d.h"
#pragma once
USING_NS_CC;

class Splash :
    public cocos2d::CCLayer
{
public:
    Splash(void);
    ~Splash(void);

    virtual bool init();
    static CCScene* scene();

    CREATE_FUNC(Splash);

    void onEnter();
    void onExit();
    void onExitTransitionDidStart();

    void finishSplash(float dt);
};

#endif