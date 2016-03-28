//
//  Segment.cpp
//  game
//
//  Created by Ein Verne on 16/3/15.
//
//

#include "Segment.hpp"

Segment::Segment(){
    name = "noname";
    seg_index = 0;
    
    plist.clear();
    
    mid_point = ccp(0.0, 0.0);
    dirangle = 0;
    len = 0;
}

Segment::~Segment(){
    plist.clear();
}



Segment::Segment(const Segment& Right)
{
    
    this->name=Right.name;  //笔段名称。只用于模板字
    this->seg_index=Right.seg_index; //笔段索引号。按照书写顺序编号，起始笔段编号为0。用于模板字和手写字
//    this->color=Right.color;  //笔段的绘制颜色。默认为黑色，匹配后定义为某种彩色。
    
    this->plist.assign(Right.plist.begin(),Right.plist.end());  //点列表;
    
    this->mid_point=Right.mid_point;
    this->dirangle=Right.dirangle;
    this->len=Right.len;
}

Segment&  Segment::operator=(const Segment& Right)
{
    
    this->name=Right.name;  //笔段名称。只用于模板字
    this->seg_index=Right.seg_index; //笔段索引号。按照书写顺序编号，起始笔段编号为0。用于模板字和手写字
//    this->color=Right.color;  //笔段的绘制颜色。默认为黑色，匹配后定义为某种彩色。
    
    this->plist.assign(Right.plist.begin(),Right.plist.end());  //点列表;
    
    this->mid_point=Right.mid_point;
    this->dirangle=Right.dirangle;
    this->len=Right.len;
    
    return (*this);
}

void Segment::Init(string name, int ind, list<CCPoint> p)
{
    this->name=name;  //笔段名称。只用于模板字
    this->seg_index=ind; //笔段索引号。按照书写顺序编号，起始笔段编号为0。用于模板字和手写字
//    this->color=RGB(0,0,0);  //笔段的绘制颜色。默认为黑色，匹配后定义为某种彩色。
    
    this->plist.assign(p.begin(),p.end());  //点列表;
    
    UpdateAttribute();
}


void Segment::UpdateAttribute()
{
//    ASSERT(this->plist.size()>=2);
    
    //////////////////////////////
//    GeometryTools gt;
//    mid_point=gt.Pathmid_point(this->plist);
//    dirangle=gt.PathDirectionAngle(this->plist);
//    len=gt.PathLength(this->plist);
}

void Segment::clear()
{
    name = "noname";
    seg_index=0;
//    color=RGB(0,0,0);
    
    plist.clear();
    
    ////
    mid_point=CCPoint(0,0);   //笔段的中点坐标
    dirangle=0;      //笔段的方向
    len=0;           //笔段的长度
}

CCPoint Segment::getpoint(int ind)
{
//    ASSERT(ind>=0);
//    ASSERT(ind<this->plist.size());
    
    list<CCPoint>::iterator it = this->plist.begin();
    
    for (int x=0;x<ind;x++)
    {
        it++;
    }
    
    return *it;
}

void Segment::setpoint(int ind, CCPoint p)
{
//    ASSERT(ind>=0);
//    ASSERT(ind<this->plist.size());
    
    list<CCPoint>::iterator it = this->plist.begin();
    
    for (int x=0;x<ind;x++)
    {
        it++;
    }
    
    *it=p;
}


void Segment::setpointlist(list<CCPoint> ptlist)
{
    if(&ptlist != &(this->plist))
    {
        this->plist.clear();
        this->plist=ptlist;
    }	
}

list<CCPoint> Segment::getpointlist()
{
    return this->plist;
}