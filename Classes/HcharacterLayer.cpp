#include "HcharacterLayer.h"
#include "JudgeManager.h"
#include "StrokeDrawnode.h"
#include "Stroke.h"
#include "MoveToRightPlace.h"
#include "TcharacterLayer.h"
#include "MoveToRightPlaceInterval.h"
#include "LianxiScene.h"
#include "SimpleAudioEngine.h"

typedef enum layers
{
	kBgLayerTag,
	kHLayerTag,
	kTLayerTag,
	kTouchLayerTag
};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
#define RIGHT_EFFECT_FILE   "right_android.ogg"
#define WRONG_EFFECT_FILE	"wrong_android.ogg"
#elif( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define RIGHT_EFFECT_FILE   "right.wav"
#define WRONG_EFFECT_FILE	"wrong.wav"
#endif


HcharacterLayer::HcharacterLayer():m_sprite_draw(NULL),
	bihuaCount(NULL),m_HDrawnode(NULL),m_sprite_info(NULL)
{
}

HcharacterLayer::~HcharacterLayer()
{
	CCLog("~HcharacterLayer %d",  this->m_uReference);
	CC_SAFE_RELEASE(m_sprite_draw);
	CC_SAFE_RELEASE(bihuaCount);
	CC_SAFE_RELEASE(m_HDrawnode);
	CC_SAFE_RELEASE(m_sprite_info);
}

bool HcharacterLayer::init(string hanzi,CCSprite* tianzige_draw){
	if (CCLayer::init())
	{
		this->hanzi = hanzi;
		this->setSprite(tianzige_draw);
		this->setm_HDrawnode(HcharacterDrawnode::create());
		m_HDrawnode->setPosition(m_sprite_draw->getPosition()-ccp(m_sprite_draw->getContentSize().width/2,m_sprite_draw->getContentSize().height/2));
		this->addChild(m_HDrawnode);

		this->setbihuaCount(CCLabelTTF::create("bihua","Marker Felt",50));
		this->addChild(bihuaCount,2000);
		bihuaCount->setPosition(tianzige_draw->getPosition()+ccp(0,tianzige_draw->getContentSize().height/2 + bihuaCount->getContentSize().height));
		
		this->setInfoSprite(CCSprite::create("right.png"));
		this->addChild(getInfoSprite(),2000);
		getInfoSprite()->setPosition(ccp(10+getInfoSprite()->getContentSize().width/2,tianzige_draw->getPositionY()));
		getInfoSprite()->setScale(0.6);
		getInfoSprite()->setVisible(false);

		CCPoint tianzige_draw_position = getSprite()->getPosition();
		CCSize tianzige_draw_size = getSprite()->getContentSize();

		CCMenuItemImage* rewriteButton = CCMenuItemImage::create("rewrite.png",
			"rewrite.png",
			this,
			menu_selector(HcharacterLayer::rewrite));
		CCPoint rewrite_position = ccp(tianzige_draw_position.x + tianzige_draw_size.width/2+20+rewriteButton->getContentSize().width/2,tianzige_draw_position.y+tianzige_draw_size.height/2-rewriteButton->getContentSize().height/2);
		rewriteButton->setPosition(rewrite_position);
		CCMenu* menu = CCMenu::create(rewriteButton,NULL);
		menu->setPosition(0,0);
		this->addChild(menu,200);



		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(RIGHT_EFFECT_FILE);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(WRONG_EFFECT_FILE);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);

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
// 	vector<StrokeDrawnode*> strokes = m_HDrawnode->getStrokeDrawnodeList();
	CCArray* strokes = m_HDrawnode->getStrokeDrawnodeList();
	string output = "";
	CCObject* ob;
	CCARRAY_FOREACH(strokes,ob){
		StrokeDrawnode* node = (StrokeDrawnode*)ob;
		vector<CCPoint> points = node->getStroke().pointList;
		for (vector<CCPoint>::iterator iter = points.begin(); iter != points.end() ; ++iter)
		{
			CCPoint temp = *iter;
//			CCLog("HcharacterLayer::judge x=%f y=%f",temp.x,temp.y);
// 			temp = m_sprite_draw->convertToNodeSpace(temp);
// 			CCPoint deltap = m_sprite_draw->getPosition()-ccp(m_sprite_draw->getContentSize().width/2,m_sprite_draw->getContentSize().height/2);
// 			temp = temp - deltap;
//			CCLog("HcharacterLayer::judge convertToNodeSpace x=%f y=%f",temp.x,temp.y);

			temp = convert512(temp);
			string t = floatToString(ceil(temp.x)) + "/" + floatToString(ceil(temp.y)) + "/";
			output += t;
		}
		output += "@";
	}

	CCLog("output %s",output.c_str());
	CharacterEntity* p =  ((LianxiScene*)this->getParent())->getCharacterP();
	string funcs = ((LianxiScene*)this->getParent())->funcs;
	string ret = JudgeManager::getResult(hanzi,output,p,funcs);
	if (ret == "0\r\n")
	{
		//ÕâÒ»±ÊÐ´´í
		this->getm_HDrawnode()->removeLastStroke();
		int t = getm_HDrawnode()->getStrokeDrawnodeList()->count();
		ostringstream ostr;
		ostr << t;
		bihuaCount->setString(ostr.str().c_str());
		getInfoSprite()->setVisible(true);
		getInfoSprite()->setTexture(CCTextureCache::sharedTextureCache()->addImage("wrong.png"));
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(WRONG_EFFECT_FILE);
	}else
	{
		//Ð´¶Ô
		int t=getm_HDrawnode()->getStrokeDrawnodeList()->count();
		ostringstream ostr;
		ostr << t;
		bihuaCount->setString(ostr.str().c_str());
		
		
		TcharacterLayer* layer = (TcharacterLayer*)this->getParent()->getChildByTag(kTLayerTag);		//get TcharacterLayer
		Stroke temp = layer->getm_TDrawnode()->getCharacter().getStroke(t);								//get No. stroke
// 		CCPoint deltpoint = this->m_sprite_draw->getPosition()-ccp(m_sprite_draw->getContentSize().width/2,m_sprite_draw->getContentSize().height/2);
// 		temp.addEveryPoint(deltpoint);
// 		MoveToRightPlace* place = MoveToRightPlace::create(t-1,temp);
		MoveToRightPlaceInterval* place = MoveToRightPlaceInterval::create(1,t-1,temp);
		m_HDrawnode->runAction(place);


		if ( t >= layer->getm_TDrawnode()->getCharacter().getStrokeCount())
		{
			getInfoSprite()->setVisible(true);
			getInfoSprite()->setTexture(CCTextureCache::sharedTextureCache()->addImage("right.png"));
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(RIGHT_EFFECT_FILE);
		}
	}
}

CCPoint HcharacterLayer::convert512(CCPoint p){
	CCSize size = this->m_sprite_draw->getContentSize();
	float fx = p.x * 512 / size.width;
	float fy = p.y * 512 / size.height;
	return ccp(fx,-(fy-512));
}

string HcharacterLayer::floatToString(float f){
	ostringstream buff;
	buff<<f;
	return buff.str();
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
