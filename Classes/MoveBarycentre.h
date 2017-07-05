#ifndef __MoveBarycentre_H__
#define __MoveBarycentre_H__

#include "cocos2d.h"
#include "BarycentreNode.h"
USING_NS_CC;

class MoveBarycentre : public CCActionInterval
{
public:
	MoveBarycentre();
	~MoveBarycentre();

	//
	// ���췽��
	// @param duration ����ʱ��
	// @return
	//
	static MoveBarycentre* create(float duration);
	virtual void update(float time);
	bool init(float duration);
	void startWithTarget(CCNode *pTarget);

};


#endif