#ifndef __Stroke_H__
#define __Stroke_H__

#include <vector>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class Stroke
{
public:
	friend class StrokeNode;

	Stroke(void);
	/**
	* create a stroke with a group of points
	* @param points
	* @return
	*/
	Stroke(vector<CCPoint> points);
	~Stroke(void);
	vector<CCDrawNode*> nodeList;
	CCPoint prePoint;		//保存每一笔首点
public:
	/**
	 * 获取点数量
	 * @return
	 */
	int getPointsCount();

	/**
	* add a point to stroke
	* @param point
	* @return
	*/
	bool addPoint(CCPoint point);

	/**
	* length of a stroke:sum of each distance between two points
	* @return
	*/
	float strokeLength();

	/**
	* 重采样，插值代码 x轴方向每10px一段
	* @param n 默认为20，可以自由设定
	* @return
	*/
	void resample(int n = 20);

	CCSize getRotateAng();						//获取尾点到首点的向量
	CCPoint getMidPoint();						//获取一笔中点，简单理解为首点和尾点的中点
	CCSize getSize();
	CCPoint getBigPoint();						//获取坐标值最大的点，包围盒最右上角的点

	/**
	* 给pointList中每个point加上一个point值
	* @param point
	* @return
	*/
	void addEveryPoint(CCPoint point);			//每个点加一个点
// 	StrokeNode* getStrokeNode();

	/**
	* get the whole pointList
	* @return
	*/
	vector<CCPoint> getpointList();

	CCPoint getpointListIndexAt(int i);

	/**
	* set the point in pointList 
	* @param index	the index of the point want to change
	* @param pointSet	the point you want to change
	* @return
	*/
	void setpointList(int index, CCPoint pointSet);

	/**
	* to construct a list of status of points
	* @param status
	* @return
	*/
	void addStatus(const char* status);

	/**
	* 产生送给Lua的字符串
	* @return
	*/
	string sendOutput();

	/**
	* 产生送给Lua的字符串
	* @return
	*/
	string sendOutputWithStatus();

private:
	int pointCount;

	float distance(CCPoint p1,CCPoint p2);		//两点间距离
	int getStrokeBox();					//记录一笔包围盒，重采样使用
	string convertToString(float f);

	vector<CCPoint> pointList;				//store a group of points
	vector<string> statusList;				//store the status read from XML to judge whether the point is inflected
};

#endif
