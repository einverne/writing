#include "MainScene.h"
#include "WallSingleScene.h"
#include "tools/DataTool.h"

using namespace std;
#include "stdlib.h"
USING_NS_CC;
#define TAG_LAYER_EXIT 1001
#define TAG_SETTING_LAYER 1002


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

	CCLayer* container_layer = CCLayer::create();
	CCSprite* sp = CCSprite::create("main_button.png");
	float lay_height = (sp->getContentSize().height+50)*20;
	container_layer->setContentSize(CCSizeMake(winSize.width,lay_height));

	
	CCSize scrollview_size = winSize;
	m_pScrollView = CCScrollView::create(scrollview_size , container_layer);
	m_pScrollView->setDirection(kCCScrollViewDirectionVertical);
	m_pScrollView->setPosition(CCPointZero);
	addChild(m_pScrollView);
	m_pScrollView->setContentOffset(ccp(0 , -(container_layer->getContentSize().height-winSize.height)));

	CCMenuItemImage* setting_btn = CCMenuItemImage::create("setting.png",
		"setting.png",
		this,
		menu_selector(MainScene::Setting)
		);
	setting_btn->setPosition(ccp(winSize.width- setting_btn->getContentSize().width -100, setting_btn->getContentSize().height));
	CCMenu* m = CCMenu::create(setting_btn,NULL);
	m->setPosition(CCPointZero);
	addChild(m,2);

// 	CCPoint menu_position[8] = {ccp(winSize.width/4,winSize.height/5*4),
// 		ccp(winSize.width/4*3,winSize.height/5*4),
// 		ccp(winSize.width/4,winSize.height/5*3),
// 		ccp(winSize.width/4*3,winSize.height/5*3),
// 		ccp(winSize.width/4,winSize.height/5*2),
// 		ccp(winSize.width/4*3,winSize.height/5*2),
// 		ccp(winSize.width/4,winSize.height/5*1),
// 		ccp(winSize.width/4*3,winSize.height/5*1),
// 	};

	CCArray* array = CCArray::create();
	for (int i = 0; i < 20 ; i++)
	{
		string s = DataTool::intTostring(i+1);
		CCLabelTTF* label = CCLabelTTF::create(s.c_str(),"Arial",50);
		CCMenuItemImage* p = CCMenuItemImage::create(
			"main_button.png",
			"main_button_2.png",
			this,
			menu_selector(MainScene::menuSelected)
			);
		p->setUserObject(CCString::create(s));
		CCPoint position = ccp(winSize.width/2,container_layer->getContentSize().height-(2*i+1)*(25+p->getContentSize().height/2));
		p->setPosition(position);
		label->setPosition(position);
		array->addObject(p);
		container_layer->addChild(label,2);
	}
	CCMenu* menu = CCMenu::createWithArray(array);
	container_layer->addChild(menu,1);
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
	CCMenuItemImage* menuItemImag = (CCMenuItemImage*)pSender;
	CCString* text = (CCString*)menuItemImag->getUserObject();
	string number = text->getCString();
	string wallfilename = "wall_"+number+".xml";
	CCDirector::sharedDirector()->replaceScene(WallSingleScene::scene(wallfilename));
}

void MainScene::isExit(CCNode* pNode){
	if (pNode->getTag() == 0) {
		//if click Y , end app
		CCDirector::sharedDirector()->end();
	}else {
		CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(TAG_LAYER_EXIT);
	}
}

void MainScene::Setting(CCObject* pSender){
	CCLog("Setting");
	if (CCDirector::sharedDirector()->getRunningScene()->getChildByTag(TAG_SETTING_LAYER) == NULL) {
		
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		PopLayer* exitDialog = PopLayer::create("pop/background.png");
		exitDialog->setContentSize(CCSizeMake(winSize.width*0.75,winSize.height*0.5));
		exitDialog->setTitle("Setting",50);
		exitDialog->setContentText("Setting Loose or tight!",60,100,150);
		exitDialog->setCallBackFunc(this,callfuncN_selector(MainScene::settingCallBack));
		exitDialog->addButton("sure_up.png","sure_down.png","Y",0);
		exitDialog->addButton("cancer_up.png","cancer_down.png","N",1);
		CCDirector::sharedDirector()->getRunningScene()->addChild(exitDialog,100,TAG_SETTING_LAYER);
	}
}

void MainScene::settingCallBack(CCNode* pNode){
	if (pNode->getTag() == 0) {
		//if click Y , end app	将设置写入配置文件 tight
		DataTool::storeToFile("2","setting.xml");
	}else {
		DataTool::storeToFile("1","setting.xml");
	}
}

void MainScene::scrollViewDidScroll(CCScrollView* view){

}

void MainScene::scrollViewDidZoom(CCScrollView* view){

}