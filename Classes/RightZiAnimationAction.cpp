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
	CCLog("Time: %f",time);
	CCActionInterval::update(time);
	TcharacterDrawnode* tnode = (TcharacterDrawnode*)m_pTarget;
	CCArray* temp = tnode->getstrokedrawList();
	CCObject* it = NULL;
	int count = tnode->getPointsCount();
	int c = count / (this->m_fDuration*60);
	int delta = count * time;
	int stroke_index = tnode->getVisibleIndex();
	int visible_stroke_index = 0;
	int sumcount = 0;
	StrokeDrawnode* snode = (StrokeDrawnode*)temp->objectAtIndex(visible_stroke_index);
	while (sumcount + snode->getStroke().getPointsCount() < delta && visible_stroke_index < tnode->getstrokedrawList()->count()-1)
	{
		sumcount += snode->getStroke().getPointsCount();
		visible_stroke_index++;
		snode = (StrokeDrawnode*)temp->objectAtIndex(visible_stroke_index);
	}
	tnode->setVisibleIndex(visible_stroke_index);
	snode->setVisibleIndex(delta-sumcount);
}
