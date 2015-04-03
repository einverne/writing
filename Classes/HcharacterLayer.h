#ifndef __Hcharacter_H__
#define __Hcharacter_H__

#include "cocos2d.h"
#include "cocos-widget.h"
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
	virtual void onExit();
	virtual void onEnter();

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

	CC_SYNTHESIZE_RETAIN(HcharacterDrawnode*,m_HDrawnode,m_HDrawnode);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, bihuaCount,bihuaCount);
	CC_SYNTHESIZE_RETAIN(CCSprite* , m_sprite_draw, Sprite);
	CC_SYNTHESIZE_RETAIN(CCSprite* , m_sprite_info, InfoSprite);
	CC_SYNTHESIZE_RETAIN(CharacterExtend* , m_exChar, ExChar);
private:
	void rewrite(CCObject* pSender);		//
	void zoomin(CCObject* pSender);			//
	void zoomout(CCObject* pSender);		//
	void writeWrong();
	void writeRight();
	int writeCount;				//书写次数
	int wrongCount;				//错误次数
	string hanzi;				//保存该层使用汉字
	float scale;				//缩放比率
	bool ijudge;				//是否评判 false不评判，true评判
	string pointsOutput;		//输出字符串
};


#endif
