#ifndef __Stroke_H__
#define __Stroke_H__

#include <vector>
#include "cocos2d.h"
#include "tools/DataTool.h"

USING_NS_CC;
using namespace std;

// single stroke include several points
// 处理点相关笔画信息
class Stroke
{
public:
	//friend class StrokeNode;

	Stroke(void);
    
	/**
	* create a stroke with a group of points
	* @param points
	* @return
	*/
	Stroke(vector<CCPoint> points);
    
	~Stroke(void);
    
    Stroke& operator=(const Stroke& Right);     // assign
    Stroke(const Stroke& Right);                // copy constructor

	/**
	 * 获取点数量
	 * @return
	 */
	int getPointsCount();

	/**
	* add a point to stroke 将点加入到pointList尾
	* @param point
	* @return
	*/
	bool AddPoint(CCPoint point);

	/**
	* length of a stroke:sum of each distance between two points
	* @return
	*/
	float StrokeLength();

	/**
	* 重采样，插值代码 x轴方向每n-1 px一段
	* @param n 默认为20，可以自由设定 n越大，点与点之间间隔越小, n 为重采样之后的点数量
	* @return
	*/
	void Resample(const int n = 20);

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

	/**
	* get the whole pointList
	* @return
	*/
	vector<CCPoint> GetPointList() const;

	CCPoint getpointListIndexAt(unsigned int i) const;

	/**
	* set the point in pointList 
	* @param index	the index of the point want to change
	* @param pointSet	the point you want to change
	* @return
	*/
	void SetPointInList(int index, CCPoint pointSet);

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
	string sendOutput() const;

	/**
	* 产生送给Lua的字符串, 携带拐点信息
	* @return
	*/
	string sendOutputWithStatus();
    
    /**
    * set first point
    * @param pre
    * @return
    */
    void setprePoint(const CCPoint pre)   {
        pre_point_ = pre;
    };
    
    CCPoint getPrePoint() const {
        return pre_point_;
    };

	/**
	* convert512 转换成512*512 并将坐标原点放置到左上角,转换坐标系由第一象限转成第四象限
	* @param size 田字格大小
	* @return
	*/
	void Convert512(CCSize size);
    
    void SetName(const string n){
        name_ = n;
    }
    
    void SetSegCount(int x){
        seg_count_ = x;
    }
    
    void setStartIndex(int x){
        start_index_ = x;
    }

public:
    string name_;                               // stroke name
    CCPoint pre_point_;                         //保存每一笔首点

	vector<CCPoint> point_list_;				//store a group of points
	vector<string> status_list_;				//store the status read from XML to judge whether the point is inflected
    
    // segment related
    int seg_count_;								// segment counts in this stroke
    int start_index_;							// first segment in this stroke index in Character
    
    // private function
    float distance(CCPoint p1,CCPoint p2);		//两点间距离
    int getStrokeBox();                         //记录一笔包围盒，重采样使用
};

#endif
