#ifndef __CeshiScene_H__
#define __CeshiScene_H__

#include "cocos2d.h"
#include "BackgroundLayer.h"
#include "TcharacterLayer.h"
#include "HcharacterLayer.h"	
#include "TouchLayer.h"
#include "CharacterExtend.h"
#include "ceshiLayer.h"
USING_NS_CC;
using namespace std;

class CeshiScene : public CCScene
{
public:
	CeshiScene();
	CeshiScene(string wallfilename,vector<string> hanzis);
	~CeshiScene();
	static CeshiScene* create(string wallfilename,vector<string> hanzis);
	bool init();

	CC_SYNTHESIZE_RETAIN(BackgroundLayer*,backgroundLayer,backgroundLayer);
	CC_SYNTHESIZE_RETAIN(TouchLayer*,touchLayer,touchLayer);
	CC_SYNTHESIZE_RETAIN(TcharacterLayer*,TLayer,TLayer);
	CC_SYNTHESIZE_RETAIN(HcharacterLayer*,HLayer,HLayer);
	//	CC_SYNTHESIZE_RETAIN(CharacterEntity*,p,CharacterP);
	CC_SYNTHESIZE_RETAIN(CharacterExtend*,ext_p,CharacterExt);

	CC_SYNTHESIZE_RETAIN(ceshiLayer* , csLayer , ceshiLayer);

	void next();

private:
	string wfilename;
	vector<string> hanziList;
	string currentCharacter;

};




#endif