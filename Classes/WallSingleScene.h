#ifndef __WallSingleScene_H__
#define __WallSingleScene_H__

#include "cocos2d.h"
#include "HanziManage.h"
#include "PopLayer.h"
#include "tinyxml.h" 
#include "PopLayer.h"
#include "tools/DataTool.h"
#include "LianxiScene.h"
#include "MainScene.h"
#include "MyToast.h"
#include "CeshiScene.h"
#include "SQLiteData.h"
#include <vector>
using namespace std;
USING_NS_CC;

class WallSingleLayer : public CCLayerColor
{
public:
	WallSingleLayer(string unitID);
	~WallSingleLayer();
	vector<CHanziManage> hanzilist;
	string selectedHanzi;
	float rescale;

	virtual bool init(string xmlfilename);
	virtual void onEnter();
	virtual void onExit();
	virtual void keyBackClicked();

	static cocos2d::CCScene* scene(string filename,string unitID);

	void menuCloseCallback(CCObject* pSender);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();

	static WallSingleLayer* create(string wallxmlname,string unitID);

	/**
	* 弹出对话框
	* @param hanzi
	* @return
	*/
	void popup(string hanzi);
	
	/**
	* 对话框回调函数
	* @param pNode
	* @return
	*/
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
	string getwallXmlName()	{ return wallXMLCurrent; }

	/**
	* 修改xml文件中汉字对应proficiency值
	* @param character 要修改prof值对应的字
	* @param proficiency要修改到的值
	* @return
	*/
	bool setProficiency(string character,string proficiency);
	
private:
	void backtoMainScene(CCNode* pNode);
	void back(CCObject* pSender);
	PopLayer* popL;
	vector<CHanziManage>::iterator selectedCHanziManageIter;

	CCPoint touchbeginpoint;
	CCPoint prePoint;
	CCPoint changepoint;
	bool touched;
	bool isMoved;
	long beginTime;

	// xml 文件名
	string wallXMLCurrent;

	/**
		测试按钮响应函数
	*/
	void ceshi(CCObject* pSender);

	/**
	* 自由练习消息响应
	* @param pSender
	* @return
	*/
	void freewriting(CCObject* pSender);

	vector<string> hanzis;			//墙上的汉字
	bool isLongPressAllow;			//是否允许长按操作
	string unitID;				//用以区别不同单元，与数据库中单元ID列对应
	vector<vector <string> > groupCharacter;		//从数据库中获取一个单元的汉字数组
};

#endif // __WallSingleScene_H__
