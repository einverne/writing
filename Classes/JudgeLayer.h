#ifndef __ceshiLayer_H__
#define __ceshiLayer_H__

#include "cocos2d.h"
#include "SQLiteData.h"
#include "constants.h"
#include "MyToast.h"
#include "ViewScene.h"
USING_NS_CC;

class JudgeLayer : public CCLayer
{
public:
	JudgeLayer();
	~JudgeLayer();
	CREATE_FUNC(JudgeLayer);
	bool init();
	virtual void onEnter();
	void SaveProToFile(float pro);
private:
	void menuNext(CCObject* pSender);
	void menuPrevious(CCObject* pSender);
	void menuSave(CCObject* pSender);
	void menuView(CCObject* pSender);
	CCMenu* menu;
};



#endif