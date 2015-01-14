
#ifndef __BackgroundLayer_H__
#define __BackgroundLayer_H__

#include "cocos2d.h"
#include "cocos-widget.h"
#include "tools\DataTool.h"
USING_NS_CC;
using namespace cocos2d::cocoswidget;

class BackgroundLayer : public CCLayerColor
{
public:
	BackgroundLayer();
	~BackgroundLayer();
	virtual bool init();
	virtual void keyBackClicked();
	CREATE_FUNC(BackgroundLayer);
	CCSprite* tianzige;
	CCSprite* tianzige_draw;
private:
	void menuBack(CCObject* pSender);			//·µ»Ø²Ëµ¥
	void onClick(CCObject* pSender);			//Toggle onClick
// 	void menuNext(CCObject* pSender);
};



#endif
