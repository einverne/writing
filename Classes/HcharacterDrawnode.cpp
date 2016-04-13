#include "HcharacterDrawnode.h"
#include "HcharacterLayer.h"

HcharacterDrawnode::HcharacterDrawnode():
	strokeDrawlist(NULL),
	tianzige_(NULL)
{
}

HcharacterDrawnode::~HcharacterDrawnode()
{
	getStrokeDrawnodeList()->release();
	CC_SAFE_RELEASE(strokeDrawlist);
}


bool HcharacterDrawnode::init(){
	tianzige_ = CCSprite::create("tianzige.png");
	addChild(tianzige_,-1);
	tianzige_->setAnchorPoint(ccp(0,0));
	setContentSize(tianzige_->getContentSize());

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

void HcharacterDrawnode::AddPoint(CCPoint point){
	StrokeDrawnode* t = (StrokeDrawnode*)(getStrokeDrawnodeList()->objectAtIndex(getStrokeDrawnodeList()->count()-1));
	t->addPoint(point);
}

void HcharacterDrawnode::AddStroke(Stroke s){
	getStrokeDrawnodeList()->addObject(StrokeDrawnode::create(s));
}

void HcharacterDrawnode::RemoveLastStroke(){
	getStrokeDrawnodeList()->removeLastObject();
}

void HcharacterDrawnode::ChangeStroke(int index,Stroke s){
	StrokeDrawnode* temp = StrokeDrawnode::create(s);
	getStrokeDrawnodeList()->replaceObjectAtIndex(index,temp);
}

Stroke HcharacterDrawnode::GetStroke(int index){
	return ((StrokeDrawnode*)getStrokeDrawnodeList()->objectAtIndex(index))->getStroke();
}

void HcharacterDrawnode::Rewrite(){
	CCLog("rewrite:	");
	while (getStrokeDrawnodeList()->lastObject() != NULL)
	{
		getStrokeDrawnodeList()->removeLastObject(true);
	}
}

int HcharacterDrawnode::GetStrokeCount(){
	return this->getStrokeDrawnodeList()->count();
}

string HcharacterDrawnode::GetOriginOutput(){
	string pointOriginOutput;
	CCObject* originob;
	CCARRAY_FOREACH(getStrokeDrawnodeList(),originob){
		StrokeDrawnode* node = (StrokeDrawnode*)originob;
		Stroke stro = node->getStroke();
		pointOriginOutput += stro.sendOutput();
	}
	return pointOriginOutput;
}

string HcharacterDrawnode::GetLuaOutput(CCSize size){
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