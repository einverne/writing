#ifndef __TouchLayer_H__
#define __TouchLayer_H__

#include "cocos2d.h"
#include "TcharacterLayer.h"
#include "HcharacterLayer.h"
USING_NS_CC;
using namespace std;

class TouchLayer : public CCLayerColor
{
public:
	TouchLayer();
	~TouchLayer();
	virtual bool init(TcharacterLayer* t,HcharacterLayer* h);
	virtual void onEnter();
	virtual void onExit();
// 	CREATE_FUNC(TouchLayer);
	static TouchLayer* create(TcharacterLayer* t,HcharacterLayer* h);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();

	CC_SYNTHESIZE_RETAIN(TcharacterLayer* ,Tlayer,Tlayer);
	CC_SYNTHESIZE_RETAIN(HcharacterLayer* ,Hlayer,Hlayer);
private:
	int index;				//保存当前书写的第几笔
	int errorType;			//当前书写笔画错误类型
	vector<CCPoint> points;		//保存书写点
	bool isOutside;
	bool isStartOutside;
};



#endif // !__TouchLayer_H__
