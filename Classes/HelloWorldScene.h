#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "CCLuaEngine.h"

USING_NS_CC;

class HelloWorld : public CCLayerColor
{
protected:
    //cocos2d::CCArray * _targets;
    //cocos2d::CCArray * _projectiles;
    //int _projectilesDestroyed;
public:
    HelloWorld();
    ~HelloWorld();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

    //add
    void draw();
    void menuStartGame(CCObject * pSender);
    void menuSecondScene(CCObject* pSender);
    void menuTestMoveScene(CCObject * pSender);
    void menuGameWallScene(CCObject* pSender);
    void keyBackClicked();

	//测试弹出框
	void popup();
	//弹出框按钮回调函数
	void buttonCallBack(CCNode* pSender);

    const char* getStringFromLua();
    //void sqlite_test();

    //void addTarget();
    //
    //void spriteMoveFinished(CCNode* sender);

    // button     void menuTestMoveScene(CCObject * pSender);
};

#endif // __HELLOWORLD_SCENE_H__
