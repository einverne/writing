#ifndef __BarycentreNode_H__
#define __BarycentreNode_H__

#include "cocos2d.h"
USING_NS_CC;

using namespace std;


class BarycentreNode : public CCNode
{
public:
	BarycentreNode(void);
	CREATE_FUNC(BarycentreNode);
	~BarycentreNode(void);
	virtual void draw();

	void setBarycentrePoint(CCPoint point);
	void setLeftPoint(CCPoint startpoint,CCPoint endpoint);
	void setRightPoint(CCPoint startpoint,CCPoint endpoint);

	CCPoint OrigionPoint;           // ≥ı ºŒª÷√
	CCPoint MovePoint;            
	CCPoint leftstart, leftend;
	CCPoint rightstart, rightend;
	CCPoint leftdelta, rightdelta;
};

#endif