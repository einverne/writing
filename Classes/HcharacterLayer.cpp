#include "HcharacterLayer.h"

HcharacterLayer::HcharacterLayer():m_sprite_draw(NULL)
{
}

HcharacterLayer::~HcharacterLayer()
{
	CC_SAFE_RELEASE(m_sprite_draw);
}

bool HcharacterLayer::init(CCSprite* tianzige_draw){
	if (CCLayer::init())
	{
		this->setSprite(tianzige_draw);
		this->m_HDrawnode = HcharacterDrawnode::create();
		this->addChild(m_HDrawnode);
		return true;
	}
	return false;
}

HcharacterLayer* HcharacterLayer::create(CCSprite* tianzige_draw){
	HcharacterLayer* pret = new HcharacterLayer();
	if (pret && pret->init(tianzige_draw))
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

void HcharacterLayer::judge(vector<CCPoint> points){

	string output = "";
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