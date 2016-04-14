//
//  Segment.cpp
//  game
//
//  Created by Ein Verne on 16/3/15.
//
//

#include "Segment.hpp"
#include "GeometryTool.hpp"

Segment::Segment(){
    name_ = "noname";
    seg_index_ = 0;
    
    point_list_.clear();
    
    mid_point_ = ccp(0.0, 0.0);
    dirangle_ = 0;
    len_ = 0;

	color_ = ccc4f(0,0,0,1);
}

Segment::~Segment(){
    point_list_.clear();
}



Segment::Segment(const Segment& Right)
{
    
    this->name_=Right.name_;  //笔段名称。只用于模板字
    this->seg_index_=Right.seg_index_; //笔段索引号。按照书写顺序编号，起始笔段编号为0。用于模板字和手写字
    this->color_=Right.color_;  //笔段的绘制颜色。默认为黑色，匹配后定义为某种彩色。
    
    //this->point_list_.assign(Right.point_list_.begin(),Right.point_list_.end());  //点列表;
    this->point_list_ = Right.point_list_;
    this->mid_point_=Right.mid_point_;
    this->dirangle_=Right.dirangle_;
    this->len_=Right.len_;
}

Segment&  Segment::operator=(const Segment& Right)
{
    
    this->name_=Right.name_;  //笔段名称。只用于模板字
    this->seg_index_=Right.seg_index_; //笔段索引号。按照书写顺序编号，起始笔段编号为0。用于模板字和手写字
    this->color_=Right.color_;  //笔段的绘制颜色。默认为黑色，匹配后定义为某种彩色。
    
    //this->point_list_.assign(Right.point_list_.begin(),Right.point_list_.end());  //点列表;
    this->point_list_ = Right.point_list_;
    this->mid_point_=Right.mid_point_;
    this->dirangle_=Right.dirangle_;
    this->len_=Right.len_;
    
    return (*this);
}

void Segment::Init(string name, int index, list<CCPoint> p)
{
    this->name_=name;  //笔段名称。只用于模板字
	CCLog("name %s",name_.c_str());
    this->seg_index_=index; //笔段索引号。按照书写顺序编号，起始笔段编号为0。用于模板字和手写字
    this->color_=ccc4f(0,0,0,1);  //笔段的绘制颜色。默认为黑色，匹配后定义为某种彩色。
    
    this->point_list_.assign(p.begin(),p.end());  //点列表;
    
    UpdateAttribute();
}


void Segment::UpdateAttribute()
{
//    ASSERT(this->plist.size()>=2);
    
    //////////////////////////////
   GeometryTool gt;
   mid_point_ =gt.PathMidpoint(this->point_list_);
   dirangle_ =gt.PathDirectionAngle(this->point_list_);
   len_ =gt.PathLength(this->point_list_);
}

void Segment::Clear()
{
    name_ = "noname";
    seg_index_=0;
//    color=RGB(0,0,0);
    
    point_list_.clear();
    
    ////
    mid_point_=CCPoint(0,0);   //笔段的中点坐标
    dirangle_=0;      //笔段的方向
    len_=0;           //笔段的长度
}

CCPoint Segment::GetPoint(int ind)
{
//    ASSERT(ind>=0);
//    ASSERT(ind<this->plist.size());
    
    list<CCPoint>::iterator it = this->point_list_.begin();
    
    for (int x=0;x<ind;x++)
    {
        it++;
    }
    
    return *it;
}

void Segment::SetPoint(int ind, CCPoint p)
{
//    ASSERT(ind>=0);
//    ASSERT(ind<this->plist.size());
    
    list<CCPoint>::iterator it = this->point_list_.begin();
    
    for (int x=0;x<ind;x++)
    {
        it++;
    }
    
    *it=p;
}


void Segment::SetPointList(list<CCPoint> ptlist)
{
    if(&ptlist != &(this->point_list_))
    {
        this->point_list_.clear();
        this->point_list_=ptlist;
    }	
}

list<CCPoint> Segment::GetPointList()
{
    return this->point_list_;
}