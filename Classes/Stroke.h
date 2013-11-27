#include <vector>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#pragma once
class Stroke
{
public:
	bool addPoint(CCPoint point);

public:
    Stroke(void);
    ~Stroke(void);
    int pointCount;
	CCPoint prePoint;
    vector<CCPoint> pointList;
	vector<CCDrawNode*> nodeList;
};