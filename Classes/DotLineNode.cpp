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
	ccDrawColor4F(1,0,0,1);				//±Ê»­ÑÕÉ«
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ccPointSize(20);
	if (orientation_ == 0)
	{
		cocos2d::ccDrawLine(ccp(centerPoint_.x, centerPoint_.y - length_/2),
			ccp(centerPoint_.x, centerPoint_.y + length_/2));
	} else if(orientation_ == 1) {
		cocos2d::ccDrawLine(ccp(centerPoint_.x - length_ /2, centerPoint_.y),
			ccp(centerPoint_.x + length_/2, centerPoint_.y));
	}
}