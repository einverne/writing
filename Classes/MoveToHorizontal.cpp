#include "MoveToHorizontal.h"
#include "HcharacterDrawnode.h"

MoveToHorizontal::MoveToHorizontal()
{
	length.clear();
	originY.clear();
}

MoveToHorizontal::~MoveToHorizontal()
{
	length.clear();
	originY.clear();
}

MoveToHorizontal* MoveToHorizontal::create(float duration){
	MoveToHorizontal* pRet = new MoveToHorizontal();
	if (pRet && pRet->init(duration))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void MoveToHorizontal::startWithTarget(CCNode *pTarget){
	CCActionInterval::startWithTarget(pTarget);

	/////////////////////////////////////////////////////////////////
	vector<CCPoint>::iterator it;
	it=((MultiPointNode*)m_pTarget)->move_point_.begin();
	float beginY=(*it).y;

	it++;
	for(;it!=((MultiPointNode*)m_pTarget)->move_point_.end();it++)
	{
		float endY=(*it).y;
		length.push_back(endY-beginY);
		originY.push_back(endY);
	}
}

bool MoveToHorizontal::init(float duration){
	CCActionInterval::initWithDuration(duration);
	return true;
}

void MoveToHorizontal::update(float time){
	CCLog("ActionA0001::update time = %f",time);

	////////////////////////////////////////////////////////////////
	vector<CCPoint>::iterator it = ((MultiPointNode*)m_pTarget)->move_point_.begin();
	it++;
	vector<float>::iterator ita = length.begin();
	vector<float>::iterator itb = originY.begin();

	for(;it!=((MultiPointNode*)m_pTarget)->move_point_.end();it++, ita++,itb++)
	{
		(*it).y = (*itb)-(*ita)*time;
	}

}