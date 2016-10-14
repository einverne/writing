#include "HcharacterDrawnode.h"
#include "HcharacterLayer.h"

HcharacterDrawnode::HcharacterDrawnode():
	strokeDrawlist(NULL),
	tianziged(NULL)
{
}

HcharacterDrawnode::~HcharacterDrawnode()
{
	getStrokeDrawnodeList()->release();
	CC_SAFE_RELEASE(strokeDrawlist);
}

void HcharacterDrawnode::onEnter(){
	CCNode::onEnter();
	
}

void HcharacterDrawnode::onExit(){
	CCNode::onExit();
}

bool HcharacterDrawnode::init(){
	tianziged = CCSprite::create("tianzige.png");
	addChild(tianziged,-1);
	tianziged->setAnchorPoint(ccp(0,0));
	setContentSize(tianziged->getContentSize());

	this->setStrokeDrawnodeList(CCArray::create());
	getStrokeDrawnodeList()->retain();
	return true;
}

void HcharacterDrawnode::draw(){
	CCObject* ob;
	CCARRAY_FOREACH(getStrokeDrawnodeList(),ob){
		((StrokeDrawnode*)(ob))->draw();
	}
}

void HcharacterDrawnode::addPoint(CCPoint point){
	StrokeDrawnode* t = (StrokeDrawnode*)(getStrokeDrawnodeList()->objectAtIndex(getStrokeDrawnodeList()->count()-1));
	t->addPoint(point);
}

void HcharacterDrawnode::addStroke(Stroke s){
	getStrokeDrawnodeList()->addObject(StrokeDrawnode::create(s));
}

void HcharacterDrawnode::removeLastStroke(){
	getStrokeDrawnodeList()->removeLastObject();
}

void HcharacterDrawnode::changeStroke(int index,Stroke s){
	StrokeDrawnode* temp = StrokeDrawnode::create(s);
	getStrokeDrawnodeList()->replaceObjectAtIndex(index,temp);
}

Stroke HcharacterDrawnode::getStroke(int index){
	return ((StrokeDrawnode*)getStrokeDrawnodeList()->objectAtIndex(index))->getStroke();
}

void HcharacterDrawnode::rewrite(){
	CCLog("rewrite:	");
	while (getStrokeDrawnodeList()->lastObject() != NULL)
	{
		getStrokeDrawnodeList()->removeLastObject(true);
	}
	error_stroke.clear();
}

int HcharacterDrawnode::getStrokeCount(){
	return this->getStrokeDrawnodeList()->count();
}

typedef multimap<int, float> IntFloatMap;
typedef IntFloatMap::iterator MapIter;

vector<CCPoint> HcharacterDrawnode::GetErrorPoints(multimap<int, float>& estroke){
	error_stroke = estroke;
	CCObject* ob;
	int i = 0;
	vector<CCPoint> error_points;

	CCARRAY_FOREACH(getStrokeDrawnodeList(),ob){
		// 一个 key 可能对应多个 value
		MapIter itStart, itEnd;
		pair<MapIter, MapIter> range = error_stroke.equal_range(i);
		for (itStart = range.first; itStart != range.second; itStart++)
		{
			float value = (*itStart).second;
			StrokeDrawnode* sd = (StrokeDrawnode*)ob;
			CCPoint p = sd->stroke.GetPercentPoint(value);
			error_points.push_back(p);
		}
		i++;
	}
	return error_points;
}

void HcharacterDrawnode::markErrorStroke(int markStroke) {
	if (markStroke < 0)
	{
		return;
	}
	CCObject* ob;
	CCARRAY_FOREACH(getStrokeDrawnodeList(), ob) {
		if (markStroke == 0)
		{
			((StrokeDrawnode*)ob)->markError();
			break;
		}
		markStroke--;
	}
}