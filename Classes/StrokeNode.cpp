#include "StrokeNode.h"
#include "Stroke.h"

USING_NS_CC;

StrokeNode::StrokeNode(Stroke stroke)
{
	n_stroke = stroke;
	//设置CCNode大小
	CCPoint p = n_stroke.getBigPoint();
	this->setContentSize(CCSizeMake(p.x,p.y));

	//添加节点,画线 在draw函数中 
	

}


StrokeNode::~StrokeNode(void)
{
}

bool StrokeNode::init(){



	return true;
}

void StrokeNode::draw(void){
// 	ccDrawLine(CCPointZero,CCPointMake(100,100));
	
	glLineWidth( 5.0f );//设置线宽
	ccDrawColor4B(255,0,0,255);//设置线的颜色
	//遍历点，画出线段
	CCPoint tmp = n_stroke.pointList[0];
	for (int i = 0 ; i < n_stroke.pointList.size() ; ++i)
	{
		ccDrawLine(tmp,n_stroke.pointList[i]);
		tmp = n_stroke.pointList[i];
	}
	CCSize tmpSize = getContentSize();

	//画中心点
	ccPointSize(4);
	ccDrawColor4B(0,255,255,255);

	ccDrawPoint(n_stroke.getMidPoint());
	ccDrawPoint(CCPointZero);
}


StrokeNode* StrokeNode::getStrokeNode(Stroke stroke){


	StrokeNode* node = new StrokeNode(stroke);
	if (node && node->init())
	{
		node->autorelease();
		return node;
	}
	CC_SAFE_DELETE(node);
	return NULL;
}

CCPoint StrokeNode::getMidStrokePoint(){
	return n_stroke.getMidPoint();
}

//将中心设置到笔画的中心，用完必须设置回去
void StrokeNode::setAnchorMidStroke(){
	CCPoint midp = n_stroke.getMidPoint();
	CCPoint p = n_stroke.getBigPoint();
	float ax = p.x/midp.x;
	float ay = p.y/midp.y;
	this->setAnchorPoint(ccp(ax,ay));
}