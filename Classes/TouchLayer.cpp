#include "TouchLayer.h"

TouchLayer::TouchLayer()
{
}

TouchLayer::~TouchLayer()
{
}

bool TouchLayer::init(){
	CCLog("TouchLayer init()");
	if (CCLayerColor::initWithColor(ccc4(255,255,255,255)))
	{
		this->setTouchEnabled(true);
		this->setOpacity(0);			//È«Í¸Ã÷
		CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);	//register touch event
		return true;
	}
	return false;
}

void TouchLayer::onEnter(){

}
void TouchLayer::onExit(){

}
void TouchLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	CCLog("TouchLayer TouchesBegan");
}
void TouchLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	CCLog("TouchLayer TouchesMoved");
}
void TouchLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	CCLog("TouchLayer TouchesEnded");
}
void TouchLayer::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}