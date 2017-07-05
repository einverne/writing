#ifndef __RectangleNode_H__
#define __RectangleNode_H__

#include "cocos2d.h"
USING_NS_CC;

using namespace std;

class RectangleNode : public CCNode
{
public:
	RectangleNode(void);
	CREATE_FUNC(RectangleNode);
	~RectangleNode(void);
	virtual void draw();

	void setPoint(CCPoint lefttop, CCPoint leftbottom, CCPoint righttop, CCPoint rightbottom);
	void setSize(CCSize MattSize);

	CCPoint m_lefttop;
	CCPoint m_leftbottom;
	CCPoint m_righttop;
	CCPoint m_rightbottom;

	CCPoint origin_lefttop;
	CCPoint origin_leftbottom;
	CCPoint origin_righttop;
	CCPoint origin_rightbottom;

	CCSize m_MattSize;
};



#endif

