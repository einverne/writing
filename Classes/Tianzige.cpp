#include "Tianzige.h"


CTianzige::CTianzige(void)
{
}


CTianzige::~CTianzige(void)
{
}

void CTianzige::draw(){
    CCSprite::draw();
    ccDrawLine(ccp(0,0),ccp(200,200));
}