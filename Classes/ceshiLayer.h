#ifndef __ceshiLayer_H__
#define __ceshiLayer_H__

#include "cocos2d.h"
#include "SQLiteData.h"
#include "MyToast.h"
USING_NS_CC;

class ceshiLayer : public CCLayer
{
public:
	ceshiLayer();
	~ceshiLayer();
	CREATE_FUNC(ceshiLayer);
	bool init();
	virtual void onEnter();
	void SaveProToFile(float pro);
private:
	void menuNext(CCObject* pSender);
	void menuPrevious(CCObject* pSender);
	void menuSave(CCObject* pSender);
	CCMenu* menu;
};



#endif