#include "ActionA0001.h"
#include "HcharacterDrawnode.h"

ActionA0001::ActionA0001()
{
	length.clear();
	originY.clear();
}

ActionA0001::~ActionA0001()
{
	length.clear();
	originY.clear();
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

	/////////////////////////////////////////////////////////////////
	vector<CCPoint>::iterator it;
	it=((ShuipingErrorNode*)m_pTarget)->move_point_.begin();
	float beginY=(*it).y;

	it++;
	for(;it!=((ShuipingErrorNode*)m_pTarget)->move_point_.end();it++)
	{
		float endY=(*it).y;
		length.push_back(endY-beginY);
		originY.push_back(endY);
	}
}

bool ActionA0001::init(float duration){
	CCActionInterval::initWithDuration(duration);
	return true;
}

void ActionA0001::update(float time){
	CCLog("ActionA0001::update time = %f",time);

	////////////////////////////////////////////////////////////////
	vector<CCPoint>::iterator it = ((ShuipingErrorNode*)m_pTarget)->move_point_.begin();
	it++;
	vector<float>::iterator ita = length.begin();
	vector<float>::iterator itb = originY.begin();

	for(;it!=((ShuipingErrorNode*)m_pTarget)->move_point_.end();it++, ita++,itb++)
	{
		(*it).y = (*itb)-(*ita)*time;
	}

}