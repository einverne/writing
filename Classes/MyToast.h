#ifndef __MyToast_H__
#define __MyToast_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#define TOAST_LONG 2
#define TOAST_SHORT 1

class MyToast : public CCLayerColor
{
public:
	MyToast();
	~MyToast();
	CREATE_FUNC(MyToast);
	virtual bool init();
	void onExit();
	void initToast(string msg,float time);
	static void showToast(CCNode* node,string msg,float dt);

private:
	CCSize visibleSize;
	CCPoint origin;
	CCScale9Sprite* bg;

	void removeSelf();
	void removeToast(CCNode* node);
};

#endif