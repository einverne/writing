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
}

int HcharacterDrawnode::getStrokeCount(){
	return this->getStrokeDrawnodeList()->count();
}

string HcharacterDrawnode::getOriginOutput(){
	string pointOriginOutput;
	CCObject* originob;
	CCARRAY_FOREACH(getStrokeDrawnodeList(),originob){
		StrokeDrawnode* node = (StrokeDrawnode*)originob;
		Stroke stro = node->getStroke();
		pointOriginOutput += stro.sendOutput();
	}
	return pointOriginOutput;
}

string HcharacterDrawnode::getLuaOutput(CCSize size){
	string output;
	CCObject* ob;
	CCARRAY_FOREACH(getStrokeDrawnodeList(),ob){
		StrokeDrawnode* node = (StrokeDrawnode*)ob;
		Stroke stro = node->getStroke();
		vector<CCPoint> points = stro.GetPointList();
		stro.Convert512(size);
		stro.Resample(100);
		output += stro.sendOutput();
	}
	return output;
}

list<RowStroke> HcharacterDrawnode::GetHandWritingPoints()
{
	list<RowStroke> rowStrokeList;
	list<CCPoint> listpoints;
	string pointOriginOutput;
	CCObject* originob;
	CCARRAY_FOREACH(getStrokeDrawnodeList(),originob){
		Stroke stro = ((StrokeDrawnode*)originob)->getStroke();
		RowStroke rowstroke;
		rowstroke.Init(stro.GetPointList());
		rowStrokeList.push_back(rowstroke);
	}
	return rowStrokeList;
}