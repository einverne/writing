//
//  TemplateCharacter.hpp
//  game
//
//  Created by Ein Verne on 16/3/16.
//
//

#ifndef TemplateCharacter_hpp
#define TemplateCharacter_hpp

#include <stdio.h>
#include "Segment.hpp"
#include "Component.hpp"
#include "Stroke.h"
#include "FlagMatrix.hpp"
#include "GeometryTool.hpp"
#include <list>
#include <string>

using namespace std;

#define R_INTERSECT  "I"
#define R_CONJOINT   "C"
#define R_APART      "A"
#define R_NOTAPART   4

//用于表示模板字
class TemplateCharacter
{
public:
    list<Stroke> stroke_list;    //笔画列表。按照书写顺序存储该汉字的所有笔画。
    list<Segment> segment_list;  //笔段列表。按照书写顺序存储该汉字的所有笔段。
    
    list<int>  instable_segment_list;  //不稳定笔段索引的列表。不稳定笔段是指：书写角度随意性较大的笔段。
    
    FlagMatrix segment_relation;  //笔段之间的关系标识(三种：I相交, A相离, C相接 )
    //相接时的细分(B首点，E尾点；N其他位置)
    
    //用于动画演示的属性
    int draw_index; //当前演示笔段的索引；
    int draw_point; //当前演示笔段的演示点索引;
    
    //归一化后的尺寸
    int normal_size;
    
    //部件信息，哪些笔段属于同一个部件
    list<CComponent>  m_components;
    string m_struct_name;
    
public:
    TemplateCharacter();
    TemplateCharacter(const TemplateCharacter& T); //拷贝
    TemplateCharacter& operator=(const TemplateCharacter& Right);    //赋值
    virtual ~TemplateCharacter();
    void clearalldata();
    
    void Add_one_component_setting(CComponent m);
    bool IsShareOnecomponent(int segment1, int segment2);
    
    void normalize(int height=512,int width=512);  //尺寸的等比例缩放，路径的均匀插值。
//    void draw(CDC* pDC);
    void AnimationProgress(bool ani);
    
    bool InsameStroke(int segindex1,int segindex2);
    bool Is_Last_segment_of_OneStroke(int segindex);
    
    //////////////////////////////
    void AppendStroke(string name, list<Segment> s);//添加一个笔画
    void addinstablesegment(int ind);//标记一个不稳定笔段
    
    void InitSegmentRelation();
    void setRelation(int first,int second, string r);//标记关系
    string  getRelation(int first,int second);
    
    //////////////////////////////
    Stroke getstroke(int num);
    Segment getsegment(int num);
    list<int> getinstablesegments();
    bool isinstablesegment(int num);
    
//    void SetSegmentColor(int num, COLORREF color);
//    void clearSegmentColor(COLORREF color);
    
    /////////////////////////////////
    list<int> find_first_apart_second_notapart_relation_segment(int k1, int k2);
    int find_noturning_connecttion_segment(int segindex);
    list<int>  find_intersect_relation_segment(int segindex);
    int find_common_connect_relation_segment(int k1, int k2);
    
protected:
    void Replacesegment(int ind, Segment seg);
};

#endif /* TemplateCharacter_hpp */
