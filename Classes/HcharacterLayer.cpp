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

void HcharacterLayer::onEnter(){

}

void HcharacterLayer::onExit(){

}