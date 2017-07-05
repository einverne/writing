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

	vector<CCPoint> centerPoint_;           // ��ʼ��λ��
	int orientation_;		// 0 Ϊ����1 Ϊ����Ĭ������
	int length_;
};



#endif


