#ifndef __LianxiScene_H__
#define __LianxiScene_H__

#include "cocos2d.h"
#include "TouchLayer.h"
#include "BackgroundLayer.h"
#include "TcharacterLayer.h"
#include "HcharacterLayer.h"
USING_NS_CC;


class LianxiScene : public CCScene
{
public:
	LianxiScene(string hanzi);
	~LianxiScene();
	//CREATE_FUNC(LianxiScene);
	static LianxiScene* create(string hanzi);
	virtual bool init();

	string testCharacter;			//当前正在练习的汉字字符
//	CharacterEntity* p;
	string funcs;

// 	BackgroundLayer* backgroundLayer;		//背景图层
// 	TouchLayer* touchLayer;			//触摸事件响应层
// 	TcharacterLayer* TLayer;			//正字图层
// 	HcharacterLayer* HLayer;
	CC_SYNTHESIZE_RETAIN(BackgroundLayer*,backgroundLayer,backgroundLayer);
	CC_SYNTHESIZE_RETAIN(TouchLayer*,touchLayer,touchLayer);
	CC_SYNTHESIZE_RETAIN(TcharacterLayer*,TLayer,TLayer);
	CC_SYNTHESIZE_RETAIN(HcharacterLayer*,HLayer,HLayer);
	CC_SYNTHESIZE_RETAIN(CharacterEntity*,p,CharacterP);
private:

};



#endif
