#include "JudgeLayer.h"
#include "JudgeScene.h"
#include "tinyxml.h"
#include "tools/DataTool.h"

JudgeLayer::JudgeLayer()
{
}

JudgeLayer::~JudgeLayer()
{
}

bool JudgeLayer::init(){
	CCLog("ceshiLayer::init()");
	if (CCLayer::init())
	{

		return true;
	}
	return false;
}

void JudgeLayer::onEnter(){
	CCLayer::onEnter();

	BackgroundLayer* backgroundLayer = (BackgroundLayer*)this->getParent()->getChildByTag(kBgLayerTag);
	CCSprite* tianzige = backgroundLayer->tianzige;
	CCSprite* title_bar = backgroundLayer->title_bar;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCMenuItemImage* next = CCMenuItemImage::create("strangedesign/Writting_next_button.png",
		"strangedesign/Writting_next_button_down.png",
		this,
		menu_selector(JudgeLayer::menuNext));
	CCPoint nextBtnPosition = ccp(winSize.width - next->getContentSize().width/2-50,tianzige->getPositionY()+tianzige->getContentSize().height/2+20);
	next->setPosition(nextBtnPosition);

	CCMenuItemImage* previous = CCMenuItemImage::create("strangedesign/Writting_pre_button.png",
		"strangedesign/Writting_pre_button_down.png",
		this,
		menu_selector(JudgeLayer::menuPrevious));
	CCPoint previousBtnPosition = ccp(previous->getContentSize().width/2+50 , tianzige->getPositionY()+ tianzige->getContentSize().height/2 + 20);
	previous->setPosition(previousBtnPosition);

	menu = CCMenu::create(next, previous, NULL);

	addChild(menu,10);
	menu->setPosition(CCPointZero);

	JudgeScene* scene = (JudgeScene*)this->getParent();
	if (!scene->getIsJudge())
	{
		//自由书写 需增加Button 作为保存
		CCMenuItemImage* saveButton = CCMenuItemImage::create("strangedesign/Free_writting_save_button_up.png",
			"strangedesign/Free_writting_save_button_down.png",
			this,
			menu_selector(JudgeLayer::menuSave));
		saveButton->setPosition(ccp(winSize.width/5*4,saveButton->getContentSize().height+5));
		menu->addChild(saveButton);

		//view button
		CCMenuItemImage* viewBtn = CCMenuItemImage::create("strangedesign/Free_writting_view_button_up.png",
			"strangedesign/Free_writting_view_button_down.png",
			this,
			menu_selector(JudgeLayer::menuView));
		viewBtn->setPosition(ccp(winSize.width-viewBtn->getContentSize().width/2, title_bar->getPositionY()));
		menu->addChild(viewBtn);
	}
}

void JudgeLayer::menuNext(CCObject* pSender){
	CCLog("next");
	JudgeScene* scene = (JudgeScene*)CCDirector::sharedDirector()->getRunningScene();
	scene->next();
}

void JudgeLayer::menuPrevious(CCObject* pSender){
	//调用上一个
	CCLog("previous");
	JudgeScene* scene = (JudgeScene*)CCDirector::sharedDirector()->getRunningScene();
	scene->previous();
}

void JudgeLayer::menuSave(CCObject* pSender){
	CCLog("save to sqlite");
	JudgeScene* scene = (JudgeScene*)CCDirector::sharedDirector()->getRunningScene();

	//判断是否写完
	if (scene->gettouchLayer()->isTimeSave())
	{
		string unit_id = scene->getUnitID();
		string zi_id = scene->getZiID();
		string note = scene->getHLayer()->getPointsOutput();

		SQLiteData::insertNote(unit_id,zi_id,note);
		MyToast::showToast(this,DataTool::getChinese("insert_into_sqlite"),TOAST_LONG);

	}else
	{
		MyToast::showToast(this,DataTool::getChinese("not_finish"),TOAST_LONG);
	}

}

void JudgeLayer::menuView(CCObject* pSender){

	CCLog("menuView");
}

void JudgeLayer::SaveProToFile(float pro){
	//filename hanzi ceshiScene  profic 计算
	JudgeScene* scene = (JudgeScene*)CCDirector::sharedDirector()->getRunningScene();
// 	string wallXMLCurrent = scene->getWallFileName();
	string currhanzi = scene->getCharacter();

	string profic = DataTool::floatToString(pro);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
// 	string wallpath = "wall/" + wallXMLCurrent;
// 	string myfilename=CCFileUtils::sharedFileUtils()->fullPathForFilename(wallpath.c_str());
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//	string myfilename = CCFileUtils::sharedFileUtils()->getWritablePath()+wallXMLCurrent;
#endif
// 	TiXmlDocument* myDocument = new TiXmlDocument(myfilename.c_str());
// 	myDocument->LoadFile();
//
// 	TiXmlElement* rootEle = myDocument->RootElement();
// 	TiXmlElement* meta = rootEle->FirstChildElement();
// 	TiXmlElement* data = meta->NextSiblingElement();
// 	TiXmlElement* stone = data->FirstChildElement();
// 	while (stone)
// 	{
// 		TiXmlElement* type = stone->FirstChildElement();
// 		string text(type->GetText());
// 		string wordbox("wordbox");
// 		if (text == wordbox)
// 		{
// 			TiXmlElement* hanzi = stone->FirstChildElement("hanzi");
// 			TiXmlElement* proficiency = stone->FirstChildElement("proficiency");
// 			string text = hanzi->GetText();
// 			if (text == currhanzi)
// 			{
// 				TiXmlElement* new_pro = new TiXmlElement("proficiency");
// 				TiXmlText* newproText = new TiXmlText(profic.c_str());
// 				new_pro->InsertEndChild(*newproText);
// 				stone->RemoveChild(proficiency);
// 				stone->InsertAfterChild(hanzi,*new_pro);
// 				break;
// 			}
// 		}
// 		stone = stone->NextSiblingElement();
// 	}
// 	bool ret = myDocument->SaveFile(myfilename.c_str());

}
