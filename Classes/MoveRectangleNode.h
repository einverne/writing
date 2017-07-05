#ifndef __MoveRectangleNode_H__
#define __MoveRectangleNode_H__

#include "cocos2d.h"
#include "RectangleNode.h"
USING_NS_CC;

class MoveRectangleNode : public CCActionInterval
{
public:
	MoveRectangleNode();
	~MoveRectangleNode();

	//
	// 构造方法
	// @param duration 动画时间
	// @return
	//
	static MoveRectangleNode* create(float duration);
	virtual void update(float time);
	bool init(float duration);
	void startWithTarget(CCNode *pTarget);

	CCPoint delta_lefttop;
	CCPoint delta_leftbottom;
	CCPoint delta_righttop;
	CCPoint delta_rightbottom;
};

#endif