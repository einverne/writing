#ifndef __SECONDSCENE_H__
#define __SECONDESCENE_H__


#include "cocos2d.h"
#pragma once

using namespace std;
USING_NS_CC;

class SecondScene :
    public CCLayerColor
{
private:
    string stroke;
public:
    SecondScene(void);
    ~SecondScene(void);
    virtual bool init();

    static cocos2d::CCScene * scene();
    CREATE_FUNC(SecondScene);

//     bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//     void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    void ccTouchesBegan(cocos2d::CCSet* pTouches,cocos2d::CCEvent* pEvent);
    void ccTouchesEnded(cocos2d::CCSet* pTouches,cocos2d::CCEvent* pEvent);
    void ccTouchesMoved(cocos2d::CCSet* pTouches,cocos2d::CCEvent* pEvent);

    void keyBackClicked();

};

#endif