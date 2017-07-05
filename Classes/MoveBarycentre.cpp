#include "MoveBarycentre.h"
#include "HcharacterDrawnode.h"

MoveBarycentre::MoveBarycentre()
{
}

MoveBarycentre::~MoveBarycentre()
{
}

MoveBarycentre* MoveBarycentre::create(float duration){
	MoveBarycentre* pRet = new MoveBarycentre();
	if (pRet && pRet->init(duration))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool MoveBarycentre::init(float duration){
	CCActionInterval::initWithDuration(duration);
	return true;
}


void MoveBarycentre::startWithTarget(CCNode *pTarget){
	CCActionInterval::startWithTarget(pTarget);	
}



void MoveBarycentre::update(float time){
	CCLog("MoveBarycentre::update time = %f",time);

	if(time<=0.4)
	{
		CCPoint cPoint = ((BarycentreNode*)getTarget())->leftstart;
		CCPoint deltaPoint = ((BarycentreNode*)getTarget())->leftdelta;
		cPoint = cPoint + deltaPoint * (time/0.4);
		((BarycentreNode*)getTarget())->leftend = cPoint;

		cPoint = ((BarycentreNode*)getTarget())->rightstart;
		deltaPoint = ((BarycentreNode*)getTarget())->rightdelta;
		cPoint = cPoint + deltaPoint * (time/0.4);
		((BarycentreNode*)getTarget())->rightend = cPoint;
	}
	else if(time<=1.0)
	{
		CCPoint origion = ((BarycentreNode*)getTarget())->OrigionPoint;
		CCPoint cPoint1 = ((BarycentreNode*)getTarget())->leftstart;
		CCPoint cPoint2 = ((BarycentreNode*)getTarget())->rightstart;
		CCPoint deltaPoint = CCPoint((cPoint1.x+cPoint2.x)/2.0,origion.y)-origion;

		CCPoint cPoint = origion + deltaPoint * ((time-0.4)/0.6);
		((BarycentreNode*)getTarget())->MovePoint = cPoint;
	}
	
}