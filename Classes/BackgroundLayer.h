
#ifndef __BackgroundLayer_H__
#define __BackgroundLayer_H__

#include "cocos2d.h"
USING_NS_CC;

class BackgroundLayer : public CCLayer
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
	void menuNext(CCObject* pSender);
};



#endif
