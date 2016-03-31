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

class RowStroke : public Stroke{
public:
    
    list<CCPoint> Rowplist;

	RowStroke();
	~RowStroke();

	///////
	RowStroke& operator=(const RowStroke& Right);    //赋值
	RowStroke(const RowStroke& Right); //拷贝构造
	
	//////
	void Init(list<CCPoint> Right);

	void clear();
	CCPoint getpoint(int ind);
	void setpoint(int ind, CCPoint p);
	void setpointlist(list<CCPoint> ptlist);
	list<CCPoint> getpointlist();

};

#endif /* RowStroke_hpp */
