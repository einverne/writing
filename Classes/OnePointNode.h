#ifndef __OnePointNode_H__
#define __OnePointNode_H__

#include "cocos2d.h"
USING_NS_CC;

class OnePointNode :
	public CCNode
{
public:
	OnePointNode(void);
	CREATE_FUNC(OnePointNode);
	~OnePointNode(void);
	virtual void draw();

	void setPoint(CCPoint start_point);
	void setRightPoint(CCPoint right_point);

	CCPoint centerPoint_;
	CCPoint rightPoint_;			// 正确的位置
};



#endif


