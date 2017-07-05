#ifndef __TwoLineNode_H__
#define __TwoLineNode_H__

#include "cocos2d.h"
USING_NS_CC;

using namespace std;


class TwoLineNode : public CCNode
{
public:
	TwoLineNode(void);
	CREATE_FUNC(TwoLineNode);
	~TwoLineNode(void);
	virtual void draw();

	void setPoint(vector<CCPoint> points);


	vector<CCPoint> OrigionPoints;           // ≥ı ºµƒŒª÷√

	CCPoint startpoint1, startpoint2;
	CCPoint endpoint1, endpoint2;
};

#endif