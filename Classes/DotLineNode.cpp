#include "DotLineNode.h"


DotLineNode::DotLineNode(void)
{
	centerPoint_ = ccp(0,0);
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

void DotLineNode::draw() {
	glLineWidth(5.0f);					//�ʻ���ϸ
	ccDrawColor4F(1,0,0,1);				//�ʻ���ɫ
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ccPointSize(20);
	cocos2d::ccDrawLine(ccp(centerPoint_.x, centerPoint_.y - length_/2),
		ccp(centerPoint_.x, centerPoint_.y + length_/2));
}