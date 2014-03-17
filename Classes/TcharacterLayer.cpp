#include "TcharacterLayer.h"

TcharacterLayer::TcharacterLayer():m_sprite(NULL){

}

TcharacterLayer::~TcharacterLayer(){
	CC_SAFE_RELEASE(m_sprite);
}

bool TcharacterLayer::init(CCSprite* tianzige){
	if (CCLayer::init())
	{
		CCLog("TcharacterLayer");
		this->setSprite(tianzige);	

		CCSprite* dog = CCSprite::create("dog.png");
		dog->setPosition(CCPointMake(300,300));
		this->addChild(dog,1000,1000);
		return true;
	}
	return false;
}

TcharacterLayer* TcharacterLayer::create(CCSprite* tianzige){
	TcharacterLayer* pret = new TcharacterLayer();
	if (pret && pret->init(tianzige))
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

void TcharacterLayer::onEnter(){

	string ba("八");
	this->m_TDrawnode = TcharacterDrawnode::create(ba,m_sprite->getContentSize());
	this->addChild(m_TDrawnode,2000);
	//不设置Anchorpoint了，直接做坐标变换
	m_TDrawnode->setPosition(m_sprite->getPosition()-ccp(m_sprite->getContentSize().width/2,m_sprite->getContentSize().height/2));
	

}

void TcharacterLayer::onExit(){

}