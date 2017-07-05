#ifndef __MoveToVertical_H__
#define __MoveToVertical_H__

#include "cocos2d.h"
#include "MultiPointNode.h"
USING_NS_CC;

using namespace std;

class MoveToVertical : public CCActionInterval
{
public:
	MoveToVertical();
	~MoveToVertical();

	/**
	* 构造MoveToVertical方法
	* @param duration 动画时间
	* @return
	*/
	static MoveToVertical* create(float duration);
	virtual void update(float time);
	bool init(float duration);
	void startWithTarget(CCNode *pTarget);

private:
	vector<float> length;				// 记录横坐标差值
	vector<float> originX;				// 记录X值
};


#endif