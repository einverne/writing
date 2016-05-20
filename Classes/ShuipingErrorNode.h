#ifndef __ShuipingErrorNode_H__
#define __ShuipingErrorNode_H__

#include "cocos2d.h"
USING_NS_CC;

class ShuipingErrorNode :
	public CCNode
{
public:
	ShuipingErrorNode(void);
	CREATE_FUNC(ShuipingErrorNode);
	~ShuipingErrorNode(void);
	virtual void draw();

	void setPoint(CCPoint start_point, CCPoint end_point);

	CCPoint start_point_;
	CCPoint end_point_;
};



#endif


