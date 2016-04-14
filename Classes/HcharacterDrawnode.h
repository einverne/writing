#ifndef _HcharacterDrawnode_H__
#define _HcharacterDrawnode_H__

#include "cocos2d.h"
#include "StrokeDrawnode.h"
#include "Stroke.h"
#include "RowStroke.hpp"
#include "ScriptCharacter.hpp"
#include "SegmentDrawnode.h"
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
	virtual void draw();

	/**
	* addPoint 在 stroke 最后一笔最后添加点
	* @param point
	* @return
	*/
	void AddPoint(CCPoint point);

	/**
	* addStroke 添加新的笔画
	* @param s Stroke
	* @return
	*/
	void AddStroke(Stroke s);

	/**
	* removeLastStroke 删除最后一笔
	* @return
	*/
	void RemoveLastStroke();

	/**
	* 改变序列为 index 的笔画为 s
	* @param index 第几笔，从0开始
	* @param s 笔画，传入的笔画
	* @return
	*/
	void ChangeStroke(int index,Stroke s);
	
	/**
	* getStroke 获取第几笔 从1开始
	* @param index
	* @return
	*/
	Stroke GetStroke(int index);

	/**
	* 重写，移除函数内容
	* @return
	*/
	void Rewrite();

	int GetStrokeCount();

	/**
	* 产生屏幕上采集到的点信息
	* @return
	*/
	string GetOriginOutput();

	/**
	* 由采集到的点信息，整理得到Lua评判的点信息
	* @param size
	* @return
	*/
	string GetLuaOutput(CCSize size);

	/**
	* 产生手写点点集列表
	* @return
	*/
	list<RowStroke> GetHandWritingPoints();

	ScriptCharacter GetScriptCharacter();

	CC_SYNTHESIZE_RETAIN(CCArray*,strokeDrawlist,StrokeDrawnodeList);	// 用来保存手写点数组
	CC_SYNTHESIZE_RETAIN(CCArray*,segmentdraw_list, SegmentDrawnodeList);			// Segment drawnode list

	CCSprite* tianzige_;
	ScriptCharacter script_char;			// 手写字
};



#endif