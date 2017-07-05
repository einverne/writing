#ifndef __MoveToUniformSpace_H__
#define __MoveToUniformSpace_H__

#include "cocos2d.h"
#include "UniformSpaceNode.h"
USING_NS_CC;

using namespace std;

class MoveToUniformSpace : public CCActionInterval
{
public:
	MoveToUniformSpace();
	~MoveToUniformSpace();

	/**
	* 构造方法
	* @param duration 动画时间
	* @return
	*/
	static MoveToUniformSpace* create(float duration);
	virtual void update(float time);
	bool init(float duration);
	void startWithTarget(CCNode *pTarget);

private:
	vector<float> length;				// 记录纵坐标差值
	vector<float> origin;				// 记录Y值
	int flag;
};


#endif