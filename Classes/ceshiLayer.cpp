#include "ceshiLayer.h"
#include "CeshiScene.h"
#include "tinyxml.h"
#include "tools/DataTool.h"

ceshiLayer::ceshiLayer()
{
}

ceshiLayer::~ceshiLayer()
{
}

bool ceshiLayer::init(){
	CCLog("ceshiLayer::init()");
	if (CCLayer::init())
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCMenuItemImage* next = CCMenuItemImage::create("strangedesign/Writting_next_button.png",
			"strangedesign/Writting_next_button_down.png",
			this,
			menu_selector(ceshiLayer::menuNext));
		next->setPosition(ccp(winSize.width - next->getContentSize().width/2-50,winSize.height/2));

		CCMenuItemImage* previous = CCMenuItemImage::create("strangedesign/Writting_pre_button.png",
			"strangedesign/Writting_pre_button_down.png",
			this,
			menu_selector(ceshiLayer::menuPrevious));
		previous->setPosition(ccp(previous->getContentSize().width/2+50 , winSize.height/2));

		menu = CCMenu::create(next, previous, NULL);


		
		addChild(menu,10);
		menu->setPosition(CCPointZero);

		return true;
	}
	return false;
}

void ceshiLayer::onEnter(){
	CCLayer::onEnter();
	CeshiScene* scene = (CeshiScene*)this->getParent();
	if (!scene->getJudge())
	{
		//自由书写 需增加Button 作为保存
		CCMenuItemImage* saveButton = CCMenuItemImage::create("strangedesign/Edit_Finish_button.png",
			"strangedesign/Edit_Finish_button_down.png",
			this,
			menu_selector(ceshiLayer::menuSave));
		saveButton->setPosition(ccp(650,500));
		menu->addChild(saveButton);
	}
}

void ceshiLayer::menuNext(CCObject* pSender){
	CCLog("next");
	CeshiScene* scene = (CeshiScene*)CCDirector::sharedDirector()->getRunningScene();
	scene->next();
}

void ceshiLayer::menuPrevious(CCObject* pSender){
	//调用上一个
	CCLog("previous");
	CeshiScene* scene = (CeshiScene*)CCDirector::sharedDirector()->getRunningScene();
	scene->previous();
}

void ceshiLayer::menuSave(CCObject* pSender){
	CCLog("save to sqlite");
	CeshiScene* scene = (CeshiScene*)CCDirector::sharedDirector()->getRunningScene();

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

void ceshiLayer::SaveProToFile(float pro){
	//filename hanzi ceshiScene  profic 计算
	CeshiScene* scene = (CeshiScene*)CCDirector::sharedDirector()->getRunningScene();
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
