#ifndef _HcharacterDrawnode_H__
#define _HcharacterDrawnode_H__

#include "cocos2d.h"
#include "StrokeDrawnode.h"
#include "Stroke.h"
#include <map>
USING_NS_CC;

// 手写汉字 Node
class HcharacterDrawnode : public CCNode
{
public:
	HcharacterDrawnode();
	~HcharacterDrawnode();
	CREATE_FUNC(HcharacterDrawnode);
	virtual bool init();
	//节点开始进入触发  
	virtual void onEnter();
	//节点退出触发  
	virtual void onExit();
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
	* 设置评判结果笔画及笔画错误位置信息，用来画图
	* 根据 lua 返回结果 1:0.5 类似的结构，找到对应的点，并返回点集列表
	* @param 
	* @param estroke
	* @return 返回错误点vector
	*/
	vector<CCPoint> GetErrorPoints(multimap<int, float>& estroke);

	/**
	* 改变笔画的颜色
	* @return
	*/
	void markErrorStroke(int markStroke);

	CC_SYNTHESIZE_RETAIN(CCArray*,strokeDrawlist,StrokeDrawnodeList);
private:
	CCSprite* tianziged;
	multimap<int, float> error_stroke;
};



#endif