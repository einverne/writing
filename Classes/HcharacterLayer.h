#ifndef __Hcharacter_H__
#define __Hcharacter_H__

#include "cocos2d.h"
#include "../CocosWidget/cocos-widget.h"
#include "HcharacterDrawnode.h"
#include "CharacterExtend.h"
USING_NS_CC;
using namespace cocos2d::cocoswidget;

class HcharacterLayer : public CCLayer
{
public:
	HcharacterLayer();
	~HcharacterLayer();
	static HcharacterLayer* create(string hanzi,CCSprite* tianzige_draw);
	virtual bool init(string hanzi,CCSprite* tianzige_draw);
	virtual void onEnter();
	virtual void onExit();

	/**
	* convert512 转换成512*512 并将坐标原点放置到左上角
	* @param p
	* @return
	*/
	CCPoint convert512(CCPoint p);
	void judge();
	void reloadChar();				//重新加载汉字信息
	float getWrongPercent();
	void isJudge(bool isjudge);
	string getPointsOutput()	{ return pointsOutput; }
	string getOriginPoints()	{ return pointsOrigin; }

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
	void zoomout(CCObject* pSender);		//
	void writeWrong();
	void writeRight();
	int writeCount;				//书写次数
	int wrongCount;				//错误次数
	string hanzi;				//保存该层使用汉字
	float scale;				//缩放比率
	bool ijudge;				//是否评判 false不评判，true评判
	string pointsOutput;		//输出字符串
	string pointsOrigin;		//手写汉字Origin字符串 未作Points 的变形
	int totalBihuaCount;				//该汉字总共有的笔画数
	float score;				//当前汉字评分
	int curBihuaWrong;			//当前笔画错误的次数
	CharacterExtend* m_exChar;	// characterExtend
};


#endif
