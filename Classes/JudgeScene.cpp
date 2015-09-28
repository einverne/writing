#include "JudgeScene.h"
#include "LianxiScene.h"
#include "SQLiteData.h"
#include <algorithm>
#include "MyToast.h"
#include "tools/DataTool.h"
using namespace std;



JudgeScene::JudgeScene():backgroundLayer(NULL),
	touchLayer(NULL),
	TLayer(NULL),
	HLayer(NULL),
	ext_p(NULL),
	judgeLayer(NULL),
	index(0)
{

}

JudgeScene::JudgeScene(string unit_id,vector<string> hanzis):backgroundLayer(NULL),
	touchLayer(NULL),
	TLayer(NULL),
	HLayer(NULL),
	ext_p(NULL),
	judgeLayer(NULL),
	index(0)
{
	this->unit_id = unit_id;
	hanziList = hanzis;
	if (!hanzis.empty())
	{
		currentCharacter = hanzis.at(0);
	}
}

JudgeScene::~JudgeScene()
{
	CC_SAFE_RELEASE(backgroundLayer);
	CC_SAFE_RELEASE(TLayer);
	CC_SAFE_RELEASE(HLayer);
	CC_SAFE_RELEASE(touchLayer);
	CC_SAFE_RELEASE(ext_p);
	CC_SAFE_RELEASE(judgeLayer);
}

JudgeScene* JudgeScene::create(string unit_id,vector<string> hanzis){
	JudgeScene* pRet = new JudgeScene(unit_id,hanzis);
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

bool JudgeScene::init(){
	bool bRet = false;
	do 
	{

		this->setCharacterExt(new CharacterExtend());
		SQLiteData::getHanziDataExtend(currentCharacter,ext_p);
		zi_id = DataTool::intTostring(ext_p->getID()->getValue());

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

		this->setJudgeLayer(JudgeLayer::create());
		CC_BREAK_IF(!judgeLayer);
		judgeLayer->setTag(kCeshiLayerTag);
		this->addChild(judgeLayer);

		CC_BREAK_IF(!CCScene::init());

		bRet = true;
	} while (0);

	return bRet;
}

void JudgeScene::next(){
	//在最后一个字提醒用户最后一个字
	
	index++;
	if (index >= 16)
	{
		//MyToast::showToast(this,DataTool::getChinese("last_one"),TOAST_LONG);
		CCSprite* backgroundIMG = CCSprite::create("strangedesign/Dlg_background_unitfinished.png");
		PopCancelLayer* dlg = PopCancelLayer::create("strangedesign/Dlg_background_unitfinished.png");
		dlg->setContentSize(backgroundIMG->getContentSize());
		dlg->setCallBackFunc(this,callfuncN_selector(JudgeScene::lastCharacter));
		dlg->addButton("strangedesign/Dlg_backfirst_button_up.png","strangedesign/Dlg_backfirst_button_down.png","Y",0);
		dlg->addButton("strangedesign/Dlg_backunits_button_up.png","strangedesign/Dlg_backunits_button_down.png","N",1);
		CCDirector::sharedDirector()->getRunningScene()->addChild(dlg,100,10000);
		index=16;
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

	if (index <= 15)
	{
		currentCharacter = hanziList.at(index);
		SQLiteData::getHanziDataExtend(currentCharacter,ext_p);
		zi_id = DataTool::intTostring(ext_p->getID()->getValue());
		getTLayer()->setCharacter(currentCharacter);
		getTLayer()->setExChar(ext_p);
		getTLayer()->reloadChar();
		getHLayer()->setExChar(ext_p);
		getHLayer()->reloadChar();
	}
}

void JudgeScene::previous(){

	index--;
	if (index<0)
	{
		MyToast::showToast(this,DataTool::getChinese("already_first"),TOAST_LONG);
		index=0;
	}
	if (index>=0)
	{
		currentCharacter = hanziList.at(index);
		SQLiteData::getHanziDataExtend(currentCharacter,ext_p);
		zi_id = DataTool::intTostring(ext_p->getID()->getValue());
		getTLayer()->setCharacter(currentCharacter);
		getTLayer()->setExChar(ext_p);
		getTLayer()->reloadChar();
		getHLayer()->setExChar(ext_p);
		getHLayer()->reloadChar();
	}
}

void JudgeScene::setIsJudge(bool isjudge){
	this->b_isJudge = isjudge;
	getHLayer()->isJudge(isjudge);
}

void JudgeScene::lastCharacter(CCNode* pNode){
	CCLog("lastCharacter");
	if (pNode->getTag() == 0)
	{
		index = -1;
		next();
	}else if (pNode->getTag() == 1)
	{
		this->unscheduleAllSelectors();
		CCDirector::sharedDirector()->getTouchDispatcher()->removeAllDelegates();
		CCDirector::sharedDirector()->popScene();
	}
}