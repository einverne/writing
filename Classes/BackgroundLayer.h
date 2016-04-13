
#ifndef __BackgroundLayer_H__
#define __BackgroundLayer_H__

#include "cocos2d.h"
#include "../CocosWidget/cocos-widget.h"
#include "tools/DataTool.h"
USING_NS_CC;
using namespace cocos2d::cocoswidget;

// set static images and set easy/difficult button
class BackgroundLayer : public CCLayerColor
{
public:
	BackgroundLayer();
	~BackgroundLayer();
	virtual bool init();
	virtual void onEnter();
	virtual void keyBackClicked();
	CREATE_FUNC(BackgroundLayer);
	CCSprite* tianzige_;                 // standard character
	CCSprite* tianzige_draw_;            // handwrting character
	CCSprite* title_bar_;                // title bar
private:
	void menuBack(CCObject* pSender);			//menu Back callback
	void onToggle(CCObject* pSender);			//Toggle onClick easy or hard
};



#endif
