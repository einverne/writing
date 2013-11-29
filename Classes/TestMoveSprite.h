#ifndef __TESTMOVESPRITE_H__
#define __TESTMOVESPRITE_H__


#include "cocos2d.h"
USING_NS_CC;

#pragma once
class TestMoveSprite:public cocos2d::CCLayerColor
{
private:
    bool is_selected;
public:
    TestMoveSprite(void);
    ~TestMoveSprite(void);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(TestMoveSprite);


    //     void onEnter();
    //     void onExit();

    //     bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    //     void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    //     void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

    void keyBackClicked();

	//测试在可触摸Layer下加popup
	void popup();
	void buttonCallback(CCNode* pSender);
};

#endif