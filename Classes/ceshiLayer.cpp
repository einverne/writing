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
	if (CCLayer::init())
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCMenuItemImage* next = CCMenuItemImage::create("dog.png",
			"dog.png",
			this,
			menu_selector(ceshiLayer::menuNext));
		next->setPosition(ccp(winSize.width - next->getContentSize().width/2-50,winSize.height/2));

		CCMenuItemImage* previous = CCMenuItemImage::create("dog.png",
			"dog.png",
			this,
			menu_selector(ceshiLayer::menuPrevious));
		previous->setPosition(ccp(previous->getContentSize().width/2+50 , winSize.height/2));

		CCMenu* menu = CCMenu::create(next, previous, NULL);
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

void ceshiLayer::menuPrevious(CCObject* pSender){
	//调用上一个
	CCLog("previous");
	CeshiScene* scene = (CeshiScene*)CCDirector::sharedDirector()->getRunningScene();
	scene->previous();
}

void ceshiLayer::SaveProToFile(float pro){
	//filename hanzi ceshiScene  profic 计算
	CeshiScene* scene = (CeshiScene*)CCDirector::sharedDirector()->getRunningScene();
	string wallXMLCurrent = scene->getWallFileName();
	string currhanzi = scene->getCharacter();

	string profic = DataTool::floatToString(pro);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	string wallpath = "wall/" + wallXMLCurrent;
	string myfilename=CCFileUtils::sharedFileUtils()->fullPathForFilename(wallpath.c_str());
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	string myfilename = CCFileUtils::sharedFileUtils()->getWritablePath()+wallXMLCurrent;
#endif
	TiXmlDocument* myDocument = new TiXmlDocument(myfilename.c_str());
	myDocument->LoadFile();

	TiXmlElement* rootEle = myDocument->RootElement();
	TiXmlElement* meta = rootEle->FirstChildElement();
	TiXmlElement* data = meta->NextSiblingElement();
	TiXmlElement* stone = data->FirstChildElement();
	while (stone)
	{
		TiXmlElement* type = stone->FirstChildElement();
		string text(type->GetText());
		string wordbox("wordbox");
		if (text == wordbox)
		{
			TiXmlElement* hanzi = stone->FirstChildElement("hanzi");
			TiXmlElement* proficiency = stone->FirstChildElement("proficiency");
			string text = hanzi->GetText();
			if (text == currhanzi)
			{
				TiXmlElement* new_pro = new TiXmlElement("proficiency");
				TiXmlText* newproText = new TiXmlText(profic.c_str());
				new_pro->InsertEndChild(*newproText);
				stone->RemoveChild(proficiency);
				stone->InsertAfterChild(hanzi,*new_pro);
				break;
			}
		}
		stone = stone->NextSiblingElement();
	}
	bool ret = myDocument->SaveFile(myfilename.c_str());

}