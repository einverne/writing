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
	//************************************
	// Method:    create
	// FullName:  MoveToRightPlace::create
	// Access:    public static 
	// Returns:   MoveToRightPlace*
	// Qualifier:
	// Parameter: int index 第几笔
	// Parameter: Stroke s 正确的笔画
	//************************************
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