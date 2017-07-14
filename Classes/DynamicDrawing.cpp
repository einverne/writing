#include "DynamicDrawing.h"
#include "HcharacterDrawnode.h"

DynamicDrawing::DynamicDrawing()
{
}

DynamicDrawing::~DynamicDrawing()
{
}

DynamicDrawing* DynamicDrawing::create(float duration){
	DynamicDrawing* pRet = new DynamicDrawing();
	if (pRet && pRet->init(duration))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void DynamicDrawing::startWithTarget(CCNode *pTarget){
	CCActionInterval::startWithTarget(pTarget);

	vector<CCPoint>::iterator it=((TwoLineNode*)pTarget)->OrigionPoints.begin();

	CCPoint startPoint1 = *it;
	it++;
	CCPoint endPoint1 = *it;
	it++;
	CCPoint startPoint2 = *it;
	it++;
	CCPoint endPoint2 = *it;

	deltaPoint1 = endPoint1 - startPoint1;
	deltaPoint2 = endPoint2 - startPoint2;
	////////////////////////////////////////
	float len1=sqrtf(deltaPoint1.x*deltaPoint1.x+deltaPoint1.y*deltaPoint1.y);
	float len2=sqrtf(deltaPoint2.x*deltaPoint2.x+deltaPoint2.y*deltaPoint2.y);

	deltaPoint3 = startPoint2 - startPoint1;
	deltaPoint4 = startPoint2 + deltaPoint2/len2*len1 - endPoint1;

	if(fabs(deltaPoint1.x)>fabs(deltaPoint1.y))
	{
		deltaPoint3.y+=10;
		deltaPoint4.y+=10;
	}
	else
	{
		deltaPoint3.x+=10;
		deltaPoint4.x+=10;
	}
}

bool DynamicDrawing::init(float duration){
	CCActionInterval::initWithDuration(duration);
	return true;
}

void DynamicDrawing::update(float time){
	CCLog("DynamicDrawing::update time = %f",time);

	if(time<=0.45)
	{
		CCPoint cPoint = ((TwoLineNode*)getTarget())->startpoint1;
		cPoint = cPoint + deltaPoint1 * (time/0.45);
		((TwoLineNode*)getTarget())->endpoint1 = cPoint;
	}
	else if(time<=0.90)
	{
		CCPoint cPoint = ((TwoLineNode*)getTarget())->startpoint2;
		cPoint = cPoint + deltaPoint2 * ((time-0.45)/0.45);
		((TwoLineNode*)getTarget())->endpoint2 = cPoint;
	}
	/*else
	{	
		vector<CCPoint>::iterator it=((TwoLineNode*)getTarget())->OrigionPoints.begin();
		CCPoint startPoint1 = *it;
		it++;
		CCPoint endPoint1 = *it;

		startPoint1 = startPoint1 + deltaPoint3 * ((time-0.70)/0.3);
		((TwoLineNode*)getTarget())->startpoint1 = startPoint1;

		endPoint1 = endPoint1 + deltaPoint4 * ((time-0.70)/0.3);
		((TwoLineNode*)getTarget())->endpoint1 = endPoint1;
	}*/
}