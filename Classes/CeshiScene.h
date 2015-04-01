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
	virtual bool init();

	CC_SYNTHESIZE_RETAIN(BackgroundLayer*,backgroundLayer,backgroundLayer);
	CC_SYNTHESIZE_RETAIN(TouchLayer*,touchLayer,touchLayer);
	CC_SYNTHESIZE_RETAIN(TcharacterLayer*,TLayer,TLayer);
	CC_SYNTHESIZE_RETAIN(HcharacterLayer*,HLayer,HLayer);
	CC_SYNTHESIZE_RETAIN(CharacterExtend*,ext_p,CharacterExt);

	CC_SYNTHESIZE_RETAIN(ceshiLayer* , csLayer , ceshiLayer);

	void previous();
	void next();
	void setJudge(bool isjudge);

	string getWallFileName()	{ return mwallfilename;}
	string getCharacter()		{ return currentCharacter;}

private:
	string mwallfilename;
	vector<string> hanziList;			//保存汉字列表
	string currentCharacter;			//当前书写汉字
	int index;							//保存当前汉字序号
};




#endif