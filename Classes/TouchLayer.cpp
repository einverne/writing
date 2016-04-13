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
		this->is_outside_ = false;
		this->is_start_outside_ = false;
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
	this->begin_point_ = touchpoint;
// 	CCSprite* tianzige = Hlayer->getSprite();
	HcharacterDrawnode* dnode = Hlayer->getm_HDrawnode();
	if (dnode->boundingBox().containsPoint(touchpoint))
	{
		//在范围内
		touchpoint = dnode->convertToNodeSpace(touchpoint);
// 		CCLog("TouchLayer::ccTouchesBegan convertToNodeSpace x= %f, y= %f",touchpoint.x,touchpoint.y);
		touch_points_.push_back(touchpoint);
		Stroke str(touch_points_);
		Hlayer->getm_HDrawnode()->AddStroke(str);
	}else
	{
		//不在范围内
		this->is_start_outside_ = true;
		touch_points_.clear();
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
	if (is_start_outside_)
	{
		return;
	}
	if (tianzige->boundingBox().containsPoint(touchp))
	{
		//在范围内，并且起笔在范围内
		touchp = tianzige->convertToNodeSpace(touchp);
		touch_points_.push_back(touchp);
		Hlayer->getm_HDrawnode()->AddPoint(touchp);
	}else if(!tianzige->boundingBox().containsPoint(touchp))
	{
		//不再范围内，并且起笔在范围内
		is_outside_ = true;
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
	if (this->begin_point_.equals(touchp) && tianzige->boundingBox().containsPoint(touchp))
	{
		Hlayer->getm_HDrawnode()->RemoveLastStroke();
	}
	if (this->begin_point_.equals(touchp))
	{
		is_start_outside_ = false;
		is_outside_ = false;
		touch_points_.clear();
		return;
	}
	if (is_start_outside_)
	{
		is_start_outside_ = false;
		is_outside_ = false;
		touch_points_.clear();
		return;
	}
	if (tianzige->boundingBox().containsPoint(touchp) && is_outside_ == false)
	{
		//在范围内，并且起笔在范围内,中途没有出田字格
		touchp = tianzige->convertToNodeSpace(touchp);
		touch_points_.push_back(touchp);
		Hlayer->getm_HDrawnode()->AddPoint(touchp);

		//一笔完成，并且起点，中途，尾点都在范围内，点数据保存在points中
		Hlayer->Judge();			//自动在HcharacterDrawnode中获取点信息

		touch_points_.clear();
	}else if (!tianzige->boundingBox().containsPoint(touchp) && is_outside_ == false)
	{
		//不在范围内，中途没出田字格
		Hlayer->getm_HDrawnode()->RemoveLastStroke();
	}else if(is_outside_ == true)
	{
		Hlayer->getm_HDrawnode()->RemoveLastStroke();
		is_outside_ = false;
	}
	//CCLog("x= %f, y= %f",touchp.x,touchp.y);
	//CCLog("isStartOutside = %d, isOutside = %d",isStartOutside,isOutside);
	is_start_outside_ = false;
	is_outside_ = false;
	touch_points_.clear();
}
void TouchLayer::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}

bool TouchLayer::isFinish(){
	//写的笔画数大于正字汉字笔画数则汉字已经写完
	if (Hlayer->getm_HDrawnode()->GetStrokeCount() > Tlayer->getm_TDrawnode()->getCharacter().getStrokeCount())
	{
		return true;
	}
	return false;
}

bool TouchLayer::isTimeSave(){
	if (Hlayer->getm_HDrawnode()->GetStrokeCount() >= Tlayer->getm_TDrawnode()->getCharacter().getStrokeCount())
	{
		return true;
	}
	return false;
}