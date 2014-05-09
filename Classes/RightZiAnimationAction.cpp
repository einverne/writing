#include "RightZiAnimationAction.h"
#include "TcharacterDrawnode.h"

RightZiAnimationAction::RightZiAnimationAction()
{
}

RightZiAnimationAction::~RightZiAnimationAction()
{
}

RightZiAnimationAction* RightZiAnimationAction::create(float duration){
	RightZiAnimationAction* pRet = new RightZiAnimationAction();
	if (pRet && pRet->init(duration))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool RightZiAnimationAction::init(float duration){
	CCActionInterval::initWithDuration(duration);

	return true;
}

void RightZiAnimationAction::startWithTarget(CCNode *pTarget){
	CCActionInterval::startWithTarget(pTarget);
	//只有初始化了pTarget才能使用
	TcharacterDrawnode* tnode = (TcharacterDrawnode*)m_pTarget;
	tnode->setVisibleIndex(0);
	CCObject* it = NULL;
	CCARRAY_FOREACH(tnode->getstrokedrawList(),it){
		StrokeDrawnode* snode = (StrokeDrawnode*)it;
		snode->setVisibleIndex(0);
	}
}

void RightZiAnimationAction::update(float time){
	CCLog("Time:%f",time);
	CCActionInterval::update(time);
	TcharacterDrawnode* tnode = (TcharacterDrawnode*)m_pTarget;
	CCArray* temp = tnode->getstrokedrawList();
	CCObject* it = NULL;
	int count = tnode->getPointsCount();
	CCARRAY_FOREACH(temp,it){
		StrokeDrawnode* snode = (StrokeDrawnode*)it;
		if (snode->getVisibleIndex() >= snode->getStroke().getPointsCount())
		{
			continue;
		}
		snode->setVisibleIndex(snode->getVisibleIndex() + 1);
		if (snode->getVisibleIndex()  >= snode->getStroke().getPointsCount())
		{
			tnode->setVisibleIndex(tnode->getVisibleIndex()+1);
			
		}
		break;
	}
}
