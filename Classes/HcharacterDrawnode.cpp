#include "HcharacterDrawnode.h"
#include "HcharacterLayer.h"

HcharacterDrawnode::HcharacterDrawnode():strokeDrawlist(NULL)
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