#ifndef __UniformSpaceNode_H__
#define __UniformSpaceNode_H__

#include "cocos2d.h"
USING_NS_CC;

using namespace std;

class UniformSpaceNode :
	public CCNode
{
public:
	UniformSpaceNode(void);
	CREATE_FUNC(UniformSpaceNode);
	~UniformSpaceNode(void);
	virtual void draw();

	void setPoint(vector<CCPoint> points);
	void setOrientation(int orientation);
	void setLength(int length);

	vector<CCPoint> centerPoint_;           // 初始的位置
	int orientation_;		// 0 为竖向，1 为横向，默认竖向
	int length_;
};



#endif


