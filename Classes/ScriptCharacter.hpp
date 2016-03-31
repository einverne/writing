//
//  ScriptCharacter.hpp
//  game
//
//  Created by Ein Verne on 16/3/16.
//
//

#ifndef ScriptCharacter_hpp
#define ScriptCharacter_hpp

#include <stdio.h>
#include <list>
#include "RowStroke.hpp"
#include "Stroke.h"
#include "Segment.hpp"
#include "cocos2d.h"

USING_NS_CC;


class ScriptCharacter{
public:
    list<RowStroke> row_stroke;
    bool divide;
    
    list<Stroke> stroke_list;
    list<Segment> segment_list;
    
    list<int> noise_list;               //疑似抖笔笔段列表。怀疑是抖笔的笔段索引号。

	//用于动画演示的属性
	int draw_index; //当前演示笔段的索引；
	int draw_point; //当前演示笔段的演示点索引;

	int normal_size;
    
    ScriptCharacter();
    ScriptCharacter(const ScriptCharacter& T);
    ScriptCharacter& operator=(const ScriptCharacter& T);    //赋值
    ~ScriptCharacter();

	void clear_divide_data();
	void clearalldata();
	void init(list<RowStroke> slist);  //原始书写笔画初始化

	void normalize(int height=512,int width=512);  //尺寸的缩放，路径的均匀插值。
	void divideSegment1();  //识别笔段。将每个笔画划分为一系列笔段。
	void divideSegment2();  //识别笔段。将每个笔画划分为一系列笔段。

	void IdentifynoiseSegment();//识别抖笔笔段 
	void new_IdentifynoiseSegment();//识别抖笔笔段 

	//void draw(CDC* pDC);
	void AnimationProgress(bool ani);

	bool InsameStroke(int segindex1,int segindex2);
	void remove_from_noistlist(int segindex);

	//////////////////////////////
	RowStroke getrowstroke(int num);
	Stroke getstroke(int num);
	Segment getsegment(int num);
	bool IsNoiseSegment(int num);

	void SetSegmentColor(int num, ccColor4F color);
	void clearSegmentColor(ccColor4F color);

	bool is_apart_relation(int h1,int h2);
	int find_noturning_connection_segment(int segindex);


protected:
	void ReplaceRowstroke(int ind, RowStroke Row);	
	void Append_divide_Stroke(list<Segment> s);//添加一个笔画,用于笔段分割

public:
	void Replacesegment(int ind, Segment seg);
	void removesegment(int ind);
};

#endif /* ScriptCharacter_hpp */
