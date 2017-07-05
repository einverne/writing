#include "SegmentDrawnode.h"


SegmentDrawnode::SegmentDrawnode(Segment segment)
{
	this->segment_ = segment; 
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

	ccColor4F color = segment_.color_;
	glLineWidth(8);					//�ʻ���ϸ
	ccDrawColor4F(color.r, color.g, color.b, color.a);                     //�ʻ���ɫ
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//ccDrawLine(ccp(0,0),ccp(200,200));

	list<CCPoint> point_list = this->segment_.point_list_;
	list<CCPoint>::iterator iter = point_list.begin();
	CCPoint head_point = *iter;

	// �׵���
	glLineWidth(4);					//�ʻ���ϸ
	ccDrawColor4F(1, 0, 0, 1);
	// ccDrawCircle("���ĵ�����","�뾶f","�Ƕ�f","�ֶ���","��ʾ�뾶","X��Ŵ���f","Y��Ŵ���f");  
	ccDrawCircle(head_point, 10, 360, 360, 0, 1, 1);


	glLineWidth(6);
	ccDrawColor4F(color.r, color.g, color.b, color.a);

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
	//        //������ʾ
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
	//        //���һ��
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