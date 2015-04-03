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
	CeshiScene(string unit_id,vector<string> hanzis);
	~CeshiScene();
	static CeshiScene* create(string unit_id,vector<string> hanzis);
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
	bool getJudge()	{ return b_isJudge; }

	string getUnitID()	{ return unit_id;}
	string getZiID()	{ return zi_id; }
	string getCharacter()		{ return currentCharacter;}

private:
	string unit_id;						//保存单元id
	string zi_id;
	vector<string> hanziList;			//保存汉字列表
	string currentCharacter;			//当前书写汉字
	int index;							//保存当前汉字序号
	bool b_isJudge;						//当前Scene是否判断 false 为自由书写
};




#endif