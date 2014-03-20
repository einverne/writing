#include "HcharacterLayer.h"
#include "JudgeManager.h"
#include "StrokeDrawnode.h"
#include "Stroke.h"

HcharacterLayer::HcharacterLayer():m_sprite_draw(NULL)
{
}

HcharacterLayer::~HcharacterLayer()
{
	CC_SAFE_RELEASE(m_sprite_draw);
}

bool HcharacterLayer::init(string hanzi,CCSprite* tianzige_draw){
	if (CCLayer::init())
	{
		this->hanzi = hanzi;
		this->setSprite(tianzige_draw);
		this->m_HDrawnode = HcharacterDrawnode::create();
		this->addChild(m_HDrawnode);
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
	vector<StrokeDrawnode*> strokes = m_HDrawnode->getStrokeDrawnodeList();
	string output = "";
	for (vector<StrokeDrawnode*>::iterator i = strokes.begin() ; i != strokes.end() ; ++i)
	{
		StrokeDrawnode* node = *i;
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
	CCLog("output %s",output.c_str());
	JudgeManager manager(hanzi);
	string ret = manager.getResult(output);
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