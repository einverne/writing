
#include "MoveToRightPlace.h"
#include "HcharacterDrawnode.h"
#include "StrokeDrawnode.h"

MoveToRightPlace::MoveToRightPlace():m_points(NULL)
{
}

MoveToRightPlace::~MoveToRightPlace()
{
	CC_SAFE_RELEASE(m_points);
}


MoveToRightPlace* MoveToRightPlace::create(int index,Stroke s){
	MoveToRightPlace* pRet = new MoveToRightPlace();
	if (pRet && pRet->initWithPoints(index,s))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

CCObject* MoveToRightPlace::copyWithZone(CCZone* pZone){
	CCZone* pNewZone = NULL;
	MoveToRightPlace* pRet = NULL;
	if (pRet && pZone->m_pCopyObject)
	{
		pRet = (MoveToRightPlace*)(pZone->m_pCopyObject);
	}else{
		pRet = new MoveToRightPlace();
		pZone = pNewZone = new CCZone(pRet);
	}
	CCActionInstant::copyWithZone(pZone);
	pRet->initWithPoints(index,m_stroke);
	CC_SAFE_DELETE(pNewZone);
	return pRet;
}

void MoveToRightPlace::update(float time){
	CC_UNUSED_PARAM(time);
	((HcharacterDrawnode*)m_pTarget)->ChangeStroke(index,m_stroke);
}

bool MoveToRightPlace::initWithPoints(int index,Stroke s){
	this->index = index;
	this->m_stroke = s;
	return true;
}