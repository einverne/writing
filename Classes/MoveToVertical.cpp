#include "MoveToVertical.h"
#include "HcharacterDrawnode.h"

MoveToVertical::MoveToVertical()
{
	length.clear();
	originX.clear();
}

MoveToVertical::~MoveToVertical()
{
	length.clear();
	originX.clear();
}

MoveToVertical* MoveToVertical::create(float duration){
	MoveToVertical* pRet = new MoveToVertical();
	if (pRet && pRet->init(duration))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void MoveToVertical::startWithTarget(CCNode *pTarget){
	CCActionInterval::startWithTarget(pTarget);

	/////////////////////////////////////////////////////////////////
	vector<CCPoint>::iterator it;
	it=((MultiPointNode*)m_pTarget)->move_point_.begin();
	float beginX=(*it).x;

	it++;
	for(;it!=((MultiPointNode*)m_pTarget)->move_point_.end();it++)
	{
		float endX=(*it).x;
		length.push_back(endX-beginX);
		originX.push_back(endX);
	}
}

bool MoveToVertical::init(float duration){
	CCActionInterval::initWithDuration(duration);
	return true;
}

void MoveToVertical::update(float time){
	CCLog("MoveToVertical::update time = %f",time);

	////////////////////////////////////////////////////////////////
	vector<CCPoint>::iterator it = ((MultiPointNode*)m_pTarget)->move_point_.begin();
	it++;
	vector<float>::iterator ita = length.begin();
	vector<float>::iterator itb = originX.begin();

	for(;it!=((MultiPointNode*)m_pTarget)->move_point_.end();it++, ita++,itb++)
	{
		(*it).x = (*itb)-(*ita)*time;
	}

}