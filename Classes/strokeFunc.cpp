#include "strokeFunc.h"

strokeFunc::strokeFunc():funcBody(NULL)
{
}

strokeFunc::~strokeFunc()
{
	CC_SAFE_RELEASE(funcBody);
}