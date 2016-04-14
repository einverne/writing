#ifndef __StrokeDrawnode_H__
#define __StrokeDrawnode_H__

#include "cocos2d.h"
#include "Stroke.h"
USING_NS_CC;
using namespace std;

//每一个笔画的绘制类，继承自CCNode
class StrokeDrawnode : public CCNode
{
public:
	/**
	* 利用Stroke构造一个Node
	* @param stro
	* @return
	*/
	StrokeDrawnode(Stroke stro);

	~StrokeDrawnode();

	/**
	* create a StrokeDrawnode
	* @param stro
	* @return
	*/
	static StrokeDrawnode* create(Stroke stro);
	void draw();
    
	/**
	* return Stroke
	* @return
	*/
	Stroke GetStroke() const {return stroke_;}

	/**
	* 设置visible_index_
	* @param vi
	* @return
	*/
	void SetVisibleIndex(int vi);

	/**
	* 返回visibleIndex
	* @return
	*/
	int GetVisibleIndex()	{ return visible_index_; }
    
    float line_width_;               // line width
    ccColor4F color_;                // line color

	Stroke stroke_;					//store points in a stroke
	int visible_index_;				//if visibleIndex == -1 then draw all points else draw visibleIndex points
};


#endif