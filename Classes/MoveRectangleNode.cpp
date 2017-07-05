#include "MoveRectangleNode.h"
#include "HcharacterDrawnode.h"

MoveRectangleNode::MoveRectangleNode()
{
}

MoveRectangleNode::~MoveRectangleNode()
{
}

MoveRectangleNode* MoveRectangleNode::create(float duration){
	MoveRectangleNode* pRet = new MoveRectangleNode();
	if (pRet && pRet->init(duration))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool MoveRectangleNode::init(float duration){
	CCActionInterval::initWithDuration(duration);
	return true;
}


void MoveRectangleNode::startWithTarget(CCNode *pTarget){
	CCActionInterval::startWithTarget(pTarget);	

	CCSize m_size=((RectangleNode*)getTarget())->m_MattSize;

	delta_leftbottom = CCPoint(m_size.width*0.15,m_size.height*0.15) - ((RectangleNode*)getTarget())->origin_leftbottom;
	delta_lefttop    = CCPoint(m_size.width*0.15,m_size.height*0.85) - ((RectangleNode*)getTarget())->origin_lefttop;
	delta_rightbottom = CCPoint(m_size.width*0.85,m_size.height*0.15) - ((RectangleNode*)getTarget())->origin_rightbottom;
	delta_righttop = CCPoint(m_size.width*0.85,m_size.height*0.85) - ((RectangleNode*)getTarget())->origin_righttop;

}



void MoveRectangleNode::update(float time){
	CCLog("MoveRectangleNode::update time = %f",time);

	CCPoint cPoint = ((RectangleNode*)getTarget())->origin_leftbottom;
	cPoint = cPoint + delta_leftbottom * time;
	((RectangleNode*)getTarget())->m_leftbottom = cPoint;

	cPoint = ((RectangleNode*)getTarget())->origin_lefttop;
	cPoint = cPoint + delta_lefttop * time;
	((RectangleNode*)getTarget())->m_lefttop = cPoint;

	cPoint = ((RectangleNode*)getTarget())->origin_rightbottom;
	cPoint = cPoint + delta_rightbottom * time;
	((RectangleNode*)getTarget())->m_rightbottom = cPoint;

	cPoint = ((RectangleNode*)getTarget())->origin_righttop;
	cPoint = cPoint + delta_righttop * time;
	((RectangleNode*)getTarget())->m_righttop = cPoint;
}