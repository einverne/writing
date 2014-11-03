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
	//	p(NULL),
	ext_p(NULL),
	csLayer(NULL)
{

}

CeshiScene::CeshiScene(string wallfilename,vector<string> hanzis):backgroundLayer(NULL),
	touchLayer(NULL),
	TLayer(NULL),
	HLayer(NULL),
	//	p(NULL),
	ext_p(NULL),
	csLayer(NULL)
{
	wfilename = wallfilename;
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
	if (hanziList.size() <= 1)
	{
		MyToast::showToast(this,DataTool::getChinese("last_one"),TOAST_LONG);
		return;
	}
	vector<string>::iterator iter = hanziList.begin();

	vector<string>::iterator next;
	string hz = *iter;
	if (currentCharacter == hz)
	{
		 next = hanziList.erase(iter);
	}
	currentCharacter = *next;
	SQLiteData::getHanziDataExtend(currentCharacter,ext_p);
	getTLayer()->setCharacter(currentCharacter);
	getTLayer()->setExChar(ext_p);
	getTLayer()->next();
	getHLayer()->setExChar(ext_p);
	getHLayer()->next();
}