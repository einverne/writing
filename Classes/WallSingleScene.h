#ifndef __WallSingleScene_H__
#define __WallSingleScene_H__

#include "cocos2d.h"
#include "HanziManage.h"
#include "PopLayer.h"
#include <vector>
using namespace std;
USING_NS_CC;

class WallSingleScene : public CCLayer
{
public:
	vector<CHanziManage> hanzilist;
	string selectedHanzi;
	float rescale;



	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void keyBackClicked();

	static cocos2d::CCScene* scene();

	void menuCloseCallback(CCObject* pSender);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();

	// implement the "static node()" method manually
	CREATE_FUNC(WallSingleScene);

	/**
	* 弹出对话框
	* @param hanzi
	* @return
	*/
	void popup(string hanzi);
	//对话框回调函数
	void buttonCallBack(CCNode* pNode);

	virtual void update(float delta);
	//长按
	void longPressUpdate(float fDelta);

	//保存到xml文件
	void saveToFile(string src,const char* dst);

	//获取当前时间 精确到毫秒数
	static inline long millisecondNow()
	{
		struct cc_timeval now;
		CCTime::gettimeofdayCocos2d(&now, NULL);
		return (now.tv_sec * 1000 + now.tv_usec / 1000);
	}

	bool isInSprite(CCTouch* pTouch);
	void singleClick(string hanzi);
	
private:
	void backtoMainScene(CCNode* pNode);
	PopLayer* popL;
	vector<CHanziManage>::iterator selectedCHanziManageIter;

	CCPoint touchbeginpoint;
	CCPoint prePoint;
	CCPoint changepoint;
	bool touched;
	bool isMoved;
	long beginTime;

	/*
		测试按钮响应函数
	*/
	void ceshi(CCObject* pSender);
};

#endif // __WallSingleScene_H__
