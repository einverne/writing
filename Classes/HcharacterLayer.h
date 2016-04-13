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
// 处理手写汉字Layer
class HcharacterLayer : public CCLayer
{
public:
	HcharacterLayer();
	~HcharacterLayer();
	static HcharacterLayer* create(string hanzi,CCSprite* tianzige_draw);
	virtual bool init(string hanzi,CCSprite* tianzige_draw);
	virtual void onEnter();
	virtual void onExit();

	void Judge();
	void ReloadChar();				//重新加载汉字信息
	float GetWrongPercent();
	void IsJudge(bool isjudge);
	
	/**
	* 输出点序列
	* @return
	*/
	string GetPointsOutput() const { return points_output_; }
	
	/**
	* 未变形的点序列
	* @return
	*/
	string GetOriginPoints() const { return points_origin_; }

	/**
	* clear all writing like function rewrite(CCOject* pSender)
	* 清楚手写 Layer 的笔迹
	* @return
	*/
	void ClearWriting();

	CC_SYNTHESIZE_RETAIN(HcharacterDrawnode*,m_HDrawnode,m_HDrawnode);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, bihuaCountAndTotal,bihuaCountAndTotal);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, scoreLabel, scoreLabel);
	CC_SYNTHESIZE_RETAIN(CCSprite* , m_sprite_draw, Sprite);
	CC_SYNTHESIZE_RETAIN(CCSprite* , m_sprite_info, InfoSprite);
	CharacterExtend* getExChar()	{ return m_exchar_; }
	void SetExChar(CharacterExtend* exchar) { m_exchar_ = exchar; }
	
private:
	/**
	* 重写回调方法
	* @param pSender
	* @return
	*/
	void Rewrite(CCObject* pSender);
	/**
	* 放大回调方法
	* @param pSender
	* @return
	*/
	void ZoomIn(CCObject* pSender);			//

	/**
	* 缩小回调方法
	* @param pSender
	* @return
	*/
	void ZoomOut(CCObject* pSender);
	void Wrong();
	void Right();

	int write_count_;				//书写次数
	int wrong_count_;				//错误次数
	string hanzi_;				//保存该层使用汉字
	float scale_;				//缩放比率
	bool is_judge_;				//是否评判 false不评判，true评判
	string points_output_;		//输出给 Lua 评判的字符串
	string points_origin_;		//手写汉字Origin字符串 未作 Points 的变形
	int total_bihua_count_;		//该汉字总共有的笔画数
	float score_;				//当前汉字评分
	int cur_bihua_wrong_;			//当前笔画错误的次数
	CharacterExtend* m_exchar_;	// characterExtend
	JudgeManager manager_;		// judge manager
};

#endif
