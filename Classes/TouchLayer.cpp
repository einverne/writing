#include "TouchLayer.h"
#include "LianxiScene.h"

TouchLayer::TouchLayer():Tlayer(NULL),
	Hlayer(NULL)
{
}

TouchLayer::~TouchLayer()
{
	CCLog("TouchLayer %d", this->m_uReference);
	CC_SAFE_RELEASE(Tlayer);
	CC_SAFE_RELEASE(Hlayer);
}

bool TouchLayer::init(TcharacterLayer* t,HcharacterLayer* h){
	CCLog("TouchLayer init()");
	if (CCLayerColor::initWithColor(ccc4(255,255,255,255)))
	{
		this->isOutside = false;
		this->isStartOutside = false;
		this->setTlayer(t);
		this->setHlayer(h);
		this->setTouchEnabled(true);
		this->setOpacity(0);			//全透明
		//CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);	//register touch event


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
	CCLog("TouchLayer TouchesBegan");
	//判断写了几笔 超过笔数就不写
	LianxiScene* temp= (LianxiScene*)(this->getParent());
	

	if (Hlayer->getm_HDrawnode()->getStrokeDrawnodeList()->count() > Tlayer->getm_TDrawnode()->m_character.getStrokeCount())
	{
		return;
	}
	CCPoint touchpoint = ((CCTouch*)pTouches->anyObject())->getLocationInView();
	touchpoint = CCDirector::sharedDirector()->convertToUI(touchpoint);
	//CCLog("x= %f, y= %f",touchpoint.x,touchpoint.y);
	CCSprite* tianzige = Hlayer->getSprite();
	if (tianzige->boundingBox().containsPoint(touchpoint))
	{
		//在范围内
		points.push_back(touchpoint);
		Stroke str(points);
		Hlayer->getm_HDrawnode()->addStroke(str);
	}else
	{
		//不在范围内
		this->isStartOutside = true;
		points.clear();
	}
	//CCLog("isStartOutside = %d, isOutside = %d",isStartOutside,isOutside);
// 	HcharacterDrawnode* t= Hlayer->getm_HDrawnode();
	
}
void TouchLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	CCLog("TouchLayer TouchesMoved");
	if (Hlayer->getm_HDrawnode()->getStrokeDrawnodeList()->count() > Tlayer->getm_TDrawnode()->m_character.getStrokeCount())
	{
		return;
	}
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
	CCLog("TouchLayer TouchesEnded");
	char str[5]={0};
	itoa(this->m_uReference,str,10);
	CCLog("TouchLayer m_uRef:");
	CCLog(str);
	if (Hlayer->getm_HDrawnode()->getStrokeDrawnodeList()->count() > Tlayer->getm_TDrawnode()->m_character.getStrokeCount())
	{
		return;
	}
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
		Hlayer->getm_HDrawnode()->addPoint(touchp);

		//一笔完成，并且起点，中途，尾点都在范围内，点数据保存在points中
		Hlayer->judge();			//自动在HcharacterDrawnode中获取点信息


		points.clear();
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
	points.clear();
}
void TouchLayer::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}