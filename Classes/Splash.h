#ifndef __SPLASH_H__
#define __SPLASH_H__

#include "cocos2d.h"
#pragma once
using namespace std;
USING_NS_CC;

class Splash : public CCLayer
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
private:
    void finishSplash(float dt);
	void initDB(string db_name);
};

#endif
