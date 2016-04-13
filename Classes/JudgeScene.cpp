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
	judgeLayer(NULL),
	index_(0)
{
	ext_p_ = new CharacterExtend();
}

JudgeScene::JudgeScene(string unit_id,vector<string> hanzis, int start_index):backgroundLayer(NULL),
	touchLayer(NULL),
	TLayer(NULL),
	HLayer(NULL),
	judgeLayer(NULL)
{
	ext_p_ = new CharacterExtend();
	this->unit_id_ = unit_id;
	this->hanzi_list_ = hanzis;
    this->index_ = start_index;
    cur_char_ = hanzi_list_.at(start_index);
}

JudgeScene::~JudgeScene()
{
	CC_SAFE_RELEASE(backgroundLayer);
	CC_SAFE_RELEASE(TLayer);
	CC_SAFE_RELEASE(HLayer);
	CC_SAFE_RELEASE(touchLayer);
	CC_SAFE_RELEASE(judgeLayer);
}

JudgeScene* JudgeScene::create(string unit_id,vector<string> hanzis, int start_index){
	JudgeScene* pRet = new JudgeScene(unit_id,hanzis, start_index);
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
        SQLiteData::getHanziDataExtend(cur_char_,ext_p_);
		CReadXML xml = CReadXML(ext_p_->getXML().c_str());
		Character char_for_radical = xml.getCharacter();
		vector<string> radicalrules = SQLiteData::getRadicalRules(char_for_radical.getRadicalSeq());
		string unit_rules;
		for (vector<string>::const_iterator iter = radicalrules.begin(); iter != radicalrules.end(); iter++)
		{
			unit_rules+="\n"+(*iter);
		}
		ext_p_->setRuleUnit(unit_rules);
		zi_id_ = DataTool::intTostring(ext_p_->getID());

		this->setbackgroundLayer(BackgroundLayer::create());
		CC_BREAK_IF(!backgroundLayer);
		backgroundLayer->setTag(kBgLayerTag);
		this->addChild(backgroundLayer);

		this->setTLayer(TcharacterLayer::create(backgroundLayer->tianzige_));
		CC_BREAK_IF(!TLayer);
		TLayer->setTag(kTLayerTag);
		this->addChild(TLayer);
		TLayer->setCharacter(cur_char_);
		TLayer->setCharacter(ext_p_);

		this->setHLayer(HcharacterLayer::create(cur_char_,backgroundLayer->tianzige_draw_));
		CC_BREAK_IF(!HLayer);
		HLayer->setTag(kHLayerTag);
		this->addChild(HLayer);
		HLayer->SetExChar(ext_p_);

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
	
	index_++;
	if (index_ >= 16)
	{
		//MyToast::showToast(this,DataTool::getChinese("last_one"),TOAST_LONG);
		CCSprite* backgroundIMG = CCSprite::create("strangedesign/Dlg_background_unitfinished.png");
		PopCancelLayer* dlg = PopCancelLayer::create("strangedesign/Dlg_background_unitfinished.png");
		dlg->setContentSize(backgroundIMG->getContentSize());
		dlg->setCallBackFunc(this,callfuncN_selector(JudgeScene::lastCharacter));
		dlg->addButton("strangedesign/Dlg_backfirst_button_up.png","strangedesign/Dlg_backfirst_button_down.png","Y",0);
		dlg->addButton("strangedesign/Dlg_backunits_button_up.png","strangedesign/Dlg_backunits_button_down.png","N",1);
		CCDirector::sharedDirector()->getRunningScene()->addChild(dlg,100,10000);
		index_=16;
	}
	if (index_ <= 15)
	{
		cur_char_ = hanzi_list_.at(index_);
		SQLiteData::getHanziDataExtend(cur_char_,ext_p_);
		zi_id_ = DataTool::intTostring(ext_p_->getID());
		getTLayer()->setCharacter(cur_char_);
		getTLayer()->setCharacter(ext_p_);
		getTLayer()->reloadChar();
		getHLayer()->SetExChar(ext_p_);
		getHLayer()->ReloadChar();
	}
}

void JudgeScene::previous(){
	index_--;
	if (index_<0)
	{
		MyToast::showToast(this,DataTool::getChinese("already_first"),TOAST_LONG);
		index_=0;
	}
	if (index_>=0)
	{
		cur_char_ = hanzi_list_.at(index_);
		SQLiteData::getHanziDataExtend(cur_char_,ext_p_);
		zi_id_ = DataTool::intTostring(ext_p_->getID());
		getTLayer()->setCharacter(cur_char_);
		getTLayer()->setCharacter(ext_p_);
		getTLayer()->reloadChar();
		getHLayer()->SetExChar(ext_p_);
		getHLayer()->ReloadChar();
	}
}

void JudgeScene::setIsJudge(bool isjudge){
	this->is_judge = isjudge;
	getHLayer()->IsJudge(isjudge);
}

void JudgeScene::lastCharacter(CCNode* pNode){
	CCLog("lastCharacter");
	if (pNode->getTag() == 0)
	{
		index_ = -1;
		next();
	}else if (pNode->getTag() == 1)
	{
		this->unscheduleAllSelectors();
		CCDirector::sharedDirector()->getTouchDispatcher()->removeAllDelegates();
		CCDirector::sharedDirector()->popScene();
	}
}