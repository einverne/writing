#include "LianxiScene.h"
#include "CharacterEntity.h"
#include "SQLiteData.h"
#include <algorithm>
using namespace std;

LianxiScene::LianxiScene():backgroundLayer(NULL),
	touchLayer(NULL),
	TLayer(NULL),
	HLayer(NULL)
{
	ext_p = new CharacterExtend();
}

LianxiScene::LianxiScene(string hanzi):backgroundLayer(NULL),
	touchLayer(NULL),
	TLayer(NULL),
	HLayer(NULL)
{
	CurrentCharacter = hanzi;
	ext_p = new CharacterExtend();
}

LianxiScene::~LianxiScene()
{
	CCLog("~~LianxiScene %d",  m_uReference);
	CC_SAFE_RELEASE(backgroundLayer);
	CC_SAFE_RELEASE(TLayer);
	CC_SAFE_RELEASE(HLayer);
	CC_SAFE_RELEASE(touchLayer);
	delete ext_p;
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
		setCharacterExt(new CharacterExtend());
		SQLiteData::getHanziDataExtend(CurrentCharacter,ext_p);

		setbackgroundLayer(BackgroundLayer::create());
		CC_BREAK_IF(!backgroundLayer);
		backgroundLayer->setTag(kBgLayerTag);
		addChild(backgroundLayer);

		setTLayer(TcharacterLayer::create(backgroundLayer->tianzige));
		CC_BREAK_IF(!TLayer);
		TLayer->setTag(kTLayerTag);
		addChild(TLayer);
		TLayer->setCharacter(CurrentCharacter);
		TLayer->setExChar(ext_p);

		setHLayer(HcharacterLayer::create(CurrentCharacter,backgroundLayer->tianzige_draw));
		CC_BREAK_IF(!HLayer);
		HLayer->setTag(kHLayerTag);
		addChild(HLayer);
		HLayer->setExChar(ext_p);
		
		settouchLayer(TouchLayer::create(TLayer,HLayer));
		CC_BREAK_IF(!touchLayer);
		touchLayer->setTag(kTouchLayerTag);
		addChild(touchLayer);


		CC_BREAK_IF(!CCScene::init());
		bRet = true;
	} while (0);

//	SQLiteData::getHanziData(CurrentCharacter,p);

// 	CCString* temp = ext_p->getSEQ();
// 	CCLog("seq %s",temp->getCString());
// 	string str(temp->getCString());
// 	vector<string> strvec = SQLiteData::splitStrokeSeq(str);
// 	//去重
// 	std::sort(strvec.begin(),strvec.end());
// 	strvec.erase(std::unique(strvec.begin(),strvec.end()),strvec.end());
// 	vector<string>::iterator iter = strvec.begin(),iter2 = strvec.end();
// 
// 	while (iter != iter2)
// 	{
// 		funcs += SQLiteData::getstrokeFunc(*iter);
// 		funcs += "\n";
// 		iter ++;
// 	}



	return bRet;
}
