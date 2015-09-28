
#ifndef __BackgroundLayer_H__
#define __BackgroundLayer_H__

#include "cocos2d.h"
#include "../CocosWidget/cocos-widget.h"
#include "tools/DataTool.h"
USING_NS_CC;
using namespace cocos2d::cocoswidget;

class BackgroundLayer : public CCLayerColor
{
public:
	BackgroundLayer();
	~BackgroundLayer();
	virtual bool init();
	virtual void onEnter();
	virtual void keyBackClicked();
	CREATE_FUNC(BackgroundLayer);
	CCSprite* tianzige;
	CCSprite* tianzige_draw;
	CCSprite* title_bar;
private:
	void menuBack(CCObject* pSender);			//·µ»Ø²Ëµ¥
	void onToggle(CCObject* pSender);			//Toggle onClick
// 	void menuNext(CCObject* pSender);
};



#endif
