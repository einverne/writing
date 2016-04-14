#ifndef __ceshiLayer_H__
#define __ceshiLayer_H__

#include "cocos2d.h"
#include "SQLiteData.h"
#include "constants.h"
#include "MyToast.h"
#include "ViewScene.h"
USING_NS_CC;

class FreeWriteLayer : public CCLayer
{
public:
	FreeWriteLayer();
	~FreeWriteLayer();
	CREATE_FUNC(FreeWriteLayer);
	bool init();
	virtual void onEnter();
	void SaveProToFile(float pro);
private:
	void menuNext(CCObject* pSender);
	void menuPrevious(CCObject* pSender);

	/**
	* ±£¥Ê∞¥≈•œÏ”¶
	* @param pSender
	* @return
	*/
	void menuSave(CCObject* pSender);

	void menuView(CCObject* pSender);
	CCMenu* menu_;
};



#endif