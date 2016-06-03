#include "ActionA0003.h"
#include "HcharacterDrawnode.h"

ActionA0003::ActionA0003()
{
}

ActionA0003::~ActionA0003()
{
}

ActionA0003* ActionA0003::create(float duration){
	ActionA0003* pRet = new ActionA0003();
	if (pRet && pRet->init(duration))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void ActionA0003::startWithTarget(CCNode *pTarget){
	CCActionInterval::startWithTarget(pTarget);
	startPoint = ((OnePointNode*)pTarget)->centerPoint_;
	endPoint = ((OnePointNode*)pTarget)->rightPoint_;
	deltaPoint = endPoint - startPoint;
}

bool ActionA0003::init(float duration){
	CCActionInterval::initWithDuration(duration);
	this->endPoint = endPoint;
	return true;
}

void ActionA0003::update(float time){
	CCLog("ActionA0003::update time = %f",time);
	CCPoint cPoint = ((OnePointNode*)getTarget())->centerPoint_;
	cPoint = startPoint + deltaPoint * time;

	((OnePointNode*)getTarget())->centerPoint_ = cPoint;
}