#include "PopCancelLayer.h"

using namespace std;

PopCancelLayer* PopCancelLayer::create(const char* backgroundImage){
	PopCancelLayer* l = PopCancelLayer::create();
	l->setSpriteBackGround(CCSprite::create(backgroundImage));
	l->setSprite9BackGround(CCScale9Sprite::create(backgroundImage));
	return l;
}

PopCancelLayer* PopCancelLayer::create(const string hanzi,const char* backgroundImage){
	PopCancelLayer* l = PopCancelLayer::create();
	l->setSpriteBackGround(CCSprite::create(backgroundImage));
	l->setSprite9BackGround(CCScale9Sprite::create(backgroundImage));
	l->setHanzi(hanzi);
	return l;
}

void PopCancelLayer::onEnter(){
	PopLayer::onEnter();
	CCMenuItemImage* cancelBtn = CCMenuItemImage::create("strangedesign/Dlg_cancel_button.png",
		"strangedesign/Dlg_cancel_button_down.png",
		this,
		menu_selector(PopCancelLayer::cancelCallback));
	CCScale9Sprite* background = getSprite9BackGround();
	CCPoint centerDlg = background->getPosition();
	CCSize backgroundSize = background->getContentSize();
	CCPoint rightCorner = centerDlg+ccp(backgroundSize.width/2, backgroundSize.height/2);
	CCSize cancelImgSize = cancelBtn->getContentSize();
	cancelBtn->setPosition(rightCorner-ccp(cancelImgSize.width/2,cancelImgSize.height/2));
	getMenuButton()->addChild(cancelBtn);
}

void PopCancelLayer::cancelCallback(CCObject* pSender){
	//close the dialog
	this->removeFromParent();
}