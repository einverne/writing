#include "RectangleNode.h"


RectangleNode::RectangleNode(void)
{

}


RectangleNode::~RectangleNode(void)
{
	
}

void RectangleNode::setPoint(CCPoint lefttop, CCPoint leftbottom, CCPoint righttop, CCPoint rightbottom)
{
	m_lefttop = lefttop;
	m_leftbottom = leftbottom;
	m_righttop = righttop;
	m_rightbottom = rightbottom;

	origin_lefttop = lefttop;
	origin_leftbottom = leftbottom;
	origin_righttop = righttop;
	origin_rightbottom = rightbottom;
}

void RectangleNode::setSize(CCSize MattSize)
{
	m_MattSize=MattSize;
}

void RectangleNode::draw() {
	ccPointSize(17);
	glLineWidth(13.0f);					//笔画粗细	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//画点
	ccDrawColor4F(1,0,0,1);				//笔画颜色
	ccDrawPoint(m_lefttop);
	ccDrawPoint(m_leftbottom);
	ccDrawPoint(m_righttop);
	ccDrawPoint(m_rightbottom);

	ccDrawColor4F(0,1,0,1);				//笔画颜色
	//画第一条线	
	CCPoint start_point_=m_lefttop;
	CCPoint end_point_=m_leftbottom;
		
	float dashLength=5.0;
	float dx = end_point_.x - start_point_.x;  
	float dy = end_point_.y - start_point_.y;  
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
		p2.x = start_point_.x + x;  
		p2.y = start_point_.y + y;  

		cocos2d::ccDrawLine(start_point_,p2);

		start_point_.x += x * 2;  
		start_point_.y += y * 2;  
	}  

	//画第二条线	
	start_point_=m_leftbottom;
	end_point_=m_rightbottom;

	dashLength=5.0;
	dx = end_point_.x - start_point_.x;  
	dy = end_point_.y - start_point_.y;  
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
		p2.x = start_point_.x + x;  
		p2.y = start_point_.y + y;  

		cocos2d::ccDrawLine(start_point_,p2);

		start_point_.x += x * 2;  
		start_point_.y += y * 2;  
	}  

	//画第三条线
	start_point_=m_rightbottom;
	end_point_=m_righttop;

	dashLength=5.0;
	dx = end_point_.x - start_point_.x;  
	dy = end_point_.y - start_point_.y;  
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
		p2.x = start_point_.x + x;  
		p2.y = start_point_.y + y;  

		cocos2d::ccDrawLine(start_point_,p2);

		start_point_.x += x * 2;  
		start_point_.y += y * 2;  
	}  

	//画第四条线
	start_point_=m_righttop;
	end_point_=m_lefttop;

	dashLength=5.0;
	dx = end_point_.x - start_point_.x;  
	dy = end_point_.y - start_point_.y;  
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
		p2.x = start_point_.x + x;  
		p2.y = start_point_.y + y;  

		cocos2d::ccDrawLine(start_point_,p2);

		start_point_.x += x * 2;  
		start_point_.y += y * 2;  
	}  

}