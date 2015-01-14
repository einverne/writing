#include "BackgroundLayer.h"
#include "WallSingleScene.h"
#include "LianxiScene.h"

BackgroundLayer::BackgroundLayer()
{
}

BackgroundLayer::~BackgroundLayer()
{
}

bool BackgroundLayer::init(){
	if (CCLayerColor::initWithColor(ccc4(255,255,255,255)))
	{
		CCLog("Background init");
		this->setKeypadEnabled(true);
		//add background picture
		CCSize winSize = CCDirector::sharedDirector()->getWinSize(); 
		CCSize visiableSize = CCDirector::sharedDirector()->getVisibleSize();
		CCSize originSize = CCDirector::sharedDirector()->getVisibleOrigin();
		//add wall-head
// 		CCSprite* wall_head = CCSprite::create("wall_head.png");
// 		this->addChild(wall_head,2);
// 		CCSize headSize = wall_head->getContentSize();
// 		wall_head->setPosition(ccp(visiableSize.width/2,visiableSize.height-wall_head->getContentSize().height/2));
// 		wall_head->setScaleX(visiableSize.width/wall_head->getContentSize().width);

		//add ceshi tail on the bottom
// 		CCSprite* wall_tail = CCSprite::create("wall_tail.png");
// 		this->addChild(wall_tail,2);
// 		CCSize tailSize = wall_tail->getContentSize();
// 		wall_tail->setPosition(ccp(visiableSize.width/2,wall_tail->getContentSize().height/2));
// 		wall_tail->setScaleX(visiableSize.width/wall_tail->getContentSize().width);
// 
// 		CCSprite* bg = CCSprite::create("lianxi_background.png");
// 		this->addChild(bg,0);
// 		bg->setPosition(ccp(visiableSize.width/2,visiableSize.height/2));
// 		bg->setScaleX(visiableSize.width/bg->getContentSize().width);
// 		bg->setScaleY((visiableSize.height-headSize.height-tailSize.height)/bg->getContentSize().height);

		//add static resource , like title bar
		CCSprite* title_bar = CCSprite::create("strangedesign/title bar_background.png");
		addChild(title_bar,1);
		title_bar->setPosition(ccp(visiableSize.width/2,visiableSize.height-title_bar->getContentSize().height/2));

		//add Button
		CCMenuItemImage* back = CCMenuItemImage::create("strangedesign/back_button.png",
			"strangedesign/back_button.png",
			this,
			menu_selector(BackgroundLayer::menuBack));
		back->setPosition(ccp(back->getContentSize().width/2+10,visiableSize.height-back->getContentSize().height/2-25));

		CCMenu* menu = CCMenu::create(back,NULL);
		this->addChild(menu,20);
		menu->setPosition(CCPointZero);

		//add tianzige
		tianzige = CCSprite::create("tianzige.png");
		this->addChild(tianzige,1);
		tianzige->setScale(0.75);
		CCSize tianzigeSize = tianzige->getContentSize();
		tianzige->setPosition(ccp(visiableSize.width/2,visiableSize.height-tianzigeSize.height/2-50-title_bar->getContentSize().height));

		tianzige_draw = CCSprite::create("tianzige.png");
		this->addChild(tianzige_draw,1,1);
		CCSize tianzigeDrawSize = tianzige_draw->getContentSize();
		tianzige_draw->setPosition(ccp(visiableSize.width/2,50+tianzigeDrawSize.height/2+title_bar->getContentSize().height));

	}
	return true;
}

void BackgroundLayer::menuBack(CCObject* pSender){
	this->unscheduleAllSelectors();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeAllDelegates();
	CCDirector::sharedDirector()->popScene();
}

void BackgroundLayer::keyBackClicked(){
	this->unscheduleAllSelectors();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeAllDelegates();
	CCDirector::sharedDirector()->popScene();
}
