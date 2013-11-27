#include "Stroke.h"

USING_NS_CC;

Stroke::Stroke(void)
{
	this->pointCount=0;
	
}


Stroke::~Stroke(void)
{
	pointList.clear();
}

bool Stroke::addPoint(CCPoint point){
	this->pointCount++;
	if (pointList.empty())
	{
		prePoint = point;
	}
	vector<CCPoint>::const_iterator it = pointList.end();
	this->pointList.insert(it,point);

	return true;
}