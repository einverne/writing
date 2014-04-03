#include "HcharacterDrawnode.h"
#include "HcharacterLayer.h"

HcharacterDrawnode::HcharacterDrawnode():strokeDrawlist(NULL)
{
}

HcharacterDrawnode::~HcharacterDrawnode()
{
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
	return true;
}

void HcharacterDrawnode::draw(){
// 	vector<StrokeDrawnode*>::iterator iter = strokeDrawlist.begin();
// 	for (iter ; iter != strokeDrawlist.end() ; ++iter)
// 	{
// 		(*iter)->draw();
// 	}
	CCObject* ob;
	CCARRAY_FOREACH(strokeDrawlist,ob){
		((StrokeDrawnode*)(ob))->draw();
	}
}

void HcharacterDrawnode::addPoint(CCPoint point){
// 	(strokeDrawlist.at(strokeDrawlist.size()-1))->addPoint(point);
	StrokeDrawnode* t = (StrokeDrawnode*)(strokeDrawlist->objectAtIndex(strokeDrawlist->count()-1));
	t->addPoint(point);
}

void HcharacterDrawnode::addStroke(Stroke s){
// 	StrokeDrawnode* drawnode = StrokeDrawnode::create(s);
// 	this->strokeDrawlist.push_back(drawnode);
// 	drawnode->retain();
	strokeDrawlist->addObject(StrokeDrawnode::create(s));
}

void HcharacterDrawnode::removeLastStroke(){
// 	(strokeDrawlist.at(strokeDrawlist.size()-1))->release();
// 	this->strokeDrawlist.pop_back();
	strokeDrawlist->removeLastObject();
}

void HcharacterDrawnode::changeStroke(int index,Stroke s){
	StrokeDrawnode* temp = StrokeDrawnode::create(s);
// 	HcharacterLayer* layer = (HcharacterLayer*)this->getParent();
// 	CCSprite* tianzige = (CCSprite*)layer->getSprite();
// 	temp->setPosition(tianzige->getPosition()-ccp(tianzige->getContentSize().width/2,tianzige->getContentSize().height/2));
	strokeDrawlist->replaceObjectAtIndex(index,temp);
}