#include "CharacterExtend.h"

CharacterExtend::CharacterExtend():id(NULL),name(NULL),xml(NULL),ruleLoose(NULL),ruleTight(NULL),strokeIDSeq(NULL)
{
}

CharacterExtend::~CharacterExtend()
{
	CC_SAFE_RELEASE(id);
	CC_SAFE_RELEASE(name);
	CC_SAFE_RELEASE(xml);
	CC_SAFE_RELEASE(ruleLoose);
	CC_SAFE_RELEASE(ruleTight);
	CC_SAFE_RELEASE(strokeIDSeq);
}