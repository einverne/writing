#ifndef __TouchLayer_H__
#define __TouchLayer_H__

#include "cocos2d.h"
#include "TcharacterLayer.h"
#include "HcharacterLayer.h"
USING_NS_CC;
using namespace std;

class TouchLayer : public CCLayerColor
{
public:
	TouchLayer();
	~TouchLayer();
	/**
	* 利用TcharacterLayer，HcharacterLayer初始化TouchLayer
	* @param t
	* @param h
	* @return
	*/
	virtual bool init(TcharacterLayer* t,HcharacterLayer* h);
	virtual void onEnter();
	virtual void onExit();
	/**
	* 利用TcharacterLayer，HcharacterLayer创建TouchLayer
	* @param t
	* @param h
	* @return
	*/
	static TouchLayer* create(TcharacterLayer* t,HcharacterLayer* h);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();

	CC_SYNTHESIZE_RETAIN(TcharacterLayer* ,Tlayer,Tlayer);
	CC_SYNTHESIZE_RETAIN(HcharacterLayer* ,Hlayer,Hlayer);

	/**
	* 判断该字是否写完
	* @return 写完返回true，否则返回false
	*/
	bool isFinish();

	/**
	* 是否是保存笔迹的时候
	* @return true 已经写完可以保存
	*/
	bool isTimeSave();
private:
	int index_;				//保存当前书写的第几笔
	int error_type_;			//当前书写笔画错误类型
	vector<CCPoint> touch_points_;		//保存书写点
	CCPoint begin_point_;			//保存书写的第一个点
	bool is_outside_;             // 书写过程是否在田字格外面
	bool is_start_outside_;        // 第一笔是否在田字格外面
};



#endif // !__TouchLayer_H__
