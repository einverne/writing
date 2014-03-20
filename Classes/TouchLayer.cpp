#include "TouchLayer.h"

TouchLayer::TouchLayer()
{
}

TouchLayer::~TouchLayer()
{
}

bool TouchLayer::init(TcharacterLayer* t,HcharacterLayer* h){
	CCLog("TouchLayer init()");
	if (CCLayerColor::initWithColor(ccc4(255,255,255,255)))
	{
		this->isOutside = false;
		this->isStartOutside = false;
		this->Tlayer = t;
		this->Hlayer = h;
		this->setTouchEnabled(true);
		this->setOpacity(0);			//全透明
		CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);	//register touch event


		return true;
	}
	return false;
}

TouchLayer* TouchLayer::create(TcharacterLayer* t,HcharacterLayer* h){
	TouchLayer* pRet = new TouchLayer();
	if (pRet && pRet->init(t,h))
	{
		pRet->autorelease();
		return pRet;
	}else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

void TouchLayer::onEnter(){

}
void TouchLayer::onExit(){

}
void TouchLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	CCLog("TouchLayer TouchesBegan");
	CCPoint touchpoint = ((CCTouch*)pTouches->anyObject())->getLocationInView();
	touchpoint = CCDirector::sharedDirector()->convertToUI(touchpoint);
	//CCLog("x= %f, y= %f",touchpoint.x,touchpoint.y);
	CCSprite* tianzige = Hlayer->getSprite();
	if (tianzige->boundingBox().containsPoint(touchpoint))
	{
		//在范围内
		points.push_back(touchpoint);
		Stroke str(points);
		Hlayer->m_HDrawnode->addStroke(str);
	}else
	{
		//不在范围内
		this->isStartOutside = true;
		points.clear();
	}
	//CCLog("isStartOutside = %d, isOutside = %d",isStartOutside,isOutside);
// 	HcharacterDrawnode* t= Hlayer->m_HDrawnode;
	
}
void TouchLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	CCLog("TouchLayer TouchesMoved");
	CCPoint touchp = ((CCTouch*)pTouches->anyObject())->getLocationInView();
	touchp = CCDirector::sharedDirector()->convertToUI(touchp);
	//CCLog("x= %f, y= %f",touchp.x,touchp.y);

	CCSprite* tianzige = Hlayer->getSprite();
	if (isStartOutside)
	{
		return;
	}
	if (tianzige->boundingBox().containsPoint(touchp))
	{
		//在范围内，并且起笔在范围内
		points.push_back(touchp);
		Hlayer->m_HDrawnode->addPoint(touchp);
	}else if(!tianzige->boundingBox().containsPoint(touchp))
	{
		//不再范围内，并且起笔在范围内
		isOutside = true;
	}
	//CCLog("x= %f, y= %f",touchp.x,touchp.y);
	//CCLog("isStartOutside = %d, isOutside = %d",isStartOutside,isOutside);
}
void TouchLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	CCLog("TouchLayer TouchesEnded");
	CCPoint touchp = ((CCTouch*)pTouches->anyObject())->getLocationInView();
	touchp = CCDirector::sharedDirector()->convertToUI(touchp);
	CCSprite* tianzige = Hlayer->getSprite();
	if (isStartOutside)
	{
		isStartOutside = false;
		isOutside = false;
		points.clear();
		return;
	}
	if (tianzige->boundingBox().containsPoint(touchp) && isOutside == false)
	{
		//在范围内，并且起笔在范围内,中途没有出田字格
		points.push_back(touchp);
		Hlayer->m_HDrawnode->addPoint(touchp);

		//一笔完成，并且起点，中途，尾点都在范围内，点数据保存在points中
		Hlayer->judge();			//自动在HcharacterDrawnode中获取点信息


		points.clear();
	}else if (!tianzige->boundingBox().containsPoint(touchp) && isOutside == false)
	{
		//不在范围内，中途没出田字格
		Hlayer->m_HDrawnode->removeLastStroke();
	}else if(isOutside == true)
	{
		Hlayer->m_HDrawnode->removeLastStroke();
		isOutside = false;
	}
	//CCLog("x= %f, y= %f",touchp.x,touchp.y);
	//CCLog("isStartOutside = %d, isOutside = %d",isStartOutside,isOutside);
	isStartOutside = false;
	isOutside = false;
	points.clear();
}
void TouchLayer::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}