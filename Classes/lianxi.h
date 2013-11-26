#ifndef __LIANXI_H__
#define __LIANXI_H__

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class lianxi : public CCLayerColor{

public:
    lianxi();
    ~lianxi();
    virtual bool init();
    virtual void onEnter();
    virtual void draw();
    static CCScene* scene(string hanzi);
    CREATE_FUNC(lianxi);
    
    bool  ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void  ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void  ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

    void keyBackClicked();
private:
    string hanzi;
    CCPoint prePoint;
    void menuBack(CCObject* pSender);
};

#endif