#include "ActionA0001.h"
#include "HcharacterDrawnode.h"

ActionA0001::ActionA0001()
{
}

ActionA0001::~ActionA0001()
{
}

ActionA0001* ActionA0001::create(float duration){
	ActionA0001* pRet = new ActionA0001();
	if (pRet && pRet->init(duration))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void ActionA0001::startWithTarget(CCNode *pTarget){
	CCActionInterval::startWithTarget(pTarget);
	CCPoint start_point = ((ShuipingErrorNode*)m_pTarget)->start_point_;
	CCPoint end_point = ((ShuipingErrorNode*)m_pTarget)->end_point_;
	length = start_point.y - end_point.y < 0 ? (end_point.y-start_point.y) : (start_point.y-end_point.y);
	bigY = start_point.y - end_point.y > 0 ? start_point.y : end_point.y;
}

bool ActionA0001::init(float duration){
	CCActionInterval::initWithDuration(duration);
	return true;
}

void ActionA0001::update(float time){
	CCLog("ActionA0001::update time = %f",time);
	CCPoint start_point = ((ShuipingErrorNode*)m_pTarget)->start_point_;
	CCPoint end_point = ((ShuipingErrorNode*)m_pTarget)->end_point_;
	// 将 坐标移动到 小的一边
	if (start_point.y - end_point.y > 0)
	{
		start_point.y = bigY - length * time;
	} else {
		end_point.y = bigY - length * time;
	}
	((ShuipingErrorNode*)m_pTarget)->start_point_ = start_point;
	((ShuipingErrorNode*)m_pTarget)->end_point_ = end_point;
}