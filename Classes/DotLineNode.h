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
	int length_;			// ��ȷ��λ��
	int orientation_;		// 0 Ϊ����1 Ϊ����Ĭ������
};



#endif


