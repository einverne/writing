#ifndef __Hcharacter_H__
#define __Hcharacter_H__

#include "cocos2d.h"
#include "../CocosWidget/cocos-widget.h"
#include "HcharacterDrawnode.h"
#include "CharacterExtend.h"
#include "JudgeManager.h"
#include "ShuipingErrorNode.h"
#include "OnePointNode.h"
#include <map>
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

	/**
	* 根据lua返回结果，解析返回值
	* WeeklyMeeting/部件级别错误类型.md
	* 
	* @param ret 返回值 string json格式
	* @return
	*/
	void ParseResult(const string ret);

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

	/**
	* 处理 A0001 水平平齐错误
	* @return
	*/
	void doA0001(multimap<int, float>& points, multimap<int, float>& rightpoints);

	/**
	* 中点切分
	* @return
	*/
	void doA0002(multimap<int, float>& points, multimap<int, float>& rightpoints);

	/**
	* 黄金分割
	* @return
	*/
	void doA0003(multimap<int, float>& points, multimap<int, float>& rightpoints);

	/**
	* 首点居正
	* @return
	*/
	void doA0004(multimap<int, float>& points, multimap<int, float>& rightpoints);

	/**
	* 横笔等距
	* @param points
	* @param rightpoints
	* @return
	*/
	void doA0005(multimap<int, float>& points, multimap<int, float>& rightpoints);

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
	void writeBihuaWrong();
	void writeBihuaRight();
	int writeCount_;				//书写次数
	int wrongCount_;				//错误次数
	string hanzi_;				//保存该层使用汉字
	float scale_;				//缩放比率
	bool ijudge_;				//是否评判 false不评判，true评判
	string pointsOutput;		//输出字符串
	string pointsOrigin;		//手写汉字Origin字符串 未作Points 的变形
	int totalBihuaCount;				//该汉字总共有的笔画数
	float score_;				//当前汉字评分
	int curBihuaWrong;			//当前笔画错误的次数
	CharacterExtend* m_exChar;	// characterExtend
	JudgeManager _manager;		// judge manager
};

#endif
