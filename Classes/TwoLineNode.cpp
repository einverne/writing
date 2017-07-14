#include "TwoLineNode.h"


TwoLineNode::TwoLineNode(void)
{
	OrigionPoints.clear();
	startpoint1=CCPoint(0,0);
	startpoint2=CCPoint(0,0);
	endpoint1=CCPoint(0,0);
	endpoint2=CCPoint(0,0);
}


TwoLineNode::~TwoLineNode(void)
{
	OrigionPoints.clear();
}

void TwoLineNode::setPoint(vector<CCPoint> points){
	OrigionPoints.clear();
	OrigionPoints.assign(points.begin(), points.end());

	vector<CCPoint>::iterator it=OrigionPoints.begin();
	startpoint1=*it;
	endpoint1=*it;
	it++;	
	it++;
	startpoint2=*it;	
	//endpoint2=*it;
	endpoint2=startpoint2;
}


void TwoLineNode::draw() {
	glLineWidth(13.0f);					//�ʻ���ϸ
	ccDrawColor4F(0,1,0,1);				//�ʻ���ɫ
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//////////////////////////////////////////////////////////////
	//��һ����
	float dashLength=5.0;
	CCPoint origin = startpoint1;
	CCPoint destination = endpoint1;
	float dx = destination.x - origin.x;  
	float dy = destination.y - origin.y;  
	float dist = sqrtf(dx * dx + dy * dy);  

	float x = 0;
	float y = 0;
	if(dist>0.0)
	{
		x = dx / dist * dashLength;  
		y = dy / dist * dashLength;
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

	//////////////////////////////////////////////////////////////
	glLineWidth(13.0f);					//�ʻ���ϸ
	ccDrawColor4F(1,0,1,1);				//�ʻ���ɫ
	//�ڶ�����
	origin = startpoint2;
	destination = endpoint2;
	dx = destination.x - origin.x;  
	dy = destination.y - origin.y;  
	dist = sqrtf(dx * dx + dy * dy);  

	if(dist>0.0)
	{
		x = dx / dist * dashLength;  
		y = dy / dist * dashLength;  
	}
	else
	{
		x=0.0;
		y=0.0;
	}
	for (float i = 0.0f; i <= dist / dashLength * .5; i++) 
	{  
		p2.x = origin.x + x;  
		p2.y = origin.y + y;  

		cocos2d::ccDrawLine(origin,p2);

		origin.x += x * 2;  
		origin.y += y * 2;  
	}  

	/////////////////////////////////////
	//�ĸ���
	ccPointSize(17);
	ccDrawColor4F(1,0,0,1);				//�ʻ���ɫ
	vector<CCPoint>::iterator it;
	for(it=OrigionPoints.begin(); it!=OrigionPoints.end(); it++)
	{
		ccDrawPoint(*it);
	}
		
}