#include <vector>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#pragma once
class Stroke
{
public:
	bool addPoint(CCPoint point);
	float strokeLength();
	void resample();
private:
	float distance(CCPoint p1,CCPoint p2);
public:
    Stroke(void);
    ~Stroke(void);
    int pointCount;
	CCPoint prePoint;		//保存每一笔首点
    vector<CCPoint> pointList;
	vector<CCDrawNode*> nodeList;
};