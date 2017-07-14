#include "BarycentreNode.h"


BarycentreNode::BarycentreNode(void)
{

}

BarycentreNode::~BarycentreNode(void)
{

}

void BarycentreNode::setBarycentrePoint(CCPoint point)
{
	OrigionPoint=point;           
	MovePoint=point;
}
void BarycentreNode::setLeftPoint(CCPoint startpoint,CCPoint endpoint)
{
	leftstart=startpoint;
	leftend=startpoint;
	leftdelta=endpoint-startpoint;
}
void BarycentreNode::setRightPoint(CCPoint startpoint,CCPoint endpoint)
{
	rightstart=startpoint;
	rightend=startpoint;	
	rightdelta=endpoint-startpoint;
}

void BarycentreNode::draw() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	////画重心
	ccPointSize(19);
	ccDrawColor4F(0,1,0,1);				//笔画颜色
	ccDrawPoint(MovePoint);

	ccPointSize(16);
	ccDrawColor4F(1,0,0,1);				//笔画颜色
	ccDrawPoint(OrigionPoint);	
	
	////画边界
	glLineWidth(13.0f);					//笔画粗细
	ccDrawColor4F(0,1,0,1);				//笔画颜色

	//第一条线
	float dashLength=5.0;
	CCPoint origin = leftstart;
	CCPoint destination = leftend;
	float dx = destination.x - origin.x;  
	float dy = destination.y - origin.y;  
	float dist = sqrtf(dx * dx + dy * dy);  

	float x = 0;
	float y = 0; 

	if(dist>0.0)
	{
		x=dx / dist * dashLength;  
		y=dy / dist * dashLength; 
	}
	CCPoint p2;

	for (float i = 0.0f; i <= dist / dashLength * .5; i++) 
	{  
		p2.x = origin.x + x;  
		p2.y = origin.y + y;  

		cocos2d::ccDrawLine(origin,p2);

		origin.x += x * 2;  
		origin.y += y * 2;  
	}  

	//第二条线
	origin = rightstart;
	destination = rightend;
	dx = destination.x - origin.x;  
	dy = destination.y - origin.y;  
	dist = sqrtf(dx * dx + dy * dy);  

	if(dist>0.0)
	{
		x=dx / dist * dashLength;  
		y=dy / dist * dashLength; 
	}
	else
	{
		x = 0;  
		y = 0;  
	}

	for (float i = 0.0f; i <= dist / dashLength * .5; i++) 
	{  
		p2.x = origin.x + x;  
		p2.y = origin.y + y;  

		cocos2d::ccDrawLine(origin,p2);

		origin.x += x * 2;  
		origin.y += y * 2;  
	} 

	////画连线
	if(fabs(leftend.y-leftstart.y)>=fabs(leftdelta.y*0.9) && fabs(rightend.y-rightstart.y)>=fabs(rightdelta.y*0.9))
	{		
		CCPoint m_left=CCPoint(leftstart.x, MovePoint.y);
		CCPoint m_right=CCPoint(rightstart.x, MovePoint.y);
		ccDrawColor4F(0,0,1,1);				//笔画颜色
		cocos2d::ccDrawLine(MovePoint-CCPoint(10,0),m_left);
		ccDrawColor4F(0,1,1,1);				//笔画颜色
		cocos2d::ccDrawLine(MovePoint+CCPoint(10,0),m_right);
	}
}