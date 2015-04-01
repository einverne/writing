#include "CeshiScene.h"
#include "LianxiScene.h"
#include "SQLiteData.h"
#include <algorithm>
#include "MyToast.h"
#include "tools\DataTool.h"
using namespace std;



CeshiScene::CeshiScene():backgroundLayer(NULL),
	touchLayer(NULL),
	TLayer(NULL),
	HLayer(NULL),
	ext_p(NULL),
	csLayer(NULL),
	index(0)
{

}

CeshiScene::CeshiScene(string wallfilename,vector<string> hanzis):backgroundLayer(NULL),
	touchLayer(NULL),
	TLayer(NULL),
	HLayer(NULL),
	ext_p(NULL),
	csLayer(NULL),
	index(0)
{
	mwallfilename = wallfilename;
	hanziList = hanzis;
	if (!hanzis.empty())
	{
		currentCharacter = hanzis.at(0);
	}
}

CeshiScene::~CeshiScene()
{
	CC_SAFE_RELEASE(backgroundLayer);
	CC_SAFE_RELEASE(TLayer);
	CC_SAFE_RELEASE(HLayer);
	CC_SAFE_RELEASE(touchLayer);
	CC_SAFE_RELEASE(ext_p);
	CC_SAFE_RELEASE(csLayer);
}

CeshiScene* CeshiScene::create(string wallfilename,vector<string> hanzis){
	CeshiScene* pRet = new CeshiScene(wallfilename,hanzis);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool CeshiScene::init(){
	bool bRet = false;
	do 
	{

		this->setCharacterExt(new CharacterExtend());
		SQLiteData::getHanziDataExtend(currentCharacter,ext_p);

		this->setbackgroundLayer(BackgroundLayer::create());
		CC_BREAK_IF(!backgroundLayer);
		backgroundLayer->setTag(kBgLayerTag);
		this->addChild(backgroundLayer);

		this->setTLayer(TcharacterLayer::create(backgroundLayer->tianzige));
		CC_BREAK_IF(!TLayer);
		TLayer->setTag(kTLayerTag);
		this->addChild(TLayer);
		TLayer->setCharacter(currentCharacter);
		TLayer->setExChar(ext_p);

		this->setHLayer(HcharacterLayer::create(currentCharacter,backgroundLayer->tianzige_draw));
		CC_BREAK_IF(!HLayer);
		HLayer->setTag(kHLayerTag);
		this->addChild(HLayer);
		HLayer->setExChar(ext_p);

		this->settouchLayer(TouchLayer::create(TLayer,HLayer));
		CC_BREAK_IF(!touchLayer);
		touchLayer->setTag(kTouchLayerTag);
		this->addChild(touchLayer);

		this->setceshiLayer(ceshiLayer::create());
		CC_BREAK_IF(!csLayer);
		csLayer->setTag(kCeshiLayerTag);
		this->addChild(csLayer);

		CC_BREAK_IF(!CCScene::init());

		bRet = true;
	} while (0);

	return bRet;
}

void CeshiScene::next(){
	//在最后一个字提醒用户最后一个字
	if (index == 14)
	{
		MyToast::showToast(this,DataTool::getChinese("last_one"),TOAST_LONG);
	}
//	getceshiLayer()->SaveProToFile(getHLayer()->getWrongPercent());

// 	vector<string>::iterator iter = hanziList.begin();
// 
// 	vector<string>::iterator next;
// 	string hz = *iter;
// 	if (currentCharacter == hz)
// 	{
// 		 next = hanziList.erase(iter);		不在list中去除汉字
// 		
// 	}
// 	currentCharacter = *next;
	index++;
	if (index > 15)
	{
		index = index%16;
	}
	currentCharacter = hanziList.at(index);
	SQLiteData::getHanziDataExtend(currentCharacter,ext_p);
	getTLayer()->setCharacter(currentCharacter);
	getTLayer()->setExChar(ext_p);
	getTLayer()->reloadChar();
	getHLayer()->setExChar(ext_p);
	getHLayer()->reloadChar();

}

void CeshiScene::previous(){

	index--;
	if (index<0)
	{
		index = index+16;
	}
	currentCharacter = hanziList.at(index);
	SQLiteData::getHanziDataExtend(currentCharacter,ext_p);
	getTLayer()->setCharacter(currentCharacter);
	getTLayer()->setExChar(ext_p);
	getTLayer()->reloadChar();
	getHLayer()->setExChar(ext_p);
	getHLayer()->reloadChar();
}