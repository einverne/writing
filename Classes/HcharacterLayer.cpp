#include "HcharacterLayer.h"
#include "JudgeManager.h"
#include "StrokeDrawnode.h"
#include "Stroke.h"
#include "MoveToRightPlace.h"
#include "TcharacterLayer.h"
#include "MoveToRightPlaceInterval.h"
#include "SimpleAudioEngine.h"
#include "MyToast.h"
#include "tools\DataTool.h"
#include "constants.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
#define RIGHT_EFFECT_FILE   "right_android.ogg"
#define WRONG_EFFECT_FILE	"wrong_android.ogg"
#elif( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define RIGHT_EFFECT_FILE   "right.wav"
#define WRONG_EFFECT_FILE	"wrong.wav"
#endif


HcharacterLayer::HcharacterLayer():m_sprite_draw(NULL),
	bihuaCount(NULL),m_HDrawnode(NULL),m_sprite_info(NULL),m_exChar(NULL),
	writeCount(0),wrongCount(0),scale(1),ijudge(false)
{
}

HcharacterLayer::~HcharacterLayer()
{
	CCLog("~HcharacterLayer %d",  this->m_uReference);
	CC_SAFE_RELEASE(m_sprite_draw);
	CC_SAFE_RELEASE(bihuaCount);
	CC_SAFE_RELEASE(m_HDrawnode);
	CC_SAFE_RELEASE(m_sprite_info);
	CC_SAFE_RELEASE(m_exChar);
}

bool HcharacterLayer::init(string hanzi,CCSprite* tianzige_draw){
	if (CCLayer::init())
	{
		CCSize visiableSize = CCDirector::sharedDirector()->getVisibleSize();

		this->hanzi = hanzi;
		this->setSprite(tianzige_draw);
		this->setm_HDrawnode(HcharacterDrawnode::create());
		m_HDrawnode->setPosition(m_sprite_draw->getPosition());
		getm_HDrawnode()->setScale(scale);
		getm_HDrawnode()->setAnchorPoint(ccp(0.5,0.5));
		addChild(m_HDrawnode);

		this->setbihuaCount(CCLabelTTF::create("","Arial",50));
		getbihuaCount()->setColor(ccc3(0,0,0));
		this->addChild(bihuaCount,10);
		bihuaCount->setPosition(ccp(visiableSize.width/4*3+100,getbihuaCount()->getContentSize().height/2+60));
		
		this->setInfoSprite(CCSprite::create("right.png"));
		this->addChild(getInfoSprite(),2000);
		getInfoSprite()->setPosition(ccp(visiableSize.width/4-100,getInfoSprite()->getContentSize().height/2+60));
		getInfoSprite()->setScale(scale);
		getInfoSprite()->setVisible(false);

		CCPoint tianzige_draw_position = getSprite()->getPosition();
		CCSize tianzige_draw_size = getSprite()->getContentSize();

// 		CCMenuItemImage* rewriteButton = CCMenuItemImage::create("strangedesign/Judge_writting_cancel_button.png",
// 			"strangedesign/Judge_writting_cancel_button.png",
// 			this,
// 			menu_selector(HcharacterLayer::rewrite));
// 		CCPoint rewrite_position = ccp(tianzige_draw_position.x + tianzige_draw_size.width/2+20+rewriteButton->getContentSize().width/2,tianzige_draw_position.y+tianzige_draw_size.height/2-rewriteButton->getContentSize().height/2);
// 		rewriteButton->setPosition(rewrite_position);
// 		CCMenu* menu = CCMenu::create(rewriteButton,NULL);
// 		menu->setPosition(0,0);
// 		this->addChild(menu,200);

		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(RIGHT_EFFECT_FILE);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(WRONG_EFFECT_FILE);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);

		//放大缩小按钮
		CButton* zoomin = CButton::create("strangedesign/Judge_writting_zoomin_button.png");
		zoomin->setPosition(ccp(visiableSize.width/4,zoomin->getContentSize().height+50));
		zoomin->setOnClickListener(this,ccw_click_selector(HcharacterLayer::zoomin));

		CButton* zoomout = CButton::create("strangedesign/Judge_writting_zoomout_button.png");
		zoomout->setPosition(ccp(visiableSize.width/4*2,zoomout->getContentSize().height+50));
		zoomout->setOnClickListener(this,ccw_click_selector(HcharacterLayer::zoomout));

		CButton* rewrite = CButton::create("strangedesign/Judge_writting_cancel_button.png");
		rewrite->setPosition(ccp(visiableSize.width/4*3,rewrite->getContentSize().height+50));
		rewrite->setOnClickListener(this,ccw_click_selector(HcharacterLayer::rewrite));

		CWidgetWindow* m_pWindow = CWidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow,10);

		m_pWindow->addChild(zoomin);
		m_pWindow->addChild(zoomout);
		m_pWindow->addChild(rewrite);

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

void HcharacterLayer::judge(){
	CCArray* strokes = m_HDrawnode->getStrokeDrawnodeList();
	string output = "";
	CCObject* ob;
	CCARRAY_FOREACH(strokes,ob){
		StrokeDrawnode* node = (StrokeDrawnode*)ob;
		vector<CCPoint> points = node->getStroke().getpointList();
		for (vector<CCPoint>::iterator iter = points.begin(); iter != points.end() ; ++iter)
		{
			CCPoint temp = *iter;
			temp = convert512(temp);
			string t = DataTool::floatToString(ceil(temp.x)) + "/" + DataTool::floatToString(ceil(temp.y)) + "/";
			output += t;
		}
		output += "@";
	}

	CCLog("output %s",output.c_str());
// 	CharacterEntity* p =  ((LianxiScene*)this->getParent())->getCharacterP();
// 	CharacterExtend* p = ((LianxiScene*)this->getParent())->getCharacterExt();
// 	string funcs = ((LianxiScene*)this->getParent())->funcs;
//	string ret = JudgeManager::getResult(hanzi,output,p,funcs);
// 	string funcs = ((LianxiScene*)this->getParent())->funcs;
	string funcs = "";
	TcharacterLayer* tlayer = (TcharacterLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(kTLayerTag);
	string points = tlayer->getm_TDrawnode()->getCharacterStandardInfo();
	string ret = JudgeManager::getResult(hanzi,output,points,m_exChar,funcs);
	CCLog("Hcharacterlay: ret : %s %d",ret.c_str(),ret.length());
	//如果不评判则跳过
	if (!ijudge)
	{
		return;
	}
	if (ret.length() == 3)
	{
		if (ret.at(0) == '0')
		{
			//这一笔写错
			MyToast::showToast(this,DataTool::getChinese("stroke_wrong"),TOAST_LONG);

			writeWrong();
			
		}else if(ret.at(0) == '1'){
			//写对
			MyToast::showToast(this,DataTool::getChinese("stroke_right"),TOAST_LONG);

			writeRight();
		}
	}else if(ret.length() == 4){
		if (ret.at(0) == '0')
		{
			//这一笔写错 或者 位置不对
			MyToast::showToast(this,DataTool::getChinese("stroke_wrong"),TOAST_LONG);
			writeWrong();
			
		}else if(ret.at(0) == '1' && ret.at(1) == '0'){
			MyToast::showToast(this,DataTool::getChinese("position_wrong"),TOAST_LONG);
			writeWrong();
		}else if(ret.at(1) == '1' && ret.at(0) == '1')
		{
			MyToast::showToast(this,DataTool::getChinese("position_right"),TOAST_LONG);
			writeRight();
		}
	}
}

void HcharacterLayer::writeWrong(){

	this->getm_HDrawnode()->removeLastStroke();
	int t = getm_HDrawnode()->getStrokeDrawnodeList()->count();
	bihuaCount->setString(DataTool::intTostring(t).c_str());
	getInfoSprite()->setVisible(true);
	getInfoSprite()->setTexture(CCTextureCache::sharedTextureCache()->addImage("wrong.png"));
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(WRONG_EFFECT_FILE);
	writeCount++;
	wrongCount++;
}

void HcharacterLayer::writeRight(){
	int t = getm_HDrawnode()->getStrokeDrawnodeList()->count();
	bihuaCount->setString(DataTool::intTostring(t).c_str());
	TcharacterLayer* layer = (TcharacterLayer*)this->getParent()->getChildByTag(kTLayerTag);		//get TcharacterLayer
	Stroke temp = layer->getm_TDrawnode()->getCharacter().getStroke(t);								//get No. stroke
	MoveToRightPlaceInterval* place = MoveToRightPlaceInterval::create(1,t-1,temp);
	m_HDrawnode->runAction(place);

	if ( t >= layer->getm_TDrawnode()->getCharacter().getStrokeCount())
	{
		getInfoSprite()->setVisible(true);
		getInfoSprite()->setTexture(CCTextureCache::sharedTextureCache()->addImage("right.png"));
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(RIGHT_EFFECT_FILE);
	}
	writeCount++;
}

CCPoint HcharacterLayer::convert512(CCPoint p){
	CCSize size = this->m_sprite_draw->getContentSize();
	float fx = p.x * 512 / size.width;
	float fy = p.y * 512 / size.height;
	return ccp(fx,-(fy-512));
}

void HcharacterLayer::onEnter(){
	CCLayer::onEnter();
}

void HcharacterLayer::onExit(){
	CCLayer::onEnter();
}

void HcharacterLayer::rewrite(CCObject* pSender){
	CCLog("HcharacterLayer::rewrite");
	if (this->getActionManager()->numberOfRunningActionsInTarget(getm_HDrawnode()) <= 0)
	{
		this->getm_HDrawnode()->rewrite();
		this->getbihuaCount()->setString("0");
		getInfoSprite()->setVisible(false);
	}
}

void HcharacterLayer::zoomin(CCObject* pSender){
	if (scale < 2.0)
	{
		scale += 0.1;
		getm_HDrawnode()->setScale(scale);
	}
}

void HcharacterLayer::zoomout(CCObject* pSender){
	if (scale > 0.5)
	{
		scale -= 0.1;
		getm_HDrawnode()->setScale(scale);
	}
}

void HcharacterLayer::reloadChar(){
	this->removeChild(getm_HDrawnode());
	this->setm_HDrawnode(HcharacterDrawnode::create());
	m_HDrawnode->setPosition(m_sprite_draw->getPosition());
	m_HDrawnode->setAnchorPoint(ccp(0.5,0.5));
	m_HDrawnode->setScale(scale);
	this->addChild(m_HDrawnode);
	rewrite(this);
}

float HcharacterLayer::getWrongPercent(){
	if (writeCount == 0)
	{
		return 0;
	}
	return (float)wrongCount/writeCount;
}

void HcharacterLayer::isJudge(bool isjudge){
	this->ijudge = isjudge;
}