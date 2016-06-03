#ifndef __ACTIONA0001_H__
#define __ACTIONA0001_H__

#include "cocos2d.h"
#include "ShuipingErrorNode.h"
USING_NS_CC;

class ActionA0001 : public CCActionInterval
{
public:
	ActionA0001();
	~ActionA0001();

	/**
	* 构造ActionA0001方法
	* @param duration 动画时间
	* @return
	*/
	static ActionA0001* create(float duration);
	virtual void update(float time);
	bool init(float duration);
	void startWithTarget(CCNode *pTarget);

private:
	float length;				// 记录纵坐标差值
	float bigY;					// 记录在上的Y值
};


#endif