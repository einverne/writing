#include "LianxiScene.h"

LianxiScene::LianxiScene()
{
}

LianxiScene::LianxiScene(string hanzi)
{
	this->testCharacter = hanzi;
}

LianxiScene::~LianxiScene()
{
}

LianxiScene* LianxiScene::create(string hanzi){
	LianxiScene* pRet = new LianxiScene(hanzi);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}else{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool LianxiScene::init(){
	bool bRet = false;
	do 
	{
		

		CC_BREAK_IF(!CCScene::init());
		touchLayer = TouchLayer::create();
		CC_BREAK_IF(!touchLayer);
		this->addChild(touchLayer);

		backgroundLayer = BackgroundLayer::create();
		CC_BREAK_IF(!backgroundLayer);
		this->addChild(backgroundLayer);

		TLayer = TcharacterLayer::create();
		CC_BREAK_IF(!TLayer);
		TLayer->setSprite(backgroundLayer->tianzige);			//将背景层中tianzige传给正字信息图层
		this->addChild(TLayer);

		HLayer = HcharacterLayer::create();
		CC_BREAK_IF(!HLayer);
		this->addChild(HLayer);

		bRet = true;
	} while (0);
	return bRet;
}