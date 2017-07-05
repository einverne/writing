#include "UniformSpaceNode.h"


UniformSpaceNode::UniformSpaceNode(void)
{
	centerPoint_.clear();
	orientation_ = 0;
}


UniformSpaceNode::~UniformSpaceNode(void)
{
	centerPoint_.clear();
	orientation_ = 0;
}

void UniformSpaceNode::setPoint(vector<CCPoint> points){
	centerPoint_.clear();
	centerPoint_.assign(points.begin(), points.end());
}

void UniformSpaceNode::setLength(int length){
	length_ = length;
}

void UniformSpaceNode::setOrientation(int orientation) {
	orientation_ = orientation;
}

void UniformSpaceNode::draw() {
	glLineWidth(5.0f);					//±Ê»­´ÖÏ¸
	ccDrawColor4F(1,0,1,1);				//±Ê»­ÑÕÉ«
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (orientation_ == 0)
	{
		vector<CCPoint>::iterator it;
		for(it=centerPoint_.begin(); it!=centerPoint_.end(); it++)
		{
			
			float dashLength=5.0;
			CCPoint origin = CCPoint((*it).x, (*it).y - length_/2);
			CCPoint destination = CCPoint((*it).x, (*it).y + length_/2);
			float dx = destination.x - origin.x;  
			float dy = destination.y - origin.y;  
			float dist = sqrtf(dx * dx + dy * dy);  

			float x = dx / dist * dashLength;  
			float y = dy / dist * dashLength;  

			CCPoint p2;

			for (float i = 0.0f; i <= dist / dashLength * .5; i++) 
			{  
				p2.x = origin.x + x;  
				p2.y = origin.y + y;  

				cocos2d::ccDrawLine(origin,p2);

				origin.x += x * 2;  
				origin.y += y * 2;  
			}  
		}
		/////////////////////////////////////
		glLineWidth(4.0f);					//±Ê»­´ÖÏ¸
		ccDrawColor4F(0,0,1,1);				//±Ê»­ÑÕÉ«
		/////////////////////////////////
		int average_y=0;
		for(it=centerPoint_.begin(); it!=centerPoint_.end(); it++)
		{
			average_y+=(*it).y ;
		}
		average_y /= centerPoint_.size();

		vector<CCPoint> arrowPoint_; 
		for(it=centerPoint_.begin(); it!=centerPoint_.end(); it++)
		{
			arrowPoint_.push_back(CCPoint((*it).x,average_y));
		}
		/////////////////////////////////
		vector<CCPoint>::iterator ita=arrowPoint_.begin();
		vector<CCPoint>::iterator itb=ita;
		itb++;
		//CCPoint start;
		for(; itb!=arrowPoint_.end(); ita++,itb++)
		{			
			ccDrawCircle( CCPointMake(((*ita).x+(*itb).x)/2, average_y), fabs((*ita).x-(*itb).x)/2, 0, 30, false);
		}
	} 
	else if(orientation_ == 1)
	{
		vector<CCPoint>::iterator it;
		for(it=centerPoint_.begin(); it!=centerPoint_.end(); it++)
		{
			float dashLength=5.0;
			CCPoint origin = CCPoint((*it).x - length_ /2, (*it).y);
			CCPoint destination = CCPoint((*it).x + length_/2, (*it).y);
			float dx = destination.x - origin.x;  
			float dy = destination.y - origin.y;  
			float dist = sqrtf(dx * dx + dy * dy);  

			float x = dx / dist * dashLength;  
			float y = dy / dist * dashLength;  

			CCPoint p2;

			for (float i = 0.0f; i <= dist / dashLength * .5; i++) 
			{  
				p2.x = origin.x + x;  
				p2.y = origin.y + y;  

				cocos2d::ccDrawLine(origin,p2);

				origin.x += x * 2;  
				origin.y += y * 2;  
			} 
		}
		/////////////////////////////////	
		glLineWidth(4.0f);					//±Ê»­´ÖÏ¸
		ccDrawColor4F(0,0,1,1);				//±Ê»­ÑÕÉ«
		/////////////////////////////////
		int average_x=0;
		for(it=centerPoint_.begin(); it!=centerPoint_.end(); it++)
		{
			average_x+=(*it).x ;
		}
		average_x /= centerPoint_.size();

		vector<CCPoint> arrowPoint_; 
		for(it=centerPoint_.begin(); it!=centerPoint_.end(); it++)
		{
			arrowPoint_.push_back(CCPoint(average_x, (*it).y));
		}
		/////////////////////////////////
		vector<CCPoint>::iterator ita=arrowPoint_.begin();
		vector<CCPoint>::iterator itb=ita;
		itb++;
		//CCPoint start;
		for(; itb!=arrowPoint_.end(); ita++,itb++)
		{			
			ccDrawCircle( CCPointMake(average_x, ((*ita).y+(*itb).y)/2), fabs((*ita).y-(*itb).y)/2, 0, 30, false);
		}
	}
}