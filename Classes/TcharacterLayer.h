#ifndef __TcharacterLayer_H__
#define __TcharacterLayer_H__

#include "cocos2d.h"
#include "TcharacterDrawnode.h"

USING_NS_CC;
using namespace std;

class TcharacterLayer : public CCLayer
{
public:
	TcharacterLayer();
	~TcharacterLayer();

	virtual bool init(CCSprite* tianzige);
	virtual void onExit();
	virtual void onEnter();
	//CREATE_FUNC(TcharacterLayer);
	static TcharacterLayer* create(CCSprite* tianzige);
	
	void refresh(CCObject* pSender);
	CC_SYNTHESIZE_RETAIN(TcharacterDrawnode*, m_TDrawnode,m_TDrawnode);
	CC_SYNTHESIZE_RETAIN(CCSprite*,m_sprite,Sprite);
};


#endif