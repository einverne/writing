#include "cocos2d.h"
#include "cocos-ext.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

#pragma once
class PopLayer:public CCLayer
{
public:
	PopLayer(void);
	~PopLayer(void);

	virtual bool init();
	CREATE_FUNC(PopLayer);

	/**
	* create a PopLayer with hanzi and backgroundImage
	* @param hanzi
	* @param backgroundImage
	* @return
	*/
	static PopLayer* create(const string hanzi,const char* backgroundImage);

	virtual void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	/**
	* setTitle
	* @param title
	* @param fontsize default fontsize = 20
	* @return
	*/
	void setTitle(const char* title, int fontsize = 20);

	/**
	* setContentText
	* @param text
	* @param fontsize
	* @param padding
	* @param paddingTop
	* @return
	*/
	void setContentText(const char* text, int fontsize = 20,int padding = 50, int paddingTop = 100);

	/**
	* 
	* @param targer
	* @param callfun
	* @return
	*/
	void setCallBackFunc(CCObject* targer, SEL_CallFuncN callfun);

	/**
	* add Button
	* @param normalImage
	* @param seletedImage
	* @param title
	* @param tag
	* @return
	*/
	bool addButton(const char* normalImage,const char* seletedImage ,const char* title, int tag=0);
	void setHanzi(string h);
	const char* getHanzi();

// 	virtual void editBoxEditingDidBegin(CCEditBox* editBox);
// 	virtual void editBoxEditingDidEnd(CCEditBox* editBox);
// 	virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
// 	virtual void editBoxReturn(CCEditBox* editBox);

	virtual void onEnter();
	virtual void onExit();
private:
	void buttonCallback(CCObject* pSender);
	int m_contentPadding;
	int m_contentPaddingTop;

	//保存汉字
	string hanzi;

	CCObject* m_callbackListener;
	SEL_CallFuncN m_callback;

	//内存管理相关
	CC_SYNTHESIZE_RETAIN(CCMenu*, m__pMenu,MenuButton);
	CC_SYNTHESIZE_RETAIN(CCSprite*, m__sfBackGround, SpriteBackGround);
	CC_SYNTHESIZE_RETAIN(CCScale9Sprite*, m__s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltContentText, LabelContentText);
	CC_SYNTHESIZE_RETAIN(CCEditBox*, m__editBox, EditBoxHanzi);
};

