#ifndef _TcharacterDrawnode_H__
#define _TcharacterDrawnode_H__

#include "cocos2d.h"
#include "Character.h"
#include "StrokeDrawnode.h"
USING_NS_CC;

class TcharacterDrawnode : public CCNode
{
public:
	TcharacterDrawnode();
	TcharacterDrawnode(string hz,CCSize showrect);
	~TcharacterDrawnode();
	virtual void draw();
	virtual bool init();
	CREATE_FUNC(TcharacterDrawnode);
	static TcharacterDrawnode* create(string hz,CCSize showrect);
	CCSize showRect;
	Character m_character;
	vector<StrokeDrawnode*> strokedrawList;
	int visibleIndex;

private:

};


#endif