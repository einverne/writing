#include "OnePointNode.h"


OnePointNode::OnePointNode(void)
{
	centerPoint_ = ccp(0,0);
	originPoint_ = ccp(0,0);
}


OnePointNode::~OnePointNode(void)
{
}

void OnePointNode::setPoint(CCPoint start_point){
	centerPoint_ = start_point;
	originPoint_ = start_point;
}

void OnePointNode::setRightPoint(CCPoint right_point){
	rightPoint_ = right_point;
}

void OnePointNode::draw() {
	glLineWidth(10.0f);					//±Ê»­´ÖÏ¸
	ccDrawColor4F(1,0,0,1);				//±Ê»­ÑÕÉ«
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ccPointSize(15);
	ccDrawPoint(centerPoint_);

	ccDrawColor4F(1,0,0,1);	
	ccPointSize(10);
	ccDrawPoint(originPoint_);
}