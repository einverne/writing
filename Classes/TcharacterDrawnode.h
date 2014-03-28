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
	~TcharacterDrawnode();
	virtual void draw();
	virtual bool init(string hz,CCSize showrect);
//	CREATE_FUNC(TcharacterDrawnode);
	static TcharacterDrawnode* create(string hz,CCSize showrect);
	CCSize showRect;
	Character m_character;
// 	vector<StrokeDrawnode*> strokedrawList;
	int visibleIndex;
	CC_SYNTHESIZE_RETAIN(CCArray*,strokedrawList,strokedrawList);
private:

};


#endif