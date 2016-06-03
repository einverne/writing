#ifndef __ActionA0003_H__
#define __ActionA0003_H__

#include "cocos2d.h"
#include "OnePointNode.h"
USING_NS_CC;

class ActionA0003 : public CCActionInterval
{
public:
	ActionA0003();
	~ActionA0003();

	/**
	* 构造ActionA0003方法
	* @param duration 动画时间
	* @return
	*/
	static ActionA0003* create(float duration);
	virtual void update(float time);
	bool init(float duration);
	void startWithTarget(CCNode *pTarget);

private:
	CCPoint deltaPoint;
	CCPoint startPoint;
	CCPoint endPoint;
};


#endif