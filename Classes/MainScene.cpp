#include "MainScene.h"
#include "NewUnit.h"
#include "PopLayer.h"
#include "PopCancelLayer.h"

#define TAG_LAYER_EXIT 1001
#define TAG_SETTING_LAYER 1002

MainScene::MainScene():unit_count(0),pGridView(NULL){

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
	CCLog("mainScene init enter");
	if (!CCLayerColor::initWithColor(ccc4(255,255,255,255)))
	{
		return false;
	}
	unit_count = SQLiteData::getUnitCount();
	unit_ids = SQLiteData::getUnitIDs();
	setKeypadEnabled(true);

	CWidgetWindow* m_pWindow = CWidgetWindow::create();
	m_pWindow->setMultiTouchEnabled(true);
	addChild(m_pWindow,4);
	
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSize visiableSize = CCDirector::sharedDirector()->getVisibleSize();

	CCSprite* titlebar = CCSprite::create("strangedesign/title bar_background.png");
	addChild(titlebar,1);
	titlebar->setPosition(ccp(visiableSize.width/2,visiableSize.height-titlebar->getContentSize().height/2));

	CCSprite* selectionMode = CCSprite::create("strangedesign/main_selection_unit.png");
	addChild(selectionMode,2);
	selectionMode->setPosition(titlebar->getPosition());
	
// 	CCMenuItemImage* setting_btn = CCMenuItemImage::create("setting.png",
// 		"setting.png",
// 		this,
// 		menu_selector(MainScene::Setting)
// 		);
// 	setting_btn->setPosition(ccp(winSize.width- setting_btn->getContentSize().width -100, setting_btn->getContentSize().height));
// 	CCMenu* m = CCMenu::create(setting_btn,NULL);
// 	m->setPosition(CCPointZero);
// 	addChild(m,3);
	
	CButton* add_btn = CButton::create("strangedesign/Main_add_button.png","strangedesign/Main_add_button_down.png");
	add_btn->setPosition(ccp(winSize.width -50, winSize.height- titlebar->getContentSize().height/2));
	add_btn->setOnClickListener(this,ccw_click_selector(MainScene::addButtonCallback));
	m_pWindow->addChild(add_btn,4);

	
	CCLog("unit count %d",unit_count);

	CCSize visualSize = CCSizeMake(winSize.width,winSize.height-titlebar->getContentSize().height-10);
	CCSize gridcellSize = CCSizeMake(360 , 350);
	pGridView = CGridView::create(
		visualSize,
		gridcellSize,
		unit_count, this,
		ccw_datasource_adapter_selector(MainScene::gridviewDataSource));
	pGridView->setColumns(2);
	pGridView->setPosition(CCSize(winSize.width/2,(winSize.height-titlebar->getContentSize().height)/2));
	m_pWindow->addChild(pGridView);
	pGridView->setAutoRelocate(true);
	pGridView->reloadData();
	
	return true;
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

void MainScene::isExit(CCNode* pNode){
	if (pNode->getTag() == 0) {
		//if click Y , end app
		CCDirector::sharedDirector()->end();
	}else {
		CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(TAG_LAYER_EXIT);
	}
}

// void MainScene::Setting(CCObject* pSender){
// 	CCLog("Setting");
// 	if (CCDirector::sharedDirector()->getRunningScene()->getChildByTag(TAG_SETTING_LAYER) == NULL) {
// 		
// 		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
// 		PopLayer* exitDialog = PopLayer::create("pop/background.png");
// 		exitDialog->setContentSize(CCSizeMake(winSize.width*0.75,winSize.height*0.5));
// 		exitDialog->setTitle("Setting",50);
// 		exitDialog->setContentText("Setting Loose or tight!",60,100,150);
// 		exitDialog->setCallBackFunc(this,callfuncN_selector(MainScene::settingCallBack));
// 		exitDialog->addButton("sure_up.png","sure_down.png","Y",0);
// 		exitDialog->addButton("cancer_up.png","cancer_down.png","N",1);
// 		CCDirector::sharedDirector()->getRunningScene()->addChild(exitDialog,100,TAG_SETTING_LAYER);
// 	}
// }

// void MainScene::settingCallBack(CCNode* pNode){
// 	if (pNode->getTag() == 0) {
// 		//if click Y , end app	将设置写入配置文件 tight
// 		DataTool::storeToFile("2","setting.xml");
// 	}else {
// 		DataTool::storeToFile("1","setting.xml");
// 	}
// }

CCObject* MainScene::gridviewDataSource(CCObject* pConvertView, unsigned int idx){
	CGridViewCell* pCell = (CGridViewCell*) pConvertView;
	CButton* pButton = NULL;
	CCLog("idx %d",idx);
	if(!pCell)
	{
		pCell = new CGridViewCell();
		pCell->autorelease();

		//pButton = CButton::createWith9Sprite(CCSizeMake(70, 70), "sprite9_btn1.png", "sprite9_btn2.png");
		pButton = CButton::create("strangedesign/main_clincher.png","strangedesign/main_clincher_down.png");
		pButton->setPosition(CCPoint(360/2, 350-pButton->getContentSize().height/2));
		pButton->getLabel()->setFontSize(40.0f);
		pButton->setTag(1);

		pCell->addChild(pButton,10);

		CCSprite* sprite = CCSprite::create("strangedesign/table4mul4.png");
		sprite->setContentSize(CCSize(320,320));
		sprite->setPosition(CCPoint(360/2,350/2));
		pCell->addChild(sprite,1);

// 		vector<string> groupCharacter = SQLiteData::getGroupCharacter(DataTool::intTostring(0));
		
		CCLog("idx %d",idx);
		vector<vector<string> > groupCharacter = SQLiteData::getUnit(unit_ids.at(idx));

		CCPoint positions[16] = {ccp(40,280),ccp(120,280),ccp(200,280),ccp(280,280),
			ccp(40,200),ccp(120,200),ccp(200,200),ccp(280,200),
			ccp(40,120),ccp(120,120),ccp(200,120),ccp(280,120),
			ccp(40,40),ccp(120,40),ccp(200,40),ccp(280,40)
		};
		for (int i = 0; i < groupCharacter.size(); i++)
		{
			string hanzi = groupCharacter.at(i).at(0);
			CCLabelTTF* clabel = CCLabelTTF::create(hanzi.c_str(),"Arial",40);
			clabel->setPosition(positions[i]);
			clabel->setColor(ccc3(0,0,0));
			sprite->addChild(clabel);
		}
	}
	else
	{
		pButton = CButton::create("strangedesign/main_clincher.png");
		pButton->setPosition(CCPoint(360/2, 350-pButton->getContentSize().height/2));
		pButton->getLabel()->setFontSize(40.0f);
		pButton->setTag(1);

		pCell->addChild(pButton,10);

		CCSprite* sprite = CCSprite::create("strangedesign/table4mul4.png");
		sprite->setContentSize(CCSize(320,320));
		sprite->setPosition(CCPoint(360/2,350/2));
		pCell->addChild(sprite,1);

		// 		vector<string> groupCharacter = SQLiteData::getGroupCharacter(DataTool::intTostring(0));
		CCLog("idx %d",idx);
		vector<vector<string> > groupCharacter = SQLiteData::getUnit(unit_ids.at(idx));

		CCPoint positions[16] = {ccp(40,280),ccp(120,280),ccp(200,280),ccp(280,280),
			ccp(40,200),ccp(120,200),ccp(200,200),ccp(280,200),
			ccp(40,120),ccp(120,120),ccp(200,120),ccp(280,120),
			ccp(40,40),ccp(120,40),ccp(200,40),ccp(280,40)
		};
		for (int i = 0; i < groupCharacter.size(); i++)
		{
			string hanzi = groupCharacter.at(i).at(0);
			CCLabelTTF* clabel = CCLabelTTF::create(hanzi.c_str(),"Arial",40);
			clabel->setPosition(positions[i]);
			clabel->setColor(ccc3(0,0,0));
			sprite->addChild(clabel);
		}
	}

	char buff[64];
	sprintf(buff, "%u", idx);
	pButton->getLabel()->setString(buff);
	pButton->setUserTag(idx);

	pButton->setOnClickListener(this,ccw_click_selector(MainScene::buttonClick));
	pButton->setOnLongClickListener(this,ccw_longclick_selector(MainScene::buttonLongClick));
	return pCell;
}

void MainScene::buttonClick(CCObject* pSender){
	CButton* pButton = (CButton*)pSender;
	int idx = pButton->getUserTag();
	string id = unit_ids.at(idx);		//将顺序数字转化成数据库中id
	CCDirector::sharedDirector()->replaceScene(WallSingleLayer::scene(id));
}

bool  MainScene::buttonLongClick(CCObject* pSender, CCTouch* pTouch){
	CCLog("Long click");
	//长按进入编辑界面

	CCSprite* backgroundIMG = CCSprite::create("strangedesign/Dlg_background.png");
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	PopCancelLayer* exitDialog = PopCancelLayer::create("strangedesign/Dlg_background.png");
	CCSize originSize = CCSizeMake(winSize.width*0.75,winSize.height*0.5);
	exitDialog->setContentSize(backgroundIMG->getContentSize());
	//exitDialog->setTitle("Setting",50);
	//exitDialog->setContentText("Setting Loose or tight!",60,100,150);
	exitDialog->setCallBackFunc(this,callfuncN_selector(MainScene::dlgCallBack));
	exitDialog->addButton("strangedesign/Dlg_delete_button.png","strangedesign/Dlg_delete_button_down.png","Y",0);
	exitDialog->addButton("strangedesign/Dlg_edit_button.png","strangedesign/Dlg_edit_button_down.png","N",1);
//	exitDialog->addButton("strangedesign/Dlg_cancel_button.png","strangedesign/Dlg_cancel_button_down.png","N",2);
	CCDirector::sharedDirector()->getRunningScene()->addChild(exitDialog,100,TAG_SETTING_LAYER);


// 	CButton* pButton = (CButton*)pSender;
// 	int idx = pButton->getUserTag();
// 	string unitID = unit_ids.at(idx);
// 	CCDirector::sharedDirector()->replaceScene(NewUnitLayer::scene(unitID));
	return true;
}

void MainScene::addButtonCallback(CCObject* pSender){
	CCLog("Add new unit");
	CCDirector::sharedDirector()->replaceScene(NewUnitLayer::scene("add"));
// 	vector<vector<string> > unit;
// 	vector<string> single;
// 	single.push_back(DataTool::getChinese("zi"));
// 	single.push_back(DataTool::intTostring(12));
// 	single.push_back(DataTool::intTostring(13));
// 	for (int i =0 ; i < 16; i++)
// 	{
// 		unit.push_back(single);
// 	}
// 	SQLiteData::insertUnit(unit);

//  	unit_count++;
//  	pGridView->setCountOfCell(unit_count);
//  	unit_ids = SQLiteData::getUnitIDs();
//  	pGridView->reloadData();
}

void MainScene::dlgCallBack(CCNode* pNode){

	CCLog("dlgCallback");

	int tag = pNode->getTag();
	if (tag == 0)
	{

	}else{

	}
	// 	CButton* pButton = (CButton*)pSender;
	// 	int idx = pButton->getUserTag();
	// 	string unitID = unit_ids.at(idx);
	// 	CCDirector::sharedDirector()->replaceScene(NewUnitLayer::scene(unitID));


}