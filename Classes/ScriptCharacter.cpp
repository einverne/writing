//
//  ScriptCharacter.cpp
//  手写汉字表示
//
//  Created by Ein Verne on 16/3/16.
//
//

#include "ScriptCharacter.hpp"
#include "GeometryTool.hpp"

ScriptCharacter::ScriptCharacter(){
    row_stroke_.clear();
    divide_ = false;

	width_ = 0;
	height_ = 0;
	center_point_ = ccp(0,0);
    
    stroke_list_.clear();
    segment_list_.clear();
    noise_list_.clear();
}

ScriptCharacter::ScriptCharacter(const ScriptCharacter& T){
    this->row_stroke_ = T.row_stroke_;
    this->divide_ = T.divide_;

	this->width_ = T.width_;
	this->height_ = T.height_;
	this->center_point_ = T.center_point_;

    this->stroke_list_ = T.stroke_list_;
    this->segment_list_ = T.segment_list_;
    this->noise_list_ = T.noise_list_;
}

ScriptCharacter& ScriptCharacter::operator=(const ScriptCharacter& T){
    this->row_stroke_ = T.row_stroke_;
    this->divide_ = T.divide_;

	this->width_ = T.width_;
	this->height_ = T.height_;
	this->center_point_ = T.center_point_;

    this->stroke_list_ = T.stroke_list_;
    this->segment_list_ = T.segment_list_;
    this->noise_list_ = T.noise_list_;
    
    return *this;
}

ScriptCharacter::~ScriptCharacter(){
    row_stroke_.clear();
    stroke_list_.clear();
    segment_list_.clear();
    noise_list_.clear();

}


void ScriptCharacter::ClearDivideData()
{
	stroke_list_.clear();   
	segment_list_.clear(); 	
	noise_list_.clear(); 
	
	//////////////////
	this->draw_index=-1;  
	this->draw_point=-1;
	divide_=false;
}

void ScriptCharacter::ClearAllData()
{
	row_stroke_.clear();   
	divide_=false;

	width_ = 0;
	height_ = 0;
	center_point_ = ccp(0,0);
	
	//下面的属性是分割后产生的
	stroke_list_.clear();   
	segment_list_.clear();	
	noise_list_.clear();  
	
	//下面的属性用于动画演示
	draw_index=-1;  
	draw_point=-1;
	
	normal_size_=-1;
}

void ScriptCharacter::RemoveFromNoistlist(int segindex)
{
	list<int>::iterator it=this->noise_list_.begin();
	for (;it!=this->noise_list_.end();it++)
	{
		if(*it== segindex)
		{
			this->noise_list_.erase(it);
			break;
		}
	}
}

bool ScriptCharacter::InsameStroke(int segindex1,int segindex2)
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

void ScriptCharacter::AnimationProgress(bool ani)
{
	if(this->row_stroke_.size()==0)
		return;
	////////////////////////////////
	if(!ani)
	{
		draw_index=-1;
		draw_point=-1;
		return;
	}
	///////////////////////////////
	//开始
	if(draw_index<0 || draw_point<0)
	{
		draw_index=0;
		draw_point=0;
		return;
	}
	//进展和结束
	if (divide_==false)
	{
		//结束
		int maxs=row_stroke_.size();
		
		list<RowStroke>::iterator it = this->row_stroke_.end();
		it--;
		int maxpt=(*it).GetPointList().size();
		
		if (draw_index==maxs-1 && draw_point==maxpt-1)
		{
			draw_index=-1;
			draw_point=-1;
			return;
		}
		
		//进展
		RowStroke tempr=GetRowStroke(draw_index);
		if(draw_point < tempr.GetPointList().size()-1)
		{
			draw_point++;
		}
		else if(draw_point == tempr.GetPointList().size()-1)
		{
			draw_index++;
			draw_point=0;
		}
	}
	else
	{
		//结束
		int maxs=segment_list_.size();
		
		list<Segment>::iterator it = this->segment_list_.end();
		it--;
		int maxpt=(*it).point_list_.size();
		
		if (draw_index==maxs-1 && draw_point==maxpt-1)
		{
			draw_index=-1;
			draw_point=-1;
			return;
		}
		
		//进展
		Segment temps=GetSegment(draw_index);
		if(draw_point < temps.point_list_.size()-1)
		{
			draw_point++;
		}
		else if(draw_point == temps.point_list_.size()-1)
		{
			draw_index++;
			draw_point=0;
		}
	}
}

Segment ScriptCharacter::GetSegment(int num)
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

bool ScriptCharacter::IsNoiseSegment(int num)
{
	//ASSERT(num>=0);
	//ASSERT(num<this->segment_list.size());

	bool yes=false;
	list<int>::iterator it = this->noise_list_.begin();
	while (it!=this->noise_list_.end())
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

Stroke ScriptCharacter::GetStroke(int num)
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

RowStroke ScriptCharacter::GetRowStroke(int num)
{
	//ASSERT(num>=0);
	//ASSERT(num<this->row_stroke.size());
	
	list<RowStroke>::iterator it = this->row_stroke_.begin();
	
	for (int x=0;x<num;x++)
	{
		it++;
	}
	
	return *it;
}

void ScriptCharacter::Init(list<RowStroke> slist)  //原始书写笔画初始化
{
	this->draw_index=-1;  
	this->draw_point=-1;
	this->normal_size_=-1;
	//////
	this->divide_=false;
	this->row_stroke_=slist;
	//////
	this->stroke_list_.clear();
	this->segment_list_.clear();
	this->noise_list_.clear();
}

void ScriptCharacter::ReplaceRowstroke(int ind, RowStroke Row)
{
	//ASSERT(ind>=0);
	//ASSERT(ind<this->row_stroke.size());
	
	list<RowStroke>::iterator it = this->row_stroke_.begin();
	
	for (int x=0;x<ind;x++)
	{
		it++;
	}
	
	*it=Row;
}

void ScriptCharacter::Replacesegment(int ind, Segment seg)
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

void ScriptCharacter::removesegment(int ind)
{
	//ASSERT(ind>=0);
	//ASSERT(ind<this->segment_list.size());

	//////////////////////////////////////
	//处理笔段
	list<Segment>::iterator it = this->segment_list_.begin();	
	for(int x=0;x<ind;x++)
	{
		it++;
	}

	this->segment_list_.erase(it);

	it = this->segment_list_.begin();	
	while(it != this->segment_list_.end())
	{
		if (it->seg_index_>ind)
		{
			it->seg_index_--;
		}
		it++;
	}
	
	//////////////////////////////
	//处理笔画
	list<Stroke>::iterator its=this->stroke_list_.begin();
	while(its!=this->stroke_list_.end())
	{
		if(its->start_index_==ind && its->seg_count_==1)
		{
			its=this->stroke_list_.erase(its);
			break;
		}
		else if(its->start_index_==ind && its->seg_count_>1)
		{
			its->seg_count_-=1;
			break;
		}
		if(its->start_index_<ind && its->start_index_+its->seg_count_ > ind)
		{
			its->seg_count_-=1;
			break;;
		}
		its++;
	}

	its=this->stroke_list_.begin();
	while(its!=this->stroke_list_.end())
	{				
		if(its->start_index_>ind)
		{
			its->start_index_-=1;			
		}
		its++;
	}
	/////////////////////////////
	//处理抖笔
	list<int>::iterator itn = this->noise_list_.begin();
	while(itn!=this->noise_list_.end())
	{
		if(*itn==ind)
		{
			this->noise_list_.erase(itn);
			break;
		}		
		itn++;
	}

	itn = this->noise_list_.begin();
	while(itn!=this->noise_list_.end())
	{
		if(*itn>ind)
		{
			*itn -=1;			
		}
		itn++;
	}
}

void ScriptCharacter::CalculateBox(){
	if (row_stroke_.size() <= 0)
	{
		return;
	}
	width_ = height_ = 0;
	center_point_ = ccp(0,0);
	float xmin, ymin, xmax, ymax;

	RowStroke temp_row_stroke = GetRowStroke(0);
	CCPoint temp_point = temp_row_stroke.GetPoint(0);

	xmin=xmax=temp_point.x;
	ymin=ymax=temp_point.y;

	for (int i=0; i< row_stroke_.size();i++)
	{
		temp_row_stroke=GetRowStroke(i);
		for (int j=0;j < temp_row_stroke.GetPointList().size();j++)
		{
			temp_point=temp_row_stroke.GetPoint(j);

			if (xmin>temp_point.x)
				xmin=temp_point.x;
			if (xmax<temp_point.x)
				xmax=temp_point.x;
			if (ymin>temp_point.y)
				ymin=temp_point.y;
			if (ymax<temp_point.y)
				ymax=temp_point.y;
		}
	}

	width_ = xmax-xmin>0 ? (xmax - xmin):(xmin-xmax);		// 保证 width_ 大于0
	height_ = ymax-ymin>0 ? (ymax - ymin):(ymin-ymax);		// 保证 height_ 大于0
	center_point_ = ccp((xmax+xmin)/2, (ymax+ymin)/2);
}

void ScriptCharacter::MoveToDefaultPlace() {
	// 用 list<segment> 中的点
	CCPoint origin_point = center_point_;
	CalculateBox();
	CCPoint now_point = center_point_;
	CCPoint delta_point = now_point - origin_point;
	Segment temps;
	CCPoint tempp;
	for (int i=0; i<this->segment_list_.size();i++)
	{
		temps=GetSegment(i);
		for (int j=0;j<temps.point_list_.size();j++)
		{
			tempp=temps.GetPoint(j);

			tempp = tempp - delta_point;
			
			temps.SetPoint(j,tempp);
		}
		Replacesegment(i,temps);
	}

}

void ScriptCharacter::Normalize(int height,int width)  //尺寸的缩放，路径的均匀插值。
{
	//1.原始笔画的缩放
	if (this->row_stroke_.size()==0)
	{
		return;
	}
	
	/////////////
	float xmin, ymin, xmax,ymax;
	
	RowStroke temp_row_stroke = GetRowStroke(0);
	CCPoint temp_point = temp_row_stroke.GetPoint(0);
	
	xmin=xmax=temp_point.x;
	ymin=ymax=temp_point.y;
	
	for (int i=0; i<this->row_stroke_.size();i++)
	{
		temp_row_stroke=GetRowStroke(i);
		for (int j=0;j<temp_row_stroke.GetPointList().size();j++)
		{
			temp_point=temp_row_stroke.GetPoint(j);
			
			if (xmin>temp_point.x)
				xmin=temp_point.x;
			if (xmax<temp_point.x)
				xmax=temp_point.x;
			if (ymin>temp_point.y)
				ymin=temp_point.y;
			if (ymax<temp_point.y)
				ymax=temp_point.y;
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
		for (int i=0; i<this->row_stroke_.size();i++)
		{
			temp_row_stroke=GetRowStroke(i);
			for (int j=0;j<temp_row_stroke.GetPointList().size();j++)
			{
				temp_point=temp_row_stroke.GetPoint(j);
				
				float tempx=(temp_point.x-midx)*rescal2+width/2.0;
				float tempy=(temp_point.y-midy)*rescal2+height/2.0;
				
				temp_row_stroke.SetPoint(j,ccp(tempx,tempy));
			}
			ReplaceRowstroke(i,temp_row_stroke);
		}
	}
	else  //以水平缩放为标准
	{
		normal_size_=width;
		for (int i=0; i<this->row_stroke_.size();i++)
		{
			temp_row_stroke=GetRowStroke(i);
			for (int j=0;j<temp_row_stroke.GetPointList().size();j++)
			{
				temp_point=temp_row_stroke.GetPoint(j);
				
				float tempx=(temp_point.x-midx)*rescal1+width/2.;
				float tempy=(temp_point.y-midy)*rescal1+height/2.;
				
				temp_row_stroke.SetPoint(j,CCPoint(tempx,tempy));
			}
			ReplaceRowstroke(i,temp_row_stroke);
		}
	}
	
	/////路径的均匀插值//////
	GeometryTool gt;	
	for (int i=0; i<this->row_stroke_.size();i++)
	{
		temp_row_stroke=GetRowStroke(i);
		list<CCPoint> templ=gt.UniformpathResample(temp_row_stroke.GetRowPoint());
		temp_row_stroke.SetPointList(templ);
		ReplaceRowstroke(i,temp_row_stroke);
	}

	//2.分割后笔画的均匀缩放
	if(divide_)
	{		
		if (this->segment_list_.size()==0)
		{
			return;
		}
		
		/////////////
		float xmin, ymin, xmax,ymax;
		
		Segment temps=GetSegment(0);
		CCPoint tempp=temps.GetPoint(0);
		
		xmin=xmax=tempp.x;
		ymin=ymax=tempp.y;
		
		for (int i=0; i<this->segment_list_.size();i++)
		{
			temps=GetSegment(i);
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
				temps=GetSegment(i);
				for (int j=0;j<temps.point_list_.size();j++)
				{
					tempp=temps.GetPoint(j);
					
					float tempx=(tempp.x-midx)*rescal2+width/2;
					float tempy=(tempp.y-midy)*rescal2+height/2;
					
					temps.SetPoint(j,CCPoint(tempx,tempy));
				}
				Replacesegment(i,temps);
			}
		}
		else  //以水平缩放为标准
		{
			normal_size_=width;
			for (int i=0; i<this->segment_list_.size();i++)
			{
				temps=GetSegment(i);
				for (int j=0;j<temps.point_list_.size();j++)
				{
					tempp=temps.GetPoint(j);
					
					float tempx=(tempp.x-midx)*rescal1+width/2;
					float tempy=(tempp.y-midy)*rescal1+height/2;
					
					temps.SetPoint(j,CCPoint(tempx,tempy));
				}
				Replacesegment(i,temps);
			}
		}
		
		///////////
		//更新笔段属性
		list<Segment>::iterator it=this->segment_list_.begin();
		while(it!=this->segment_list_.end())
		{
			(*it).UpdateAttribute();
			it++;
		}				
	}
}


///////////////////////////////////////
//分割笔段：将每个笔画划分为一系列笔段。
///////////////////////////////////////
void ScriptCharacter::divideSegment1()  //动射线方法
{
	if(row_stroke_.size()==0)
		return;
	/////////////////////
	this->stroke_list_.clear();
	this->segment_list_.clear();
	RowStroke temps;
	GeometryTool gt;
	
	int ind=0;  //笔段计数
	//路径拐点切分
	for (int i=0; i<this->row_stroke_.size();i++)
	{
		temps=GetRowStroke(i);
		list<int> turning_ind;
		turning_ind.clear();
		gt.TurningDivide(temps.GetRowPoint(), turning_ind); //动射线算法切分笔画
		
		//将点序列反向，然后再次计算拐点
		list<CCPoint>  templist=temps.GetRowPoint();
		templist.reverse();
		list<int> tempturning;
		gt.TurningDivide(templist, tempturning); //动射线算法切分笔画
		
		tempturning.erase(--tempturning.end());
        tempturning.push_front(0);
		
		list<int>::iterator itx=tempturning.begin();
		while(itx!=tempturning.end())
		{
			*itx=templist.size()-1-(*itx);
			itx++;
		}

		/////将正反两次得到的拐点进行合并
		list<int> Mturing;
		gt.MergeTurning(turning_ind,tempturning,Mturing);
		
		turning_ind.clear();
		turning_ind=Mturing;
		turning_ind.sort();

		//构造笔画、笔段
		int start=0;		
		list<Segment> seglist;
		seglist.clear();

		list<int>::iterator it =turning_ind.begin();
		while(it!=turning_ind.end())
		{
			list<CCPoint> plist;
			plist.clear();
			while(start!=*it)
			{
                plist.push_back(CCPoint(temps.GetPoint(start)));
				start++;
			}
			plist.push_back(CCPoint(temps.GetPoint(start)));
			
			Segment seg;			
			seg.Init("noname", ind, plist);
			seglist.push_back(seg);
			ind++;

			it++;
		}

		this->Append_divide_Stroke(seglist);
		////////////////
	}

	/////////////////////
	//更新笔段属性
	list<Segment>::iterator it=this->segment_list_.begin();
	while(it!=this->segment_list_.end())
	{
		(*it).UpdateAttribute();
		it++;
	}
	/////////////////////
	divide_=true;
}

///////////////////////////////////////
//分割笔段：将每个笔画划分为一系列笔段。
///////////////////////////////////////
void ScriptCharacter::divideSegment2()   //点到直线距离，以及夹角方法
{
	if(row_stroke_.size()==0 || normal_size_<=0)
		return;
	/////////////////////
	ClearDivideData();
	RowStroke row_stroke;
	GeometryTool gt;
	
	int ind=0;  //笔段计数
	//路径拐点切分
	for (int i=0; i<this->row_stroke_.size();i++)
	{
		row_stroke=GetRowStroke(i);
		list<int> turning_ind;
		turning_ind.clear();
		gt.New_TurningDivide(row_stroke.GetRowPoint(), turning_ind, normal_size_); //将笔画切分为多个笔段（根据点到直线距离，各个分段长度，以及夹角）
		
		//构造笔画、笔段
		int start=0;		
		list<Segment> seglist;
		seglist.clear();
		
		list<int>::iterator it =turning_ind.begin();
		while(it!=turning_ind.end())
		{
			list<CCPoint> plist;
			plist.clear();
			while(start!=*it)
			{
                plist.push_back(CCPoint(row_stroke.GetPoint(start)));
				start++;
			}
			plist.push_back(CCPoint(row_stroke.GetPoint(start)));
			
			Segment seg;			
			seg.Init("noname", ind, plist);
			seglist.push_back(seg);
			ind++;
			
			it++;
		}
		
		this->Append_divide_Stroke(seglist);
		////////////////
	}
	
	/////////////////////
	//更新笔段属性
	list<Segment>::iterator it=this->segment_list_.begin();
	while(it!=this->segment_list_.end())
	{
		(*it).UpdateAttribute();
		it++;
	}
	/////////////////////
	divide_=true;
}


int ScriptCharacter::find_noturning_connection_segment(int segindex)
{
	int m_res=-1;
	GeometryTool gt;
	Segment s1=GetSegment(segindex);
	////////////////////
	//方向相同，端点接近
	for (int ind=0;ind<this->segment_list_.size();ind++)
	{
		if (ind!=segindex && !IsNoiseSegment(ind))
		{
			Segment s2=GetSegment(ind);

			///////情况1:两个起点
			CCPoint p1 = s1.GetPoint(0);
			CCPoint p2 = s2.GetPoint(0);
			float angdiff=max(s1.dirangle_,s2.dirangle_)-min(s1.dirangle_,s2.dirangle_);
			angdiff=fabs(180-angdiff);
			if (gt.pointDistance(p1,p2)<normal_size_/19.0  && angdiff<30)
			{
				m_res=ind;
				break;
			}

			///////情况2：两个终点
			p1=s1.GetPoint(s1.GetPointList().size()-1);
			p2=s2.GetPoint(s2.GetPointList().size()-1);
			angdiff=max(s1.dirangle_,s2.dirangle_)-min(s1.dirangle_,s2.dirangle_);
			angdiff=fabs(180-angdiff);
			if (gt.pointDistance(p1,p2)<normal_size_/19.0  && angdiff<30)
			{
				m_res=ind;
				break;
			}

			///////情况3
			p1 = s1.GetPoint(0);
			p2 = s2.GetPoint(s2.GetPointList().size()-1);
			angdiff = max(s1.dirangle_,s2.dirangle_)-min(s1.dirangle_,s2.dirangle_);
			angdiff = min(angdiff, 360-angdiff);
			if (gt.pointDistance(p1,p2)<normal_size_/19.0  && angdiff<30)
			{
				m_res=ind;
				break;
			}

			///////情况4
			p1 = s1.GetPoint(s1.GetPointList().size()-1);
			p2 = s2.GetPoint(0);
			angdiff = max(s1.dirangle_,s2.dirangle_)-min(s1.dirangle_,s2.dirangle_);
			angdiff = min(angdiff, 360-angdiff);
			if (gt.pointDistance(p1,p2)<normal_size_/19.0  && angdiff<30)
			{
				m_res=ind;
				break;
			}
		}
	}
	////////////////////
	return m_res;
}

bool ScriptCharacter::is_apart_relation(int h1,int h2)
{
	bool yes=true;
	GeometryTool gt;
	//////////
	Segment s1=GetSegment(h1);
	Segment s2=GetSegment(h2);

	CCPoint p1=s1.GetPoint(0);
	CCPoint p2=s1.GetPoint(s1.GetPointList().size()-1);
	CCPoint p3=s2.GetPoint(0);
	CCPoint p4=s2.GetPoint(s2.GetPointList().size()-1);
	
	//判断相交
	if(gt.judge_intersection(p1,p2,p3,p4))
	{
		yes=false;
		return yes;
	}

	//如果不相交，判断相邻
	float dis1=gt.pointToSegment(p1,p2,p3);
	float dis2=gt.pointToSegment(p1,p2,p4);
	float dis3=gt.pointToSegment(p3,p4,p1);
	float dis4=gt.pointToSegment(p3,p4,p2);

	if(dis1<normal_size_/20.1 || dis2<normal_size_/20.1 || dis3<normal_size_/20.1 || dis4<normal_size_/20.1)
	{
		yes=false;
		return yes;
	}

	//////////
	return yes;
}

void ScriptCharacter::clearSegmentColor(ccColor4F color)
{
	list<Segment>::iterator it=this->segment_list_.begin();
	
	while (it!=this->segment_list_.end())
	{
		it->color_=color;
		it++;
	}	
}

void ScriptCharacter::SetSegmentColor(int num, ccColor4F color)
{
	//ASSERT(num<this->segment_list.size());
	//ASSERT(num>=0);
	///////////////////////////////////////
	list<Segment>::iterator it=this->segment_list_.begin();

	for (int i=0;i<num;i++)
	{
		it++;
	}

	it->color_=color;
}

void ScriptCharacter::Append_divide_Stroke(list<Segment> s)  //用于分割后的笔画
{
	Stroke temp;
	temp.SetName("noname");
	temp.SetSegCount(s.size());
	if(this->stroke_list_.size()>0)
	{
		list<Stroke>::iterator it; 
		it = this->stroke_list_.end();
		--it;
		temp.setStartIndex((*it).start_index_+(*it).seg_count_);
	}
	else
	{
		temp.setStartIndex(0);
	}
	
	stroke_list_.push_back(temp);
	
	///////
	list<Segment>::iterator it;
	for (it=s.begin(); it != s.end();it++)
	{
		this->segment_list_.push_back((*it));
	}
	////////
}

void ScriptCharacter::NewIdentifynoiseSegment()
{
	noise_list_.clear();
	//////////////////////////////////////////////////
	list<Stroke>::iterator itstr=stroke_list_.begin();
	list<Segment>::iterator itseg;

	float seg_len_threshold1=normal_size_/24.38;
	float angdiff_threshold2=17.0;
	float seg_len_threshold2=normal_size_/12.30;
	float seg_piecewise_threshold2=normal_size_/2.88;
	float angdiff_threshold3=17.0;
	float seg_len_threshold3=normal_size_/14.20;
	float seg_piecewise_threshold3=normal_size_/2.88;

	GeometryTool gt;
	for ( ; itstr!=stroke_list_.end(); itstr++)
	{		
		if((*itstr).seg_count_==1)//如果笔画中只有一段
		{
			int start=(*itstr).start_index_;
			itseg=segment_list_.begin();
			for (int x=0;x<start;x++)
			{
				itseg++;
			}

			if(gt.PathLength((*itseg).point_list_)<seg_len_threshold1)
			{
				noise_list_.push_back(start);
			}
		}
		else if((*itstr).seg_count_==2)//如果笔画有两段
		{
			int start=(*itstr).start_index_;
			itseg=segment_list_.begin();
			for (int x=0;x<start;x++)
			{
				itseg++;
			}
			CCPoint myps=(*itseg).mid_point_;
			float len1=gt.PathLength((*itseg).point_list_);
			float ang1=(*itseg).dirangle_;
			itseg++;
			CCPoint mypm=(*itseg).GetPoint(0);
			CCPoint mype=(*itseg).mid_point_;
			float len2=gt.PathLength((*itseg).point_list_);
			float ang2=(*itseg).dirangle_;
			
			////////夹角计算
			float angdiff=max(180,max(ang1,ang2)-min(ang1,ang2))-min(180,max(ang1,ang2)-min(ang1,ang2));
						
			float mya1=gt.AngleInDegrees(myps,mypm,true);
			float mya2=gt.AngleInDegrees(mypm,mype,true);
			float mydiff2=max(180,max(mya1,mya2)-min(mya1,mya2))-min(180,max(mya1,mya2)-min(mya1,mya2));

			////////
			float ma=max(len1,len2);
			if((angdiff<=angdiff_threshold2 && mydiff2<angdiff_threshold2*1.8) && ( max(len1,len2)/min(len1,len2)>1.3 || min(len1,len2)/normal_size_<0.32))
			{
				if(len1<len2)
					noise_list_.push_back(start);
				else
					noise_list_.push_back(start+1);					
			}
			else if(ma>=seg_piecewise_threshold2)
			{
				if (len1*5.7<len2 || len1<=seg_len_threshold2)
				{
					noise_list_.push_back(start);
				}
				else if(len2*5.7<len1 || len2<=seg_len_threshold2)
				{
					noise_list_.push_back(start+1);
				}
				////////////////////
				else if(len1>normal_size_/2 && len1/len2>4.5 && angdiff<27)
				{
					noise_list_.push_back(start+1);
				}
			}
			else
			{
				if (len1*3.1<=len2 || (len1<=seg_len_threshold2 && len1<=len2))
				{
					noise_list_.push_back(start);
				}
				else if(len2*3.1<=len1 || (len2<=seg_len_threshold2 && len2<=len1))
				{
					noise_list_.push_back(start+1);
				}
				////////////////////
				else if(len1>normal_size_/2 && len1/len2>4.5 && angdiff<27)
				{
					noise_list_.push_back(start+1);
				}	
			}
			
		}
		else if((*itstr).seg_count_>2)//如果笔画有三段及以上
		{			
			int start=(*itstr).start_index_;	
			itseg=segment_list_.begin();
			for (int x=0;x<start;x++)
			{
				itseg++;
			}
			float  len=gt.PathLength((*itseg).point_list_);
			float len1=gt.PathLength((*itseg).point_list_);

			/////补充角度
			float a1=(*itseg).dirangle_;
			itseg++;
			float a2=(*itseg).dirangle_;
			float bending_a2 = gt.PathLength((*itseg).point_list_)/
				               gt.pointDistance((*itseg).GetPoint(0), (*itseg).GetPoint((*itseg).point_list_.size()-1));
			itseg--;

			/////
			for (int a=0;a<(*itstr).seg_count_;a++)
			{
				if(gt.PathLength((*itseg).point_list_)>len)
					len=gt.PathLength((*itseg).point_list_);
				itseg++;
			}
			itseg--;
			float len2=gt.PathLength((*itseg).point_list_);
			
			//////补充角度
			float b1=(*itseg).dirangle_;
			itseg--;
			float b2=(*itseg).dirangle_;
			float bending_b2 = gt.PathLength((*itseg).point_list_)/
				               gt.pointDistance((*itseg).GetPoint(0), (*itseg).GetPoint((*itseg).point_list_.size()-1));

			//////
			float angdiff1=max(180,max(a1,a2)-min(a1,a2))-min(180,max(a1,a2)-min(a1,a2));
			float angdiff2=max(180,max(b1,b2)-min(b1,b2))-min(180,max(b1,b2)-min(b1,b2));

			///////////////////////////////////////////////////////////////////////////////////
			if ((*itstr).seg_count_==3 && len1*1.2<len && len2*1.2<len && len<normal_size_/8.53)
			{
				noise_list_.push_back(start);
				noise_list_.push_back(start+(*itstr).seg_count_-1);
				continue;
			}
			///////////////////////////////////////////////////////////////////////////////////
			if(len>=seg_piecewise_threshold3)
			{			
				if(len1*6.0<len || len1<=seg_len_threshold3 || ( len1*3.0<len && angdiff1<=angdiff_threshold3 && bending_a2<1.39))
				{
					noise_list_.push_back(start);
				}
				if(len2*6.0<len || len2<=seg_len_threshold3 || ( len2*3.03<len && angdiff2<=angdiff_threshold3 && bending_b2<1.39) || (len2*2.5<len && angdiff2*1.8<=angdiff_threshold3 && bending_b2<1.09)  ||(len2*3.5<len && angdiff2<=22 && bending_b2<1.009))
				{
					noise_list_.push_back(start+(*itstr).seg_count_-1);
				}
			}
			else
			{
				if(len1*4.0<len || len1<=seg_len_threshold3 || ( len1*2.0<=len && angdiff1<=angdiff_threshold3))
				{
					noise_list_.push_back(start);
				}
				if(len2*4.0<len || len2<=seg_len_threshold3 || ( len2*2.0<=len && angdiff2<=angdiff_threshold3) || (len2*3.68<=len && angdiff2<=27.0 && len<160))
				{
					noise_list_.push_back(start+(*itstr).seg_count_-1);
				}
			}
		}
	}
}

//////////////////////////////////////
//识别抖笔笔段
//本函数将汉字外围矩形框默认为512×512。所以调用本函数前需要将汉字归一化为512×512。
//////////////////////////////////////
void ScriptCharacter::IdentifynoiseSegment() 
{
	noise_list_.clear();
	//////////////////////////////////////////////////
	list<Stroke>::iterator itstr=stroke_list_.begin();
	list<Segment>::iterator itseg;

	for ( ; itstr!=stroke_list_.end(); itstr++)
	{		
		if((*itstr).seg_count_==1)//如果笔画中只有一段，那么该段小于8个点就是抖笔
		{
			int start=(*itstr).start_index_;
			itseg=segment_list_.begin();
			for (int x=0;x<start;x++)
			{
				itseg++;
			}

			if((*itseg).point_list_.size()<8)
			{
				noise_list_.push_back(start);
			}
		}
		else if((*itstr).seg_count_==2)//如果笔画有两段，那么如果小段是大段的1/6至1/5.5以下：小段就是抖笔
		{
			int start=(*itstr).start_index_;
			itseg=segment_list_.begin();
			for (int x=0;x<start;x++)
			{
				itseg++;
			}
			int len1=(*itseg).point_list_.size()-1;
			float ang1=(*itseg).dirangle_;
			itseg++;
			int len2=(*itseg).point_list_.size()-1;
			float ang2=(*itseg).dirangle_;
			
			////////补充：夹角计算
			float angdiff=max(180,max(ang1,ang2)-min(ang1,ang2))-min(180,max(ang1,ang2)-min(ang1,ang2));
			
			////////
			int ma=max(len1,len2);
			if(angdiff<=17)
			{
				if(len1<len2)
					noise_list_.push_back(start);
				else
					noise_list_.push_back(start+1);					
			}
			else if(ma>=60)
			{
				if (len1*5.7<len2 || len1<=17)
				{
					noise_list_.push_back(start);
				}
				else if(len2*5.7<len1 || len2<=17)
				{
					noise_list_.push_back(start+1);
				}
			}
			else
			{
				if (len1*3.0<=len2 || len1<=17)
				{
					noise_list_.push_back(start);
				}
				else if(len2*3.0<=len1 || len2<=17)
				{
					noise_list_.push_back(start+1);
				}
			}
			
		}
		else if((*itstr).seg_count_>2)//如果笔画有三段及以上，那么：首段点数小于最长段1/7，则首段是抖笔；
			                                                     //未段点数小于最长段1/7，则未段是抖笔
		{
			int start=(*itstr).start_index_;	
			itseg=segment_list_.begin();
			for (int x=0;x<start;x++)
			{
				itseg++;
			}
			int len=(*itseg).point_list_.size()-1;
			int len1=(*itseg).point_list_.size()-1;

			/////补充角度
			float a1=(*itseg).dirangle_;
			itseg++;
			float a2=(*itseg).dirangle_;
			itseg--;
			/////

			for (int a=0;a<(*itstr).seg_count_;a++)
			{
				if((*itseg).point_list_.size()-1>len)
					len=(*itseg).point_list_.size()-1;
				itseg++;
			}
			itseg--;
			int len2=(*itseg).point_list_.size()-1;
			
			//////补充角度
			float b1=(*itseg).dirangle_;
			itseg--;
			float b2=(*itseg).dirangle_;
			//////
			float angdiff1=max(180,max(a1,a2)-min(a1,a2))-min(180,max(a1,a2)-min(a1,a2));
			float angdiff2=max(180,max(b1,b2)-min(b1,b2))-min(180,max(b1,b2)-min(b1,b2));

			///////////			
			if(len>=60)
			{			
				if(len1*6.0<len || len1<=12 || ( len1*3.0<len && angdiff1<=17))
				{
					noise_list_.push_back(start);
				}
				if(len2*6.0<len || len2<=12 || ( len2*3.0<len && angdiff2<=17))
				{
					noise_list_.push_back(start+(*itstr).seg_count_-1);
				}
			}
			else
			{
				if(len1*4.0<len || len1<=12 || ( len1*2.0<=len && angdiff1<=17))
				{
					noise_list_.push_back(start);
				}
				if(len2*4.0<len || len2<=12 || ( len2*2.0<=len && angdiff2<=17))
				{
					noise_list_.push_back(start+(*itstr).seg_count_-1);
				}
			}
		}
	}
}

