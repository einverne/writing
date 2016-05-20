//
//  RowStroke.hpp
//  用于表示没有进行笔段分割处理的手写笔画
//
//  Created by Ein Verne on 16/3/16.
//
//

#ifndef RowStroke_hpp
#define RowStroke_hpp

#include <stdio.h>
#include <list>
#include "Stroke.h"
using namespace std;

#include "cocos2d.h"
USING_NS_CC;

class RowStroke{
public:
	RowStroke();
	~RowStroke();

	///////
	RowStroke& operator=(const RowStroke& Right);    //赋值
	RowStroke(const RowStroke& Right); //拷贝构造
	
	//////
	void Init(vector<CCPoint> Right);

	void Clear();

	/**
	* 获取笔画中的 index 点
	* @param ind 点序号， 从0开始
	* @return CCPoint 点信息
	*/
	CCPoint GetPoint(int ind);

	/**
	* 设置第index位置，点为p
	* @param ind 点序号
	* @param p 点信息
	* @return
	*/
	void SetPoint(int ind, CCPoint p);
	
	/**
	* 
	* @param ptlist
	* @return
	*/
	void SetPointList(vector<CCPoint> ptlist);

	/**
	* 从 list 设置点集信息
	* @param ptlist
	* @return
	*/
	void SetPointList(list<CCPoint> ptlist);

	vector<CCPoint> GetPointList();

	/**
	* 将 RowStroke 中的 vector 转换成 list
	* @return
	*/
	list<CCPoint> GetRowPoint();

private:
	vector<CCPoint> row_list_;
};

#endif /* RowStroke_hpp */
