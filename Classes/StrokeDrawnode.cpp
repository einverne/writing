#include "StrokeDrawnode.h"

StrokeDrawnode::StrokeDrawnode(Stroke stro):visible_index_(-1){
    this->line_width_ = 8.0f;
    this->color_ = ccc4f(0, 0, 0, 1);

    this->stroke_ = stro;
}

StrokeDrawnode* StrokeDrawnode::create(Stroke stro){
	StrokeDrawnode* pRet = new StrokeDrawnode(stro);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}else{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

StrokeDrawnode::~StrokeDrawnode()
{

}

void StrokeDrawnode::draw(){
	//在这里设定笔画的粗细和颜色。 具体要查看
	glLineWidth(line_width_);					//笔画粗细
	ccDrawColor4F(color_.r, color_.g, color_.b, color_.a);                     //笔画颜色
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
// 	glEnable(GL_LINE_SMOOTH);
	if (visible_index_ == -1)         // draw all points
	{
		CCPoint pre = stroke_.getpointListIndexAt(0);					//直接拿了pointList第一个值
		for (int i = 1; i< stroke_.getPointsCount(); i++)
		{
			ccDrawLine(pre,stroke_.getpointListIndexAt(i));
			pre = stroke_.getpointListIndexAt(i);
		}
	}else                           // draw visibleIndex points to create animaion
	{
		CCPoint pre = stroke_.getpointListIndexAt(0);
		for (int i = 1; i< stroke_.getPointsCount() && i < visible_index_; i++)
		{
			ccDrawLine(pre,stroke_.getpointListIndexAt(i));
			pre = stroke_.getpointListIndexAt(i);
		}
	}
// 	glDisable(GL_LINE_SMOOTH);
}

void StrokeDrawnode::SetVisibleIndex(int vi){
	if (vi >= stroke_.getPointsCount())
	{
		this->visible_index_ = stroke_.getPointsCount();
	}else{
		this->visible_index_ = vi;
	}
}
