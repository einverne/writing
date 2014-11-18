#include "MoveToRightPlaceInterval.h"
#include "HcharacterDrawnode.h"

MoveToRightPlaceInterval::MoveToRightPlaceInterval()
{
}

MoveToRightPlaceInterval::~MoveToRightPlaceInterval()
{
}

MoveToRightPlaceInterval* MoveToRightPlaceInterval::create(float duration,int index,Stroke s){
	MoveToRightPlaceInterval* pRet = new MoveToRightPlaceInterval();
	if (pRet && pRet->init(duration,index,s))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void MoveToRightPlaceInterval::startWithTarget(CCNode *pTarget){
	CCActionInterval::startWithTarget(pTarget);
}

// CCObject* MoveToRightPlaceInterval::copyWithZone(CCZone* pZone){
// 	CCZone* pNewZone = NULL;
// 	MoveToRightPlaceInterval* pRet = NULL;
// 	if (pRet && pZone->m_pCopyObject)
// 	{
// 		pRet = (MoveToRightPlaceInterval*)(pZone->m_pCopyObject);
// 	}else{
// 		pRet = new MoveToRightPlaceInterval();
// 		pZone = pNewZone = new CCZone(pRet);
// 	}
// 	CCActionInterval::copyWithZone(pZone);
// 	pRet->init(index,m_stroke);
// 	CC_SAFE_DELETE(pNewZone);
// 	return pRet;
// }

bool MoveToRightPlaceInterval::init(float duration,int index,Stroke s){
	CCActionInterval::initWithDuration(duration);
	this->index = index;
	this->m_stroke = s;
	return true;
}

void MoveToRightPlaceInterval::update(float time){
// 	CCLog("MoveToRightPlaceInterval::update time = %f",time);
// 	CCActionInterval::update(time);
	Stroke stro = ((HcharacterDrawnode*)m_pTarget)->getStroke(index);
	stro.resample();
	for (int i = 0; i < this->m_stroke.getpointList().size() ; i++)
	{
		CCPoint p = stro.getpointListIndexAt(i) - m_stroke.getpointListIndexAt(i);
		CCPoint temp = stro.getpointListIndexAt(i) - p * time;
		stro.setpointList(i,temp);		
	}
	((HcharacterDrawnode*)m_pTarget)->changeStroke(index,stro);
}