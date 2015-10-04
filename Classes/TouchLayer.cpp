#include "TouchLayer.h"

TouchLayer::TouchLayer():Tlayer(NULL),
	Hlayer(NULL)
{
}

TouchLayer::~TouchLayer()
{
	CCLog("~TouchLayer %d", this->m_uReference);
	CC_SAFE_RELEASE(Tlayer);
	CC_SAFE_RELEASE(Hlayer);
}

bool TouchLayer::init(TcharacterLayer* t,HcharacterLayer* h){
	if (CCLayerColor::initWithColor(ccc4(255,255,255,255)))
	{
		this->isOutside = false;
		this->isStartOutside = false;
		this->setTlayer(t);
		this->setHlayer(h);
		this->setTouchEnabled(true);
		this->setOpacity(0);			//全透明

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
	CCLayer::onEnter();
}
void TouchLayer::onExit(){
	CCLayer::onExit();
}
void TouchLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	getHlayer()->getInfoSprite()->setVisible(false);
	//判断写了几笔 超过笔数就不
	if (isFinish())
	{
		return;
	}

	CCPoint touchpoint = ((CCTouch*)pTouches->anyObject())->getLocationInView();
// 	CCLog("TouchLayer::ccTouchesBegan getLocationInView x= %f, y= %f",touchpoint.x,touchpoint.y);
	touchpoint = CCDirector::sharedDirector()->convertToUI(touchpoint);
// 	CCLog("TouchLayer::ccTouchesBegan convertToUI x= %f, y= %f",touchpoint.x,touchpoint.y);
	this->beginPoint = touchpoint;
// 	CCSprite* tianzige = Hlayer->getSprite();
	HcharacterDrawnode* dnode = Hlayer->getm_HDrawnode();
	if (dnode->boundingBox().containsPoint(touchpoint))
	{
		//在范围内
		touchpoint = dnode->convertToNodeSpace(touchpoint);
// 		CCLog("TouchLayer::ccTouchesBegan convertToNodeSpace x= %f, y= %f",touchpoint.x,touchpoint.y);
		touchPoints.push_back(touchpoint);
		Stroke str(touchPoints);
		Hlayer->getm_HDrawnode()->addStroke(str);
	}else
	{
		//不在范围内
		this->isStartOutside = true;
		touchPoints.clear();
	}
	
}

void TouchLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	if (isFinish())
	{
		return;
	}
	CCPoint touchp = ((CCTouch*)pTouches->anyObject())->getLocationInView();
	touchp = CCDirector::sharedDirector()->convertToUI(touchp);
	//CCLog("x= %f, y= %f",touchp.x,touchp.y);

// 	CCSprite* tianzige = Hlayer->getSprite();
	HcharacterDrawnode* tianzige = Hlayer->getm_HDrawnode();
	if (isStartOutside)
	{
		return;
	}
	if (tianzige->boundingBox().containsPoint(touchp))
	{
		//在范围内，并且起笔在范围内
		touchp = tianzige->convertToNodeSpace(touchp);
		touchPoints.push_back(touchp);
		Hlayer->getm_HDrawnode()->addPoint(touchp);
	}else if(!tianzige->boundingBox().containsPoint(touchp))
	{
		//不再范围内，并且起笔在范围内
		isOutside = true;
	}
	//CCLog("x= %f, y= %f",touchp.x,touchp.y);
	//CCLog("isStartOutside = %d, isOutside = %d",isStartOutside,isOutside);
}
void TouchLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	if (isFinish())
	{
		return;
	}
	CCPoint touchp = ((CCTouch*)pTouches->anyObject())->getLocationInView();
	touchp = CCDirector::sharedDirector()->convertToUI(touchp);
// 	CCSprite* tianzige = Hlayer->getSprite();
	HcharacterDrawnode* tianzige = Hlayer->getm_HDrawnode();
	if (this->beginPoint.equals(touchp) && tianzige->boundingBox().containsPoint(touchp))
	{
		Hlayer->getm_HDrawnode()->removeLastStroke();
	}
	if (this->beginPoint.equals(touchp))
	{
		isStartOutside = false;
		isOutside = false;
		touchPoints.clear();
		return;
	}
	if (isStartOutside)
	{
		isStartOutside = false;
		isOutside = false;
		touchPoints.clear();
		return;
	}
	if (tianzige->boundingBox().containsPoint(touchp) && isOutside == false)
	{
		//在范围内，并且起笔在范围内,中途没有出田字格
		touchp = tianzige->convertToNodeSpace(touchp);
		touchPoints.push_back(touchp);
		Hlayer->getm_HDrawnode()->addPoint(touchp);

		//一笔完成，并且起点，中途，尾点都在范围内，点数据保存在points中
		Hlayer->judge();			//自动在HcharacterDrawnode中获取点信息

		touchPoints.clear();
	}else if (!tianzige->boundingBox().containsPoint(touchp) && isOutside == false)
	{
		//不在范围内，中途没出田字格
		Hlayer->getm_HDrawnode()->removeLastStroke();
	}else if(isOutside == true)
	{
		Hlayer->getm_HDrawnode()->removeLastStroke();
		isOutside = false;
	}
	//CCLog("x= %f, y= %f",touchp.x,touchp.y);
	//CCLog("isStartOutside = %d, isOutside = %d",isStartOutside,isOutside);
	isStartOutside = false;
	isOutside = false;
	touchPoints.clear();
}
void TouchLayer::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}

bool TouchLayer::isFinish(){
	//写的笔画数大于正字汉字笔画数则汉字已经写完
	if (Hlayer->getm_HDrawnode()->getStrokeCount() > Tlayer->getm_TDrawnode()->getCharacter().getStrokeCount())
	{
		return true;
	}
	return false;
}

bool TouchLayer::isTimeSave(){
	if (Hlayer->getm_HDrawnode()->getStrokeCount() >= Tlayer->getm_TDrawnode()->getCharacter().getStrokeCount())
	{
		return true;
	}
	return false;
}