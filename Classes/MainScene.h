#ifndef __MainScene_H__
#define __MainScene_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "WallSingleScene.h"
#include "tools/DataTool.h"
#include "SQLiteData.h"
#include "../CocosWidget/cocos-widget.h"
using namespace cocos2d::cocoswidget;
using namespace std;
#include "stdlib.h"
USING_NS_CC;
USING_NS_CC_EXT;

class MainScene : public CCLayerColor
{
public:
	MainScene();
	~MainScene();
	CREATE_FUNC(MainScene);
	static CCScene* scene();
	virtual bool init();
	virtual void keyBackClicked();

private:
	void isExit(CCNode* pNode);
	void settingCallBack(CCNode* pNode);
	void buttonClick(CCObject* pSender);
	bool buttonLongClick(CCObject* pSender,CCTouch* pTouch);
	void addButtonCallback(CCObject* pSender);
	void dlgCallBack(CCNode* pNode);			//Dialog callback
	CCObject* gridviewDataSource(CCObject* pConvertView, unsigned int idx);

	int unit_count;
	vector<string> unit_ids;
	CGridView* pGridView;
	string longClickSelectUnitID;			//store the unit_id long click selected
};



#endif
