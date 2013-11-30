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
	string selectedHanzi;
	float rescale;

	CCPoint touchbeginpoint;
	CCPoint changepoint;
	bool touched;
	bool isMoved;
	long beginTime;

	virtual bool init();
	virtual void onEnter();

	static cocos2d::CCScene* scene();

	void menuCloseCallback(CCObject* pSender);

// 	bool  ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
// 	void  ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
// 	void  ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

	void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	// implement the "static node()" method manually
	CREATE_FUNC(WallScene);

	//弹出对话框
	void popup(string hanzi);
	//对话框回调函数
	void buttonCallBack(CCNode* pNode);

	virtual void update(float delta);
	//长按
	void longPressUpdate(float fDelta);

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