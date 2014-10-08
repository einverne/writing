#include "MainScene.h"
#include "WallScene.h"
#include "tools/DataTool.h"

using namespace std;
#include "stdlib.h"
USING_NS_CC;
#define TAG_LAYER_EXIT 1001


MainScene::MainScene(){

}
MainScene::~MainScene(){

}

CCScene* MainScene::scene(){
	CCScene* scene = CCScene::create();
	MainScene* layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

bool MainScene::init(){
	if (!CCLayer::init())
	{
		return false;
	}
	setKeypadEnabled(true);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite* bg = CCSprite::create("main_background.jpg");
	CCSize bgsize = bg->getContentSize();
	bg->setPosition(ccp(winSize.width/2,winSize.height/2));
	addChild(bg,0);
	bg->setScaleX(winSize.width/bgsize.width);
	bg->setScaleY(winSize.height/bgsize.height);

	CCPoint menu_position[8] = {ccp(winSize.width/4,winSize.height/5*4),
		ccp(winSize.width/4*3,winSize.height/5*4),
		ccp(winSize.width/4,winSize.height/5*3),
		ccp(winSize.width/4*3,winSize.height/5*3),
		ccp(winSize.width/4,winSize.height/5*2),
		ccp(winSize.width/4*3,winSize.height/5*2),
		ccp(winSize.width/4,winSize.height/5*1),
		ccp(winSize.width/4*3,winSize.height/5*1),
	};
	CCArray* array = CCArray::create();
	for (int i = 0; i < 8 ; i++)
	{
		string s = DataTool::intTostring(i);
		CCLabelTTF* label = CCLabelTTF::create(s.c_str(),"Arial",50);
		CCMenuItemImage* p = CCMenuItemImage::create(
			"main_button.png",
			"main_button_2.png",
			this,
			menu_selector(MainScene::menuSelected)
			);
		p->setPosition(menu_position[i]);
		label->setPosition(menu_position[i]);
		array->addObject(p);
		addChild(label,2);
	}
	CCMenu* menu = CCMenu::createWithArray(array);
	addChild(menu,1);
	menu->setPosition(CCPointZero);

	return true;
}

void MainScene::onEnter(){
	CCLayer::onEnter();

}

void MainScene::onExit(){
	CCLayer::onExit();
}

void MainScene::keyBackClicked(){
	CCLog("MainScene::keyBackClicked");
	if (CCDirector::sharedDirector()->getRunningScene()->getChildByTag(1001) == NULL) {
		CCLog("MainScene::NULL");
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		PopLayer* exitDialog = PopLayer::create("pop/background.png");
		exitDialog->setContentSize(CCSizeMake(winSize.width*0.8,winSize.height*0.5));
		exitDialog->setTitle("Exit",50);
		exitDialog->setContentText("Are you sure to exit app!",60,100,150);
		exitDialog->setCallBackFunc(this,callfuncN_selector(MainScene::isExit));
		exitDialog->addButton("sure_up.png","sure_down.png","Y",0);
		exitDialog->addButton("cancer_up.png","cancer_down.png","N",1);
		CCDirector::sharedDirector()->getRunningScene()->addChild(exitDialog,100,TAG_LAYER_EXIT);
	}
}

void MainScene::menuSelected(CCObject* pSender){
	CCDirector::sharedDirector()->replaceScene(WallScene::scene());
}

void MainScene::isExit(CCNode* pNode){
	if (pNode->getTag() == 0) {
		//if click Y , end app
		CCDirector::sharedDirector()->end();
	}else {
		CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(TAG_LAYER_EXIT);
	}
}
