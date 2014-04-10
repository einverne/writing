#ifndef __Stroke_H__
#define __Stroke_H__

#include <vector>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class Stroke
{
public:
	Stroke(void);
	Stroke(vector<CCPoint> points);				//传入一串点，构造一笔
	~Stroke(void);
	int pointCount;
	CCPoint prePoint;		//保存每一笔首点
	vector<CCPoint> pointList;
	vector<CCDrawNode*> nodeList;
public:
	bool addPoint(CCPoint point);
	float strokeLength();				//一笔的长度
	//************************************
	// Method:    resample 重采样，插值代码
	// FullName:  Stroke::resample
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void resample();
	CCSize getRotateAng();						//获取尾点到首点的向量
	CCPoint getMidPoint();						//获取一笔中点，简单理解为首点和尾点的中点
	CCSize getSize();
	CCPoint getBigPoint();						//获取坐标值最大的点，包围盒最右上角的点
	string sendOutput();						//产生送给Lua的字符串
	//************************************
	// Method:    addEveryPoint 给pointList中每个point加上一个point值
	// FullName:  Stroke::addEveryPoint
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: CCPoint point
	//************************************
	void addEveryPoint(CCPoint point);			//每个点加一个点
// 	StrokeNode* getStrokeNode();
private:
	float distance(CCPoint p1,CCPoint p2);		//两点间距离
	int getStrokeBox();					//记录一笔包围盒，重采样使用
	string convertToString(float f);
};

#endif