#ifndef __Hcharacter_H__
#define __Hcharacter_H__

#include "cocos2d.h"
#include "HcharacterDrawnode.h"
USING_NS_CC;

class HcharacterLayer : public CCLayer
{
public:
	HcharacterLayer();
	~HcharacterLayer();
	//CREATE_FUNC(HcharacterLayer);
	static HcharacterLayer* create(CCSprite* tianzige_draw);
	virtual bool init(CCSprite* tianzige_draw);
	virtual void onExit();
	virtual void onEnter();
	CCPoint convert512(CCPoint p);
	string floatToString(float f);
	void judge(vector<CCPoint> points);

	HcharacterDrawnode* m_HDrawnode;

	CC_SYNTHESIZE_RETAIN(CCSprite* , m_sprite_draw, Sprite);
private:

};


#endif