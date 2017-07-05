#ifndef __MoveToRightPlace_H__
#define __MoveToRightPlace_H__

#include "cocos2d.h"
#include "OnePointNode.h"
USING_NS_CC;

class MoveToRightPlace : public CCActionInterval
{
public:
	MoveToRightPlace();
	~MoveToRightPlace();

	/**
	* 构造MoveToRightPlace方法
	* @param duration 动画时间
	* @return
	*/
	static MoveToRightPlace* create(float duration);
	virtual void update(float time);
	bool init(float duration);
	void startWithTarget(CCNode *pTarget);

private:
	CCPoint deltaPoint;
	CCPoint startPoint;
	CCPoint endPoint;
};


#endif