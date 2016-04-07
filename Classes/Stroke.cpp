#include "Stroke.h"

USING_NS_CC;
#include <cmath>

Stroke::Stroke(void)
{
	point_list_.clear();
	status_list_.clear();
}

Stroke::Stroke(vector<CCPoint> points){
	if (points.size() > 0)
	{
		this->point_list_ = points;
		this->pre_point_ = points.at(0);
	}
}


Stroke::~Stroke(void)
{
	point_list_.clear();
	status_list_.clear();
}

int Stroke::getPointsCount(){
	return point_list_.size();
}

Stroke& Stroke::operator=(const Stroke& Right){
    this->name_ = Right.name_;
    this->pre_point_ = Right.pre_point_;
    this->point_list_ = Right.point_list_;
    this->status_list_ = Right.status_list_;
    
    this->start_index_ = Right.start_index_;
    this->seg_count_ = Right.seg_count_;
    return *this;
}


Stroke::Stroke(const Stroke& Right){
    this->name_ = Right.name_;
    this->pre_point_ = Right.pre_point_;
    this->point_list_ = Right.point_list_;
    this->status_list_ = Right.status_list_;
    
    this->start_index_ = Right.start_index_;
    this->seg_count_ = Right.seg_count_;
}

bool Stroke::AddPoint(CCPoint point){
	if (point_list_.empty())
	{
		pre_point_ = point;
	}
	point_list_.push_back(point);
	return true;
}

float Stroke::distance(CCPoint p1,CCPoint p2){
	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	return sqrt(dx*dx + dy*dy);
}

float Stroke::StrokeLength(){
	float length = 0;
	for (unsigned int i= 1; i < point_list_.size() ; ++i)
	{
		length += distance(point_list_.at(i-1),point_list_.at(i));
	}
	return length;
}

int Stroke::getStrokeBox(){
	float dx = fabs(point_list_.at(0).x-point_list_.at(point_list_.size()-1).x);
	float dy = fabs(point_list_.at(0).y-point_list_.at(point_list_.size()-1).y);
	return dx>dy?dx:dy;
}

void Stroke::Resample(const int n){
// 	int n = getStrokeBox()/10;			//获取长宽最大像素值
	CCAssert(n > 0,"n > 0");
	float I = StrokeLength() / (n-1);		// I个像素一段
	float D = 0;

	vector<CCPoint> dstpoList;
	dstpoList.push_back(point_list_.at(0));

	for (unsigned int i = 1; i < point_list_.size() ; ++i )
	{
		CCPoint p1 = point_list_[i-1];
		CCPoint p2 = point_list_[i];

		float dist = distance(p1,p2);
		if ( (D+dist) >= I)
		{
			float qx = p1.x + ((I - D) / dist) * (p2.x - p1.x);
			float qy = p1.y + ((I - D) / dist) * (p2.y - p1.y);
			vector<CCPoint>::iterator it = point_list_.begin();
			point_list_.insert(it+i,CCPointMake(qx,qy));
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
		dstpoList.insert(dstpoList.end(),point_list_.at(point_list_.size()-1));
	}
	point_list_ = dstpoList;
	pre_point_ = point_list_[0];
}

CCSize Stroke::getRotateAng(){
	CCPoint lastPoint = point_list_[point_list_.size()-1];
	float width = pre_point_.x - lastPoint.x;
	float height = pre_point_.y - lastPoint.y;
	return CCSizeMake(width,height);
}

CCPoint Stroke::getMidPoint(){
	return ccpMidpoint(point_list_[0],point_list_[point_list_.size()-1]);
}

CCSize Stroke::getSize(){
	return CCSizeMake(getStrokeBox(),getStrokeBox());
}
CCPoint Stroke::getBigPoint(){
	float tx=0,ty=0;
	for (vector<CCPoint>::const_iterator it = point_list_.begin(); it != point_list_.end(); ++it)
	{
		CCPoint tmp = (*it);
		if (tmp.x > tx)
		{
			tx = tmp.x;
		}
		if (tmp.y > ty)
		{
			ty = tmp.y;
		}
	}
	return ccp(tx,ty);
}

void Stroke::addEveryPoint(CCPoint point){
	for (unsigned int i = 0 ; i < point_list_.size(); ++i)
	{
		point_list_[i] = point_list_[i] + point;
	}
}


string Stroke::sendOutput() const{
	string ret;
	for (vector<CCPoint>::const_iterator it = point_list_.begin(); it != point_list_.end() ; ++it)
	{
		CCPoint temp = (*it);
        ret += DataTool::to_string(ceil(temp.x))+ "/" + DataTool::to_string(ceil(temp.y)) + "/";
	}
	ret += "@";
	return ret;
}

vector<CCPoint> Stroke::GetPointList() const{
	return point_list_;
}

CCPoint Stroke::getpointListIndexAt(unsigned int i) const{
	CCPoint p = ccp(0,0);
	if (i < point_list_.size())
	{
		p = point_list_.at(i);
	}
	return p;
}

void Stroke::SetPointInList(int index,CCPoint pointSet){
	point_list_[index] = pointSet;
}

void Stroke::addStatus(const char* status){
	string s = status;
	status_list_.push_back(s);
}

string Stroke::sendOutputWithStatus(){
	string ret;
	vector<string>::const_iterator statusIter = status_list_.begin();
	vector<CCPoint>::const_iterator pointIter;
	if (point_list_.size() != status_list_.size())
	{
		return ret;
	}
	for (pointIter = point_list_.begin(); pointIter != point_list_.end() && statusIter!= status_list_.end(); pointIter++,statusIter++)
	{
		CCPoint temp = (*pointIter);
		ret += DataTool::to_string(ceil(temp.x))+ "/" + DataTool::to_string(ceil(temp.y)) + "/";
		string status = *statusIter;
		ret += status + "/";
	}
	ret += "@";
	return ret;
}

void Stroke::Convert512(CCSize size){
	for (vector<CCPoint>::iterator it = point_list_.begin(); it != point_list_.end(); ++it)
	{
		CCPoint p = *it;
		float fx = p.x * 512 / size.width;
		float fy = p.y * 512 / size.height;
		*it = ccp(fx,-(fy-512));			// 将第一象限坐标系转换成第四象限坐标
	}
}