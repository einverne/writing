#ifndef __MainScene_H__
#define __MainScene_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class MainScene : public CCLayerColor
{
public:
	MainScene();
	~MainScene();
	CREATE_FUNC(MainScene);
	static CCScene* scene();
	virtual bool init();
// 	virtual void onEnter();
// 	virtual void onExit();
	virtual void keyBackClicked();

// 	void scrollViewDidScroll(CCScrollView* view);
// 	void scrollViewDidZoom(CCScrollView* view);

private:
	void menuSelected(CCObject* pSender);
	void isExit(CCNode* pNode);
	void Setting(CCObject* pSender);
	void settingCallBack(CCNode* pNode);
	void buttonClick(CCObject* pSender);
	CCObject* gridviewDataSource(CCObject* pConvertView, unsigned int idx);
};



#endif
