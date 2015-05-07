#include "cocos2d.h"
#include "cocos-ext.h"
#include "PopLayer.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

#pragma once
/** 类PopCancelLayer继承自PopLayer
* 增加右上角Cancel按钮,点击之后关闭对话框
*/

class PopCancelLayer : public PopLayer
{
public:


	CREATE_FUNC(PopCancelLayer);

	/**
	 * create a PopLayer just with backgroundImage
	 * @param backgroundImage
	 * @return
	 */
	static PopCancelLayer* create(const char* backgroundImage);

	/**
	* create a PopLayer with hanzi and backgroundImage
	* @param hanzi
	* @param backgroundImage
	* @return
	*/
	static PopCancelLayer* create(const string hanzi,const char* backgroundImage);

	virtual void onEnter();

private:
	void cancelCallback(CCObject* pSender);
};

