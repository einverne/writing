//
//  Segment.hpp
//  笔段类
//
//  Created by Ein Verne on 16/3/15.
//
//

#ifndef Segment_hpp
#define Segment_hpp

#include <stdio.h>
#include "cocos2d.h"

#include <list>
#include "FlagMatrix.hpp"
using namespace std;
USING_NS_CC;

class Segment{
public:
    string name_;
    int seg_index_;
    
    list<CCPoint> point_list_;
    CCPoint mid_point_;
    float dirangle_;
    float len_;
    
	ccColor4F color_;
    
public:
    Segment();
    ~Segment();
    
    ///////
    Segment& operator=(const Segment& Right);    //赋值
    Segment(const Segment& Right); //拷贝构造
    
    
    /**
    * 名字，笔段索引号，点列表
    * @param name 笔段名字
    * @param ind 笔段索引
    * @param p 笔段点list
    * @return
    */
    void Init(string name, int ind, list<CCPoint> p);
    void Clear();
    CCPoint GetPoint(int ind);
    void SetPoint(int ind, CCPoint p);
    void SetPointList(list<CCPoint> ptlist);
    list<CCPoint> GetPointList();
    
    ////
    void UpdateAttribute();
};

#endif /* Segment_hpp */
