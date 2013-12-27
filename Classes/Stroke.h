#include <vector>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#pragma once
class Stroke
{
public:
	bool addPoint(CCPoint point);
	float strokeLength();				//一笔的长度
	void resample();
	CCSize getRotateAng();						//获取尾点到首点的向量
	CCPoint getMidPoint();						//获取一笔中点，简单理解为首点和尾点的中点
private:
	float distance(CCPoint p1,CCPoint p2);		//两点间距离
	int strokeBox();					//记录一笔包围盒，重采样使用
public:
    Stroke(void);
    ~Stroke(void);
    int pointCount;
	CCPoint prePoint;		//保存每一笔首点
    vector<CCPoint> pointList;
	vector<CCDrawNode*> nodeList;
};