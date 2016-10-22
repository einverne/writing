#ifndef __DotLineNode_H__
#define __DotLineNode_H__

#include "cocos2d.h"
USING_NS_CC;

class DotLineNode :
	public CCNode
{
public:
	DotLineNode(void);
	CREATE_FUNC(DotLineNode);
	~DotLineNode(void);
	virtual void draw();

	void setCenterPoint(CCPoint center_point);
	void setLength(int length);
	void setOrientation(int orientation);

	CCPoint centerPoint_;
	int length_;			// 正确的位置
	int orientation_;		// 0 为竖向，1 为横向，默认竖向
};



#endif


