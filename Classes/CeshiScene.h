#ifndef __CeshiScene_H__
#define __CeshiScene_H__

#include "cocos2d.h"
#include "BackgroundLayer.h"
#include "TcharacterLayer.h"
#include "HcharacterLayer.h"	
#include "TouchLayer.h"
#include "CharacterExtend.h"
#include "LianxiScene.h"
USING_NS_CC;
using namespace std;

class CeshiScene : public LianxiScene
{
public:
	CeshiScene();
	CeshiScene(string wallfilename,vector<string> hanzis);
	~CeshiScene();
	static CeshiScene* create(string wallfilename,vector<string> hanzis);
	bool init();



private:
	string wallfilename;
	vector<string> hanzis;
	string curhanzi;

};




#endif