#ifndef __MoveToRightPlaceInterval_H__
#define __MoveToRightPlaceInterval_H__

#include "cocos2d.h"
#include "Stroke.h"
USING_NS_CC;

class MoveToRightPlaceInterval : public CCActionInterval
{
public:
	MoveToRightPlaceInterval();
	~MoveToRightPlaceInterval();

	/**
	* 构造MoveToRightPlaceInterval方法
	* @param duration 动画时间
	* @param index
	* @param s
	* @return
	*/
	static MoveToRightPlaceInterval* create(float duration,int index,Stroke s);
// 	CCObject* copyWithZone(CCZone* pZone);
	virtual void update(float time);
	bool init(float duration,int index,Stroke s);
	void startWithTarget(CCNode *pTarget);
private:
	int index;
	Stroke m_stroke;
};


#endif