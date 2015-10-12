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

		//add static resource , like title bar
		title_bar = CCSprite::create("strangedesign/title bar_background.png");
		addChild(title_bar,1);
		title_bar->setPosition(ccp(visiableSize.width/2,visiableSize.height-title_bar->getContentSize().height/2));

		//add Button
		CCMenuItemImage* back = CCMenuItemImage::create("strangedesign/back_button.png",
			"strangedesign/back_button_down.png",
			this,
			menu_selector(BackgroundLayer::menuBack));
		back->setPosition(ccp(back->getContentSize().width/2+10,visiableSize.height-back->getContentSize().height/2-25));

		CCMenu* menu = CCMenu::create(back,NULL);
		this->addChild(menu,20);
		menu->setPosition(CCPointZero);

		//add tianzige
		tianzige = CCSprite::create("tianzige.png");
		CCSize tianzigeSize = tianzige->getContentSize();
		tianzige->setPosition(ccp(visiableSize.width/2,visiableSize.height-tianzigeSize.height/2- 100 -title_bar->getContentSize().height));

		tianzige_draw = CCSprite::create("tianzige.png");
		CCSize tianzigeDrawSize = tianzige_draw->getContentSize();
		tianzige_draw->setPosition(ccp(visiableSize.width/2,100+tianzigeDrawSize.height/2+title_bar->getContentSize().height));

	}
	return true;
}

void BackgroundLayer::onEnter(){
	CCLayerColor::onEnter();

	// 只有评分书写时才显示 难易
	// only judgement to show the easy and hard button
	JudgeScene* parentScene = (JudgeScene*)this->getParent();
	bool isjudge = parentScene->getIsJudge();
	if (isjudge == true)
	{
		CWidgetWindow* m_pWindow = CWidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow,10);

		CToggleView* pToggle = CToggleView::create("strangedesign/Judge_writting_easy_button.png","strangedesign/Judge_writting_difficult_button.png");
		pToggle->setOnClickListener(this,ccw_click_selector(BackgroundLayer::onToggle));
		pToggle->setPosition(title_bar->getPosition());
		m_pWindow->addChild(pToggle);

		string str = DataTool::readFromFile("setting.xml");
		if (str == "1")
		{
			pToggle->setChecked(false);
		}else if (str == "2")
		{
			pToggle->setChecked(true);
		}
	}
}


void BackgroundLayer::menuBack(CCObject* pSender){
	this->keyBackClicked();
}

void BackgroundLayer::keyBackClicked(){
	this->unscheduleAllSelectors();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeAllDelegates();
	CCDirector::sharedDirector()->popScene();
}

void BackgroundLayer::onToggle(CCObject* pSender){
	CToggleView* pToggle = (CToggleView*) pSender;
	if( pToggle->isChecked() )
	{
		// 将设置写入配置文件 write the setting to config setting.xml
		CCLog("checked");
		DataTool::storeToFile("2","setting.xml");
	}
	else
	{
		DataTool::storeToFile("1","setting.xml");
	}
}