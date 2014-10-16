#ifndef __CharacterExtend_H__
#define __CharacterExtend_H__

#include "cocos2d.h"
USING_NS_CC;

class CharacterExtend : public CCObject
{
public:
	CharacterExtend();
	~CharacterExtend();
	CC_SYNTHESIZE_RETAIN(CCInteger*,id,ID);			//id
	CC_SYNTHESIZE_RETAIN(CCString*,name,Name);		//Character name
	CC_SYNTHESIZE_RETAIN(CCString*,xml,XML);			//Character xml from sqlite
	CC_SYNTHESIZE_RETAIN(CCString*,ruleLoose,ruleLoose);	//Character rules
	CC_SYNTHESIZE_RETAIN(CCString*,ruleTight,RuleTight);	//
	CC_SYNTHESIZE_RETAIN(CCString*,strokeIDSeq,SEQ);	//sequence
private:

};

#endif