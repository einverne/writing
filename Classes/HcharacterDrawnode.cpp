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
// 	vector<StrokeDrawnode*>::iterator iter = strokeDrawlist.begin();
// 	for (iter ; iter != strokeDrawlist.end() ; ++iter)
// 	{
// 		(*iter)->draw();
// 	}
	CCObject* ob;
	CCARRAY_FOREACH(getStrokeDrawnodeList(),ob){
		((StrokeDrawnode*)(ob))->draw();
	}
}

void HcharacterDrawnode::addPoint(CCPoint point){
// 	(strokeDrawlist.at(strokeDrawlist.size()-1))->addPoint(point);
	StrokeDrawnode* t = (StrokeDrawnode*)(getStrokeDrawnodeList()->objectAtIndex(getStrokeDrawnodeList()->count()-1));
	t->addPoint(point);
}

void HcharacterDrawnode::addStroke(Stroke s){
// 	StrokeDrawnode* drawnode = StrokeDrawnode::create(s);
// 	this->strokeDrawlist.push_back(drawnode);
// 	drawnode->retain();
	getStrokeDrawnodeList()->addObject(StrokeDrawnode::create(s));
}

void HcharacterDrawnode::removeLastStroke(){
// 	(strokeDrawlist.at(strokeDrawlist.size()-1))->release();
// 	this->strokeDrawlist.pop_back();
	getStrokeDrawnodeList()->removeLastObject();
}

void HcharacterDrawnode::changeStroke(int index,Stroke s){
	StrokeDrawnode* temp = StrokeDrawnode::create(s);
// 	HcharacterLayer* layer = (HcharacterLayer*)this->getParent();
// 	CCSprite* tianzige = (CCSprite*)layer->getSprite();
// 	temp->setPosition(tianzige->getPosition()-ccp(tianzige->getContentSize().width/2,tianzige->getContentSize().height/2));
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