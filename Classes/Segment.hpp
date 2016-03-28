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
using namespace std;
USING_NS_CC;

class Segment{
public:
    string name;
    int seg_index;
    
    list<CCPoint> plist;
    CCPoint mid_point;
    float dirangle;
    float len;
    
    
    
public:
    Segment();
    ~Segment();
    
    ///////
    Segment& operator=(const Segment& Right);    //赋值
    Segment(const Segment& Right); //拷贝构造
    
    ////
    /**
     * 名字，笔段索引号，点列表
     *
     */
    void Init(string name, int ind, list<CCPoint> p);
    void clear();
    CCPoint getpoint(int ind);
    void setpoint(int ind, CCPoint p);
    void setpointlist(list<CCPoint> ptlist);
    list<CCPoint> getpointlist();
    
    ////
    void UpdateAttribute();
};

#endif /* Segment_hpp */
