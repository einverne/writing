#ifndef _GAMEWALL_SCENE_H_
#define _GAMEWALL_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class GameWall : public cocos2d::CCLayer
{
public:
    GameWall(void);
    ~GameWall(void);
    virtual bool init();

    static cocos2d::CCScene* scene();

    void menuCloseCallback(CCObject* pSender);
    CREATE_FUNC(GameWall);
    void keyBackClicked();
    virtual void onEnter();
    virtual void onExit();
    virtual void registerWithTouchDispatcher();  
    virtual bool ccTouchBegan (CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved (CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded (CCTouch *pTouch, CCEvent *pEvent);
//     virtual void ccTouchesBegan (CCSet *pTouches, CCEvent *pEvent);
//     virtual void ccTouchesMoved (CCSet *pTouches, CCEvent *pEvent);
//     virtual void ccTouchesEnded (CCSet *pTouches, CCEvent *pEvent);

private:
    int tagMap;
    int tagLabel;
    CCSprite* oneCharacter;
    CCArray* characters;
    CCTMXTiledMap* map;
    CCArray* sprites;
    void update(CCPoint diff);

    void menuCall(CCObject* pSender);
    void menuCallCeshiButton(CCObject* pSender);
};


#endif // _GAMEWALL_SCENE_H_