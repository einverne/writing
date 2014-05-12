#include "HcharacterLayer.h"
#include "JudgeManager.h"
#include "StrokeDrawnode.h"
#include "Stroke.h"
#include "MoveToRightPlace.h"
#include "TcharacterLayer.h"
#include "MoveToRightPlaceInterval.h"

typedef enum layers
{
	kBgLayerTag,
	kHLayerTag,
	kTLayerTag,
	kTouchLayerTag
};

HcharacterLayer::HcharacterLayer():m_sprite_draw(NULL),
	bihuaCount(NULL),m_HDrawnode(NULL),duicuo(NULL)
{
}

HcharacterLayer::~HcharacterLayer()
{
	CCLog("~~HcharacterLayer %d",  this->m_uReference);
	CC_SAFE_RELEASE(m_sprite_draw);
	CC_SAFE_RELEASE(bihuaCount);
	CC_SAFE_RELEASE(duicuo);
	CC_SAFE_RELEASE(m_HDrawnode);
}

bool HcharacterLayer::init(string hanzi,CCSprite* tianzige_draw){
	if (CCLayer::init())
	{
		this->hanzi = hanzi;
		this->setSprite(tianzige_draw);
		this->setm_HDrawnode(HcharacterDrawnode::create());
		m_HDrawnode->setPosition(m_sprite_draw->getPosition()-ccp(m_sprite_draw->getContentSize().width/2,m_sprite_draw->getContentSize().height/2));
		this->addChild(m_HDrawnode);

		this->setbihuaCount(CCLabelTTF::create("bihua","Arial",50));
		this->addChild(bihuaCount,2000);
		bihuaCount->setPosition(tianzige_draw->getPosition()+ccp(0,tianzige_draw->getContentSize().height/2 + bihuaCount->getContentSize().height));
		
		this->setduicuo(CCLabelTTF::create("wu","Arial",50));
		this->addChild(duicuo,2000);
		duicuo->setPosition(ccp(40,tianzige_draw->getPositionY()));


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
			CCLog("HcharacterLayer::judge x=%f y=%f",temp.x,temp.y);
// 			temp = m_sprite_draw->convertToNodeSpace(temp);
// 			CCPoint deltap = m_sprite_draw->getPosition()-ccp(m_sprite_draw->getContentSize().width/2,m_sprite_draw->getContentSize().height/2);
// 			temp = temp - deltap;
			CCLog("HcharacterLayer::judge convertToNodeSpace x=%f y=%f",temp.x,temp.y);

			temp = convert512(temp);
			string t = floatToString(ceil(temp.x)) + "/" + floatToString(ceil(temp.y)) + "/";
			output += t;	
		}
		output += "@";
	}
// 	for (vector<StrokeDrawnode*>::iterator i = strokes.begin() ; i != strokes.end() ; ++i)
// 	{
// 		StrokeDrawnode* node = *i;
// 		vector<CCPoint> points = node->stroke.pointList;
// 		for (vector<CCPoint>::iterator iter = points.begin(); iter != points.end() ; ++iter)
// 		{
// 			CCPoint temp = *iter;
// 			temp = m_sprite_draw->convertToNodeSpace(temp);
// 			temp = convert512(temp);
// 			string t = floatToString(ceil(temp.x)) + "/" + floatToString(ceil(temp.y)) + "/";
// 			output += t;	
// 		}
// 		output += "@";
// 	}
	CCLog("output %s",output.c_str());
	JudgeManager manager(hanzi);
	string ret = manager.getResult(output);
	if (ret == "0\r\n")
	{
		//ÕâÒ»±ÊÐ´´í
		this->m_HDrawnode->removeLastStroke();
		int t = m_HDrawnode->getStrokeDrawnodeList()->count();
		ostringstream ostr;
		ostr << t;
		bihuaCount->setString(ostr.str().c_str());
		duicuo->setString("Cuowu");
	}else
	{
		//Ð´¶Ô
		int t=m_HDrawnode->getStrokeDrawnodeList()->count();
		ostringstream ostr;
		ostr << t;
		bihuaCount->setString(ostr.str().c_str());
		duicuo->setString("zhengque");

		TcharacterLayer* layer = (TcharacterLayer*)this->getParent()->getChildByTag(kTLayerTag);		//get TcharacterLayer
		Stroke temp = layer->getm_TDrawnode()->getCharacter().getStroke(t);								//get No. stroke
// 		CCPoint deltpoint = this->m_sprite_draw->getPosition()-ccp(m_sprite_draw->getContentSize().width/2,m_sprite_draw->getContentSize().height/2);
// 		temp.addEveryPoint(deltpoint);
// 		MoveToRightPlace* place = MoveToRightPlace::create(t-1,temp);
		MoveToRightPlaceInterval* place = MoveToRightPlaceInterval::create(2,t-1,temp);
		m_HDrawnode->runAction(place);
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
}