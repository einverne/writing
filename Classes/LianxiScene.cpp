#include "LianxiScene.h"

LianxiScene::LianxiScene(string hanzi):backgroundLayer(NULL),
	touchLayer(NULL),
	TLayer(NULL),
	HLayer(NULL)
{
	this->testCharacter = hanzi;
}

LianxiScene::~LianxiScene()
{
	CCLog("~~LianxiScene %d",  this->m_uReference);
	CC_SAFE_RELEASE(backgroundLayer);
	CC_SAFE_RELEASE(TLayer);
	CC_SAFE_RELEASE(HLayer);
	CC_SAFE_RELEASE(touchLayer);
}

LianxiScene* LianxiScene::create(string hanzi){
	LianxiScene* pRet = new LianxiScene(hanzi);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}else{
        CC_SAFE_DELETE(pRet);        
		return NULL;
	}
}

bool LianxiScene::init(){
	bool bRet = false;
	do
	{
		this->setbackgroundLayer(BackgroundLayer::create());
		CC_BREAK_IF(!backgroundLayer);
		this->addChild(backgroundLayer);

		this->setTLayer(TcharacterLayer::create(backgroundLayer->tianzige));
		CC_BREAK_IF(!TLayer);
		//TLayer->setSprite(backgroundLayer->tianzige);			将背景层中tianzige传给正字信息图层
		this->addChild(TLayer);

		this->setHLayer(HcharacterLayer::create(testCharacter,backgroundLayer->tianzige_draw));
		CC_BREAK_IF(!HLayer);
		this->addChild(HLayer);
		
		this->settouchLayer(TouchLayer::create(TLayer,HLayer));
		CC_BREAK_IF(!touchLayer);
		this->addChild(touchLayer);

		CCLog("LianxiScene ref: %d",this->m_uReference);

		CC_BREAK_IF(!CCScene::init());
		bRet = true;
	} while (0);
	return bRet;
}