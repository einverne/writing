#include "HcharacterLayer.h"
#include "JudgeManager.h"
#include "StrokeDrawnode.h"
#include "Stroke.h"
#include "UTF8ToGBK.h"

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
		this->addChild(m_HDrawnode);


		this->setbihuaCount(CCLabelTTF::create(UTF8ToGBK::UTF8TOGBK(string("±Ê»­")).c_str(),"Arial",50));
		this->addChild(bihuaCount,2000);
		bihuaCount->setPosition(tianzige_draw->getPosition()+ccp(0,tianzige_draw->getContentSize().height/2 + bihuaCount->getContentSize().height));
		
		this->setduicuo(CCLabelTTF::create(UTF8ToGBK::UTF8TOGBK(string("")).c_str(),"Arial",50));
		this->addChild(duicuo,2000);
		duicuo->setPosition(ccp(40,tianzige_draw->getPositionY()));
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
		vector<CCPoint> points = node->stroke.pointList;
		for (vector<CCPoint>::iterator iter = points.begin(); iter != points.end() ; ++iter)
		{
			CCPoint temp = *iter;
			temp = m_sprite_draw->convertToNodeSpace(temp);
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
		bihuaCount->setString((UTF8ToGBK::UTF8TOGBK(string("±Ê»­:")) + to_string(m_HDrawnode->getStrokeDrawnodeList()->count())).c_str());
		duicuo->setString(UTF8ToGBK::UTF8TOGBK(string("´íÎó")).c_str());
	}else
	{
		//Ð´¶Ô
		bihuaCount->setString((UTF8ToGBK::UTF8TOGBK(string("±Ê»­:")) + to_string(m_HDrawnode->getStrokeDrawnodeList()->count())).c_str());
		duicuo->setString(UTF8ToGBK::UTF8TOGBK(string("ÕýÈ·")).c_str());
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

}

void HcharacterLayer::onExit(){

}