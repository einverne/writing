//
//  TemplateCharacter.cpp
//  game
//
//  Created by Ein Verne on 16/3/16.
//
//

#include "TemplateCharacter.hpp"
#include <string>
#include <list>
#include <algorithm>

TemplateCharacter::TemplateCharacter()
{
    stroke_list_.clear();
    segment_list_.clear();
    
    instable_segment_list_.clear();
    segment_relation_.clear();
    
    m_components_.clear();
    m_struct_name_ = "nostruct";
    
    ///////////
    draw_index_=-1;
    draw_point_=-1;
    
    normal_size_=-1;
}

TemplateCharacter::TemplateCharacter(const TemplateCharacter& T)//拷贝
{
    this->draw_index_=-1;
    this->draw_point_=-1;
    this->normal_size_=T.normal_size_;
    
    ////
    this->stroke_list_=T.stroke_list_;
    this->segment_list_=T.segment_list_;
    this->instable_segment_list_=T.instable_segment_list_;
    
    ///
    this->segment_relation_.clear();
    this->segment_relation_=T.segment_relation_;
    
    ///
    this->m_components_=T.m_components_;
    this->m_struct_name_=T.m_struct_name_;
}


TemplateCharacter& TemplateCharacter::operator=(const TemplateCharacter& Right)    //赋值
{
    if(this!=&Right)
    {
        this->draw_index_=-1;
        this->draw_point_=-1;
        this->normal_size_=Right.normal_size_;
        ////
        this->stroke_list_=Right.stroke_list_;
        this->segment_list_=Right.segment_list_;
        this->instable_segment_list_=Right.instable_segment_list_;
        ///
        this->segment_relation_.clear();
        this->segment_relation_=Right.segment_relation_;
        this->m_components_=Right.m_components_;
        this->m_struct_name_=Right.m_struct_name_;
    }
    return (*this);
}

TemplateCharacter::~TemplateCharacter()
{
    stroke_list_.clear();
    segment_list_.clear();
    
    instable_segment_list_.clear();
    segment_relation_.clear();
    m_components_.clear();
}

void TemplateCharacter::clearalldata()
{
    stroke_list_.clear();
    segment_list_.clear();
    
    instable_segment_list_.clear();
    segment_relation_.clear();
    m_components_.clear();
//    m_struct_name.Format("nostruct");
    
    ///////////
    draw_index_=-1;
    draw_point_=-1;
    
    normal_size_=-1;
}

void TemplateCharacter::AppendStroke(string name, list<Segment> s)//添加一个笔画
{
    Stroke temp_stroke;
    temp_stroke.SetName(name);
    temp_stroke.SetSegCount(s.size());
    if(this->stroke_list_.size()>0)
    {
        list<Stroke>::iterator it;
        it = this->stroke_list_.end();
        --it;
        temp_stroke.setStartIndex((*it).start_index_+(*it).seg_count_);
    }
    else
    {
        temp_stroke.setStartIndex(0);
    }
    
    stroke_list_.push_back(temp_stroke);
    
    ///////
    list<Segment>::iterator it;
    for (it=s.begin(); it != s.end();it++)
    {
        this->segment_list_.push_back((*it));
    }
    ////////
    
}

void TemplateCharacter::AddInstableSegment(int ind)//标记一个不稳定笔段
{
    list<int>::iterator it;
    it = find (this->instable_segment_list_.begin(), this->instable_segment_list_.end(), ind);
    if(it==this->instable_segment_list_.end())
    {
        this->instable_segment_list_.push_back(ind);
    }
}

void TemplateCharacter::InitSegmentRelation()
{
    segment_relation_.clear();
    segment_relation_.Init(this->segment_list_.size(),this->segment_list_.size());
}


void TemplateCharacter::SetRelation(int first,int second, string r)//标记笔段之间的关系
{
    segment_relation_.SetElement(first, second, r);
}

string  TemplateCharacter::GetRelation(int first,int second)
{
    string res;
    res=segment_relation_.GetElement(first, second);
    return res;
}

Stroke TemplateCharacter::getstroke(int num)
{
    //ASSERT(num>=0);
    //ASSERT(num<stroke_list.size());
    
    ////////////
    list<Stroke>::iterator it=this->stroke_list_.begin();
    for (int i=0;i<num;i++)
    {
        it++;
    }
    
    return *it;
}

void TemplateCharacter::Add_one_component_setting(CComponent m)
{
    this->m_components_.push_back(m);
}

bool TemplateCharacter::IsShareOnecomponent(int segment1, int segment2)
{
    bool yes=false;
    //////
    list<CComponent>::iterator itt=m_components_.begin();
    while(itt!=m_components_.end())
    {
        list<int> temp=(*itt).segments_list;
        list<int>::iterator it1;
        list<int>::iterator it2;
        it1 = find (temp.begin(), temp.end(), segment1);
        it2 = find (temp.begin(), temp.end(), segment2);
        if (it1!=temp.end() && it2!=temp.end())
        {
            yes=true;
            break;
        }
        itt++;
    }
    //////
    return yes;
}

void TemplateCharacter::Normalize(int height,int width) //尺寸的缩放，路径的均匀插值。
{
    if (this->segment_list_.size()==0)
    {
        return;
    }
    
    /////////////
    float xmin, ymin, xmax,ymax;
    
    Segment temps=getsegment(0);
    CCPoint tempp=temps.GetPoint(0);
    
    xmin=xmax=tempp.x;
    ymin=ymax=tempp.y;
    
    for (int i=0; i<this->segment_list_.size();i++)
    {
        temps=getsegment(i);
        for (int j=0;j<temps.point_list_.size();j++)
        {
            tempp=temps.GetPoint(j);
            
            if (xmin>tempp.x)
                xmin=tempp.x;
            if (xmax<tempp.x)
                xmax=tempp.x;
            if (ymin>tempp.y)
                ymin=tempp.y;
            if (ymax<tempp.y)
                ymax=tempp.y;
        }
    }
    
    float midx=(xmax+xmin)/2;
    float midy=(ymax+ymin)/2;
    
    float rescal1=width/(xmax-xmin);
    float rescal2=height/(ymax-ymin);
    
    ////////////
    if(rescal1>=rescal2)  //以竖直缩放为标准
    {
        normal_size_=height;
        for (int i=0; i<this->segment_list_.size();i++)
        {
            temps=getsegment(i);
            for (int j=0;j<temps.point_list_.size();j++)
            {
                tempp=temps.GetPoint(j);
                
                float tempx=(tempp.x-midx)*rescal2+width/2;
                float tempy=(tempp.y-midy)*rescal2+height/2;
                
                temps.SetPoint(j,CCPoint(tempx,tempy));
            }
            ReplaceSegment(i,temps);
        }
    }
    else  //以水平缩放为标准
    {
        normal_size_=width;
        for (int i=0; i<this->segment_list_.size();i++)
        {
            temps=getsegment(i);
            for (int j=0;j<temps.point_list_.size();j++)
            {
                tempp=temps.GetPoint(j);
                
                float tempx=(tempp.x-midx)*rescal1+width/2;
                float tempy=(tempp.y-midy)*rescal1+height/2;
                
                temps.SetPoint(j,CCPoint(tempx,tempy));
            }
            ReplaceSegment(i,temps);
        }
    }
    
    /////路径的均匀插值//////
    GeometryTool gt;
    
    for (int i=0; i<this->segment_list_.size();i++)
    {
        temps=getsegment(i);
        list<CCPoint> templ=gt.UniformpathResample(temps.GetPointList());
        temps.SetPointList(templ);
        temps.UpdateAttribute();   //更新属性；
        ReplaceSegment(i,temps);
    }
    
}

void TemplateCharacter::ReplaceSegment(int ind, Segment seg)
{
    //ASSERT(ind>=0);
    //ASSERT(ind<this->segment_list.size());
    
    list<Segment>::iterator it = this->segment_list_.begin();
    
    for (int x=0;x<ind;x++)
    {
        it++;
    }
    
    *it=seg;
}

bool TemplateCharacter::isinstablesegment(int num)
{
    //ASSERT(num>=0);
    //ASSERT(num<this->segment_list.size());
    
    bool yes=false;
    list<int>::iterator it = this->instable_segment_list_.begin();
    while (it!=this->instable_segment_list_.end())
    {
        if(*it==num)
        {
            yes=true;
            break;
        }
        else
        {
            it++;
        }
    }
    
    return yes;
}

list<int>  TemplateCharacter::find_intersect_relation_segment(int segindex)
{
    list<int> m_res;
    ////////////////////////
    for (int x=0; x<this->segment_list_.size();x++)
    {
        if (x!=segindex)
        {
            string r=this->segment_relation_.GetElement(segindex,x);
            if (r==R_INTERSECT)
            {
                m_res.push_back(x);
            }
            else if(r[0]=='C' && r[1]=='N' && r[2]!='N')
            {
                m_res.push_back(x);
            }
        }
    }
    ////////////////////////
    return m_res;
}

int TemplateCharacter::find_noturning_connecttion_segment(int segindex)
{
    int m_res=-1;
    
    GeometryTool gt;
    Segment s1=getsegment(segindex);
    //同方向
    for(int x=0; x<this->segment_list_.size(); x++)
    {
        if(x!=segindex)
        {
            Segment s2=getsegment(x);
            ////////////////////////////////
            CCPoint p1=s1.GetPoint(0);
            CCPoint p2=s1.GetPoint(1);
            CCPoint p3=s2.GetPoint(s2.point_list_.size()-1);
            CCPoint p4=s2.GetPoint(s2.point_list_.size()-2);
            float ang1=gt.AngleInDegrees(p1,p2,true);
            float ang2=gt.AngleInDegrees(p3,p4,true);
            
            if(fabs(180-fabs(ang2-ang1))<10 && gt.pointDistance(p1,p3)<=normal_size_/34.0)
            {
                m_res=x;
                break;
            }
            
            ////////////////////////////////
            p1=s2.GetPoint(0);
            p2=s2.GetPoint(1);
            p3=s1.GetPoint(s1.point_list_.size()-1);
            p4=s1.GetPoint(s1.point_list_.size()-2);
            ang1=gt.AngleInDegrees(p1,p2,true);
            ang2=gt.AngleInDegrees(p3,p4,true);
            
            if(fabs(180-fabs(ang2-ang1))<10 && gt.pointDistance(p1,p3)<=normal_size_/34.0)
            {
                m_res=x;
                break;
            }
        }
    }
    return m_res;
}

int TemplateCharacter::find_common_connect_relation_segment(int k1, int k2)
{
    int m_res=-1;
    for (int x=0; x<this->segment_list_.size();x++)
    {
        if (x!=k1 && x!=k2)
        {
            string r1=this->segment_relation_.GetElement(k1,x);
            string r2=this->segment_relation_.GetElement(k2,x);
            if (r1[0]=='C' && r2[0]=='C')
            {
                m_res=x;
                break;
            }
        }
    }
    ////////////////////////
    return m_res;
}

list<int> TemplateCharacter::find_first_apart_second_notapart_relation_segment(int k1, int k2)
{
    list<int> m_res;
    ////////////////////////
    for (int x=0; x<this->segment_list_.size();x++)
    {
        if (x!=k1 && x!=k2)
        {
            if (this->segment_relation_.GetElement(k1,x)==R_APART && this->segment_relation_.GetElement(k2,x)!=R_APART)
            {
                m_res.push_back(x);
            }
        }
    }
    ////////////////////////
    return m_res;
}

void TemplateCharacter::clearSegmentColor(ccColor4F color)
{
   list<Segment>::iterator it = this->segment_list_.begin();
   
   while (it!=this->segment_list_.end())
   {
       it->color_=color;
       it++;
   }
}

void TemplateCharacter::SetSegmentColor(int num, ccColor4F color)
{
   ///////////////////////////////////////
   list<Segment>::iterator it=this->segment_list_.begin();
   
   for (int i=0;i<num;i++)
   {
       it++;
   }
   
   it->color_=color;
}

Segment TemplateCharacter::getsegment(int num)
{
    //ASSERT(num>=0);
    //ASSERT(num<this->segment_list.size());
    
    list<Segment>::iterator it = this->segment_list_.begin();
    
    for (int x=0;x<num;x++)
    {
        it++;
    }
    
    return *it;
}

//void TemplateCharacter::draw(CDC* pDC)
//{
//    CPen *pen, *pOldPen;
//    Segment temps;
//    CCPoint tempp(0,0);
//    
//    CBrush   Brush;
//    Brush.CreateSolidBrush(RGB(255,0,0));
//    CBrush* oldbrush=pDC->SelectObject(&Brush);
//    
//    
//    //////////
//    if (this->segment_list.size()==0)
//    {
//        pDC->SelectObject(oldbrush);
//        Brush.DeleteObject();
//        return;
//    }
//    
//    //////////
//    if(draw_index<0   || draw_point<0)
//    {
//        for (int i=0; i<this->segment_list.size();i++)
//        {
//            temps=getsegment(i);
//            tempp=temps.getpoint(0);
//            pDC->BeginPath();
//            pDC->Ellipse(tempp.getx()-6,tempp.gety()-6,tempp.getx()+6,tempp.gety()+6);
//            pDC->EndPath();
//            pDC->FillPath();
//            
//            pen=new CPen(PS_SOLID, 4, temps.color);
//            pOldPen=pDC->SelectObject(pen);
//            
//            ///
//            string numtag;
//            numtag.Format(_T("%d"),i);
//            pDC->TextOut(tempp.getx()-10,tempp.gety()-15,numtag);
//            
//            //////
//            pDC->MoveTo(tempp.getx(),tempp.gety());
//            for (int j=1;j<temps.plist.size();j++)
//            {
//                tempp=temps.getpoint(j);
//                pDC->LineTo(tempp.getx(),tempp.gety());
//            }
//            pDC->SelectObject(pOldPen);
//            delete pen;
//        }
//    }
//    else if(draw_index>=0 && draw_point>=0)
//    {
//        //动画演示
//        for (int i=0; i<draw_index;i++)
//        {
//            temps=getsegment(i);
//            tempp=temps.getpoint(0);
//            
//            pen=new CPen(PS_SOLID, 4, temps.color);
//            pOldPen=pDC->SelectObject(pen);
//            
//            //////			
//            pDC->MoveTo(tempp.getx(),tempp.gety());
//            for (int j=1;j<temps.plist.size();j++)
//            {
//                tempp=temps.getpoint(j);
//                pDC->LineTo(tempp.getx(),tempp.gety());
//            }
//            pDC->SelectObject(pOldPen);
//            delete pen;
//        }
//        //最后一笔
//        temps=getsegment(draw_index);
//        tempp=temps.getpoint(0);
//        
//        pen=new CPen(PS_SOLID, 4, temps.color);
//        pOldPen=pDC->SelectObject(pen);
//        
//        //////			
//        pDC->MoveTo(tempp.getx(),tempp.gety());
//        for (int j=1;j<=draw_point;j++)
//        {
//            tempp=temps.getpoint(j);
//            pDC->LineTo(tempp.getx(),tempp.gety());
//        }
//        pDC->SelectObject(pOldPen);
//        delete pen;
//    }	
//    
//    pDC->SelectObject(oldbrush);
//    Brush.DeleteObject();
//}

bool TemplateCharacter::InsameStroke(int segindex1,int segindex2)
{
    bool yes=false;
    list<Stroke>::iterator it=this->stroke_list_.begin();
    for (;it!=this->stroke_list_.end();it++)
    {
        if(segindex1>=it->start_index_ && segindex1<it->start_index_+it->seg_count_ &&
           segindex2>=it->start_index_ && segindex2<it->start_index_+it->seg_count_)
        {
            yes=true;
            break;
        }
    }
    
    return yes;
}

bool TemplateCharacter::Is_Last_segment_of_OneStroke(int segindex)
{
    bool yes=false;
    list<Stroke>::iterator it=this->stroke_list_.begin();
    for(;it!=this->stroke_list_.end();it++)
    {
        if(segindex == it->start_index_+it->seg_count_-1)
        {
            yes=true;
            break;
        }
    }
    
    return yes;
}

void TemplateCharacter::AnimationProgress(bool ani)
{
    if (this->segment_list_.size()==0)
    {
        return;
    }
    
    if(!ani)
    {
        draw_index_=-1;
        draw_point_=-1;
        return;
    }
    
    /////////////////////////////////////
    //开始
    if(draw_index_<0 || draw_point_<0)
    {
        draw_index_=0;
        draw_point_=0;
        return;
    }
    
    //结束
    int maxseg=segment_list_.size();
    
    list<Segment>::iterator it = this->segment_list_.end();
    it--;
    int maxpt=(*it).point_list_.size();
    
    if (draw_index_==maxseg-1 && draw_point_==maxpt-1)
    {
        draw_index_=-1;
        draw_point_=-1;
        return;
    }
    
    //进展
    Segment temps=getsegment(draw_index_);
    if(draw_point_ < temps.point_list_.size()-1)
    {
        draw_point_++;
    }
    else if(draw_point_ == temps.point_list_.size()-1)
    {
        draw_index_++;
        draw_point_=0;
    }	
}

void TemplateCharacter::TransferCoordinate(int size){
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

void TemplateCharacter::TransferCoordinateToFour(int size) {
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