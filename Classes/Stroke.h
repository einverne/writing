#include <vector>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#pragma once
class Stroke
{
public:
    Stroke(void);
    ~Stroke(void);
    int pointCount;
    vector<CCPoint> pointList;
	vector<CCDrawNode> nodeList;
};