#ifndef __NewUnit_H__
#define __NewUnit_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "HanziManage.h"
#include "PopLayer.h"
#include "tinyxml.h" 
#include "PopLayer.h"
#include "tools/DataTool.h"
#include "LianxiScene.h"
#include "MainScene.h"
#include "MyToast.h"
#include "JudgeScene.h"
#include "SQLiteData.h"
#include <vector>
using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;

class NewUnitLayer : public CCLayerColor, CCEditBoxDelegate
{
public:
	NewUnitLayer(string unitID);
	~NewUnitLayer();
	vector<CHanziManage> hanzilist;
	string selectedHanzi;
	float rescale;

	/**
	* 传入add 则增加一个单元，如果传入>=0的值则为修改该ID的单元
	* @param unitID add 或者 >=0 的数字ID 单元ID
	* @return
	*/
	static cocos2d::CCScene* scene(string unitID);

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void keyBackClicked();

	void menuCloseCallback(CCObject* pSender);

// 	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
// 	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
// 	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();

	static NewUnitLayer* create(string unitID);

	//开始编辑时
	virtual void editBoxEditingDidBegin(CCEditBox* editBox);
	//结束编辑时
	virtual void editBoxEditingDidEnd(CCEditBox* editBox);
	//编辑框文字改变时
	virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
	//触发return后
	virtual void editBoxReturn(CCEditBox* editBox);
	
	/**
	* 修改xml文件中汉字对应proficiency值
	* @param character 要修改prof值对应的字
	* @param proficiency要修改到的值
	* @return
	*/
	bool setProficiency(string character,string proficiency);
	
	CC_SYNTHESIZE_RETAIN(CCEditBox*, m_editBox, m_editBox);

private:
// 	void backtoMainScene(CCNode* pNode);
	void back(CCObject* pSender);
	void finishAddUnit(CCObject* pSender);
	void deleteUnit(CCObject* pSender);
	void updateUnit(CCObject* pSender);
	PopLayer* popL;
	vector<CHanziManage>::iterator selectedCHanziManageIter;

	vector<string> hanzis;			//墙上的汉字
	string unitID;				//用以区别不同单元，与数据库中单元ID列对应
	vector<vector <string> > groupCharacter;		//从数据库中获取一个单元的汉字数组
	int tag;
};

#endif // __NewUnit_H__
