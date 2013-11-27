#include "Character.h"


Character::Character(void)
{
	bujianCount = 0;
}


Character::~Character(void)
{
	bujianList.clear();
}

bool Character::addBujian(Bujian bujian){
	
	vector<Bujian>::const_iterator it = bujianList.end();
	bujianList.insert(it,bujian);
	bujianCount++;
	return true;
}

CCSize Character::getBox(){
	float xmin=1000000,ymin=1000000;
	float xmax=0,ymax=0;
	vector<Bujian>::iterator iter = bujianList.begin();
	for (iter;iter != bujianList.end(); ++iter)
	{
		Bujian bujian = (Bujian)*iter;
		vector<Stroke> strokeList = bujian.strokeList;
		vector<Stroke>::iterator stro_iter = strokeList.begin();
		for (stro_iter; stro_iter != strokeList.end(); ++stro_iter)
		{
			Stroke stroke = (Stroke)*stro_iter;
			vector<CCPoint> pointList = stroke.pointList;
			vector<CCPoint>::iterator point_iter = pointList.begin();
			for (point_iter; point_iter != pointList.end() ; ++point_iter)
			{
				CCPoint point = (CCPoint)*point_iter;
				if (xmin>point.x)
				{
					xmin = point.x;
				}
				if (xmax<point.x)
				{
					xmax = point.x;
				}
				if (ymin>point.y)
				{
					ymin = point.y;
				}
				if (ymax<point.y)
				{
					ymax = point.y;
				}
			}
		}
	}
	float xd = xmax - xmin;
	float yd = ymax - ymin;
	
	//transform coordinate
	transformCoordinate(ccp(xmin,ymin),fabs(xd-yd));

	if (xd > yd)
	{
		return CCSize(xd,xd);
	}else{
		return CCSize(yd,yd);
	}
}

void Character::transformCoordinate(CCPoint point,float length){
	vector<Bujian>::iterator iter = bujianList.begin();
	for (iter;iter != bujianList.end(); ++iter)
	{
		Bujian bujian = (Bujian)*iter;
		vector<Stroke> strokeList = bujian.strokeList;
		vector<Stroke>::iterator stro_iter = strokeList.begin();
		for (stro_iter; stro_iter != strokeList.end(); ++stro_iter)
		{
			Stroke stroke = (Stroke)*stro_iter;
			vector<CCPoint> pointList = stroke.pointList;
			for (int i = 0;i < stroke.pointCount; i++)
			{
				CCPoint temppoint = pointList.at(i);
				temppoint = temppoint - point;
				temppoint.y = - temppoint.y;
				temppoint.y = temppoint.y + length;
				pointList.erase(pointList.begin()+i);
				vector<CCPoint>::iterator po_iter = pointList.begin();
				pointList.insert(po_iter+i,ccp(temppoint.x,temppoint.y));		//×ø±ê×ª»»
				pointList[i] = ccp(temppoint.x,temppoint.y);
			}
		}
	}
}

int Character::getStrokeCount(){
	int count = 0;
	for (vector<Bujian>::iterator iter = bujianList.begin(); iter != bujianList.end(); ++ iter)
	{
		count += ((Bujian)*iter).strokeCount;
	}
	return count;
}

void Character::prepareDrawNode(){
	vector<Bujian>::iterator iter = bujianList.begin();
	for (iter;iter != bujianList.end(); ++iter)
	{
		Bujian bujian = (Bujian)*iter;
		vector<Stroke> strokeList = bujian.strokeList;
		vector<Stroke>::iterator stro_iter = strokeList.begin();
		for (stro_iter; stro_iter != strokeList.end(); ++stro_iter)
		{
			Stroke stroke = (Stroke)*stro_iter;
			vector<CCPoint> pointList = stroke.pointList;
			vector<CCDrawNode*> nodeList = stroke.nodeList;
			vector<CCPoint>::iterator point_iter = pointList.begin();
			CCPoint prePoint = (CCPoint)(*point_iter);
			point_iter++;
			for (point_iter; point_iter != pointList.end() ; ++point_iter)
			{
				CCPoint point = (CCPoint)*point_iter;
				CCDrawNode* node = CCDrawNode::create();
				node->drawSegment(prePoint,point,5.0,ccc4f(180,180,180,180));
				prePoint = point;
				
				vector<CCDrawNode*>::iterator nodeIter = nodeList.end();
				nodeList.insert(nodeIter,node);
			}
		}
	}
}