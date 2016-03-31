#include "HcharacterLayer.h"
#include "StrokeDrawnode.h"
#include "Stroke.h"
#include "MoveToRightPlace.h"
#include "TcharacterLayer.h"
#include "MoveToRightPlaceInterval.h"
#include "SimpleAudioEngine.h"
#include "MyToast.h"
#include "tools/DataTool.h"
#include "constants.h"
#include "JudgeScene.h"
#include "Error.h"
#include <iomanip>

#include "../rapidjson/document.h"

using namespace rapidjson;

HcharacterLayer::HcharacterLayer():m_sprite_draw(NULL),
	bihuaCountAndTotal(NULL),
	m_HDrawnode(NULL),
	m_sprite_info(NULL),
	_writeCount(0),
	_wrongCount(0),
	_ijudge(false),
	totalBihuaCount(0),
	scoreLabel(NULL),
	score(0.0),
	curBihuaWrong(0)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
#define RIGHT_EFFECT_FILE   "right_android.ogg"
#define WRONG_EFFECT_FILE	"wrong_android.ogg"
#elif( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define RIGHT_EFFECT_FILE   "right.wav"
#define WRONG_EFFECT_FILE	"wrong.wav"
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define RIGHT_EFFECT_FILE   "right_android.ogg"
#define WRONG_EFFECT_FILE	"wrong_android.ogg"
#endif
	_scale=1.6f;
}

HcharacterLayer::~HcharacterLayer()
{
	CCLog("~HcharacterLayer %d",  this->m_uReference);
	CC_SAFE_RELEASE(m_sprite_draw);
	CC_SAFE_RELEASE(bihuaCountAndTotal);
	CC_SAFE_RELEASE(m_HDrawnode);
	CC_SAFE_RELEASE(m_sprite_info);
	CC_SAFE_RELEASE(scoreLabel);
}

bool HcharacterLayer::init(string hanzi,CCSprite* tianzige_draw){
	if (CCLayer::init())
	{
		this->_hanzi = hanzi;
		this->setSprite(tianzige_draw);
		this->setm_HDrawnode(HcharacterDrawnode::create());
		m_HDrawnode->setPosition(m_sprite_draw->getPosition());
		getm_HDrawnode()->setScale(_scale);
		getm_HDrawnode()->setAnchorPoint(ccp(0.5,0.5));
		addChild(m_HDrawnode);

		_manager.initLuaEngine();
		return true;
	}
	return false;
}

HcharacterLayer* HcharacterLayer::create(string hanzi,CCSprite* tianzige_draw){
	HcharacterLayer* pret = new HcharacterLayer();
	if (pret && pret->init(hanzi,tianzige_draw))
	{
		pret->autorelease();
		return pret;
	}else
	{
		delete pret;
		pret = NULL;
		return pret;
	}
}

void HcharacterLayer::onEnter(){
	CCLayer::onEnter();

	TcharacterLayer* layer = (TcharacterLayer*)this->getParent()->getChildByTag(kTLayerTag);		//get TcharacterLayer
	totalBihuaCount = layer->getm_TDrawnode()->getCharacter().getStrokeCount();

	CCSize visiableSize = CCDirector::sharedDirector()->getVisibleSize();

	JudgeScene* scene = (JudgeScene*)this->getParent();
	BackgroundLayer* backgroundLayer = scene->getbackgroundLayer();
	CCSprite* tianzige = backgroundLayer->tianzige;

	setInfoSprite(CCSprite::create("right.png"));
	addChild(getInfoSprite(),2000);
	getInfoSprite()->setPosition(ccp(visiableSize.width/4*3+50,tianzige->getPositionY()-50));
	getInfoSprite()->setVisible(false);

	CCPoint tianzige_draw_position = getSprite()->getPosition();
	CCSize tianzige_draw_size = getSprite()->getContentSize();

	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(RIGHT_EFFECT_FILE);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(WRONG_EFFECT_FILE);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);

	//放大缩小按钮
	CButton* zoomin = CButton::create("strangedesign/Judge_writting_zoomin_button.png",
		"strangedesign/Judge_writting_zoomin_button_down.png");
	zoomin->setPosition(ccp(visiableSize.width/4,zoomin->getContentSize().height+5));
	zoomin->setOnClickListener(this,ccw_click_selector(HcharacterLayer::zoomin));

	CButton* zoomout = CButton::create("strangedesign/Judge_writting_zoomout_button.png",
		"strangedesign/Judge_writting_zoomout_button_down.png");
	zoomout->setPosition(ccp(visiableSize.width/4*2,zoomout->getContentSize().height+5));
	zoomout->setOnClickListener(this,ccw_click_selector(HcharacterLayer::zoomout));

	CButton* rewriteBtn = CButton::create("strangedesign/Judge_writting_cancel_button.png",
		"strangedesign/Judge_writting_cancel_button_down.png");
	rewriteBtn->setPosition(ccp(visiableSize.width/4*3,rewriteBtn->getContentSize().height+5));
	rewriteBtn->setOnClickListener(this,ccw_click_selector(HcharacterLayer::rewrite));

	if (scene->getIsJudge() == false)
	{
		zoomin->setPosition(ccp(visiableSize.width/5,zoomin->getContentSize().height+5));
		zoomout->setPosition(ccp(visiableSize.width/5*2,zoomout->getContentSize().height+5));
		rewriteBtn->setPosition(ccp(visiableSize.width/5*3,rewriteBtn->getContentSize().height+5));

	}else{
		string countAndTotal = DataTool::getChinese("bihuashu")+"0/"+ DataTool::intTostring(totalBihuaCount);
		setbihuaCountAndTotal(CCLabelTTF::create(countAndTotal.c_str(),"Arial",40));
		getbihuaCountAndTotal()->setColor(ccc3(0,0,0));
		addChild(getbihuaCountAndTotal(),10);
		CCPoint bihuaCountPosition = ccp(visiableSize.width/4-70, tianzige->getPositionY()-50);
		getbihuaCountAndTotal()->setPosition(bihuaCountPosition);

		JudgeScene* scene = (JudgeScene*)this->getParent();
		string unit_id = scene->getUnitID();
		string curChar = scene->getCurChar();
		vector< vector<string> > groupUnit = SQLiteData::getUnit(unit_id);
		string scorestr;
		for (unsigned int i = 0; i < groupUnit.size(); i++)
		{
			if (curChar == groupUnit[i][0])
			{
				scorestr = groupUnit[i][2];
				break;
			}
		}
		float scoretemp = DataTool::stringToFloat(scorestr);
		string scorestring = DataTool::getChinese("defen")+DataTool::to_string_with_precision(scoretemp);
		setscoreLabel(CCLabelTTF::create(scorestring.c_str(),"Arial",40));
		getscoreLabel()->setColor(ccBLACK);
		addChild(getscoreLabel(),10);
		CCPoint scoreLabelPos = ccp(visiableSize.width/4-70, tianzige->getPositionY()+50);
		getscoreLabel()->setPosition(scoreLabelPos);
	}

	CWidgetWindow* m_pWindow = CWidgetWindow::create();
	m_pWindow->setMultiTouchEnabled(true);
	addChild(m_pWindow,10);

	m_pWindow->addChild(zoomin);
	m_pWindow->addChild(zoomout);
	m_pWindow->addChild(rewriteBtn);
}

void HcharacterLayer::onExit(){
	CCLayer::onEnter();
	_manager.exitLuaEngine();
}

void HcharacterLayer::judge(){
	CCArray* strokes = m_HDrawnode->getStrokeDrawnodeList();
	string output = "";					// 手写汉字的点集信息

	CCObject* originob;
	CCARRAY_FOREACH(strokes,originob){
		StrokeDrawnode* node = (StrokeDrawnode*)originob;
		Stroke stro = node->getStroke();
		this->pointsOrigin += stro.sendOutput();
	}

	CCObject* ob;
	CCARRAY_FOREACH(strokes,ob){
		StrokeDrawnode* node = (StrokeDrawnode*)ob;
		Stroke stro = node->getStroke();
		vector<CCPoint> points = stro.getpointList();
		stro.convert512(this->m_sprite_draw->getContentSize());
		stro.resample(100);
		output += stro.sendOutput();
	}
	this->pointsOutput=output;

// 	CharacterEntity* p =  ((LianxiScene*)this->getParent())->getCharacterP();
// 	CharacterExtend* p = ((LianxiScene*)this->getParent())->getCharacterExt();
// 	string funcs = ((LianxiScene*)this->getParent())->funcs;
//	string ret = JudgeManager::getResult(hanzi,output,p,funcs);
// 	string funcs = ((LianxiScene*)this->getParent())->funcs;
	string funcs = "";
	TcharacterLayer* tlayer = (TcharacterLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(kTLayerTag);
	string points = tlayer->getm_TDrawnode()->getCharacterStandardInfo();		//获取正字信息
    CCLog("output %s",output.c_str());
    CCLog("right Character info %s",points.c_str());

	// 松弛匹配
	//schar.divide();

    
	string ret = _manager.getResult(_hanzi,output,points,m_exChar,funcs);
	//CCLog("Hcharacterlay: retstring:%s length:%d",ret.c_str(),ret.length());
	//如果不评判则跳过
	if (!_ijudge || ret.length() <= 0)
	{
		return;
	}
	CCLog("%s\n", ret.c_str());

	//ret = "{\"ret\":\"111\",\"error\": [] }";
	//ret = "{\"error\":[{\"errortype\":\"B0001\",\"errorstroke\":{}}],\"ret\":\"011\"}";
	//ret = "{\"ret\":\"101\",\"error\":[{\"errortype\":\"A0001\",\"errorstroke\":{\"0\":\"0.2\",\"1\":\"0.3\"}},{\"errortype\":\"A0021\",\"errorstroke\":{\"0\":\"0.2\",\"1\":\"0.3\"}}]}";

	// parse result get from lua
	rapidjson::Document doc;
	doc.Parse<kParseDefaultFlags>(ret.c_str());
	vector<Error> errors;
	if (doc.HasMember("error"))
	{
		Error oneError;
		const Value& errorjson = doc["error"];
		for (SizeType i = 0 ; i < errorjson.Size(); ++i)
		{
			if(errorjson[i].HasMember("errortype")){
				string errortype = errorjson[i]["errortype"].GetString();
				oneError.errortype = errortype;
			}
			if(errorjson[i].HasMember("errorstroke")){
				const Value& v = errorjson[i]["errorstroke"];
				for (Value::ConstMemberIterator iter = v.MemberonBegin(); iter != v.MemberonEnd(); ++iter)
				{
					string key = iter->name.GetString();
					string value = iter->value.GetString();
					int n_key = DataTool::stringToInt(key);
					float f_value = DataTool::stringToFloat(value);
					oneError.errorstroke.insert(make_pair(n_key,f_value));
				}
			}
			errors.push_back(oneError);
		}
	}
	if(doc.HasMember("ret")){
		string retjson = doc["ret"].GetString();
		if (retjson.length() == 3)
		{
			if (retjson.at(0) == '1')
			{
				// whether a stroke is right
				Right();
			}else{
				Wrong();
				if (errors.size()>0)
				{
					string type = errors.at(0).errortype;
					MyToast::showToast(this,DataTool::getChinese(type),TOAST_LONG);
				}
			}
			if (retjson.at(1) == '1')
			{
				// whether a radical is right
				//MyToast::showToast(this,"radical right",TOAST_LONG);
			}else
			{
				//MyToast::showToast(this,"radical wrong",TOAST_LONG);
				if (errors.size()>0)
				{
					string type = errors.at(0).errortype;
					MyToast::showToast(this,DataTool::getChinese(type),TOAST_LONG);
				}
			}
		}
	}

	//if (ret.length() == 3)
	//{
	//	if (ret.at(0) == '0')
	//	{
	//		//这一笔写错
	//		MyToast::showToast(this,DataTool::getChinese("stroke_wrong"),TOAST_LONG);
	//		
	//		Wrong();
	//		
	//	}else if(ret.at(0) == '1'){
	//		//写对
	//		//MyToast::showToast(this,DataTool::getChinese("stroke_right"),TOAST_LONG);

	//		Right();
	//	}
	//}else if(ret.length() == 4){
	//	if (ret.at(0) == '0')
	//	{
	//		//这一笔写错 或者 位置不对
	//		MyToast::showToast(this,DataTool::getChinese("stroke_wrong"),TOAST_LONG);
	//		Wrong();
	//		
	//	}else if(ret.at(0) == '1' && ret.at(1) == '0'){
	//		MyToast::showToast(this,DataTool::getChinese("position_wrong"),TOAST_LONG);
	//		Wrong();
	//	}else if(ret.at(1) == '1' && ret.at(0) == '1')
	//	{
	//		//MyToast::showToast(this,DataTool::getChinese("position_right"),TOAST_LONG);
	//		Right();
	//	}
	//}
}

void HcharacterLayer::Wrong(){

	this->getm_HDrawnode()->removeLastStroke();
	int t = getm_HDrawnode()->getStrokeDrawnodeList()->count();
	string strToshow = DataTool::getChinese("bihuashu")+DataTool::intTostring(t)+"/"+DataTool::intTostring(totalBihuaCount);
	bihuaCountAndTotal->setString(strToshow.c_str());
	//getInfoSprite()->setVisible(true);
	//getInfoSprite()->setTexture(CCTextureCache::sharedTextureCache()->addImage("wrong.png"));
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(WRONG_EFFECT_FILE);
	_writeCount++;
	_wrongCount++;
	curBihuaWrong++;

	string strLabel = DataTool::getChinese("defen")+DataTool::to_string_with_precision(score);
	getscoreLabel()->setString(strLabel.c_str());

}

void HcharacterLayer::Right(){
	// set stroke count Label
	int t = getm_HDrawnode()->getStrokeDrawnodeList()->count();
	string strToshow = DataTool::getChinese("bihuashu")+DataTool::intTostring(t)+"/"+DataTool::intTostring(totalBihuaCount);
	getbihuaCountAndTotal()->setString(strToshow.c_str());

	// Actions
	TcharacterLayer* layer = (TcharacterLayer*)this->getParent()->getChildByTag(kTLayerTag);		//get TcharacterLayer
	//Stroke temp = layer->getm_TDrawnode()->getCharacter().getStroke(t);								//get No. stroke
	//MoveToRightPlaceInterval* place = MoveToRightPlaceInterval::create(1,t-1,temp);
	//m_HDrawnode->runAction(place);

	if ( t >= layer->getm_TDrawnode()->getCharacter().getStrokeCount())
	{
		getInfoSprite()->setVisible(true);
		getInfoSprite()->setTexture(CCTextureCache::sharedTextureCache()->addImage("right.png"));
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(RIGHT_EFFECT_FILE);
	}

	// calculate score
	switch (curBihuaWrong)
	{
	case 0:
		score+=10.0/totalBihuaCount;
		break;
	case 1:
		score+=0.75*10/totalBihuaCount;
		break;
	case 2:
		score+=0.5*10/totalBihuaCount;
		break;
	case 3:
		score+=0.25*10/totalBihuaCount;
		break;
	default:
		score+=0*10/totalBihuaCount;
		break;
	}
	string strLabel = DataTool::getChinese("defen")+DataTool::to_string_with_precision(score);
	getscoreLabel()->setString(strLabel.c_str());
	curBihuaWrong=0;
	_writeCount++;

	// update Character score into db
	JudgeScene* scene = (JudgeScene*)this->getParent();
	string unit_id = scene->getUnitID();
	string curChar = scene->getCurChar();

	vector<vector<string> > groupUnit = SQLiteData::getUnit(unit_id);
	for (unsigned int i = 0; i < groupUnit.size(); i++)
	{
		if (curChar == groupUnit[i][0])
		{
			string scorestr = DataTool::to_string(score);
			groupUnit[i][2]=scorestr;
			break;
		}
	}
	SQLiteData::updateUnit(unit_id,groupUnit);

}



void HcharacterLayer::rewrite(CCObject* pSender){
	CCLog("HcharacterLayer::rewrite");
	if (this->getActionManager()->numberOfRunningActionsInTarget(getm_HDrawnode()) <= 0)
	{
		_manager.exitLuaEngine();
		_manager.initLuaEngine();
		this->getm_HDrawnode()->rewrite();
		JudgeScene* scene = (JudgeScene*)this->getParent();
		if (scene->getIsJudge()==true)
		{
			string strToshow = DataTool::getChinese("bihuashu")+"0/"+ DataTool::intTostring(totalBihuaCount);
			this->getbihuaCountAndTotal()->setString(strToshow.c_str());
			string unit_id = scene->getUnitID();
			string curChar = scene->getCurChar();
			vector< vector<string> > groupUnit = SQLiteData::getUnit(unit_id);
			string scorestr;
			for (unsigned int i = 0; i < groupUnit.size(); i++)
			{
				if (curChar == groupUnit[i][0])
				{
					scorestr = groupUnit[i][2];
					break;
				}
			}
			float scoretemp = DataTool::stringToFloat(scorestr);
			string scorestring = DataTool::getChinese("defen")+DataTool::to_string_with_precision(scoretemp);
			getscoreLabel()->setString(scorestring.c_str());
			score=0;
			curBihuaWrong=0;
		}
		getInfoSprite()->setVisible(false);
	}
}

void HcharacterLayer::clearWriting(){
	rewrite(this);
}

void HcharacterLayer::zoomin(CCObject* pSender){
	if (_scale < 1.6)
	{
		_scale += 0.1f;
		getm_HDrawnode()->setScale(_scale);
	}
}

void HcharacterLayer::zoomout(CCObject* pSender){
	if (_scale > 0.5)
	{
		_scale -= 0.1f;
		getm_HDrawnode()->setScale(_scale);
	}
}

void HcharacterLayer::reloadChar(){
	this->removeChild(getm_HDrawnode());
	this->setm_HDrawnode(HcharacterDrawnode::create());
	m_HDrawnode->setPosition(m_sprite_draw->getPosition());
	m_HDrawnode->setAnchorPoint(ccp(0.5,0.5));
	m_HDrawnode->setScale(_scale);
	this->addChild(m_HDrawnode);
	TcharacterLayer* layer = (TcharacterLayer*)this->getParent()->getChildByTag(kTLayerTag);		//get TcharacterLayer
	this->totalBihuaCount = layer->getm_TDrawnode()->getCharacter().getStrokeCount();
	curBihuaWrong=0;
	score=0;
	rewrite(this);
}

float HcharacterLayer::getWrongPercent(){
	if (_writeCount == 0)
	{
		return 0;
	}
	return (float)_wrongCount/_writeCount;
}

void HcharacterLayer::isJudge(bool isjudge){
	this->_ijudge = isjudge;
}
