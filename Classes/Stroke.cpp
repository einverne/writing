#include "Stroke.h"

USING_NS_CC;

#include <cmath>

Stroke::Stroke(void)
{
	this->pointCount=0;

}


Stroke::~Stroke(void)
{
	pointList.clear();
}

/************************************************************************/
/* 将点加入到pointList尾                                                                     */
/************************************************************************/
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

float Stroke::distance(CCPoint p1,CCPoint p2){
	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	return sqrt(dx*dx + dy*dy);
}

float Stroke::strokeLength(){
	float length = 0;
	for (int i= 1; i < pointList.size() ; ++i)
	{
		length += distance(pointList.at(i-1),pointList.at(i));
	}
	return length;
}

/************************************************************************/
/* 插值代码，如果只有两点，有问题，待解决，uncheck other comditions*/
/************************************************************************/
void Stroke::resample(){
	int n = 10;
	float I = strokeLength() / n;
	float D = 0;

	vector<CCPoint> dstpoList;
	dstpoList.push_back(pointList.at(0));

	for (int i = 1; i < pointCount ; ++i )
	{
		CCPoint p1 = pointList[i-1];
		CCPoint p2 = pointList[i];

		float dist = distance(p1,p2);
		if ( (D+dist) >= I)
		{
			float qx = p1.x + ((I - D) / dist) * (p2.x - p1.x);
			float qy = p1.y + ((I - D) / dist) * (p2.y - p2.y);
			dstpoList.push_back(CCPointMake(qx,qy));
			D = 0.0;
		}
		else
		{
			D += dist;
		}
	}
	if (dstpoList.size() == n-1)
	{
		dstpoList.insert(dstpoList.end(),pointList.at(pointList.size()-1));
	}
	pointList = dstpoList;
	pointCount = n;
}