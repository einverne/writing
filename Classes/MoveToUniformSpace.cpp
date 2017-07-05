#include "MoveToUniformSpace.h"
#include "HcharacterDrawnode.h"

MoveToUniformSpace::MoveToUniformSpace()
{
	length.clear();
	origin.clear();
}

MoveToUniformSpace::~MoveToUniformSpace()
{
	length.clear();
	origin.clear();
}

MoveToUniformSpace* MoveToUniformSpace::create(float duration){
	MoveToUniformSpace* pRet = new MoveToUniformSpace();
	if (pRet && pRet->init(duration))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void MoveToUniformSpace::startWithTarget(CCNode *pTarget){
	CCActionInterval::startWithTarget(pTarget);

	/////////////////////////////////////////////////////////////////
	vector<CCPoint>::iterator it;
	flag=((UniformSpaceNode*)m_pTarget)->orientation_;
	////////////////////////////////////////////////////////////////
	if(flag==1)
	{
		it=((UniformSpaceNode*)m_pTarget)->centerPoint_.begin();
		float beginY=(*it).y;

		it=((UniformSpaceNode*)m_pTarget)->centerPoint_.end();
		it--;
		float endY=(*it).y;

		float space=(endY-beginY)/(((UniformSpaceNode*)m_pTarget)->centerPoint_.size()-1);

		it=((UniformSpaceNode*)m_pTarget)->centerPoint_.begin();
		int ind=0;
		for(;it!=((UniformSpaceNode*)m_pTarget)->centerPoint_.end();it++)
		{
			float CurrentY=(*it).y;
			origin.push_back(CurrentY);
			length.push_back(CurrentY-(ind*space+beginY));
			ind++;
		}
	}
	if(flag==0)
	{
		it=((UniformSpaceNode*)m_pTarget)->centerPoint_.begin();
		float beginX=(*it).x;

		it=((UniformSpaceNode*)m_pTarget)->centerPoint_.end();
		it--;
		float endX=(*it).x;

		float space=(endX-beginX)/(((UniformSpaceNode*)m_pTarget)->centerPoint_.size()-1);

		it=((UniformSpaceNode*)m_pTarget)->centerPoint_.begin();
		int ind=0;
		for(;it!=((UniformSpaceNode*)m_pTarget)->centerPoint_.end();it++)
		{
			float CurrentX=(*it).x;
			origin.push_back(CurrentX);
			length.push_back(CurrentX-(ind*space+beginX));
			ind++;
		}
	}
	//////////////////////////////////////////////////////////////
}

bool MoveToUniformSpace::init(float duration){
	CCActionInterval::initWithDuration(duration);
	return true;
}

void MoveToUniformSpace::update(float time){
	CCLog("ActionA0005::update time = %f",time);

	////////////////////////////////////////////////////////////////
	vector<CCPoint>::iterator it = ((UniformSpaceNode*)m_pTarget)->centerPoint_.begin();
	vector<float>::iterator ita = length.begin();
	vector<float>::iterator itb = origin.begin();

	if(flag==1)
	{
		for(;it!=((UniformSpaceNode*)m_pTarget)->centerPoint_.end();it++, ita++,itb++)
		{
			(*it).y = (*itb)-(*ita)*time;
		}
	}
	if(flag==0)
	{
		for(;it!=((UniformSpaceNode*)m_pTarget)->centerPoint_.end();it++, ita++,itb++)
		{
			(*it).x = (*itb)-(*ita)*time;
		}
	}

}