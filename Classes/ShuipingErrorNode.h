#ifndef __ShuipingErrorNode_H__
#define __ShuipingErrorNode_H__

#include "cocos2d.h"
USING_NS_CC;

using namespace std;

class ShuipingErrorNode :
	public CCNode
{
public:
	ShuipingErrorNode(void);
	CREATE_FUNC(ShuipingErrorNode);
	~ShuipingErrorNode(void);
	virtual void draw();

	//void setPoint(CCPoint start_point, CCPoint end_point);

	//CCPoint start_point_;
	//CCPoint end_point_;

	void setPoint(vector<CCPoint> m_point);
	vector<CCPoint> key_point_;
	vector<CCPoint> move_point_;
};



#endif


