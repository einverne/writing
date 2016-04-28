#include "HcharacterDrawnode.h"
#include "HcharacterLayer.h"

HcharacterDrawnode::HcharacterDrawnode():
	strokeDrawlist(NULL),
	segmentdraw_list(NULL),
	tianzige_(NULL)
{
}

HcharacterDrawnode::~HcharacterDrawnode()
{
	getStrokeDrawnodeList()->release();
	getSegmentDrawnodeList()->release();
	CC_SAFE_RELEASE(segmentdraw_list);
	CC_SAFE_RELEASE(strokeDrawlist);
}


bool HcharacterDrawnode::init(){
	tianzige_ = CCSprite::create("tianzige.png");
	addChild(tianzige_,-1);
	tianzige_->setAnchorPoint(ccp(0,0));
	setContentSize(tianzige_->getContentSize());

	this->setStrokeDrawnodeList(CCArray::create());
	getStrokeDrawnodeList()->retain();

	this->setSegmentDrawnodeList(CCArray::create());
	getSegmentDrawnodeList()->retain();


	return true;
}

void HcharacterDrawnode::draw(){

	// 未切割
	if (script_char_.divide_ == false)
	{
		CCObject* ob;
		CCARRAY_FOREACH(getStrokeDrawnodeList(),ob){
			StrokeDrawnode* node = (StrokeDrawnode*)ob;
			// 设置颜色
			node->color_ = ccc4f(0.5, 0.128, 0.128, 1);
			// 调用 StrokeDrawnode
			node->draw();
		}
	}else{
		// script_char 保存了手写点信息
		CCObject* ob;
		CCARRAY_FOREACH(getSegmentDrawnodeList(), ob){
			SegmentDrawnode* node = (SegmentDrawnode*)ob;
			Segment segment = node->segment_;
			int number_index = segment.seg_index_;
			CCPoint head_point = segment.point_list_.front();
			CCLabelTTF* number_ttf = CCLabelTTF::create(DataTool::intTostring(number_index).c_str(), "Arial", 50);
			number_ttf->setPosition(head_point-ccp(20,10));		// 首点左偏20px，下10px
			number_ttf->setColor(ccBLUE);						// 蓝色数字
			this->addChild(number_ttf, 1);


			// 调用 Segment 的 draw 方法
			node->draw();
		}


// 		Segment segment;
// 		for (int i=0; i<this->script_char.segment_list_.size();i++)
// 		{
// 			segment = script_char.getsegment(i);
// 			CCPoint point = segment.GetPoint(0);
// 			pDC->BeginPath();   
// 			pDC->Ellipse(tempp.x-6,tempp.y-6,tempp.x+6,tempp.y+6);
// 			pDC->EndPath();	   
// 			pDC->FillPath();			
// 
// 
// 
// 			////
// 			list<int>::iterator tempit;
// 			tempit= find(noise_list.begin(), noise_list.end(), i);
// 			if (tempit != noise_list.end())
// 			{
// 				pen=new CPen(PS_SOLID, 4, RGB(128,128,128));
// 			} 
// 			else
// 			{
// 				pen=new CPen(PS_SOLID, 4, temps.color);
// 			}				
// 			pOldPen=pDC->SelectObject(pen);
// 
// 			///
// 			CString numtag;
// 			numtag.Format(_T("%d"),i);
// 			pDC->TextOut(tempp.x-10,tempp.y-15,numtag);
// 
// 			//////			
// 			pDC->MoveTo(tempp.x,tempp.y);
// 			for (int j=1;j<temps.plist.size();j++)
// 			{
// 				tempp=temps.getpoint(j);
// 				pDC->LineTo(tempp.x,tempp.y);
// 			}
// 			pDC->SelectObject(pOldPen);
// 			delete pen;
// 		}			

	}

}

/*
void ScriptCharacter::draw(CDC* pDC)
{
	CPen *pen, *pOldPen;	
	CCPoint tempp(0,0);
	CBrush   Brush;   
	Brush.CreateSolidBrush(RGB(255,0,0));
	CBrush* oldbrush=pDC->SelectObject(&Brush);
	if (divide==false)  //未分割
	{
		RowStroke Rstroke;
		if(this->row_stroke.size()==0)
		{
			pDC->SelectObject(oldbrush);
			Brush.DeleteObject();
			return;
		}
		//////////
		if(draw_index<0   || draw_point<0) //无动画
		{					
			for (int i=0; i<this->row_stroke.size();i++)
			{
				Rstroke=getrowstroke(i);
				tempp=Rstroke.getpoint(0);
				pDC->BeginPath();   
				pDC->Ellipse(tempp.x-6,tempp.y-6,tempp.x+6,tempp.y+6);
				pDC->EndPath();	   
				pDC->FillPath();			
				
				pen=new CPen(PS_SOLID, 4, RGB(0,0,0));
				pOldPen=pDC->SelectObject(pen);				

				//////			
				pDC->MoveTo(tempp.x,tempp.y);
				for (int j=1;j<Rstroke.Rowplist.size();j++)
				{
					tempp=Rstroke.getpoint(j);
					pDC->LineTo(tempp.x,tempp.y);
				}
				pDC->SelectObject(pOldPen);
				delete pen;
			}			
		}
		else if(draw_index>=0 && draw_point>=0)  //有动画
		{
			//动画演示
			for (int i=0; i<draw_index;i++)
			{
				Rstroke=getrowstroke(i);
				tempp=Rstroke.getpoint(0);
				
				pen=new CPen(PS_SOLID, 4, RGB(0,0,0));
				pOldPen=pDC->SelectObject(pen);
				
				//////			
				pDC->MoveTo(tempp.x,tempp.y);
				for (int j=1;j<Rstroke.Rowplist.size();j++)
				{
					tempp=Rstroke.getpoint(j);
					pDC->LineTo(tempp.x,tempp.y);
				}
				pDC->SelectObject(pOldPen);
				delete pen;
			}
			//最后一笔
			Rstroke=getrowstroke(draw_index);
			tempp=Rstroke.getpoint(0);
			
			pen=new CPen(PS_SOLID, 4, RGB(0,0,0));
			pOldPen=pDC->SelectObject(pen);
			
			//////			
			pDC->MoveTo(tempp.x,tempp.y);
			for (int j=1;j<=draw_point;j++)
			{
				tempp=Rstroke.getpoint(j);
				pDC->LineTo(tempp.x,tempp.y);
			}
			pDC->SelectObject(pOldPen);
			delete pen;
		}
	}
	else//已分割
	{
		Segment temps;
		//////////
		if (this->segment_list.size()==0)
		{
			pDC->SelectObject(oldbrush);
			Brush.DeleteObject();
			return;
		}
		
		//////////
		if(draw_index<0   || draw_point<0) //无动画
		{		
			for (int i=0; i<this->segment_list.size();i++)
			{
				temps=getsegment(i);
				tempp=temps.getpoint(0);
				pDC->BeginPath();   
				pDC->Ellipse(tempp.x-6,tempp.y-6,tempp.x+6,tempp.y+6);
				pDC->EndPath();	   
				pDC->FillPath();			
				
				////
				list<int>::iterator tempit;
				tempit= find(noise_list.begin(), noise_list.end(), i);
				if (tempit != noise_list.end())
				{
					pen=new CPen(PS_SOLID, 4, RGB(128,128,128));
				} 
				else
				{
					pen=new CPen(PS_SOLID, 4, temps.color);
				}				
				pOldPen=pDC->SelectObject(pen);
				
				///
				CString numtag;
				numtag.Format(_T("%d"),i);
				pDC->TextOut(tempp.x-10,tempp.y-15,numtag);

				//////			
				pDC->MoveTo(tempp.x,tempp.y);
				for (int j=1;j<temps.plist.size();j++)
				{
					tempp=temps.getpoint(j);
					pDC->LineTo(tempp.x,tempp.y);
				}
				pDC->SelectObject(pOldPen);
				delete pen;
			}			
		}
		else if(draw_index>=0 && draw_point>=0)  //有动画
		{
			//动画演示
			for (int i=0; i<draw_index;i++)
			{
				temps=getsegment(i);
				tempp=temps.getpoint(0);
				
				pen=new CPen(PS_SOLID, 4, temps.color);
				pOldPen=pDC->SelectObject(pen);
				
				//////			
				pDC->MoveTo(tempp.x,tempp.y);
				for (int j=1;j<temps.plist.size();j++)
				{
					tempp=temps.getpoint(j);
					pDC->LineTo(tempp.x,tempp.y);
				}
				pDC->SelectObject(pOldPen);
				delete pen;
			}
			//最后一笔
			temps=getsegment(draw_index);
			tempp=temps.getpoint(0);
			
			pen=new CPen(PS_SOLID, 4, temps.color);
			pOldPen=pDC->SelectObject(pen);
			
			//////			
			pDC->MoveTo(tempp.x,tempp.y);
			for (int j=1;j<=draw_point;j++)
			{
				tempp=temps.getpoint(j);
				pDC->LineTo(tempp.x,tempp.y);
			}
			pDC->SelectObject(pOldPen);
			delete pen;
		}
	}

	pDC->SelectObject(oldbrush);
	Brush.DeleteObject();
}
*/


void HcharacterDrawnode::AddPoint(CCPoint point){
	CCLog("add point: x:%f y:%f", point.x, point.y);
	StrokeDrawnode* t = (StrokeDrawnode*)(getStrokeDrawnodeList()->objectAtIndex(getStrokeDrawnodeList()->count()-1));
	t->stroke_.AddPoint(point);
}

void HcharacterDrawnode::AddStroke(Stroke stroke){
	getStrokeDrawnodeList()->addObject(StrokeDrawnode::create(stroke));
}

void HcharacterDrawnode::RemoveLastStroke(){
	getStrokeDrawnodeList()->removeLastObject();
}

void HcharacterDrawnode::ChangeStroke(int index,Stroke stroke){
	StrokeDrawnode* temp = StrokeDrawnode::create(stroke);
	getStrokeDrawnodeList()->replaceObjectAtIndex(index,temp);
}

Stroke HcharacterDrawnode::GetStroke(int index){
	return ((StrokeDrawnode*)getStrokeDrawnodeList()->objectAtIndex(index))->GetStroke();
}

void HcharacterDrawnode::Rewrite(){
	CCLog("HcharacterDrawnode::Rewrite	");
	while (getStrokeDrawnodeList()->lastObject() != NULL)
	{
		getStrokeDrawnodeList()->removeLastObject(true);
	}
}

int HcharacterDrawnode::GetStrokeCount(){
	return this->getStrokeDrawnodeList()->count();
}

string HcharacterDrawnode::GetOriginOutput(){
	string point_origin_output;
	CCObject* originob;
	CCARRAY_FOREACH(getStrokeDrawnodeList(),originob){
		StrokeDrawnode* node = (StrokeDrawnode*)originob;
		Stroke stro = node->GetStroke();
		point_origin_output += stro.SendOutputFormat();
	}
	return point_origin_output;
}

string HcharacterDrawnode::GetLuaOutput(CCSize size){
	string output;
	CCObject* ob;
	CCARRAY_FOREACH(getStrokeDrawnodeList(),ob){
		StrokeDrawnode* node = (StrokeDrawnode*)ob;
		Stroke stro = node->GetStroke();
		stro.Convert512(size);
		stro.Resample(100);
		output += stro.SendOutputFormat();
	}
	return output;
}

list<RowStroke> HcharacterDrawnode::GetHandWritingPoints()
{
	list<CCPoint> list_points;
	list<RowStroke> row_stroke_list;
	string point_origin_output;
	CCObject* originob;
	CCARRAY_FOREACH(getStrokeDrawnodeList(),originob){
		Stroke stro = ((StrokeDrawnode*)originob)->GetStroke();
		RowStroke row_stroke;
		row_stroke.Init(stro.GetPointList());
		row_stroke_list.push_back(row_stroke);
	}

	return row_stroke_list;
}

void HcharacterDrawnode::InitScriptCharacter(){
	script_char_.Init(GetHandWritingPoints());
}

void HcharacterDrawnode::GetScriptCharacterSegmentDrawnodeReady(){
	
	list<Segment>::iterator iter = script_char_.segment_list_.begin();
	for (iter ; iter != script_char_.segment_list_.end(); iter++)
	{
		Segment seg = *iter;
		this->getSegmentDrawnodeList()->addObject(SegmentDrawnode::create(seg));
	}
}