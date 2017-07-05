#ifndef __DynamicDrawing_H__
#define __DynamicDrawing_H__

#include "cocos2d.h"
#include "TwoLineNode.h"
USING_NS_CC;

class DynamicDrawing : public CCActionInterval
{
public:
	DynamicDrawing();
	~DynamicDrawing();

	//
	// 构造方法
	// @param duration 动画时间
	// @return
	//
	static DynamicDrawing* create(float duration);
	virtual void update(float time);
	bool init(float duration);
	void startWithTarget(CCNode *pTarget);

private:
	CCPoint deltaPoint1;
	CCPoint deltaPoint2;
	CCPoint deltaPoint3;
	CCPoint deltaPoint4;
};


#endif