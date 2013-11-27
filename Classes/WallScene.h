#ifndef __WallScene_H__
#define __WallScene_H__

#include "cocos2d.h"
#include "HanziManage.h"
#include <vector>
using namespace std;
USING_NS_CC;

class WallScene : public CCLayerColor
{
public:
    vector<CHanziManage> hanzilist;
    float rescale;

    CCPoint touchbeginpoint;
    CCPoint changepoint;
    bool touched;
    bool isMoved;
    long beginTime;

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
	void onEnter();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    bool  ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void  ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void  ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

    // implement the "static node()" method manually
    CREATE_FUNC(WallScene);

    //获取当前时间 精确到毫秒数
    static inline long millisecondNow()
    {
	struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
    }

    // 判断是不是 双击
//     static inline bool isDoubleTouch(){
// 	static long lastTouchTime=0;
// 	long thisTouchTime=millisecondNow();
// 
// 	if(abs(thisTouchTime-lastTouchTime)<250){
// 	    lastTouchTime=0;
// 	    return true;
// 	}else{
// 	    lastTouchTime=millisecondNow();
// 	    return false;
// 	}
//     }

    bool isInSprite(CCTouch* pTouch);
    void singleClick(string hanzi);
};

#endif // __WallScene_H__