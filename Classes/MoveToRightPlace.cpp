#include "MoveToRightPlace.h"
#include "HcharacterDrawnode.h"

MoveToRightPlace::MoveToRightPlace()
{
}

MoveToRightPlace::~MoveToRightPlace()
{
}

MoveToRightPlace* MoveToRightPlace::create(float duration){
	MoveToRightPlace* pRet = new MoveToRightPlace();
	if (pRet && pRet->init(duration))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void MoveToRightPlace::startWithTarget(CCNode *pTarget){
	CCActionInterval::startWithTarget(pTarget);
	startPoint = ((OnePointNode*)pTarget)->centerPoint_;
	endPoint = ((OnePointNode*)pTarget)->rightPoint_;
	deltaPoint = endPoint - startPoint;
}

bool MoveToRightPlace::init(float duration){
	CCActionInterval::initWithDuration(duration);
	//this->endPoint = endPoint;
	return true;
}

void MoveToRightPlace::update(float time){
	CCLog("MoveToRightPlace::update time = %f",time);
	CCPoint cPoint = ((OnePointNode*)getTarget())->centerPoint_;
	cPoint = startPoint + deltaPoint * time;

	((OnePointNode*)getTarget())->centerPoint_ = cPoint;
}