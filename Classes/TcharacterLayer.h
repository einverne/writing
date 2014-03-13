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

	virtual bool init();
	virtual void onExit();
	virtual void onEnter();
	CREATE_FUNC(TcharacterLayer);
	TcharacterDrawnode* m_TDrawnode;

	CC_SYNTHESIZE_RETAIN(CCSprite*,m_sprite,Sprite);
};


#endif