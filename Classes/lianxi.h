#ifndef __LIANXI_H__
#define __LIANXI_H__

#include "cocos2d.h"
#include "Character.h"
#include "StrokeNode.h"
#include "LuaScriptReader.h"

USING_NS_CC;
using namespace std;

class lianxi : public CCLayer{

public:
    lianxi();
    ~lianxi();
    virtual bool init();
	virtual void onExit();
	virtual void onEnter();
//    virtual void draw();
    static CCScene* scene(string hanzi);
    CREATE_FUNC(lianxi);
    
//     bool  ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//     void  ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
//     void  ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();

    void keyBackClicked();
	void clearStroke(float fDelta);		//写完一笔之后1s响应函数
private:
	Character charac;
	Character character;			//store writing character not used yet
	Character newCharac;
	Character Originccccc;
	CCNode* writing_points_node;
	vector<CCDrawNode*> writing_stroke;
	vector<CCPoint> writing_points;			//保存写下来的字点集信息
    string hanzi;
    CCPoint writing_prePoint;				//store first writing point 画连续点时变化
	CCPoint centerPoint;			//保存手写字，中心点信息
    void menuBack(CCObject* pSender);
	string convertToString(float f);
	CCPoint convert512(CCPoint p);					//将坐标转化成512范围内
	string output;
	CCSprite* tianzige;
	CCSprite* tianzige_draw;
	void DoAnimation();

	StrokeNode* my_strokenode;
	vector<StrokeNode*> strokeNodes;
	int current_writing_stroke;				//当前写第几笔 1,2,3,4....
};

#endif