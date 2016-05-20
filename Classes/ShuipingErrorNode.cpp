#include "ShuipingErrorNode.h"


ShuipingErrorNode::ShuipingErrorNode(void)
{
	start_point_ = ccp(0,0);
	end_point_ = ccp(0,0);
}


ShuipingErrorNode::~ShuipingErrorNode(void)
{
}

void ShuipingErrorNode::setPoint(CCPoint start_point, CCPoint end_point){
	start_point_ = start_point;
	end_point_ = end_point;
}

void ShuipingErrorNode::draw() {
	glLineWidth(4.0f);					//±Ê»­´ÖÏ¸
	ccDrawColor4F(1,0,0,1);				//±Ê»­ÑÕÉ«
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	ccDrawLine(start_point_, end_point_);
}