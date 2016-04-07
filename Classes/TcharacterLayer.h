#ifndef __TcharacterLayer_H__
#define __TcharacterLayer_H__

#include "cocos2d.h"
#include "TcharacterDrawnode.h"
#include "ScriptCharacter.hpp"

USING_NS_CC;
using namespace std;

// Right hanzi layer create using a Sprite
class TcharacterLayer : public CCLayer
{
public:
	TcharacterLayer();
	~TcharacterLayer();

	virtual bool init(CCSprite* tianzige);
	virtual void onEnter();
	static TcharacterLayer* create(CCSprite* tianzige);
	
	void refresh(CCObject* pSender);
	void setCharacter(const string curChar);
	void reloadChar();

	CharacterExtend* getCharacter() const { return m_exChar; }
	void setCharacter(CharacterExtend* exchar) { m_exChar = exchar; }
    
    CC_SYNTHESIZE_RETAIN(TcharacterDrawnode*, m_TDrawnode,m_TDrawnode);
    CC_SYNTHESIZE_RETAIN(CCSprite*,m_sprite,Sprite);
    
private:
	bool isPause;					//动画是否暂停
	string m_curChar;               //当前字符
	CharacterExtend* m_exChar;
};


#endif