#ifndef __JudgeScene_H__
#define __JudgeScene_H__

#include "cocos2d.h"
#include "BackgroundLayer.h"
#include "TcharacterLayer.h"
#include "HcharacterLayer.h"	
#include "TouchLayer.h"
#include "CharacterExtend.h"
#include "JudgeLayer.h"
USING_NS_CC;
using namespace std;

class JudgeScene : public CCScene
{
public:
	JudgeScene();
	/**
	* 传入unit_id 和 hanzis 列表
	* @param unit_id 对应数据库中单元ID
	* @param hanzis 汉字列表
	* @return
	*/
	JudgeScene(string unit_id,vector<string> hanzis);
	~JudgeScene();
	static JudgeScene* create(string unit_id,vector<string> hanzis);
	virtual bool init();

	CC_SYNTHESIZE_RETAIN(BackgroundLayer*,backgroundLayer,backgroundLayer);
	CC_SYNTHESIZE_RETAIN(TouchLayer*,touchLayer,touchLayer);
	CC_SYNTHESIZE_RETAIN(TcharacterLayer*,TLayer,TLayer);
	CC_SYNTHESIZE_RETAIN(HcharacterLayer*,HLayer,HLayer);
	CC_SYNTHESIZE_RETAIN(CharacterExtend*,ext_p,CharacterExt);

	CC_SYNTHESIZE_RETAIN(JudgeLayer* , judgeLayer , JudgeLayer);

	/**
	* 上一个汉字
	* @return
	*/
	void previous();

	/**
	* 下一个汉字
	* @return
	*/
	void next();

	
	/**
	* 设置是否评判书写 true为评判 false为不评判
	* @param isjudge
	* @return
	*/
	void setIsJudge(bool isjudge);
	bool getIsJudge()	{ return b_isJudge; }

	string getUnitID()	{ return unit_id;}
	string getZiID()	{ return zi_id; }
	string getCharacter()		{ return currentCharacter;}

private:

	void lastCharacter(CCNode* pNode);	//单元最后一个汉字回调函数
	string unit_id;						//保存单元id
	string zi_id;
	vector<string> hanziList;			//保存汉字列表
	string currentCharacter;			//当前书写汉字
	int index;							//保存当前汉字序号
	bool b_isJudge;						//当前Scene是否判断 false 为自由书写
};




#endif