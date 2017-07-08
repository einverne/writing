#ifndef __WallScene_H__
#define __WallScene_H__

#include "cocos2d.h"
#include "HanziManage.h"
#include "PopLayer.h"
#include <vector>
using namespace std;
USING_NS_CC;

class WallScene : public CCLayer
{
public:
	vector<CHanziManage> hanzilist;
	string selectedHanzi;
	float rescale;

	CCPoint touchbeginpoint;
	CCPoint prePoint;
	CCPoint changepoint;
	bool touched;
	bool isMoved;
	long beginTime;

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
	CREATE_FUNC(WallScene);

	/**
	* �����Ի���
	* @param hanzi
	* @return
	*/
	void popup(string hanzi);
	//�Ի���ص�����
	void buttonCallBack(CCNode* pNode);

	virtual void update(float delta);
	//����
	void longPressUpdate(float fDelta);

	//���浽xml�ļ�
	void saveToFile(string src,const char* dst);

	//��ȡ��ǰʱ�� ��ȷ��������
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
};

#endif // __WallScene_H__
