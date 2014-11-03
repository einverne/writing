#include "ceshiLayer.h"
#include "CeshiScene.h"

ceshiLayer::ceshiLayer()
{
}

ceshiLayer::~ceshiLayer()
{
}

bool ceshiLayer::init(){
	if (CCLayer::init())
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCMenuItemImage* next = CCMenuItemImage::create("dog.png",
			"dog.png",
			this,
			menu_selector(ceshiLayer::menuNext));
		next->setPosition(ccp(winSize.width - next->getContentSize().width/2-50,winSize.height/2));
		CCMenu* menu = CCMenu::create(next,NULL);
		addChild(menu,10);
		menu->setPosition(CCPointZero);

		return true;
	}
	return false;
}

void ceshiLayer::menuNext(CCObject* pSender){
	CCLog("next");
	CeshiScene* scene = (CeshiScene*)CCDirector::sharedDirector()->getRunningScene();
	scene->next();
}