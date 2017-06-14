#include "ShuipingErrorNode.h"


ShuipingErrorNode::ShuipingErrorNode(void)
{
	//start_point_ = ccp(0,0);
	//end_point_ = ccp(0,0);
	key_point_.clear();
	move_point_.clear();
}


ShuipingErrorNode::~ShuipingErrorNode(void)
{
	key_point_.clear();
	move_point_.clear();
}

void ShuipingErrorNode::setPoint(vector<CCPoint> m_point)//CCPoint start_point, CCPoint end_point){
{
	//start_point_ = start_point;
	//end_point_ = end_point;

	vector<CCPoint>::iterator it;
	for(it=m_point.begin();it!=m_point.end();it++)
	{
		key_point_.push_back(*it);
		move_point_.push_back(*it);
	}
}

void ShuipingErrorNode::draw() {
	ccPointSize(10);
	glLineWidth(3.0f);					//±Ê»­´ÖÏ¸
	ccDrawColor4F(1,0,0,1);				//±Ê»­ÑÕÉ«
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	vector<CCPoint>::iterator it;
	for(it=key_point_.begin();it!=key_point_.end();it++)
	{
		ccDrawPoint(*it);
	}

	vector<CCPoint>::iterator it2;
	for(it2=move_point_.begin();it2!=move_point_.end();it2++)
	{
		CCPoint start_point_=*it2;
		it2++;
		if(it2!=move_point_.end())
		{
			CCPoint end_point_=*it2;
			ccDrawLine(start_point_, end_point_);
		}
		it2--;
	}
}