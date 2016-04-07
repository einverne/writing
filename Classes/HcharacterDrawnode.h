#ifndef _HcharacterDrawnode_H__
#define _HcharacterDrawnode_H__

#include "cocos2d.h"
#include "StrokeDrawnode.h"
#include "Stroke.h"
#include "RowStroke.hpp"
#include <string>
#include <list>
USING_NS_CC;
using namespace std;

// 手写汉字 Node
class HcharacterDrawnode : public CCNode
{
public:
	HcharacterDrawnode();
	~HcharacterDrawnode();
	CREATE_FUNC(HcharacterDrawnode);
	virtual bool init();
	//节点开始进入触发  
//	virtual void onEnter();
	//节点退出触发  
//	virtual void onExit();
	virtual void draw();

	/**
	* addPoint 在最后一笔最后添加点
	* @param point
	* @return
	*/
	void addPoint(CCPoint point);

	/**
	* addStroke 添加一笔
	* @param s Stroke
	* @return
	*/
	void addStroke(Stroke s);

	//************************************
	// Method:    removeLastStroke 删除最后一笔
	// FullName:  HcharacterDrawnode::removeLastStroke
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void removeLastStroke();


	//************************************
	// Method:    changeStroke 
	// FullName:  HcharacterDrawnode::changeStroke
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int index 第几笔，从0开始
	// Parameter: Stroke s 将num笔替换成传入的stroke
	//************************************
	void changeStroke(int index,Stroke s);

	//************************************
	// Method:    getStroke 获取第几笔 从1开始
	// FullName:  HcharacterDrawnode::getStroke
	// Access:    public 
	// Returns:   Stroke
	// Qualifier:
	// Parameter: int index
	//************************************
	Stroke getStroke(int index);

	/**
	* 重写，移除函数内容
	* @return
	*/
	void rewrite();

	int getStrokeCount();

	/**
	* 产生屏幕上采集到的点信息
	* @return
	*/
	string getOriginOutput();

	/**
	* 由采集到的点信息，整理得到Lua评判的点信息
	* @param size
	* @return
	*/
	string getLuaOutput(CCSize size);

	/**
	* 产生手写点点集列表
	* @return
	*/
	list<RowStroke> GetHandWritingPoints();

	CC_SYNTHESIZE_RETAIN(CCArray*,strokeDrawlist,StrokeDrawnodeList);	// 用来保存手写点
private:
	CCSprite* tianziged;
};



#endif