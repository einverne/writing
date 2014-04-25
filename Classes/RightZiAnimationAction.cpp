#include "RightZiAnimationAction.h"
#include "TcharacterDrawnode.h"

RightZiAnimationAction::RightZiAnimationAction()
{
}

RightZiAnimationAction::~RightZiAnimationAction()
{
}

RightZiAnimationAction* RightZiAnimationAction::create(float duration,CCArray* strokelist){
	RightZiAnimationAction* pRet = new RightZiAnimationAction();
	if (pRet && pRet->init(duration,strokelist))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool RightZiAnimationAction::init(float duration,CCArray* strokelist){
	CCActionInterval::initWithDuration(duration);
	this->strokes = strokelist;
	return true;
}

void RightZiAnimationAction::update(float time){
	CCActionInterval::update(time);
	CCArray* temp = ((TcharacterDrawnode*)m_pTarget)->getstrokedrawList();
	CCObject* it = NULL;
	CCARRAY_FOREACH(this->strokes,it){

	}
}