#include "BackgroundLayer.h"
#include "WallSingleScene.h"

BackgroundLayer::BackgroundLayer()
{
}

BackgroundLayer::~BackgroundLayer()
{
//	CCLog("~BackgroundLayer %d",  this->m_uReference);
}

bool BackgroundLayer::init(){
	if (CCLayer::init())
	{
		CCLog("Background init");
		this->setKeypadEnabled(true);
		//add background picture
		CCSize winSize = CCDirector::sharedDirector()->getWinSize(); 
		CCSize visiableSize = CCDirector::sharedDirector()->getVisibleSize();
		CCSize originSize = CCDirector::sharedDirector()->getVisibleOrigin();
		//add wall-head
		CCSprite* wall_head = CCSprite::create("wall_head.png");
		this->addChild(wall_head,2);
		CCSize headSize = wall_head->getContentSize();
		wall_head->setPosition(ccp(visiableSize.width/2,visiableSize.height-wall_head->getContentSize().height/2));
		wall_head->setScaleX(visiableSize.width/wall_head->getContentSize().width);

		//add ceshi tail on the bottom
		CCSprite* wall_tail = CCSprite::create("wall_tail.png");
		this->addChild(wall_tail,2);
		CCSize tailSize = wall_tail->getContentSize();
		wall_tail->setPosition(ccp(visiableSize.width/2,wall_tail->getContentSize().height/2));
		wall_tail->setScaleX(visiableSize.width/wall_tail->getContentSize().width);

		CCSprite* bg = CCSprite::create("lianxi_background.png");
		this->addChild(bg,0);
		bg->setPosition(ccp(visiableSize.width/2,visiableSize.height/2));
		bg->setScaleX(visiableSize.width/bg->getContentSize().width);
		bg->setScaleY((visiableSize.height-headSize.height-tailSize.height)/bg->getContentSize().height);

		CCMenuItemImage* back = CCMenuItemImage::create("back_1.png",
			"back_2.png",
			this,
			menu_selector(BackgroundLayer::menuBack));
		CCMenu* menu = CCMenu::create(back,NULL);
		this->addChild(menu,20);
		back->setPosition(ccp(winSize.width - back->getContentSize().width/2 ,back->getContentSize().height/2));
		menu->setPosition(CCPointZero);


		//add tianzige
		tianzige = CCSprite::create("tianzige.png");
		this->addChild(tianzige,1);
		CCSize tianzigeSize = tianzige->getContentSize();
		tianzige->setPosition(ccp(visiableSize.width/2,visiableSize.height-headSize.height-tianzigeSize.height/2-50));
		//tianzige->setAnchorPoint(ccp(0, 0));


		tianzige_draw = CCSprite::create("tianzige.png");
		this->addChild(tianzige_draw,1,1);
		CCSize tianzigeDrawSize = tianzige_draw->getContentSize();
		tianzige_draw->setPosition(ccp(visiableSize.width/2,tailSize.height+50+tianzigeDrawSize.height/2));
	}
	return true;
}

void BackgroundLayer::menuBack(CCObject* pSender){
/*	CCDirector::sharedDirector()->popScene();*/
	this->unscheduleAllSelectors();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeAllDelegates();
	CCDirector::sharedDirector()->replaceScene(WallSingleScene::scene());
}

void BackgroundLayer::keyBackClicked(){
	this->unscheduleAllSelectors();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeAllDelegates();
	CCDirector::sharedDirector()->replaceScene(WallSingleScene::scene());
}
