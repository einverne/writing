#include "MyToast.h"

MyToast::MyToast(){

}

MyToast::~MyToast(){

}

bool MyToast::init(){
	bool bRet = false;  
	do {  
		CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)));
		visibleSize = CCDirector::sharedDirector()->getVisibleSize();  
		origin = CCDirector::sharedDirector()->getVisibleOrigin();  
		bg = CCScale9Sprite::create("toast_bg.png");
		bg->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+130*visibleSize.height/960));  
		bRet = true;  
	} while (0);  
	return bRet;  
	return true;
}

void MyToast::onExit(){
	CCLayerColor::onExit();
}

void MyToast::initToast(string msg,float time){
	CCLabelTTF* pLabel = CCLabelTTF::create(msg.c_str(),"Arial",50);
	pLabel->setColor(ccWHITE);
	bg->setContentSize(CCSizeMake(pLabel->getContentSize().width+35, pLabel->getContentSize().height+20));
	bg->addChild(pLabel,1);
	this->addChild(bg,10);
	pLabel->setPosition(ccp(bg->getContentSize().width/2, bg->getContentSize().height/2));
	pLabel->runAction(CCSequence::create(CCFadeIn::create(time/5),
		CCDelayTime::create(time/5*3), CCFadeOut::create(time/5),NULL));
	bg->runAction(CCSequence::create(CCFadeIn::create(time/5),
		CCDelayTime::create(time/5*3), CCFadeOut::create(time/5),
		CCCallFunc::create(this,callfunc_selector(MyToast::removeSelf)),NULL));
}

void MyToast::removeSelf(){
	this->removeFromParentAndCleanup(true);
}

void MyToast::showToast(CCNode* node,string msg,float dt){
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();  
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();  
	CCScale9Sprite* bg = CCScale9Sprite::create("toast_bg.png");  
	bg->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+130*visibleSize.height/960));  
	CCLabelTTF* pLabel = CCLabelTTF::create(msg.c_str(), "Arial", 50);  
	pLabel->setColor(ccWHITE);  
	bg->setContentSize(CCSizeMake(pLabel->getContentSize().width+35,pLabel->getContentSize().height+20));  
	bg->addChild(pLabel, 1);  
	node->addChild(bg,10);  
	pLabel->setPosition(ccp(bg->getContentSize().width/2,bg->getContentSize().height/2));  
	pLabel->runAction(CCSequence::create(CCFadeIn::create(dt/5),  
		CCDelayTime::create(dt/5*3),CCFadeOut::create(dt/5),NULL));  
	bg->runAction(CCSequence::create(CCFadeIn::create(dt/5),  
		CCDelayTime::create(dt/5*3),CCFadeOut::create(dt/5),  
		CCCallFuncN::create(bg,callfuncN_selector(MyToast::removeToast)),  
		NULL));  
}

void MyToast::removeToast(CCNode* node){
	node->removeFromParentAndCleanup(true);
}