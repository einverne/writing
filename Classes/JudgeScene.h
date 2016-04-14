#ifndef __JudgeScene_H__
#define __JudgeScene_H__

#include "cocos2d.h"
#include "BackgroundLayer.h"
#include "TcharacterLayer.h"
#include "HcharacterLayer.h"	
#include "TouchLayer.h"
#include "CharacterExtend.h"
#include "FreeWriteLayer.h"
USING_NS_CC;
using namespace std;

/**
 * 以16个字为单元进行练习 Judge Scene 处理一个单元自由练习与评分书写
 * 期中 FreeWriteLayer 处理自由书写中 Button
 */
class JudgeScene : public CCScene
{
public:
	JudgeScene();
	/**
	* 传入unit_id 和 hanzis 列表
	* @param unit_id 对应数据库中单元ID
	* @param hanzis 汉字列表
	* @param start_index 单元开始汉字位置，默认为0
	* @return
	*/
	JudgeScene(string unit_id,vector<string> hanzis, int start_index = 0);
	~JudgeScene();
	static JudgeScene* create(string unit_id,vector<string> hanzis, int start_index = 0);
	bool init();

	CC_SYNTHESIZE_RETAIN(BackgroundLayer*,backgroundLayer,backgroundLayer);
	CC_SYNTHESIZE_RETAIN(TouchLayer*,touchLayer,touchLayer);
	CC_SYNTHESIZE_RETAIN(TcharacterLayer*,TLayer,TLayer);
	CC_SYNTHESIZE_RETAIN(HcharacterLayer*,HLayer,HLayer);

	CC_SYNTHESIZE_RETAIN(FreeWriteLayer* , free_write_layer , FreeWriteLayer);

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

	CharacterExtend* getCharacterExt()	{ return ext_p_; }
	void setCharacterExt(CharacterExtend* exp)	{ ext_p_ = exp; }
	
	/**
	* 设置是否评判书写 true为评判 false为不评判
	* @param isjudge
	* @return
	*/
	void setIsJudge(bool isjudge);
	bool getIsJudge() const	{ return is_judge; }
    
	string getUnitID() const	{ return unit_id_;}
	string getZiID() const	{ return zi_id_; }
	string getCurChar() const		{ return cur_char_;}

private:

	void lastCharacter(CCNode* pNode);	//单元最后一个汉字回调函数
	string unit_id_;						//保存单元id
	string zi_id_;                       //存储汉字id
	vector<string> hanzi_list_;			//保存汉字列表
	string cur_char_;						//当前书写汉字
	int index_;							//保存当前汉字序号,序号从0-15
	bool is_judge;						//当前Scene是否判断 false 为自由书写
	CharacterExtend* ext_p_;				//从XML中读取的汉字信息
};

#endif