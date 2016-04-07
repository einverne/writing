#ifndef __Hcharacter_H__
#define __Hcharacter_H__

#include "cocos2d.h"
#include "../CocosWidget/cocos-widget.h"
#include "HcharacterDrawnode.h"
#include "CharacterExtend.h"
#include "JudgeManager.h"
USING_NS_CC;
using namespace cocos2d::cocoswidget;

// writing character Layer to deal with writing action
class HcharacterLayer : public CCLayer
{
public:
	HcharacterLayer();
	~HcharacterLayer();
	static HcharacterLayer* create(string hanzi,CCSprite* tianzige_draw);
	virtual bool init(string hanzi,CCSprite* tianzige_draw);
	virtual void onEnter();
	virtual void onExit();


	void judge();
	void reloadChar();				//重新加载汉字信息
	float getWrongPercent();
	void isJudge(bool isjudge);
	string getPointsOutput() const { return pointsOutput; }
	string getOriginPoints() const { return pointsOrigin; }

	/**
	* clear all writing like function rewrite(CCOject* pSender)
	* @return
	*/
	void clearWriting();

	CC_SYNTHESIZE_RETAIN(HcharacterDrawnode*,m_HDrawnode,m_HDrawnode);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, bihuaCountAndTotal,bihuaCountAndTotal);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, scoreLabel, scoreLabel);
	CC_SYNTHESIZE_RETAIN(CCSprite* , m_sprite_draw, Sprite);
	CC_SYNTHESIZE_RETAIN(CCSprite* , m_sprite_info, InfoSprite);
	CharacterExtend* getExChar()	{ return m_exChar; }
	void setExChar(CharacterExtend* exchar) { m_exChar = exchar; }
	
private:
	/**
	* 重写回调方法
	* @param pSender
	* @return
	*/
	void rewrite(CCObject* pSender);
	/**
	* 放大回调方法
	* @param pSender
	* @return
	*/
	void zoomin(CCObject* pSender);			//

	/**
	* 缩小回调方法
	* @param pSender
	* @return
	*/
	void zoomout(CCObject* pSender);
	void Wrong();
	void Right();
	int _writeCount;				//书写次数
	int _wrongCount;				//错误次数
	string _hanzi;				//保存该层使用汉字
	float _scale;				//缩放比率
	bool _ijudge;				//是否评判 false不评判，true评判
	string pointsOutput;		//输出给 Lua 评判的字符串
	string pointsOrigin;		//手写汉字Origin字符串 未作 Points 的变形
	int totalBihuaCount;		//该汉字总共有的笔画数
	float score;				//当前汉字评分
	int curBihuaWrong;			//当前笔画错误的次数
	CharacterExtend* m_exChar;	// characterExtend
	JudgeManager _manager;		// judge manager
	ScriptCharacter schar;			// 手写字
};

#endif
