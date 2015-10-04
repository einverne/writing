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

class NewUnitLayer : public CCLayerColor, CCTextFieldDelegate
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
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	virtual void registerWithTouchDispatcher();

	static NewUnitLayer* create(string unitID);

	//当用户启动虚拟键盘时的回调函数
	//启用键盘false; 不启用键盘true
	virtual bool onTextFieldAttachWithIME(CCTextFieldTTF* sender);

	//当用户关闭虚拟键盘时的回调函数
	//关闭键盘false; 不关闭键盘true
	virtual bool onTextFieldDetachWithIME(CCTextFieldTTF* sender);

	//当用户输入时的回调函数
	//允许输入字符false; 不允许输入字符true
	virtual bool onTextFieldInsertText(CCTextFieldTTF* sender, const char* text, int nLen);

	//当用户删除文字时的回调函数
	//允许删除字符false; 不允许删除字符true
	virtual bool onTextFieldDeleteBackward(CCTextFieldTTF* sender, const char* delText, int nLen);


	/**
	* 修改xml文件中汉字对应proficiency值
	* @param character 要修改prof值对应的字
	* @param proficiency要修改到的值
	* @return
	*/
	bool setProficiency(string character,string proficiency);
	
	CC_SYNTHESIZE_RETAIN(CCEditBox*, m_editBox, m_editBox);
	CC_SYNTHESIZE_RETAIN(CCArray*, m_TextList, m_TextList);

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
