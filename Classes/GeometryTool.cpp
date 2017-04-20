//
//  GeometryTool.cpp
//  几何计算相关
//
//  Created by Ein Verne on 16/3/16.
//
//

#include "GeometryTool.hpp"
#include <algorithm>

#define  PI 3.1415926

#define R_INTERSECT  "I"
#define R_CONJOINT   "C"
#define R_APART      "A"
#define R_NOTAPART   4

GeometryTool::GeometryTool(){

}

GeometryTool::~GeometryTool(){

}

//判断线段p1p2和p3p4是否相交
bool GeometryTool::judge_intersection(CCPoint p1,CCPoint p2,CCPoint p3,CCPoint p4)
{
    float d1 = direction(p3,p4,p1);
    float d2 = direction(p3,p4,p2);
    float d3 = direction(p1,p2,p3);
    float d4 = direction(p1,p2,p4);
    if(d1*d2<0.0 && d3*d4<0.0)
        return true;
    else
        return false;
}

//计算向量pkpi和向量pjpi的叉积
float GeometryTool::direction(CCPoint pi,CCPoint pj,CCPoint pk)
{
    return (pi.x-pk.x)*(pi.y-pj.y)-(pi.y-pk.y)*(pi.x-pj.x);
}

//计算点到线段的最短距离：pts，pte为线段端点，pt为外面一点。
float GeometryTool::pointToSegment(CCPoint pts,CCPoint pte,  CCPoint pt)
{
    float space = 0;
    float a, b, c;
    a = pointDistance(pts, pte);//线段长度
    b = pointDistance(pts, pt);// 线段起点到pt距离
    c = pointDistance(pte, pt);//线段终点到pt距离
    
    if (c <= 0.000001 || b <= 0.000001)
    {
        space = 0;
        return space;
    }
    
    if (a <= 0.000001)
    {
        space = b;
        return space;
    }
    
    if (c * c >= a * a + b * b)
    {
        space = b;
        return space;
    }
    if (b * b >= a * a + c * c)
    {
        space = c;
        return space;
    }
    
    double p = (a + b + c) / 2;//三角形半周长
    double s = sqrt(p * (p - a) * (p - b) * (p - c));//海伦公式求面积
    space = 2 * s / a;//返回点到线的距离（利用三角形面积公式求高）
    return space;
}

float GeometryTool::pointDistance(CCPoint pt1,CCPoint pt2)
{
    float dx = pt2.x - pt1.x;
    float dy = pt2.y - pt1.y;
    return sqrtf(dx * dx + dy * dy);
}

float GeometryTool::PathLength_forpart(list<CCPoint> ptlist,int s_indx, int e_indx)
{
    //ASSERT(s_indx>=0);
    //ASSERT(e_indx<=ptlist.size()-1);
    //ASSERT(s_indx<e_indx);
    //////////////////////////////
    float length = 0;
    list<CCPoint>::iterator it=ptlist.begin();
    list<CCPoint>::iterator it2=ptlist.begin();
    int temp_count=0;
    
    while(temp_count!=s_indx)
    {
        temp_count++;
        it++;
    }
    
    temp_count=0;
    while(temp_count!=e_indx)
    {
        temp_count++;
        it2++;
    }
    
    ///////
    while (it!= it2)
    {
        CCPoint p1=*it;
        it++;
        CCPoint p2=*it;
        it--;
        length += pointDistance(p1, p2);
        it++;
    }
    
    return length;
}

float GeometryTool::PathLength(list<CCPoint> ptlist)
{
    float length = 0;
    list<CCPoint>::iterator it=ptlist.begin();
    list<CCPoint>::iterator it2=ptlist.begin();
    it++;
    ///////
    while (it!= ptlist.end())
    {
        length += pointDistance(*it2, *it);
        it2++;
        it++;
    }
    return length;
    
}

list<CCPoint>  GeometryTool::UniformpathResample(list<CCPoint> ptlist)  //相邻点之间间隔3个单位
{
    list<CCPoint>  templist;
    templist.clear();
    
    //路径长度判断
    float len=PathLength(ptlist);
    if (len<=3.0)
    {
        list<CCPoint>::iterator it=ptlist.begin();
        templist.push_back(*it);
        it=ptlist.end();
        it--;
        templist.push_back(*it);
        return templist;
    }
    
    //
    float I = 3.0; // 相邻点之间间隔3个单位
    float D = 0.0;
    list<CCPoint>::iterator it0=ptlist.begin();
    list<CCPoint>::iterator it1=ptlist.begin();
    templist.push_back(*it0);
    it1++;
    
    while(it1 != ptlist.end())
    {
        CCPoint pt1 = *(it0);
        CCPoint pt2 = *(it1);
        
        float d = pointDistance(pt1, pt2);
        
        if ((D + d) >= I)
        {
            float qx = pt1.x + ((I - D) / d) * (pt2.x - pt1.x);
            float qy = pt1.y + ((I - D) / d) * (pt2.y - pt1.y);
            
            CCPoint q(qx, qy);
            
            templist.push_back(q); // append new point 'q'
            it1=ptlist.insert(it1,q);
            
            D = 0.0;
            it0=it1;
            it1++;
        }
        else
        {
            D += d;
            it0++;
            it1++;
        }
    }
    
    CCPoint pt1 = *(--ptlist.end());
    CCPoint pt2 = *(--templist.end());
    
    if (pointDistance(pt1, pt2)>0.05)
    {
        templist.push_back(pt1);
    }
    
    //
    return templist;
}

int GeometryTool::Find_nearest_index_for_length(list<CCPoint> ptlist, int m_s, float len_threshold, bool dir)
{
    int m_result;
    
    if (dir)
    {
        int temp1=m_s+1;
        int temp2=m_s+1;
        float len1=PathLength_forpart(ptlist,m_s,temp1);
        float len2=PathLength_forpart(ptlist,m_s,temp2);
        while(len2<len_threshold && temp2<ptlist.size()-1)
        {
            temp1=temp2;
            len1=len2;
            temp2++;
            len2=PathLength_forpart(ptlist,m_s,temp2);
        }
        
        if(fabs(len1-len_threshold)<fabs(len2-len_threshold))
            m_result=temp1;
        else
            m_result=temp2;
    }
    else
    {
        int temp1=m_s-1;
        int temp2=m_s-1;
        float len1=PathLength_forpart(ptlist,temp1,m_s);
        float len2=PathLength_forpart(ptlist,temp2,m_s);
        while(len2<len_threshold && len2>0)
        {
            temp1=temp2;
            len1=len2;
            temp2--;
            len2=PathLength_forpart(ptlist,temp2,m_s);
        }
        
        if(fabs(len1-len_threshold)<fabs(len2-len_threshold))
            m_result=temp1;
        else
            m_result=temp2;
    }
    
    return m_result;
}

//使用新方法查找拐点，利用的信息包括：点到直线距离、两个相邻段的夹角、每个段的长度等。
//本函数接口中已经传入了整个汉字的包围盒边长(boundinglength)。
//list<CCPoint> ptlist：待分割的原始笔画
//list<int>& turning_ind：分割算法得到的拐点索引，不包括首点，包括尾点。
//boundinglength：整个汉字的包围盒边长
void GeometryTool::New_TurningDivide(list<CCPoint> ptlist, list<int>& turning_ind, int boundinglength)
{
    //ASSERT(ptlist.size()>=2);
    ///////////////////////
    list<int> turning_can;			// 可能拐点list
    turning_can.clear();
    
    float point_seg_dis_threshold = boundinglength/75.0;
    float angle_merge_threshold = 128.0;
    float not_end_seg_len_threshold = boundinglength/12.0;
    
    float end_seg_len_threshold = boundinglength/8.8;
    float end_seg_ang_threshold = 140.0;
    
    //1.根据点到直线距离进行切分。
    turning_can.push_back(0);
    turning_can.push_back(ptlist.size()-1);
    
    list<int>::iterator it_can = turning_can.begin();
    for (; it_can!=(--turning_can.end()); )
    {
        int m_b=*it_can;		// begin index
        it_can++;
        int m_e=*it_can;		// end index
        it_can--;
        
        int can=m_b;
        float can_dis=0.0;
        CCPoint pts;
        CCPoint pte;
        CCPoint pt;
        list<CCPoint>::iterator it_pp;
        int temp_ind;
        for (int x=m_b; x<=m_e;x++)
        {
            ///////////////////
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<m_b)
            {
                temp_ind++;
                it_pp++;
            }
            pts=*it_pp;				// start point
            
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<m_e)
            {
                temp_ind++;
                it_pp++;
            }
            pte=*it_pp;				// end point
            
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<x)
            {
                temp_ind++;
                it_pp++;
            }
            pt=*it_pp;
            /////////////////////
            float dis=pointToSegment(pts,pte,pt);	// 点到笔段距离
            if (dis>can_dis)
            {
                can=x;
                can_dis=dis;
            }
        }
        
        if (can_dis>point_seg_dis_threshold)
        {
            it_can++;
            it_can=turning_can.insert(it_can,can);
            it_can--;
        }
        else
        {
            it_can++;
        }
    }
    
    //2.根据夹角进行合并。
    bool m_yes=true;
    while(m_yes)
    {
        if (turning_can.size()<=2)
            break;
        
        ///////
        int mm;
        float max_ang=0.0;
        list<CCPoint>::iterator it_pp;
        int temp_ind;
        it_can=turning_can.begin();
        it_can++;
        
        float mm_min_len;
        float curve_dis1, curve_dis2;
        while (it_can!=(--turning_can.end()))
        {
            it_can--;
            int temp_ms=*it_can;
            it_can++;
            int temp_mm=*it_can;
            it_can++;
            int temp_me=*it_can;
            it_can--;
            
            /////////////////////////////////////
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<temp_ms)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint pts=*it_pp;
            
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<temp_mm)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint ptm=*it_pp;
            
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<temp_me)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint pte=*it_pp;
            
            /////////////////////////////
            int temp_mid_sm=(temp_ms+temp_mm)/2;
            int temp_mid_me=(temp_mm+temp_me)/2;
            
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<temp_mid_sm)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint ptsm=*it_pp;
            
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<temp_mid_me)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint ptme=*it_pp;
            
            ////////////////////////////
            float ang1 = AngleInDegrees(ptm, pts, true);
            float ang2 = AngleInDegrees(ptm, pte, true);
            float len1 = pointDistance(ptm,pts);
            float len2 = pointDistance(ptm,pte);
            
            float dis = fabs(ang2 - ang1);
            if (dis > 180.0)
                dis = 360.0 - dis;
            
            if (dis>max_ang)
            {
                max_ang=dis;
                mm=temp_mm;
                mm_min_len=min(len1,len2);
                
                ///////////////////////////////
                //找到包围盒中心
                float minx=min(pts.x,min(pte.x,ptm.x));
                float miny=min(pts.y,min(pte.y,ptm.y));
                float maxx=max(pts.x,max(pte.x,ptm.x));
                float maxy=max(pts.y,max(pte.y,ptm.y));
                
                //计算距离差别
                curve_dis1=pointDistance(CCPoint((minx+maxx)/2,(miny+maxy)/2),CCPoint((pts.x+ptm.x)/2,(pts.y+ptm.y)/2))-
                pointDistance(CCPoint((minx+maxx)/2,(miny+maxy)/2),ptsm);
                curve_dis2=pointDistance(CCPoint((minx+maxx)/2,(miny+maxy)/2),CCPoint((pte.x+ptm.x)/2,(pte.y+ptm.y)/2))-
                pointDistance(CCPoint((minx+maxx)/2,(miny+maxy)/2),ptme);
                ///////////////////////////////
            }
            it_can++;
        }
        
        /////////////////////////
        //添加弧度弯曲方向
        if(max_ang<136 && mm_min_len>boundinglength/6.9 && max(curve_dis1,curve_dis2)>2.55 && min(curve_dis1,curve_dis2)>-3)
        {
            m_yes=false;
        }
        else if(max_ang<153 && mm_min_len>boundinglength/7.4 && max(curve_dis1,curve_dis2)>7.5 && min(curve_dis1,curve_dis2)>-3.7)
        {
            m_yes=false;
        }
        else if(max_ang<110 && mm_min_len>boundinglength/4.2 && max(curve_dis1,curve_dis2)<-13.0 && min(curve_dis1,curve_dis2)<-17.0)
        {
            m_yes=false;;
        }
        /////////////////////////
        else if(max_ang>angle_merge_threshold || (max_ang>105.7 && turning_can.size()>=6 && mm_min_len<boundinglength/3.9))
        {
            turning_can.remove(mm);
        }
        else
        {
            m_yes=false;
        }
    }
    
    //3.首尾笔段的合并：根据首尾笔段长度、与相邻笔段夹角进行合并
    bool m_yes1=true;
    bool m_yes2=true;
    while(m_yes1 && m_yes2)
    {
        if (turning_can.size()<3)
            break;
        
        //首笔段
        it_can=turning_can.begin();
        int ms,mm,me;
        ms=*it_can;
        it_can++;
        mm=*it_can;
        it_can++;
        me=*it_can;
        
        float s_len=PathLength_forpart(ptlist, ms,mm);
        float m_len=PathLength_forpart(ptlist, mm,me);
        
        if (s_len<end_seg_len_threshold && 3.5*s_len<m_len && m_yes1)
        {
            int new_me=Find_nearest_index_for_length(ptlist, mm, s_len,true);
            list<CCPoint>::iterator it_pp;
            int temp_ind;
            
            ///////////////////
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<ms)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint pts=*it_pp;
            
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<mm)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint ptm=*it_pp;
            
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<new_me)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint pte=*it_pp;
            
            float ang1 = AngleInDegrees(ptm, pts, true);
            float ang2 = AngleInDegrees(ptm, pte, true);
            
            float dis = fabs(ang2 - ang1);
            if (dis > 180.0)
                dis = 360.0 - dis;
            
            if (dis>end_seg_ang_threshold || (dis>134.0 && s_len<boundinglength/17.0))
                turning_can.remove(mm);
            else
                m_yes1=false;
        }
        else
            m_yes1=false;
        
        //尾笔段
        if (turning_can.size()<3)
            break;
        
        it_can=turning_can.end();
        it_can--;
        me=*it_can;
        it_can--;
        mm=*it_can;
        it_can--;
        ms=*it_can;
        
        s_len=PathLength_forpart(ptlist, ms,mm);
        m_len=PathLength_forpart(ptlist, mm,me);
        
        if (m_len<end_seg_len_threshold && 3.5*m_len<s_len && m_yes2)
        {
            int new_ms=Find_nearest_index_for_length(ptlist, mm, m_len,false);
            list<CCPoint>::iterator it_pp;
            int temp_ind;
            
            ///////////////////
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<new_ms)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint pts=*it_pp;
            
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<mm)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint ptm=*it_pp;
            
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<me)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint pte=*it_pp;
            
            float ang1 = AngleInDegrees(ptm, pts, true);
            float ang2 = AngleInDegrees(ptm, pte, true);
            
            float dis = fabs(ang2 - ang1);
            if (dis > 180.0)
                dis = 360.0 - dis;
            
            if (dis>end_seg_ang_threshold || (dis>134.0 && m_len<boundinglength/17.0))
                turning_can.remove(mm);
            else
                m_yes2=false;
        }
        else
            m_yes2=false;
    }
    
    //4.中间笔段的合并：根据中间笔段的长度进行合并。
    m_yes=true;
    while(m_yes)
    {
        if (turning_can.size()<4)
            break;
        
        it_can=turning_can.begin();
        it_can++;
        it_can++;
        it_can++;
        float min_len=boundinglength*3;
        int mms;
        int mm1;
        int mm2;
        int mme;
        list<CCPoint>::iterator it_pp;
        int temp_ind;
        /////////
        while (it_can!=turning_can.end())
        {
            int temp_mme=*it_can;
            it_can--;
            int temp_mm2=*it_can;
            it_can--;
            int temp_mm1=*it_can;
            it_can--;
            int temp_mms=*it_can;
            it_can++;
            it_can++;
            it_can++;
            ////////
            float temp_len=PathLength_forpart(ptlist, temp_mm1,temp_mm2);
            if(temp_len<min_len)
            {
                min_len=temp_len;
                mms=temp_mms;
                mm1=temp_mm1;
                mm2=temp_mm2;
                mme=temp_mme;
            }
            ////////
            it_can++;
        }
        /////////////////////////////////////////////////
        //计算夹角
        it_pp=ptlist.begin();
        temp_ind=0;
        while(temp_ind<mms)
        {
            temp_ind++;
            it_pp++;
        }
        CCPoint pts=*it_pp;
        
        it_pp=ptlist.begin();
        temp_ind=0;
        while(temp_ind<mm1)
        {
            temp_ind++;
            it_pp++;
        }
        CCPoint ptm1=*it_pp;
        
        it_pp=ptlist.begin();
        temp_ind=0;
        while(temp_ind<mm2)
        {
            temp_ind++;
            it_pp++;
        }
        CCPoint ptm2=*it_pp;
        
        it_pp=ptlist.begin();
        temp_ind=0;
        while(temp_ind<mme)
        {
            temp_ind++;
            it_pp++;
        }
        CCPoint ptme=*it_pp;
        
        float ang1 = AngleInDegrees(ptm1, pts, true);
        float ang2 = AngleInDegrees(ptm1, ptm2, true);
        
        float dis1 = fabs(ang2 - ang1);
        if (dis1 > 180.0)
            dis1 = 360.0 - dis1;
        
        float ang3 = AngleInDegrees(ptm2, ptm1, true);
        float ang4 = AngleInDegrees(ptm2, ptme, true);
        
        float dis2 = fabs(ang4 - ang3);
        if (dis2 > 180.0)
            dis2 = 360.0 - dis2;
        
        float dis3 = fabs(ang4 - ang1);
        if (dis3 > 180.0)
            dis3 = 360.0 - dis3;
        /////////////////////////////////////////////////
        if( (min_len<boundinglength/4.3 && dis1>102 && dis2>118 && dis3<72)||
           (min_len<not_end_seg_len_threshold &&
            (PathLength_forpart(ptlist, mms,mm1)>3*min_len || PathLength_forpart(ptlist, mm2,mme)>3*min_len) &&
            ((PathLength_forpart(ptlist, mms,mm1)>min_len || mms==0) && (PathLength_forpart(ptlist, mm2,mme)>min_len || mme==ptlist.size()-1))))
        {
            //与某一端合并，还是均分？
            ///////////////////
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<mms)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint pts=*it_pp;
            
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<mm1)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint ptm1=*it_pp;
            
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<mm2)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint ptm2=*it_pp;
            
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<mme)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint ptme=*it_pp;
            
            float ang1 = AngleInDegrees(ptm1, pts, true);
            float ang2 = AngleInDegrees(ptm1, ptm2, true);
            
            float dis1 = fabs(ang2 - ang1);
            if (dis1 > 180.0)
                dis1 = 360.0 - dis1;
            
            float ang3 = AngleInDegrees(ptm2, ptm1, true);
            float ang4 = AngleInDegrees(ptm2, ptme, true);
            
            float dis2 = fabs(ang4 - ang3);
            if (dis2 > 180.0)
                dis2 = 360.0 - dis2;
            
            //////
            if (dis1>123 && dis2>123)
            {
                turning_can.remove(mm2);
                list<int>::iterator it = find(turning_can.begin(), turning_can.end(), mm1);
                *it=(mm1+mm2)/2;
            }
            else if (dis1>dis2 && dis1>110)
            {
                turning_can.remove(mm1);
            }
            else if(dis2>dis1 && dis2>110)
            {
                turning_can.remove(mm2);
            }
            else if( min(dis1,dis2)<65 && max(dis1,dis2)<89)
            {
                m_yes=false;
            }
            else
            {
                turning_can.remove(mm2);
                list<int>::iterator it = find(turning_can.begin(), turning_can.end(), mm1);
                *it=(mm1+mm2)/2;
            }
        }
        else
        {
            m_yes=false;
        }
    }
    
    //补充：两个小笔段合并；
    if (turning_can.size()==3)
    {
        it_can=turning_can.begin();
        int ms,mm,me;
        ms=*it_can;
        it_can++;
        mm=*it_can;
        it_can++;
        me=*it_can;
        
        float s_len=PathLength_forpart(ptlist, ms,mm);
        float m_len=PathLength_forpart(ptlist, mm,me);
        
        if((s_len<boundinglength/23.0  && m_len<boundinglength/23.0) || (min(s_len,m_len)<boundinglength/30.0 && max(s_len,m_len)<boundinglength/21.0))
            turning_can.remove(mm);
        else
        {
            list<CCPoint>::iterator it_pp;
            it_pp=ptlist.begin();
            int temp_ind=0;
            while(temp_ind<ms)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint pts=*it_pp;
            
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<mm)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint ptm=*it_pp;
            
            it_pp=ptlist.begin();
            temp_ind=0;
            while(temp_ind<me)
            {
                temp_ind++;
                it_pp++;
            }
            CCPoint pte=*it_pp;
            
            float ang1 = AngleInDegrees(ptm, pts, true);
            float ang2 = AngleInDegrees(ptm, pte, true);
            
            float dis = fabs(ang2 - ang1);
            if (dis > 180.0)
                dis = 360.0 - dis;
            
            if(s_len<boundinglength/11.38  && m_len<boundinglength/11.38 && dis>125.0)
                turning_can.remove(mm);
        }
        
        
    }
    
    //5.返回
    turning_ind.clear();
    list<int>::iterator itt=turning_can.begin();
    itt++;
    while(itt!=turning_can.end())
    {
        turning_ind.push_back(*itt);
        itt++;
    }
}

//使用动射线算法查找拐点，其中的一些参数是根据汉字外围矩形框的大小设置的。
//本函数将汉字外围矩形框默认为512×512。所以调用本函数前需要将汉字归一化为512×512。
void GeometryTool::TurningDivide(list<CCPoint> ptlist, list<int>& turning_ind)
{
    int height=512; //汉字包围框大小，这是默认值
    float threshold_len=height/28.0;  //查找拐点的长度阈值
    int threshold_count=7;  //查找拐点的点数阈值
    
    /////////////////////
    turning_ind.clear();
    double len = PathLength(ptlist);
    if(len<threshold_len || ptlist.size()<threshold_count)
    {
        turning_ind.push_back(ptlist.size()-1);
        return;
    }
    
    /////////////////////
    int index_begin = 0; //起始点
    int index_candidate = 3;  //中间点，与起始间隔3个点
    int index_end = 6; //结尾与中间间隔3个点
    
    while (index_end < ptlist.size())
    {
        list<CCPoint>::iterator it0=ptlist.begin();
        list<CCPoint>::iterator it1=ptlist.begin();
        list<CCPoint>::iterator it2=ptlist.begin();
        
        for(int x=0; x<index_begin; x++)
            it0++;
        for(int x=0; x<index_candidate; x++)
            it1++;
        for(int x=0; x<index_end; x++)
            it2++;
        
        //计算方向夹角
        CCPoint pt1 = *it0;
        CCPoint pt2 = *it1;
        CCPoint pt3 = *it2;
        
        float ang1 = AngleInDegrees(pt1, pt2, true);
        float ang2 = AngleInDegrees(pt2, pt3, true);
        
        float dis = fabs(ang2 - ang1);
        if (dis > 180.0)
            dis = 360.0 - dis;
        //如果夹角大于阈值
        if (dis>55)
        {
            //应该在连续的三个点中找一个夹角最大的，作为拐点
            if (ptlist.size()-1 - index_candidate<5)
            {
                turning_ind.push_back(index_candidate);
                index_begin = index_candidate;
                index_candidate = index_begin + 3;
                index_end = index_candidate + 3;
            }
            else
            {
                it1++;
                it2++;
                float a1=AngleInDegrees((*it0), (*it1), true);
                float a2=AngleInDegrees((*it1), (*it2), true);
                float dis1 = fabs(a2 - a1);
                if (dis1 > 180.0)
                    dis1 = 360.0 - dis1;
                
                it1++;
                it2++;
                a1=AngleInDegrees((*it0), (*it1), true);
                a2=AngleInDegrees((*it1), (*it2), true);
                float dis2 = fabs(a2 - a1);
                if (dis2 > 180.0)
                    dis2 = 360.0 - dis2;
                //////////////////////////////////
                float f=max3(dis,dis1,dis2);
                if (f==dis)
                {
                    turning_ind.push_back(index_candidate);
                    index_begin = index_candidate;
                    index_candidate += 3;
                    index_end = index_candidate + 3;
                }
                if (f==dis1)
                {
                    turning_ind.push_back(index_candidate+1);
                    index_begin = index_candidate+1;
                    index_candidate += 4;
                    index_end = index_candidate + 3;
                }
                if (f==dis2)
                {
                    turning_ind.push_back(index_candidate+2);
                    index_begin = index_candidate+2;
                    index_candidate += 5;
                    index_end = index_candidate + 3;
                }
                /////////////////////////////////
                it1--;
                it1--;
                it2--;
                it2--;
            }
        }
        else
        {
            index_candidate++;
            index_end++;
        }
    }
    //最后一个点添加到列表中；
    turning_ind.push_back(ptlist.size()-1);
    
    //首尾细碎点的处理:
    //-------------------------------------1.处于首点，点数小于4
    //2.处于首点，点数小于等于5个，且夹角小于80
    list<int>::iterator mys=turning_ind.begin();
    /*if (turning_ind.size()>1 && (*mys)<4)
     {
     turning_ind.erase(mys);
     }
     else */if (turning_ind.size()>1 && (*mys)<=6)
     {
         CCPoint p1(0,0),p2(0,0),p3(0,0);
         list<CCPoint>::iterator it=ptlist.begin();
         p1=*it;
         
         it=ptlist.begin();
         for (int x=0; x!=(*turning_ind.begin()); x++)
         {it++;}
         p2=*it;
         
         it=ptlist.begin();
         for (int x=0; x!=(*(++turning_ind.begin())); x++)
         {it++;}
         p3=*it;
         
         float a1=AngleInDegrees(p2, p1, true);
         float a2=AngleInDegrees(p3, p2, true);
         float dis1 = fabs(a2 - a1);
         if (dis1 > 180.0)
             dis1 = 360.0 - dis1;
         
         //
         float b1=pointDistance(p2,p1);
         float b2=pointDistance(p3,p2);
         //
         
         if (dis1<80 || max(b1,b2)<30)
         {
             turning_ind.erase(mys);
         }
     }
    
    //------------1.处于尾点，点数小于4
    //2.处于尾点，点数小于等于5个，且夹角小于80
    list<int>::iterator mye1=turning_ind.end();
    mye1--;
    list<int>::iterator mye2=mye1;
    mye2--;
    list<int>::iterator mye3=mye2;
    mye3--;
    
    /*if(turning_ind.size()>1 && (*mye1)-(*mye2)<4)
     {
     turning_ind.erase(mye2);
     }
     else */if (turning_ind.size()>1 && (*mye1)-(*mye2)<=6)
     {
         CCPoint p1(0,0),p2(0,0),p3(0,0);
         
         list<CCPoint>::iterator it=ptlist.begin();
         for (int x=0; x!=(*mye1); x++)
         {it++;}
         p1=*it;
         
         it=ptlist.begin();
         for (int x=0; x!=(*mye2); x++)
         {it++;}
         p2=*it;
         
         if (turning_ind.size()>2)
         {
             it=ptlist.begin();
             for (int x=0; x!=(*mye3); x++)
             {it++;}
             p3=*it;
         }
         else
         {
             p3=*(ptlist.begin());
         }
         
         float a1=AngleInDegrees(p2, p1, true);
         float a2=AngleInDegrees(p3, p2, true);
         float dis1 = fabs(a2 - a1);
         if (dis1 > 180.0)
             dis1 = 360.0 - dis1;
         
         //
         float b1=pointDistance(p2,p1);
         float b2=pointDistance(p3,p2);
         //
         
         if (dis1<80 || max(b1,b2)<30)
         {
             turning_ind.erase(mye2);
         }
     }
    
    /////////////////////////////////////////
    /////////////////////////////////////////
    if (turning_ind.size()<=2)
    {
        return;
    }
    //处理细碎拐点:不处于首尾，点数小于等于10个。
    //如果与两边笔段夹角小于75度，均分
    //如果与两边笔段夹角大于75度和小于75度，归入一方
    list<int>::iterator ita=turning_ind.begin();
    
    list<int>::iterator itb=turning_ind.begin();
    itb++;
    
    list<int>::iterator itc=turning_ind.end();
    itc--;
    
    while(itb != itc)
    {
        int len=(*itb)-(*ita);
        if (len<=10)
        {
            int start,end;
            if (ita==turning_ind.begin())
                start=0;
            else
            {
                ita--;
                start=*ita;
                ita++;
            }
            itb++;
            end=*itb;
            itb--;
            
            ////////////////////////
            CCPoint p1(0,0),p2(0,0),p3(0,0),p4(0,0);
            
            list<CCPoint>::iterator it=ptlist.begin();
            for (int x=0; x!=start; x++)
            {
                it++;
            }
            p1=*it;
            
            it=ptlist.begin();
            for (int x=0; x!=*ita; x++)
            {
                it++;
            }
            p2=*it;
            
            it=ptlist.begin();
            for (int x=0; x!=*itb; x++)
            {
                it++;
            }
            p3=*it;
            
            it=ptlist.begin();
            for (int x=0; x!=end; x++)
            {
                it++;
            }
            p4=*it;
            
            float a1=AngleInDegrees(p2, p1, true);
            float a2=AngleInDegrees(p3, p2, true);
            float a3=AngleInDegrees(p4, p3, true);
            float dis1 = fabs(a2 - a1);
            if (dis1 > 180.0)
                dis1 = 360.0 - dis1;
            float dis2 = fabs(a3 - a2);
            if (dis2 > 180.0)
                dis2 = 360.0 - dis2;
            
            /////////////////
            if (dis1<80 && dis2<80)
            {
                *ita=(*ita+*itb)/2;
                itb=turning_ind.erase(itb);
                itb--;
                ita=itb;
                itb++;
                itc=turning_ind.end();
                itc--;
            }
            else if(dis1>=80 && dis2<80)
            {
                itb=turning_ind.erase(itb);
                itb--;
                ita=itb;
                itb++;
                itc=turning_ind.end();
                itc--;
            }
            else if (dis1<80 && dis2>=80)
            {
                ita=turning_ind.erase(ita);
                ita++;
                itb=ita;
                ita--;
                itc=turning_ind.end();
                itc--;
            }
            else
            {
                ita++;
                itb++;
            }
        }
        else
        {
            ita++;
            itb++;
        }
    }
    
}

float GeometryTool::max3(float dis,float dis1, float dis2)
{
    if(dis>=dis1 && dis>=dis2)
    {
        return dis;
    }
    if (dis1>=dis && dis1 >=dis2)
    {
        return dis1;
    }
    
    return dis2;
    
}

//将正反两次得到的拐点平均
//pl1是正序得到的拐点序列，pl2是逆序得到的拐点序列。
//处理思路：1.判断逆序中最后的两个拐点是否需要合并；
//          2.以拐点较多的序列为基本，将拐点较少的序列并入。
void GeometryTool::MergeTurning(list<int> pl1,list<int> pl2,list<int>& result)
{
    pl1.sort();
    pl2.sort();
    ////////////////////////////////////////////////////////////////////////
    //1.判断逆序中最后的两个拐点是否需要合并；
    if (pl2.size()>=2)
    {
        list<int>::iterator itend=pl2.end();
        itend--;
        itend--;
        
        list<int>::iterator ittemp1=pl1.begin();
        list<int>::iterator ittemp2=ittemp1;
        int dis=fabs((*ittemp1-*itend)*1.0);
        while(ittemp1 !=pl1.end())
        {
            if (fabs((*ittemp1-*itend)*1.0)<dis)
            {
                ittemp2=ittemp1;
                dis=fabs((*ittemp1-*itend)*1.0);
            }
            ittemp1++;
        }
        
        if(ittemp2==(--pl1.end()))
            pl2.erase(itend);
    }
    
    ////////////////////////////////////////////////////////////////////////
    //2.以拐点较多的序列为基准，将拐点较少的序列并入。
    list<int> a;
    list<int> b;
    if (pl1.size() >= pl2.size())
    {
        a=pl1;
        b=pl2;
    }
    else
    {
        a=pl2;
        b=pl1;
    }
    /////////////
    result.clear();
    list<int>::iterator ita=a.begin();
    
    while(ita !=a.end())
    {
        list<int>::iterator itb=b.begin();
        list<int>::iterator it=itb;
        int dis=fabs((*itb-*ita)*1.0);
        while(itb !=b.end())
        {
            if (fabs((*itb-*ita)*1.0)<dis)
            {
                it=itb;
                dis=fabs((*itb-*ita)*1.0);
            }
            itb++;
        }
        ////////////
        list<int>::iterator itx=a.begin();
        list<int>::iterator itf=itx;
        int disx=fabs((*it - *itx)*1.0);
        while(itx != a.end())
        {
            if(fabs((*it-*itx)*1.0)<disx)
            {
                itf=itx;
                disx=fabs((*it-*itx)*1.0);
            }
            itx++;
        }
        ////////////
        if(itf==ita)  //如果正逆向的所有拐点中，这两个拐点距离是最近的
        {
            int dis_inter=abs(*it-*ita);
            if (ita==a.begin())   //起点
            {
                int dis_self=abs(*ita-*(++a.begin()));
                
                if(dis_inter<dis_self)
                    result.push_back((*it+*ita)/2);
                else
                    result.push_back(*ita);
            }
            else if(ita==(--a.end()))//终点
            {
                result.push_back((*it+*ita)/2);
            }
            else //其他点
            {
                list<int>::iterator itt1=ita;
                itt1--;
                list<int>::iterator itt2=ita;
                itt2++;
                int dis_self1=abs(*ita-*itt1);
                int dis_self2=abs(*ita-*itt2);
                int dis_self=min(dis_self1,dis_self2);
                
                if(dis_inter<dis_self)
                    result.push_back((*it+*ita)/2);
                else
                    result.push_back(*ita);
            }
        }
        else  //如果正逆向的所有拐点中，这两个拐点距离不是最近的
        {
            result.push_back(*ita);
        }
        ita++;
    }
}

float GeometryTool::AngleInDegrees(CCPoint start, CCPoint end, bool positiveOnly)
{
    float radians = AngleInRadians(start, end, positiveOnly);
    return Rad2Deg(radians);
}


float GeometryTool::AngleInRadians(CCPoint start, CCPoint end, bool positiveOnly)
{
    float radians = 0.0;
    if (fabs((start.x - end.x)*1.0)>0.00001)
    {
        radians =atan2(end.y - start.y, end.x - start.x);
    }
    else // pure vertical movement
    {
        if (end.y < start.y)
            radians = -PI / 2.0; // -90 degrees is straight up
        else if (end.y > start.y)
            radians = PI / 2.0; // 90 degrees is straight down
    }
    if (positiveOnly && radians < 0.0)
    {
        radians += PI * 2.0;
    }
    return radians;
}

float GeometryTool::Rad2Deg(float rad)
{
    return (rad * 180.0 / PI);
}


CCPoint GeometryTool::PathMidpoint(list<CCPoint> ptlist)
{
    float templen=PathLength(ptlist)/2.0;
    
    /////////////
    float length = 0;
    list<CCPoint>::iterator it=ptlist.begin();
    list<CCPoint>::iterator it2=ptlist.begin();
    it++;
    
    ///////
    while (it!= ptlist.end())
    {
        length += pointDistance(*it2, *it);
        
        if (length>=templen)
        {
            break;
        }
        else
        {
            it2++;
            it++;
        }
    }
    
    ////////插值
    
    CCPoint pt1=*it2;
    CCPoint pt2=*it;
    float w1=(length-templen)/pointDistance(pt1,pt2);
    if(w1<0.0) w1=0.0;
    if(w1>1.0) w1=1.0;
    float w2=1.0-w1;
    if(w2<0.0) w2=0.0;
    if(w2>1.0) w2=1.0;
    
    //ASSERT(w1>=0.0 && w1<=1.0);
    //ASSERT(w2>=0.0 && w2<=1.0);
    
    float tempx=w1*pt1.x+w2*pt2.x;
    float tempy=w1*pt1.y+w2*pt2.y;
    
    CCPoint result(tempx,tempy);
    return result;
}

float GeometryTool::PathDirectionAngle(list<CCPoint> ptlist)
{
    
    int count=ptlist.size();
    //ASSERT(count>=2);
    
    /////////////////////////
    list<CCPoint>::iterator itbegin=ptlist.begin();
    list<CCPoint>::iterator itend=ptlist.end();
    itend--;
    
    CCPoint start=*itbegin;
    CCPoint end=*itend;
    
    float angle=AngleInDegrees(start,end,true);
    return angle;
}

//从三个方面衡量两个笔段的相似性：长度c2、位置c3、朝向c1。
//将来考虑补充c0:对于直笔段，在单独考虑一个弯曲程度。
float GeometryTool::PathSimilarity(const Segment &spoint, const Segment &mpoint, int normal_size, bool stable, bool dense)
{
    float similarity=0.0;
    
    float slen=spoint.len_;
    float mlen=mpoint.len_;
    
    CCPoint pt1=spoint.mid_point_;
    CCPoint pt2=mpoint.mid_point_;
    
    /////////////////////////////////////////////////////////////////////////////
    /*
     float c0=0.0;
     Segment temps=spoint;
     Segment tempm=mpoint;
     CCPoint pts1=temps.getpoint(0);
     CCPoint pts2=temps.getpoint(temps.getpointlist().size()-1);
     CCPoint ptm1=tempm.getpoint(0);
     CCPoint ptm2=tempm.getpoint(tempm.getpointlist().size()-1);
     float diss=pointDistance(pts1,pts2);
     float dism=pointDistance(ptm1,ptm2);
     
     float len_ratio_s=spoint.len/diss;
     float len_ratio_m=mpoint.len/dism;
     if (len_ratio_m<1.001 &&
     (fabs(0-mpoint.dirangle)<3 || fabs(90-mpoint.dirangle)<3 || fabs(180-mpoint.dirangle)<3 || fabs(270-mpoint.dirangle)<3))
     {
     if (len_ratio_s<1.1)
     c0=0;
     else if(len_ratio_s>1.156)
     c0=100;
     else
     c0=len_ratio_s*len_ratio_s*len_ratio_s*len_ratio_s*len_ratio_s*len_ratio_s;
     }
     */
    /////////////////////////////////////////////////////////////////////////////
    float c2; //长度分量:长笔段变化不会很大，阈值紧些；短笔段的变化可能很大，阈值松些;不稳定笔段长度变化更大，阈值更松。
    //长度分量与笔段数量的疏密程度无关。
    if(stable)
    {
        if (mlen>0.67*normal_size)
        {
            if(mlen/slen>4.2 || slen/mlen>4.2)
                c2=100;
            else if(max(slen,mlen)/min(slen,mlen)<1.6)
                c2=0;
            else
                c2=0.1;
        }
        else
        {
            if(max(slen,mlen)/min(slen,mlen)>4.5)
                c2=100;
            else if(max(slen,mlen)/min(slen,mlen)<2.0)
                c2=0;
            else
                c2=0.1;
        }
    }
    else
    {
        if(slen/mlen>3.5 || mlen/slen>6.8)
            c2=100;
        else if(max(slen,mlen)/min(slen,mlen)<2.3)
            c2=0;
        else
            c2=0.1;
    }
    
    /////////////////////////////////////////////////////////////////////////////
    float c3;//距离分量：总体上阈值松一些，但也有区分：多笔画的汉字变形不会很大，阈值紧一些；少笔画的汉字变形可能很大，阈值松一些。
    //距离分量与笔段数量的疏密程度有关,与笔段稳定性无关。
    if (dense)
    {
        if (pointDistance(pt1,pt2)<=0.23*normal_size)
            c3=0;
        else if(pointDistance(pt1,pt2)<=0.34*normal_size)
            c3=0.005;
        else
            c3=100;
    }
    else
    {
        if (pointDistance(pt1,pt2)<=0.25*normal_size)
            c3=0;
        else if(pointDistance(pt1,pt2)<=0.41*normal_size)
            c3=0.005;
        else
            c3=100;
    }
    
    /////////////////////////////////////////////////////////////////////////////
    if (!stable)//1.如果stable==false，只根据位置、长度进行计算
    {
        //综合
        if(c2>1.0 || c3>1.0)
            similarity=0.0;
        else
            similarity=1.0/(1.0+c2*max(slen,mlen)/min(slen,mlen)+c3*pointDistance(pt1,pt2)*512.0/normal_size);
        
    }
    else//2.如果stable==true，根据角度、位置、长度进行计算
    {
        float angle1=spoint.dirangle_;
        float angle2=mpoint.dirangle_;
        
        float c1;//角度分量
        float anglediff;
        
        float temp1=max(angle2,angle1)-min(angle2,angle1);
        float temp2=min(angle2,angle1)-max(angle2,angle1)+360;
        float temp3=max(max(angle2,angle1)-180,min(angle2,angle1))-min(max(angle2,angle1)-180,min(angle2,angle1));
        anglediff=min(min(temp1,temp2),temp3);
        
        if (fabs(0-angle2)<5 || fabs(90-angle2)<5 || fabs(180-angle2)<5 || fabs(270-angle2)<5)//横竖阈值小一些
        {
            if (anglediff<=20.0)
                c1=0;
            else if(anglediff<=50.0)
                c1=0.05;
            else
                c1=100;
        }
        else //其他阈值大一些
        {
            if (anglediff<=25.0)
                c1=0;
            else if(anglediff<=55.0)
                c1=0.05;
            else
                c1=100;
        }
        
        
        //综合
        if(c1>1.0 || c2>1.0 || c3>1.0)
            similarity=0.0;
        else
            similarity=1.0/(1.0+c1*anglediff+c2*max(slen,mlen)/min(slen,mlen)+c3*pointDistance(pt1,pt2)*512.0/normal_size);
    }
    
    return similarity;
}

//pi,ph属于手写笔段；pj，pk属于模板笔段
float GeometryTool::Evaluate_relation_similarity(const Segment &pi, const Segment &pj, const Segment &ph, const Segment &pk,
                                                  string relation, int normal_size, bool dense)
{
    Segment m_pi=pi;
    Segment m_pj=pj;
    Segment m_ph=ph;
    Segment m_pk=pk;
    
    float prob=0.0;
    
    ////////////////
    if(relation=="I")//判断相交
    {
        //如果直接相交，100%
        //如果端点靠近，90%——75%(区分疏密)
        //否则0%
        CCPoint pt1s=m_pi.GetPoint(0);
        CCPoint pt1e=m_pi.GetPoint(m_pi.GetPointList().size()-1);
        CCPoint pt2s=m_ph.GetPoint(0);
        CCPoint pt2e=m_ph.GetPoint(m_ph.GetPointList().size()-1);
        
        bool inter=judge_intersection(pt1s,pt1e,pt2s,pt2e);
        if (inter==true)//相交
        {
            //计算交点
            CCPoint interp=segment_interpoint(pt1s,pt1e,pt2s,pt2e);
            float ratio1=pointDistance(pt1s,interp)/pointDistance(pt1e,interp);
            float ratio2=pointDistance(pt2s,interp)/pointDistance(pt2e,interp);
            float ratio3=pointDistance(pt1e,interp)/pointDistance(pt1s,interp);
            float ratio4=pointDistance(pt2e,interp)/pointDistance(pt2s,interp);
            float max_ratio=max(max(max(ratio1,ratio2),ratio3),ratio4);
            
            if (max_ratio>8.0)
            {
                prob=0.88;
            }
            else
                prob=1.0;
        }
        else//不相交，可能相离、共线、重叠、相接
        {
            float dis1=pointToSegment(pt1s,pt1e,pt2s);
            float dis2=pointToSegment(pt1s,pt1e,pt2e);
            float dis3=pointToSegment(pt2s,pt2e,pt1s);
            float dis4=pointToSegment(pt2s,pt2e,pt1e);
            
            float mindis=min(min(min(dis1,dis2),dis3),dis4);
            
            if (dense)
            {
                if(mindis<=normal_size/25.0)
                {
                    prob=(mindis)/(normal_size/25.0)*(0.65-0.85)+0.85;
                }
            }
            else
            {
                if(mindis<=normal_size/21.0)
                {
                    prob=(mindis)/(normal_size/21.0)*(0.65-0.85)+0.85;
                }
            }
        }
    }
    
    ////////////////
    if(relation=="A")//判断相离
    {
        //判断是否相交
        //不相交，100%
        //交点距离二者中心较远90%
        //交点距离二者中心较近75%
        CCPoint pt1s=m_pi.GetPoint(0);
        CCPoint pt1e=m_pi.GetPoint(m_pi.GetPointList().size()-1);
        CCPoint pt2s=m_ph.GetPoint(0);
        CCPoint pt2e=m_ph.GetPoint(m_ph.GetPointList().size()-1);
        
        bool inter=judge_intersection(pt1s,pt1e,pt2s,pt2e);
        if (inter==false)//不相交，可能相离、共线、重叠、相接
        {
            prob=1.0;
        }
        else//相交
        {
            //计算交点
            CCPoint interp=segment_interpoint(pt1s,pt1e,pt2s,pt2e);
            float ratio1=pointDistance(pt1s,interp)/pointDistance(pt1e,interp);
            float ratio2=pointDistance(pt2s,interp)/pointDistance(pt2e,interp);
            float ratio3=pointDistance(pt1e,interp)/pointDistance(pt1s,interp);
            float ratio4=pointDistance(pt2e,interp)/pointDistance(pt2s,interp);
            float max_ratio=max(max(max(ratio1,ratio2),ratio3),ratio4);
            
            if (max_ratio>6.0)
            {
                prob=1.0;
            }
            else if(max_ratio>2.0)
            {
                prob=(max_ratio-2.0)/(6-2.0)*(0.9-0.75)+0.75;
            }
        }
    }
    
    ////////////////
    if(relation[0]=='C')//判断相接
    {
        //根据疏密判断距离100%,90%—65%,0.
        float dis=0.0;
        if(relation[1]=='B' && relation[2]=='B')
        {
            CCPoint pt1;
            CCPoint pt2;
            float ang=max(m_pi.dirangle_,m_pj.dirangle_)-min(m_pi.dirangle_,m_pj.dirangle_);
            if(ang>=90 && ang<=270)
                pt1=m_pi.GetPoint(m_pi.GetPointList().size()-1);
            else
                pt1=m_pi.GetPoint(0);
            
            ang=max(m_ph.dirangle_,m_pk.dirangle_)-min(m_ph.dirangle_,m_pk.dirangle_);
            if(ang>=90 && ang<=270)
                pt2=m_ph.GetPoint(m_ph.GetPointList().size()-1);
            else
                pt2=m_ph.GetPoint(0);
            
            dis=pointDistance(pt1,pt2);
        }
        
        if(relation[1]=='B' && relation[2]=='E')
        {
            CCPoint pt1;
            CCPoint pt2;
            float ang=max(m_pi.dirangle_,m_pj.dirangle_)-min(m_pi.dirangle_,m_pj.dirangle_);
            if(ang>=90 && ang<=270)
                pt1=m_pi.GetPoint(m_pi.GetPointList().size()-1);
            else
                pt1=m_pi.GetPoint(0);
            
            ang=max(m_ph.dirangle_,m_pk.dirangle_)-min(m_ph.dirangle_,m_pk.dirangle_);
            if(ang>=90 && ang<=270)
                pt2=m_ph.GetPoint(0);
            else
                pt2=m_ph.GetPoint(m_ph.GetPointList().size()-1);
            
            dis=pointDistance(pt1,pt2);
        }
        
        if(relation[1]=='B' && relation[2]=='N')
        {
            CCPoint pt1;
            CCPoint pt2;
            CCPoint pt3;
            float ang=max(m_pi.dirangle_,m_pj.dirangle_)-min(m_pi.dirangle_,m_pj.dirangle_);
            if(ang>=90 && ang<=270)
                pt1=m_pi.GetPoint(m_pi.GetPointList().size()-1);
            else
                pt1=m_pi.GetPoint(0);
            
            pt2=m_ph.GetPoint(0);
            pt3=m_ph.GetPoint(m_ph.GetPointList().size()-1);
            
            dis=pointToSegment(pt2,pt3,pt1);
        }
        if(relation[1]=='E' && relation[2]=='B')
        {
            CCPoint pt1;
            CCPoint pt2;
            float ang=max(m_pi.dirangle_,m_pj.dirangle_)-min(m_pi.dirangle_,m_pj.dirangle_);
            if(ang>=90 && ang<=270)
                pt1=m_pi.GetPoint(0);
            else
                pt1=m_pi.GetPoint(m_pi.GetPointList().size()-1);
            
            ang=max(m_ph.dirangle_,m_pk.dirangle_)-min(m_ph.dirangle_,m_pk.dirangle_);
            if(ang>=90 && ang<=270)
                pt2=m_ph.GetPoint(m_ph.GetPointList().size()-1);
            else
                pt2=m_ph.GetPoint(0);
            
            dis=pointDistance(pt1,pt2);
            
        }		
        if(relation[1]=='E' && relation[2]=='E')
        {
            CCPoint pt1;
            CCPoint pt2;
            float ang=max(m_pi.dirangle_,m_pj.dirangle_)-min(m_pi.dirangle_,m_pj.dirangle_);
            if(ang>=90 && ang<=270)
                pt1=m_pi.GetPoint(0);
            else
                pt1=m_pi.GetPoint(m_pi.GetPointList().size()-1);
            
            ang=max(m_ph.dirangle_,m_pk.dirangle_)-min(m_ph.dirangle_,m_pk.dirangle_);
            if(ang>=90 && ang<=270)
                pt2=m_ph.GetPoint(0);
            else
                pt2=m_ph.GetPoint(m_ph.GetPointList().size()-1);
            
            dis=pointDistance(pt1,pt2);
        }		
        if(relation[1]=='E' && relation[2]=='N')
        {
            CCPoint pt1;
            CCPoint pt2;
            CCPoint pt3;
            float ang=max(m_pi.dirangle_,m_pj.dirangle_)-min(m_pi.dirangle_,m_pj.dirangle_);
            if(ang>=90 && ang<=270)
                pt1=m_pi.GetPoint(0);
            else
                pt1=m_pi.GetPoint(m_pi.GetPointList().size()-1);
            
            pt2=m_ph.GetPoint(0);
            pt3=m_ph.GetPoint(m_ph.GetPointList().size()-1);
            
            dis=pointToSegment(pt2,pt3,pt1);
        }
        
        if(relation[1]=='N' && relation[2]=='B')
        {
            CCPoint pt1;
            CCPoint pt2;
            CCPoint pt3;
            pt1=m_pi.GetPoint(0);
            pt2=m_pi.GetPoint(m_pi.GetPointList().size()-1);
            
            float ang=max(m_ph.dirangle_,m_pk.dirangle_)-min(m_ph.dirangle_,m_pk.dirangle_);
            if(ang>=90 && ang<=270)
                pt3=m_ph.GetPoint(m_ph.GetPointList().size()-1);
            else
                pt3=m_ph.GetPoint(0);
            
            dis=pointToSegment(pt1,pt2,pt3);
        }		
        if(relation[1]=='N' && relation[2]=='E')
        {
            CCPoint pt1;
            CCPoint pt2;
            CCPoint pt3;
            pt1=m_pi.GetPoint(0);
            pt2=m_pi.GetPoint(m_pi.GetPointList().size()-1);
            
            float ang=max(m_ph.dirangle_,m_pk.dirangle_)-min(m_ph.dirangle_,m_pk.dirangle_);
            if(ang>=90 && ang<=270)
                pt3=m_ph.GetPoint(0);
            else
                pt3=m_ph.GetPoint(m_ph.GetPointList().size()-1);
            
            dis=pointToSegment(pt1,pt2,pt3);
        }
        
        /////////////////
        if(relation[1]!='N' && relation[2]!='N')
        {
            if (dis<0.018*normal_size)
                prob=1.0;
            else if(dis<0.2*normal_size)
                prob=(dis-0.018*normal_size)/(0.2*normal_size-0.018*normal_size)*(0.53-0.9)+0.9;
        }
        else if(dense)
        {
            if (dis<0.05*normal_size)
                prob=1.0;
            else if(dis<0.25*normal_size)
                prob=(dis-0.05*normal_size)/(0.25*normal_size-0.05*normal_size)*(0.53-0.9)+0.9;
        }
        else
        {
            if (dis<0.1*normal_size)
                prob=1.0;
            else if(dis<0.3*normal_size)
                prob=(dis-0.1*normal_size)/(0.3*normal_size-0.1*normal_size)*(0.6-0.9)+0.9;
        }
    }
    
    /////
    return prob;
}

//计算两个线段的交点，前提条件：判断是否相交。
CCPoint GeometryTool::segment_interpoint(const CCPoint pt1s, const CCPoint pt1e, const CCPoint pt2s, const CCPoint pt2e)
{
    //ASSERT(judge_intersection(pt1s,pt1e,pt2s,pt2e));
    ////
    double f1=fabs((pt1e.x-pt1s.x)*(pt2s.y-pt1s.y) - (pt1e.y-pt1s.y)*(pt2s.x-pt1s.x));
    double f2=fabs((pt1e.x-pt1s.x)*(pt2e.y-pt1s.y) - (pt1e.y-pt1s.y)*(pt2e.x-pt1s.x));	
    ///
    double k = f1 / f2;  
    return CCPoint((pt2s.x + k*pt2e.x)/(1+k),(pt2s.y + k*pt2e.y)/(1+k));
}


//pi,ph属于手写笔段；pj，pk属于模板笔段
float GeometryTool::computecompatibility(const Segment &pi, const Segment &pj, const Segment &ph, const Segment &pk, bool samecompnent, bool instable, string relation, int normal_size, bool dense)
{
    ////////////
    //首先判断交搭关系是否可行，如果不可行直接返回。
    //如果可行，给出可行概率。利用这个概率对最终反馈结果进行微调。	
    //思路：
    //——相离：100%,90%—75%,0；
    //——相交：100%,90%—75%,0；
    //——相接：100%,90%—65%,0；
    float c_rel=0;
    c_rel=Evaluate_relation_similarity(pi, pj, ph, pk, relation, normal_size, dense);
    
    if (c_rel<0.1)
    {
        return -1.0;
    }
    
    //////////////////////////////////////////////////////////////////////
    //pi,ph属于手写笔段；pj，pk属于模板笔段
    float c_dis=0; //距离：全部起作用(这个参数不是很重要，放松一些)
    float c_scale=0;  //比例：在samecompnent=true时起作用(这个参数不是很重要，放松一些)
    float c_dir=0; //方向：相接相离时起作用。(同一部件紧一些，不同部件松一些)
    float c_angle=0; //夹角：instable=false时起作用。(平行紧一些，其他松一些)
    /////////////////////////////////////////////////////////////////////
    
    //1.计算距离
    float diff_dis;
    CCPoint pt1=pi.mid_point_;
    CCPoint pt2=ph.mid_point_;
    float dis1=pointDistance(pt1,pt2);
    
    pt1=pj.mid_point_;
    pt2=pk.mid_point_;
    float dis2=pointDistance(pt1,pt2);
    
    diff_dis=fabs(dis1-dis2);
    
    if (diff_dis<=0.26*normal_size)
        c_dis=0;
    else if(diff_dis<=0.52*normal_size)
    {
        diff_dis-=0.26*normal_size;
        c_dis=0.006;
    }
    else 
        c_dis=100;	
    //////////////////////////////////////////////////////////////////////////
    if(c_dis>1.0)
        return -1.0;
    //////////////////////////////////////////////////////////////////////////
    
    //2.计算比例
    float diff_scall=0.0;
    if (samecompnent==true)
    {
        float len1=pi.len_;
        float len2=ph.len_;
        float len3=pj.len_;
        float len4=pk.len_;
        
        if (len1>=len2)
            diff_scall=(len1/len2)/(len3/len4);
        else
            diff_scall=(len2/len1)/(len4/len3);
        
        if(diff_scall<1.0)
            diff_scall=1.0/diff_scall;
        
        if(instable)
        {
            if(diff_scall<=4.4 )
                c_scale=0;
            else if(diff_scall<=7.8)
            {
                diff_scall-=4.4;
                c_scale=0.1;
            }
            else
                c_scale=100;
        }
        else
        {
            if(diff_scall<=2.9 )
                c_scale=0;
            else if(diff_scall<=5.0)
            {
                diff_scall-=2.9;
                c_scale=0.1;
            }
            else
                c_scale=100;
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    if(c_scale>1.0)
        return -1.0;
    //////////////////////////////////////////////////////////////////////////
    
    //3.计算方向
    float diff_dir=0.0;
    if(relation != R_INTERSECT)
    {
        CCPoint pti=pi.mid_point_;
        CCPoint pth=ph.mid_point_;
        CCPoint ptj=pj.mid_point_;
        CCPoint ptk=pk.mid_point_;
        
        float angle1=AngleInDegrees(pti,pth,true);
        float angle2=AngleInDegrees(ptj,ptk,true);
        
        float maxang=max(angle1,angle2);
        float minang=min(angle1,angle2);
        
        diff_dir=maxang-minang;
        if (diff_dir>180.0)
            diff_dir=360-diff_dir;
        
        if(samecompnent)
        {
            if(fabs(angle2-0.0)<3.0 || fabs(angle2-90.0)<3.0 || fabs(angle2-180.0)<3.0 || fabs(angle2-270.0)<3.0)
            {
                if(diff_dir<18)
                    c_dir=0;
                else if(diff_dir<47)
                {
                    diff_dir-=18;
                    c_dir=0.02;
                }
                else
                    c_dir=100;
            }
            else
            {		
                if(diff_dir<25)
                    c_dir=0;
                else if(diff_dir<55)
                {
                    diff_dir-=25;
                    c_dir=0.016;
                }
                else
                    c_dir=100;
            }
        }
        else
        {				
            if(diff_dir<35)
                c_dir=0;
            else if(diff_dir<60)
            {
                diff_dir-=35;
                c_dir=0.014;
            }
            else
                c_dir=100;
        }
        
    }
    
    //////////////////////////////////////////////////////////////////////////
    if(c_dir>1.0)
        return -1.0;
    //////////////////////////////////////////////////////////////////////////
    
    //4.计算夹角差异
    float diff_angle=0;
    if(!instable)
    {
        //1.判断j是否笔向有问题
        float pi_dir=pi.dirangle_;
        float pj_dir=pj.dirangle_;
        
        if(pi_dir>=315 && pj_dir<=45)
            pi_dir=pi_dir-360;
        if(pj_dir>=315 && pi_dir<=45)
            pj_dir=pj_dir-360;
        
        if(max(pi_dir,pj_dir)-min(pi_dir,pj_dir)>90)
            pj_dir=fabs(pj_dir+180);
        
        if(pj_dir>=360)
            pj_dir=pj_dir-360;
        
        if(pi_dir>=315 && pj_dir<=45)
            pi_dir=pi_dir-360;
        if(pj_dir>=315 && pi_dir<=45)
            pj_dir=pj_dir-360;
        //2.判断k是否笔向有问题
        float ph_dir=ph.dirangle_;
        float pk_dir=pk.dirangle_;
        
        if(ph_dir>=315 && pk_dir<=45)
            ph_dir=ph_dir-360;
        if(pk_dir>=315 && ph_dir<=45)
            pk_dir=pk_dir-360;
        
        if(max(ph_dir,pk_dir)-min(ph_dir,pk_dir)>90)
            pk_dir=pk_dir+180;
        
        if(pk_dir>=360)
            pk_dir=pk_dir-360;
        
        if(ph_dir>=315 && pk_dir<=45)
            ph_dir=ph_dir-360;
        if(pk_dir>=315 && ph_dir<=45)
            pk_dir=pk_dir-360;
        
        //3.计算角度差异
        float s_angle=ph_dir-pi_dir;
        float m_angle=pk_dir-pj_dir;
        diff_angle=fabs(s_angle-m_angle);
        
        //4.计算权重
        if(fabs(m_angle)<=3.0)
        {
            if(diff_angle<25)
                c_angle=0.0;
            else if (diff_angle<65)
            {
                diff_angle-=25;
                c_angle=0.013;
            }
            else 
                c_angle=100;
        }
        else
        {		
            if(diff_angle<40)
                c_angle=0.0;
            else if (diff_angle<90)
            {
                diff_angle-=40;
                c_angle=0.011;
            }
            else 
                c_angle=100;		
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    if(c_angle>1.0)
        return -1.0;
    //////////////////////////////////////////////////////////////////////////
    
    //汇总
    float result=-1.0+2.0/(1+c_dis*diff_dis*512.0/normal_size+c_scale*diff_scall+c_dir*diff_dir+c_angle*diff_angle);
    
    //补偿
    if (result>=0)
    {
        result=result*c_rel;
    } 
    else
    {
        result=result+result*(1-c_rel);
        if(result<-1.0)
            result=-1.0;
    }
    
    return result;
}