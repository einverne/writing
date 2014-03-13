#ifndef __TouchLayer_H__
#define __TouchLayer_H__

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class TouchLayer : public CCLayerColor
{
public:
	TouchLayer();
	~TouchLayer();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	CREATE_FUNC(TouchLayer);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();

private:
	int index;				//保存当前书写的第几笔
	int errorType;			//当前书写笔画错误类型

};



#endif // !__TouchLayer_H__
