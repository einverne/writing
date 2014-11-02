#include "CeshiScene.h"
#include "LianxiScene.h"
#include "SQLiteData.h"
#include <algorithm>
using namespace std;



CeshiScene::CeshiScene()
{

}

CeshiScene::CeshiScene(string wallfilename,vector<string> hanzis){
	wallfilename = wallfilename;
	hanzis = hanzis;
}

CeshiScene::~CeshiScene()
{

}

CeshiScene* CeshiScene::create(string wallfilename,vector<string> hanzis){
	CeshiScene* pRet = new CeshiScene(wallfilename,hanzis);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool CeshiScene::init(){
	bool bRet = false;
	do 
	{


		if (!hanzis.empty())
		{
			curhanzi = hanzis.at(0);
		}
		
		LianxiScene::init();
		
		bRet = true;
	} while (0);

	return bRet;
}
