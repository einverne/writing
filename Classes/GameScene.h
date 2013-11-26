#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"

#pragma once
class GameScene:public cocos2d::CCLayerColor
{
protected:
    cocos2d::CCArray * _targets;
    cocos2d::CCArray * _projectiles;
    int _projectilesDestroyed;
public:
    GameScene(void);
    ~GameScene(void);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);

    //add
    void addTarget();

    void spriteMoveFinished(CCNode* sender);

    void gameLogic(float dt);
    void update(float dt);
    void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

    void keyBackClicked();

};


#endif