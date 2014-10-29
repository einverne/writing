#include "CeshiScene.h"
#include "LianxiScene.h"
#include "SQLiteData.h"
#include <algorithm>
using namespace std;

typedef enum layers
{
	kBgLayerTag,
	kHLayerTag,
	kTLayerTag,
	kTouchLayerTag
};


CeshiScene::CeshiScene():backgroundLayer(NULL),
	touchLayer(NULL),
	TLayer(NULL),
	HLayer(NULL),
	ext_p(NULL)
{

}

CeshiScene::~CeshiScene()
{
	CC_SAFE_RELEASE(backgroundLayer);
	CC_SAFE_RELEASE(TLayer);
	CC_SAFE_RELEASE(HLayer);
	CC_SAFE_RELEASE(touchLayer);
	CC_SAFE_RELEASE(ext_p);
}

CeshiScene* CeshiScene::create(string wallfilename,vector<string> hanzis){
	CeshiScene* pRet = new CeshiScene();
	if (pRet && pRet->init(wallfilename,hanzis))
	{
		pRet->autorelease();
		return pRet;
	}
	else{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool CeshiScene::init(string wallfilename,vector<string> hanzis){
	bool bRet = false;
	do 
	{
		wallfilename = wallfilename;
		hanzis = hanzis;

		if (!hanzis.empty())
		{
			curhanzi = hanzis.at(0);
		}
		this->setbackgroundLayer(BackgroundLayer::create());
		CC_BREAK_IF(!backgroundLayer);
		backgroundLayer->setTag(kBgLayerTag);
		this->addChild(backgroundLayer);

		this->setTLayer(TcharacterLayer::create(backgroundLayer->tianzige));
		CC_BREAK_IF(!TLayer);
		TLayer->setTag(kTLayerTag);
		this->addChild(TLayer);

		this->setHLayer(HcharacterLayer::create(curhanzi,backgroundLayer->tianzige_draw));
		CC_BREAK_IF(!HLayer);
		HLayer->setTag(kHLayerTag);
		this->addChild(HLayer);

		this->settouchLayer(TouchLayer::create(TLayer,HLayer));
		CC_BREAK_IF(!touchLayer);
		touchLayer->setTag(kTouchLayerTag);
		this->addChild(touchLayer);

		this->setCharacterExt(new CharacterExtend());
		


		CC_BREAK_IF(!CCScene::init());
		bRet = true;
	} while (0);
	SQLiteData::getHanziDataExtend(this->curhanzi,ext_p);

	CCString* temp = ext_p->getSEQ();
	CCLog("seq %s",temp->getCString());
	string str(temp->getCString());
	vector<string> strvec = SQLiteData::splitStrokeSeq(str);
	//ШЅжи
	std::sort(strvec.begin(),strvec.end());
	strvec.erase(std::unique(strvec.begin(),strvec.end()),strvec.end());
	vector<string>::iterator iter = strvec.begin(),iter2 = strvec.end();

	while (iter != iter2)
	{
		funcs += SQLiteData::getstrokeFunc(*iter);
		funcs += "\n";
		iter ++;
	}

	return bRet;
}

void CeshiScene::onEnter(){
	CCScene::onEnter();
}

void CeshiScene::onExit(){
	CCScene::onExit();
}