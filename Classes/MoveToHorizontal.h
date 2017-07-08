#ifndef __ACTIONA0001_H__
#define __ACTIONA0001_H__

#include "cocos2d.h"
#include "MultiPointNode.h"
USING_NS_CC;

using namespace std;

class MoveToHorizontal : public CCActionInterval
{
public:
	MoveToHorizontal();
	~MoveToHorizontal();

	/**
	* 构造ActionA0001方法
	* @param duration 动画时间
	* @return
	*/
	static MoveToHorizontal* create(float duration);
	virtual void update(float time);
	bool init(float duration);
	void startWithTarget(CCNode *pTarget);

private:
	vector<float> length;				// 记录纵坐标差值
	vector<float> originY;				// 记录Y值
};


#endif