#ifndef __RightZiAnimationAction_H__
#define __RightZiAnimationAction_H__

#include "cocos2d.h"
USING_NS_CC;
class RightZiAnimationAction : public CCActionInterval
{
public:
	RightZiAnimationAction();
	~RightZiAnimationAction();
	static RightZiAnimationAction* create(float duration);
	virtual void update(float time);
	bool init(float duration);
	void startWithTarget(CCNode *pTarget);

private:

};


#endif
