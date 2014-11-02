#include "TcharacterLayer.h"
#include "LianxiScene.h"
#include "RightZiAnimationAction.h"
#include "CharacterEntity.h"

TcharacterLayer::TcharacterLayer():m_sprite(NULL),
	m_TDrawnode(NULL),isPause(false)
{

}

TcharacterLayer::~TcharacterLayer(){
	CCLog("~TcharacterLayer %d",this->m_uReference);
	CC_SAFE_RELEASE(m_sprite);
	CC_SAFE_RELEASE(m_TDrawnode);
}

bool TcharacterLayer::init(CCSprite* tianzige){
	if (CCLayer::init())
	{
		CCLog("TcharacterLayer init");
		this->setSprite(tianzige);	

 		CCPoint tiangzige_position = this->getSprite()->getPosition();
 		CCSize tianzige_size = this->getSprite()->getContentSize();

 		CCMenuItemImage* refreshButton = CCMenuItemImage::create("replay.png",
 			"replay.png",
 			this,
 			menu_selector(TcharacterLayer::refresh));
 		CCPoint refresh_position = ccp(tiangzige_position.x+tianzige_size.width/2+20+refreshButton->getContentSize().width/2,tiangzige_position.y+tianzige_size.height/2-refreshButton->getContentSize().height/2);
 		refreshButton->setPosition(refresh_position);

 		CCMenu* menu = CCMenu::create(refreshButton,NULL);
 		menu->setPosition(0,0);
 		this->addChild(menu,2000);
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
	CCLog("TcharacterLayer::onEnter");
	CCLayer::onEnter();
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	string LianxiHanzi = ((LianxiScene*)scene)->CurrentCharacter;
//	CharacterEntity* p = ((LianxiScene*)this->getParent())->getCharacterP();
	CharacterExtend* p = ((LianxiScene*)this->getParent())->getCharacterExt();
	this->setm_TDrawnode(TcharacterDrawnode::create(LianxiHanzi, m_sprite->getContentSize(), p));
	this->addChild(m_TDrawnode,2000);
	//不设置Anchorpoint了，直接做坐标变换
	m_TDrawnode->setPosition(m_sprite->getPosition()-ccp(m_sprite->getContentSize().width/2,m_sprite->getContentSize().height/2));
	
	CCLog("TcharacterLayer::onEnter end");
}

void TcharacterLayer::onExit(){
	CCLayer::onExit();
}

void TcharacterLayer::refresh(CCObject* pSender){
	CCLog("TcharacterLayer::refresh");
	RightZiAnimationAction* animation = RightZiAnimationAction::create(5.0);
	if (m_TDrawnode->getActionManager()->numberOfRunningActionsInTarget(m_TDrawnode) <= 0)
	{
		m_TDrawnode->runAction(animation);
	}else if(!isPause)
	{
		this->getActionManager()->pauseTarget(m_TDrawnode);
		isPause = true;
	}else if (isPause)
	{
		this->getActionManager()->resumeTarget(m_TDrawnode);
		isPause = false;
	}

}
