//
//  GeometryTool.hpp
//  game
//
//  Created by Ein Verne on 16/3/16.
//
//

#ifndef GeometryTool_hpp
#define GeometryTool_hpp

#include <stdio.h>
#include "cocos2d.h"
#include <list>
#include "Segment.hpp"

USING_NS_CC;
using namespace std;

class GeometryTool
{
public:
    GeometryTool();
    ~GeometryTool();
    
public:
    bool judge_intersection(CCPoint p1,CCPoint p2,CCPoint p3,CCPoint p4); //判断线段p1p2和p3p4是否相交
    
    CCPoint segment_interpoint(const CCPoint pt1s, const CCPoint pt1e, const CCPoint pt2s, const CCPoint pt2e);//计算交点
    
    float pointToSegment(CCPoint pts,CCPoint pte,  CCPoint pt); //点到线段的最短距离
    float pointDistance(CCPoint pt1,CCPoint pt2); //两点之间的距离
    float PathLength(list<CCPoint> ptlist); //路径长度
    float PathLength_forpart(list<CCPoint> ptlist,int m_s, int m_e); //部分路径长度，起始点索引为m_s，终止点所因为m_e
    
    list<CCPoint>  UniformpathResample(list<CCPoint> ptlist); //路径的均匀采样；
    
    //从索引m_s开始，按照dir方向计算，找到当路径长度与len_threshold最接近时的索引点。
    int Find_nearest_index_for_length(list<CCPoint> ptlist, int m_s, float len_threshold, bool dir);
    
    //笔画切分
    void TurningDivide(list<CCPoint> ptlist, list<int>& turning_ind);
    void MergeTurning(list<int> pl1, list<int> pl2, list<int>& result);
    void New_TurningDivide(list<CCPoint> ptlist, list<int>& turning_ind, int boundinglength);
    
    //角度
    float AngleInDegrees(CCPoint start, CCPoint end, bool positiveOnly);
    float AngleInRadians(CCPoint start, CCPoint end, bool positiveOnly);
    float Rad2Deg(float rad);
    float max3(float dis,float dis1, float dis2);
    
    ////笔段相似性
    float PathSimilarity(const Segment &spoint, const Segment &mpoint, int normal_size, bool stable, bool dense);
    
    ////路径中点
    CCPoint PathMidpoint(list<CCPoint> ptlist);
    
    ////路径方向
    float PathDirectionAngle(list<CCPoint> ptlist);
    
    ////相容性计算
    float computecompatibility(const Segment &pi, const Segment &pj, const Segment &ph, const Segment &pk, bool samecompnent, bool instable, string relation, int normal_size, bool dense);
    float Evaluate_relation_similarity(const Segment &pi, const Segment &pj, const Segment &ph, const Segment &pk, string relation, int normal_size, bool dense);
    
private:
    float direction(CCPoint pi,CCPoint pj,CCPoint pk); //计算向量pkpi和向量pjpi的叉积
};

#endif /* GeometryTool_hpp */
