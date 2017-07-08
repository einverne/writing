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
	StrokeDrawnode();
	/**
	* 用Stroke构造一个Node
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
	//节点开始进入触发
	virtual void onEnter();
	//节点退出触发
	virtual void onExit();
	virtual void draw();

	/**
	* add a point to stroke
	* @param point
	* @return
	*/
	void addPoint(CCPoint point);
	/**
	* return Stroke
	* @return
	*/
	Stroke getStroke(){return stroke;}

	/**
	* 设置visibleIndex
	* @param vi
	* @return
	*/
	void setVisibleIndex(int vi);

	/**
	* 返回visibleIndex
	* @return
	*/
	int getVisibleIndex()	{ return visibleIndex; }
private:
	Stroke stroke;					//store points in a stroke
	ccColor4B mcolor;				//color if this stroke:have not used yet
	int visibleIndex;				//if visibleIndex == -1 then draw all points else draw visibleIndex points
};


#endif