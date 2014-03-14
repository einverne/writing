#include "StrokeDrawnode.h"

StrokeDrawnode::StrokeDrawnode()
{
}

//************************************
// Method:    StrokeDrawnode
// FullName:  StrokeDrawnode::StrokeDrawnode
// Access:    public 
// Returns:   
// Qualifier:
// Parameter: Stroke stro 传入一笔，构造一笔Node
//************************************
StrokeDrawnode::StrokeDrawnode(Stroke stro){
	this->stroke = stro;
}

StrokeDrawnode::~StrokeDrawnode()
{
}

void StrokeDrawnode::draw(){
	//CCLog("StrokeDrawnode::draw");
	//在这里设定笔画的粗细和颜色。 具体要查看
	glLineWidth(6.0f);					//笔画粗细
	ccDrawColor4F(0,0,0,1);				//笔画颜色

	CCPoint pre = stroke.pointList[0];					//直接拿了pointList第一个值
	for (int i = 1; i< stroke.pointCount; i++)
	{
		ccDrawLine(pre,stroke.pointList[i]);
		pre = stroke.pointList[i];
	}
}