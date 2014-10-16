#ifndef __MainScene_H__
#define __MainScene_H__

#include "cocos2d.h"
USING_NS_CC;

class MainScene : public CCLayer
{
public:
	MainScene();
	~MainScene();
	CREATE_FUNC(MainScene);
	static CCScene* scene();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void keyBackClicked();


private:
	void menuSelected(CCObject* pSender);
	void isExit(CCNode* pNode);
	void Setting(CCObject* pSender);
	void settingCallBack(CCNode* pNode);
};



#endif
