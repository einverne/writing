#include "SegmentDrawnode.h"


SegmentDrawnode::SegmentDrawnode(Segment segment)
{
	this->segment_ = segment;
	color_ = ccc4f(1,0,0,1);
}


SegmentDrawnode::~SegmentDrawnode(void)
{
}

SegmentDrawnode* SegmentDrawnode::create(Segment segment){
	SegmentDrawnode* pRet = new SegmentDrawnode(segment);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}else{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

void SegmentDrawnode::draw(){
	CCLog("Enter here");
	ccColor4F color = ccc4f(1,0,0,1);
	glLineWidth(8);					//±Ê»­´ÖÏ¸
	ccDrawColor4F(color.r, color.g, color.b, color.a);                     //±Ê»­ÑÕÉ«
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//ccDrawLine(ccp(0,0),ccp(200,200));

	list<CCPoint> point_list = this->segment_.point_list_;
	list<CCPoint>::iterator iter = point_list.begin();
	CCPoint head_point = *iter;
	iter++;
	for (iter; iter != point_list.end(); iter++)
	{
		CCPoint point = *iter;
		ccDrawLine(head_point, point);
		head_point = point;
	}


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
	//        //¶¯»­ÑÝÊ¾
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
	//        //×îºóÒ»±Ê
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
}