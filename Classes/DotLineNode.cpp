#include "DotLineNode.h"


DotLineNode::DotLineNode(void)
{
	centerPoint_ = ccp(0,0);
	orientation_ = 0;
}


DotLineNode::~DotLineNode(void)
{
}

void DotLineNode::setCenterPoint(CCPoint start_point){
	centerPoint_ = start_point;
}

void DotLineNode::setLength(int length){
	length_ = length;
}

void DotLineNode::setOrientation(int orientation) {
	orientation_ = orientation;
}

void DotLineNode::draw() {
	glLineWidth(5.0f);					//±Ê»­´ÖÏ¸
	ccDrawColor4F(1,0,1,1);				//±Ê»­ÑÕÉ«
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (orientation_ == 0)
	{
		float dashLength=5.0;
		CCPoint origin = CCPoint(centerPoint_.x, centerPoint_.y - length_/2);
		CCPoint destination = CCPoint(centerPoint_.x, centerPoint_.y + length_/2);
		float dx = destination.x - origin.x;  
		float dy = destination.y - origin.y;  
		float dist = sqrtf(dx * dx + dy * dy);  

		float x = dx / dist * dashLength;  
		float y = dy / dist * dashLength;  

		CCPoint p2;

		for (float i = 0.0f; i <= dist / dashLength * .5; i++) 
		{  
			p2.x = origin.x + x;  
			p2.y = origin.y + y;  

			cocos2d::ccDrawLine(origin,p2);

			origin.x += x * 2;  
			origin.y += y * 2;  
		}  

	} 
	else if(orientation_ == 1)
	{
		float dashLength=5.0;
		CCPoint origin = CCPoint(centerPoint_.x - length_ /2, centerPoint_.y);
		CCPoint destination = CCPoint(centerPoint_.x + length_/2, centerPoint_.y);
		float dx = destination.x - origin.x;  
		float dy = destination.y - origin.y;  
		float dist = sqrtf(dx * dx + dy * dy);  

		float x = dx / dist * dashLength;  
		float y = dy / dist * dashLength;  

		CCPoint p2;

		for (float i = 0.0f; i <= dist / dashLength * .5; i++) 
		{  
			p2.x = origin.x + x;  
			p2.y = origin.y + y;  

			cocos2d::ccDrawLine(origin,p2);

			origin.x += x * 2;  
			origin.y += y * 2;  
		} 
	}
}