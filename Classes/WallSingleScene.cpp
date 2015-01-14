#include "WallSingleScene.h"
#include "tinyxml.h" 
#include "PopLayer.h"
#include "tools/DataTool.h"
#include "LianxiScene.h"
#include "MainScene.h"
#include "MyToast.h"
#include "CeshiScene.h"
#include "SQLiteData.h"
#include <iostream>
#include <string>
USING_NS_CC;
using namespace std;

#define TAG_LAYER_EXIT 1001

WallSingleScene::WallSingleScene(){
	isLongPressAllow = false;
}

WallSingleScene::~WallSingleScene(){

}

//////////////////////////////////////////
CCScene* WallSingleScene::scene(string filename)
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	WallSingleScene *layer = WallSingleScene::create(filename);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

WallSingleScene* WallSingleScene::create(string wallxmlname){
	WallSingleScene* pret = new WallSingleScene();
	if (pret && pret->init(wallxmlname))
	{
		pret->autorelease();
		return pret;
	}else{
		delete pret;
		pret = NULL;
		return pret;
	}
}

// on "init" you need to initialize your instance
bool WallSingleScene::init(string xmlfilename)
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayerColor::initWithColor(ccc4(255,255,255,255)) )
	{
		return false;
	}
	CCLog("WallSingleScene::init");
	wallXMLCurrent = xmlfilename;
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	isMoved = false;
	touched = false;

	CCLog("WallSingleScene:init");



	//注册触摸事件
	CCPoint changepoint=ccp(0,0);
	touched=false;
	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);			//android back key
	//原本如果没有重载register那个函数，需要调用如下两个其中之一
	//CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,1);
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	return true;
}

void WallSingleScene::onEnter(){
	CCLayer::onEnter();
	CCLog("WallSingleScene::onEnter()");
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//add Menu Item
	CCMenuItemImage* ceshi_button = CCMenuItemImage::create("ceshi_1.png",
		"ceshi_2.png",
		this,
		menu_selector(WallSingleScene::ceshi));
	CCMenuItemImage* back_button = CCMenuItemImage::create("strangedesign/back_button.png",
		"strangedesign/back_button.png",
		this,
		menu_selector(WallSingleScene::back));
	back_button->setPosition(ccp(back_button->getContentSize().width/2+10,visibleSize.height-back_button->getContentSize().height/2-50));

	CCMenuItemImage* judge_button = CCMenuItemImage::create("strangedesign/Page_judgewritting_button.png",
		"strangedesign/Page_judgewritting_button.png",
		this,
		menu_selector(WallSingleScene::ceshi));
	CCMenuItemImage* free_button = CCMenuItemImage::create("strangedesign/Page_freewritting_button.png",
		"strangedesign/Page_freewritting_button.png",
		this,
		menu_selector(WallSingleScene::ceshi));
	judge_button->setPosition(ccp(visibleSize.width - judge_button->getContentSize().width/2 ,judge_button->getContentSize().height/2));
	free_button->setPosition(ccp(free_button->getContentSize().width/2,free_button->getContentSize().height/2));

	CCMenu* menu = CCMenu::create(judge_button,free_button,back_button,NULL);
	this->addChild(menu,20);
	ceshi_button->setPosition(ccp(visibleSize.width - ceshi_button->getContentSize().width/2 ,ceshi_button->getContentSize().height/2));
	menu->setPosition(CCPointZero);

	CCSprite* titlebar = CCSprite::create("strangedesign/title bar_background.png");
	addChild(titlebar,10);
	titlebar->setPosition(ccp(visibleSize.width/2,visibleSize.height-titlebar->getContentSize().height/2));

	CCSprite* selectionMode = CCSprite::create("strangedesign/Page_selectionmode_character.png");
	addChild(selectionMode,11);
	selectionMode->setPosition(titlebar->getPosition());


	////////////////////////////////////////////////////////////////////////////////
// 	CCSprite* pSprite = CCSprite::create("wall.JPG");
// 	CCSize spriteSize = pSprite->getContentSize();
// 	pSprite->setPosition(ccp(spriteSize.width/2,spriteSize.height/2));
// 	pSprite->setScale(15);
// 	this->addChild(pSprite, 0);

	CCSprite* wall_tail = CCSprite::create("strangedesign/title bar_background.png");
	this->addChild(wall_tail,2);
	CCSize tailSize = wall_tail->getContentSize();
	wall_tail->setPosition(ccp(visibleSize.width/2,wall_tail->getContentSize().height/2));
	wall_tail->setScaleX(visibleSize.width/wall_tail->getContentSize().width);

	//Screenshot function 截屏功能待后续加入
	CCSprite* screenshot = CCSprite::create("strangedesign/Page_Screenshot_button.png");
	addChild(screenshot,12);
	screenshot->setPosition(wall_tail->getPosition());

	vector<string> groupCharacter = SQLiteData::getGroupCharacter(DataTool::intTostring(0));
//	SQLiteData::updateGroupCharacter(DataTool::intTostring(1),DataTool::intTostring(1),DataTool::getChinese("zi"));


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	string myfilename = CCFileUtils::sharedFileUtils()->getWritablePath()+wallXMLCurrent;
	unsigned long size = 0;
	char* pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(myfilename.c_str(),"r",&size);
	TiXmlDocument* myDocument = new TiXmlDocument();
	myDocument->Parse(pFileContent,0,TIXML_ENCODING_UTF8);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
// 	string str_filename = "wall/" + wallXMLCurrent;
	string str_filename = "strangedesign/scoretable.xml";
	string myfilename=CCFileUtils::sharedFileUtils()->fullPathForFilename(str_filename.c_str());
	TiXmlDocument* myDocument = new TiXmlDocument(myfilename.c_str());
	myDocument->LoadFile();
#endif

	TiXmlElement* rootElement = myDocument->RootElement();  // Class
	TiXmlElement* metaElement = rootElement->FirstChildElement();  // meta   
	TiXmlElement* heightElement = metaElement->FirstChildElement(); //获得meta的height元素
	string mheight=heightElement->GetText();
	int height=atoi(mheight.c_str());
	TiXmlElement* widthElement = heightElement->NextSiblingElement(); //获得meta的width元素
	string mwidth=widthElement->GetText(); 
	int width=atoi(mwidth.c_str());

	rescale=(visibleSize.height - wall_tail->getContentSize().height)/(float)height;
	float width_rescale = visibleSize.width/(float)width;
	//////////////
	TiXmlElement* dataElement = metaElement->NextSiblingElement();  // data
	TiXmlElement* stoneElement = dataElement->FirstChildElement();  // stone

	hanzilist.clear();
	int indexOfCharacter = 0;
	while (stoneElement) {
		CHanziManage temphanziManage;
		TiXmlElement* typeElement = stoneElement->FirstChildElement();  // type

		string flag1("wordbox");
// 		string flag2("nouse");
		string flag3(typeElement->GetText());
		if(flag3==flag1)
		{
			//wordbox
			TiXmlElement* xElement = typeElement->NextSiblingElement();  // x
			TiXmlElement* yElement = xElement->NextSiblingElement();  // y
			TiXmlElement* wElement = yElement->NextSiblingElement();  // w
			TiXmlElement* hElement = wElement->NextSiblingElement();  // h
			TiXmlElement* imgElement = hElement->NextSiblingElement();  // img
			TiXmlElement* hanziElement = imgElement->NextSiblingElement();  // hanzi
			TiXmlElement* proficiencyElement = hanziElement->NextSiblingElement();  // proficiency

			string tempx=xElement->GetText();
			int x=atoi((tempx.substr(0,tempx.size()-2)).c_str());
			string tempy=yElement->GetText();
			int y=atoi((tempy.substr(0, tempy.size()-2)).c_str());
			string tempw=wElement->GetText();
			int w=atoi((tempw.substr(0,tempw.size()-2)).c_str());
			string temph=hElement->GetText();
			int h=atoi((temph.substr(0, temph.size()-2)).c_str());

			//坐标系重定位
			x=x+w/2;
			y=height-y-h/2;

			//缩放；
			x*=width_rescale;
			y*=rescale;
			w*=width_rescale;
			h*=rescale;
			y += wall_tail->getContentSize().height;

			string tempfilename=imgElement->GetText();
// 			string temphanzi=hanziElement->GetText();
			//CCLog("temphanzi %s",temphanzi.c_str());
			//			string GBKhanzi = UTF8ToGBK::UTF8TOGBK(temphanzi);
// 			string temppro=proficiencyElement->GetText();
// 			float profic = DataTool::stringToFloat(temppro);

			//stone sprite
			//CCLog("tempfilename %s",tempfilename.c_str());
			CCSprite* pSprite1 = CCSprite::create(tempfilename.c_str());
			pSprite1->setScaleY(rescale);
			pSprite1->setScaleX(width_rescale);
			pSprite1->setPosition(ccp(origin.x+x, origin.y+y));
			this->addChild(pSprite1, 1);

			//color of sprite to show a word proficient
// 			if(profic > 0){
// 				ccColor3B color;
// 				color.r = profic*256;
// 				color.g = color.b = 0;
// 				pSprite1->setColor(color);
// 				pSprite1->setOpacityModifyRGB(true);
// 				pSprite1->setOpacity(125);
// 			}
			//文本框


			hanzis.push_back(groupCharacter.at(indexOfCharacter));


			CCLabelTTF* pLabel = CCLabelTTF::create(groupCharacter.at(indexOfCharacter).c_str(), "Arial", 100);
			pLabel->setPosition(ccp(origin.x + x, origin.y + y));
			pLabel->setColor(ccc3(0,0,0));
			this->addChild(pLabel, 2);

			//汉字管理
			temphanziManage.character=groupCharacter.at(indexOfCharacter);
			temphanziManage.textbox=pLabel;
			temphanziManage.pos=CCPoint(x,y);
			temphanziManage.filename=tempfilename;
// 			temphanziManage.proficiency=atoi(temppro.c_str());

			vector<CHanziManage>::iterator it = hanzilist.end(); 
			hanzilist.insert(it,temphanziManage);
			stoneElement=stoneElement->NextSiblingElement();

			indexOfCharacter++;
		}
// 		if(flag3==flag2)
// 		{
// 			//nouse
// 			TiXmlElement* xElement = typeElement->NextSiblingElement();  // x
// 			TiXmlElement* yElement = xElement->NextSiblingElement();  // y
// 			TiXmlElement* wElement = yElement->NextSiblingElement();  // w
// 			TiXmlElement* hElement = wElement->NextSiblingElement();  // h
// 			TiXmlElement* imgElement = hElement->NextSiblingElement();  // img
// 
// 			string tempx=xElement->GetText();
// 			int x=atoi((tempx.substr(0,tempx.size()-2)).c_str());
// 			string tempy=yElement->GetText();
// 			int y=atoi((tempy.substr(0, tempy.size()-2)).c_str());
// 			string tempw=wElement->GetText();
// 			int w=atoi((tempw.substr(0,tempw.size()-2)).c_str());
// 			string temph=hElement->GetText();
// 			int h=atoi((temph.substr(0, temph.size()-2)).c_str());
// 
// 			//坐标系重定位
// 			x=x+w/2;
// 			y=height-y-h/2;
// 
// 			//缩放；
// 			x*=width_rescale;
// 			y*=rescale;
// 			w*=width_rescale;
// 			h*=rescale;
// 			y += wall_tail->getContentSize().height;
// 
// 			//stone sprite
// 			string tempfilename=imgElement->GetText();
// 			CCSprite* pSprite2 = CCSprite::create(tempfilename.c_str());
// 			pSprite2->setScaleY(rescale);
// 			pSprite2->setScaleX(width_rescale);
// 			pSprite2->setPosition(ccp(origin.x+x, origin.y+y));
// 			this->addChild(pSprite2, 1);
// 			/////////////
// 			stoneElement=stoneElement->NextSiblingElement();
// 		}
	}
}

void WallSingleScene::onExit(){
	CCLayer::onExit();
	CCLog("WallSingleScene::onExit");
	removeAllChildren();
	hanzis.clear();
}


void WallSingleScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	CCLog("Touches Began~~~");
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();

	touchbeginpoint = ccp(pTouch->getLocation().x , pTouch->getLocation().y);
	prePoint = touchbeginpoint;
	touched=true;

	beginTime = millisecondNow();
	//定时器,直接使用scheduleUpdate无效
	//this->scheduleUpdate();
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(WallSingleScene::longPressUpdate),this,1.5f,false);
	for (vector<CHanziManage>::iterator iter = hanzilist.begin();iter!=hanzilist.end();++iter)
	{
		CCPoint hanziPos = iter->pos;
		//CCLog("hanziPos %f %f",hanziPos.x,hanziPos.y);
		CCPoint realPos = ccp(hanziPos.x+changepoint.x,hanziPos.y+changepoint.y);
		//CCLog("hanziPos %f %f",hanziPos.x,hanziPos.y);
		CCRect rect = CCRectMake(realPos.x-100,realPos.y-100,200,200);

		if (rect.containsPoint(touchbeginpoint))
		{
			selectedHanzi = iter->character;
			selectedCHanziManageIter = iter;
		}
	}

}
void WallSingleScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	CCLog("ccTouchesMoved");
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
	if (ccpDistance(prePoint,pTouch->getLocation()) > 50)
	{
		isMoved = true;
	}
	CCPoint newpos=ccp(pTouch->getLocation().x , pTouch->getLocation().y);
	CCPoint temppoint=ccp(newpos.x-touchbeginpoint.x, newpos.y-touchbeginpoint.y);
	changepoint =ccp(changepoint.x+temppoint.x, changepoint.y+temppoint.y);
	////////////////
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	if(changepoint.y!=0)
		changepoint.y=0;

	if(changepoint.x>=0)
		changepoint.x=0;

	if (changepoint.x<=-900*rescale+visibleSize.width)
		changepoint.x=-900*rescale+visibleSize.width;
	////////////////
	touchbeginpoint = newpos;
}

void WallSingleScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
	long endTime = millisecondNow();
	float length = ccpDistance(prePoint,pTouch->getLocation());

	if (endTime-beginTime < 1000 && length <= 50)
	{
		//single click
		CCPoint touchpoint = pTouch->getLocation();
		for (vector<CHanziManage>::iterator iter = hanzilist.begin();iter!=hanzilist.end();++iter)
		{
			CCPoint hanziPos = iter->pos;
			//CCLog("hanziPos %f %f",hanziPos.x,hanziPos.y);
			CCPoint realPos = ccp(hanziPos.x+changepoint.x,hanziPos.y+changepoint.y);
			//CCLog("hanziPos %f %f",hanziPos.x,hanziPos.y);
			CCRect rect = CCRectMake(realPos.x-100,realPos.y-100,200,200);
			//在字周围200像素内，判断为点中
			if (rect.containsPoint(touchpoint))
			{
				CCLog(iter->character.c_str());
				this->singleClick(iter->character);
				return;
			}
		}
	}

	touched=false;
	isMoved = false;
	selectedHanzi = "";

	//解除定时器
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(WallSingleScene::longPressUpdate),this);

}

void WallSingleScene::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,1);
}

void WallSingleScene::update(float delta){
	CCLog("update");
}



void WallSingleScene::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

}

bool WallSingleScene::isInSprite(CCTouch* pTouch){
	// 返回当前触摸位置在OpenGL坐标 
	CCPoint touchPoint=pTouch->getLocation();
	// 将世界坐标转换为当前父View的本地坐标系

	CCPoint reallyPoint=this->getParent()->convertToNodeSpace(touchPoint);
	// 获取当前基于父view的坐标系

	CCRect rect=this->boundingBox();
	// CCnode->convertToNodeSpace 或者  convertToWorldSpace 是基于当前Node的  与当前Node相关

	if(rect.containsPoint(reallyPoint)){
		return true;
	}
	return false;
}

/************************************************************************/
/* string hanzi 传给lianxi界面书写                                                                      */
/************************************************************************/
void WallSingleScene::singleClick(string hanzi){
	//解除schedule,不然可能出现不可预测问题。
	//判断该字在数据库中是否存在，存在跳转，否则提示
	if (SQLiteData::isExist(hanzi))
	{
		this->unscheduleAllSelectors();
		CCDirector::sharedDirector()->getTouchDispatcher()->removeAllDelegates();
		CCDirector::sharedDirector()->pushScene(LianxiScene::create(hanzi));
	}else{
		this->unscheduleAllSelectors();
		MyToast::showToast(this,DataTool::getChinese("not_exist"),TOAST_LONG);
	}

}

void WallSingleScene::popup(string hanzi){
// 	CCLog("popup wall");
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	popL = PopLayer::create(hanzi,"pop/background.png");
	popL->setContentSize(CCSizeMake(winSize.width*0.75,winSize.height*0.75));
	popL->setTitle("test");
	popL->setEditBox();
	popL->setCallBackFunc(this,callfuncN_selector(WallSingleScene::buttonCallBack));
	popL->addButton("sure_up.png","sure_down.png","Y",0);
	popL->addButton("cancer_up.png","cancer_down.png","N",1);
	CCDirector::sharedDirector()->getRunningScene()->addChild(popL,100);
	//解除定时器
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(WallSingleScene::longPressUpdate),this);
}

void WallSingleScene::buttonCallBack(CCNode* pNode){
	CCLog("button call back. tag: %d", pNode->getTag());
	if (pNode->getTag() == 0)
	{
		//弹出对话框，确认，将汉字写到对应位置
		//const char* h = popL->getHanzi();
		for (vector<CHanziManage>::iterator iter = hanzilist.begin(); iter != hanzilist.end(); ++iter)
		{
			if (iter == selectedCHanziManageIter)
			{
				// 将改动的汉字写入到xml文件，saveToFile()
				const char* h = popL->getHanzi();
				//				string dst(h,strlen(h)+1);
				saveToFile(iter->character, h);

				iter->character = string(popL->getHanzi());
				CCLabelTTF* t = (CCLabelTTF*)iter->textbox;
				t->setColor(ccc3(0,0,0));
				t->setString(popL->getHanzi());
			}
		}


	}else
	{
		//弹出对话框，取消，什么都不做
	}
}

void WallSingleScene::longPressUpdate(float fDelta){
	CCLog("longPressUpdate %f",fDelta);

	if (isMoved == false && selectedHanzi.length() > 0 && isLongPressAllow == true)
	{
		popup(selectedHanzi);
	}
	//解除定时器
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(WallSingleScene::longPressUpdate),this);
}

/**
	src 原字
	dst 目标字
	将原字替换成目标字，替换xml文件中查找到的第一个字
*/
void WallSingleScene::saveToFile(string src,const char* dst){
	int i = 0;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	string wallpath = "wall/" + wallXMLCurrent;
	string myfilename=CCFileUtils::sharedFileUtils()->fullPathForFilename(wallpath.c_str());
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	string myfilename = CCFileUtils::sharedFileUtils()->getWritablePath()+wallXMLCurrent;
#endif
	TiXmlDocument* myDocument = new TiXmlDocument(myfilename.c_str());
	myDocument->LoadFile();

	TiXmlElement* rootEle = myDocument->RootElement();
	TiXmlElement* meta = rootEle->FirstChildElement();
	TiXmlElement* data = meta->NextSiblingElement();
	TiXmlElement* stone = data->FirstChildElement();
	while (stone)
	{

		TiXmlElement* type = stone->FirstChildElement();
		string text(type->GetText());
		string wordbox("wordbox");
		if (text == wordbox)
		{
			TiXmlElement* img = stone->FirstChildElement("img");
			TiXmlElement* hanzi = stone->FirstChildElement("hanzi");
			string text(hanzi->GetText());
			if (text == src)
			{
				TiXmlElement* new_hanzi = new TiXmlElement("hanzi");
				TiXmlText* newText = new TiXmlText(dst);
				new_hanzi->InsertEndChild(*newText);
				stone->RemoveChild(hanzi);
				stone->InsertAfterChild(img,*new_hanzi);
				break;
			}
		}
		stone = stone->NextSiblingElement();
	}
	bool ret = myDocument->SaveFile(myfilename.c_str());
}




void WallSingleScene::keyBackClicked(){
	CCLog("WallSingleScene::keyBackClicked");
	if (CCDirector::sharedDirector()->getRunningScene()->getChildByTag(TAG_LAYER_EXIT) == NULL) {
		CCLog("WallSingleScene::NULL");
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		PopLayer* exitDialog = PopLayer::create("pop/background.png");
		exitDialog->setContentSize(CCSizeMake(winSize.width*0.8,winSize.height*0.5));
		exitDialog->setTitle("back",50);
		exitDialog->setContentText("back",60,100,150);
		exitDialog->setCallBackFunc(this,callfuncN_selector(WallSingleScene::backtoMainScene));
		exitDialog->addButton("sure_up.png","sure_down.png","Y",0);
		exitDialog->addButton("cancer_up.png","cancer_down.png","N",1);
		CCDirector::sharedDirector()->getRunningScene()->addChild(exitDialog,100,TAG_LAYER_EXIT);
	}
}

void WallSingleScene::backtoMainScene(CCNode* pNode){
	if (pNode->getTag() == 0)
	{
		CCDirector::sharedDirector()->replaceScene(MainScene::scene());
	}else{
		CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(TAG_LAYER_EXIT);
	}
}

void WallSingleScene::ceshi(CCObject* pSender){
	CCLog("WallSingleScene::ceshi clicked");
// 	MyToast::showToast(this,DataTool::getChinese("stroke_wrong"),2);

	CeshiScene* scene = CeshiScene::create(wallXMLCurrent,hanzis);
	CCDirector::sharedDirector()->pushScene(scene);
}

void WallSingleScene::back(CCObject* pSender){
	CCDirector::sharedDirector()->replaceScene(MainScene::scene());
}