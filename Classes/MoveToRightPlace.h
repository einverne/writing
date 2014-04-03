#ifndef __MoveToRightPlacee_H__
#define __MoveToRightPlacee_H__

#include "cocos2d.h"
#include "Stroke.h"
USING_NS_CC;

class MoveToRightPlace : public CCActionInstant
{
public:
	MoveToRightPlace();
	~MoveToRightPlace();
	static MoveToRightPlace* create(int index,Stroke s);
	CCObject* copyWithZone(CCZone* pZone);
	void update(float time);
	bool initWithPoints(int index,Stroke s);

private:
	CC_SYNTHESIZE_RETAIN(CCArray*,m_points,Points);
	int index;
	Stroke m_stroke;
};



#endif