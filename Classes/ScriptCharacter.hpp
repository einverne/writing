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

// 手写字 Character
class ScriptCharacter{
public:
    list<RowStroke> row_stroke_;
    bool divide_;
    
    list<Stroke> stroke_list_;
    list<Segment> segment_list_;
    
    list<int> noise_list_;               //疑似抖笔笔段列表。怀疑是抖笔的笔段索引号。

	//用于动画演示的属性 弃用
	int draw_index; //当前演示笔段的索引；
	int draw_point; //当前演示笔段的演示点索引;

	int normal_size_;

	// 保存原始笔迹包围盒以及中心点
	float width_;
	float height_;
	CCPoint center_point_;

    
    ScriptCharacter();
    ScriptCharacter(const ScriptCharacter& T);
    ScriptCharacter& operator=(const ScriptCharacter& T);    //赋值
    ~ScriptCharacter();

	void ClearDivideData();
	void ClearAllData();
	void Init(list<RowStroke> slist);  //原始书写笔画初始化

	void Normalize(int height=512,int width=512);  //尺寸的缩放，路径的均匀插值。

	/**
	* 计算手写字的包围盒，获得原始笔迹 width, height, center_point
	* 在 Normalize 之前调用，保存原始手写信息
	* 调用函数之后，将手写字的包围盒 宽度，高度和中心点保存到成员变量中
	* @return
	*/
	void CalculateBox();

	/**
	* 在匹配算法完成之后，调用此方法，将Normalize 之后的点，移动到原来的位置
	* 相当于在每个点上加上一个位移量
	* 目的为使手写汉字在 变形 匹配之后恢复到原先的位置
	* @return
	*/
	void MoveToDefaultPlace();

	void divideSegment1();  //识别笔段。将每个笔画划分为一系列笔段。
	void divideSegment2();  //识别笔段。将每个笔画划分为一系列笔段。

	void IdentifynoiseSegment();//识别抖笔笔段 
	void NewIdentifynoiseSegment();//识别抖笔笔段 

	//void draw(CDC* pDC);
	void AnimationProgress(bool ani);

	bool InsameStroke(int segindex1,int segindex2);
	void RemoveFromNoistlist(int segindex);

	//////////////////////////////
	RowStroke GetRowStroke(int num);
	Stroke GetStroke(int num);
	Segment GetSegment(int num);
	bool IsNoiseSegment(int num);

	void SetSegmentColor(int num, ccColor4F color);
	void clearSegmentColor(ccColor4F color);

	bool is_apart_relation(int h1,int h2);
	int find_noturning_connection_segment(int segindex);

	void TransferCoordinate(int size=512){
		for (list<Segment>::iterator itr = segment_list_.begin();
			itr != segment_list_.end(); itr ++)
		{
			Segment seg = *itr;
			list<CCPoint> temp_list_points;
			for (list<CCPoint>::iterator point_iter = seg.point_list_.begin();
				point_iter != seg.point_list_.end(); point_iter++)
			{
				CCPoint point = *point_iter;
				float y =  - point.y;			// default 512
				y = y + size;
				CCPoint temp_point = ccp(point.x , y);
				temp_list_points.push_back(temp_point);
			}
			itr->point_list_.assign(temp_list_points.begin(),temp_list_points.end());
			temp_list_points.clear();
		}
	}

	void TransferCoordinateToFour(int size=512){
		for (list<Segment>::iterator itr = segment_list_.begin();
			itr != segment_list_.end(); itr ++)
		{
			Segment seg = *itr;
			list<CCPoint> temp_list_points;
			for (list<CCPoint>::iterator point_iter = seg.point_list_.begin();
				point_iter != seg.point_list_.end(); point_iter++)
			{
				CCPoint point = *point_iter;
				float y = -(point.y - size);			// default 512
				CCPoint temp_point = ccp(point.x , y);
				temp_list_points.push_back(temp_point);
			}
			itr->point_list_.assign(temp_list_points.begin(),temp_list_points.end());
			temp_list_points.clear();
		}
	}

protected:
	void ReplaceRowstroke(int ind, RowStroke Row);	
	void Append_divide_Stroke(list<Segment> s);//添加一个笔画,用于笔段分割

public:
	void Replacesegment(int ind, Segment seg);
	void removesegment(int ind);
};

#endif /* ScriptCharacter_hpp */
