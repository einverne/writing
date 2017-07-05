#include "MultiPointNode.h"


MultiPointNode::MultiPointNode(void)
{
	//start_point_ = ccp(0,0);
	//end_point_ = ccp(0,0);
	key_point_.clear();
	move_point_.clear();
}


MultiPointNode::~MultiPointNode(void)
{
	key_point_.clear();
	move_point_.clear();
}

void MultiPointNode::setPoint(vector<CCPoint> m_point)//CCPoint start_point, CCPoint end_point){
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

void MultiPointNode::draw() {
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

	ccDrawColor4F(0,0,1,1);				//±Ê»­ÑÕÉ«
	vector<CCPoint>::iterator it2;
	for(it2=move_point_.begin();it2!=move_point_.end();it2++)
	{
		CCPoint start_point_=*it2;
		it2++;
		if(it2!=move_point_.end())
		{
			CCPoint end_point_=*it2;
			//ccDrawLine(start_point_, end_point_);
			//////////////////////
			float dashLength=5.0;
			float dx = end_point_.x - start_point_.x;  
			float dy = end_point_.y - start_point_.y;  
			float dist = sqrtf(dx * dx + dy * dy);  

			float x = dx / dist * dashLength;  
			float y = dy / dist * dashLength;  

			CCPoint p2;

			for (float i = 0.0f; i <= dist / dashLength * .5; i++) 
			{  
				p2.x = start_point_.x + x;  
				p2.y = start_point_.y + y;  

				cocos2d::ccDrawLine(start_point_,p2);

				start_point_.x += x * 2;  
				start_point_.y += y * 2;  
			}  
			////////////////////
		}
		it2--;
	}
}