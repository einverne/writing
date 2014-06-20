#ifndef __strokeFunc_H__
#define __strokeFunc_H__

#include "cocos2d.h"
USING_NS_CC;

class strokeFunc  : public CCObject
{
public:
	strokeFunc();
	~strokeFunc();
	
	CC_SYNTHESIZE_RETAIN(CCString*,funcBody,Func);		//Character name

};



#endif