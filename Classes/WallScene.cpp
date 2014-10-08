#include "WallScene.h"
#include "tinyxml.h" 
#include "PopLayer.h"
#include "tools/DataTool.h"
#include "LianxiScene.h"
#include "MainScene.h"

USING_NS_CC;

#define TAG_LAYER_EXIT 1001

//////////////////////////////////////////
CCScene* WallScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	WallScene *layer = WallScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool WallScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	isMoved = false;
	touched = false;

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	//     CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
	// 	"CloseNormal.png",
	// 	"CloseSelected.png",
	// 	this,
	// 	menu_selector(WallScene::menuCloseCallback));
	// 
	//     pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
	// 	origin.y + pCloseItem->getContentSize().height/2));
	// 
	//     // create menu, it's an autorelease object
	//     CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	//     pMenu->setPosition(CCPointZero);
	//     this->addChild(pMenu, 3);

	/////////////////////////////
	// 3. add your codes below...
	//////////////////////////////////////////////////////////////////////////////
	//添加地图：
	//1.读取xml文件，确定缩放比例//
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	string myfilename = CCFileUtils::sharedFileUtils()->getWritablePath()+"wall.xml";
	unsigned long size = 0;
	char* pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(myfilename.c_str(),"r",&size);
	TiXmlDocument* myDocument = new TiXmlDocument();
	myDocument->Parse(pFileContent,0,TIXML_ENCODING_UTF8);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	string myfilename=CCFileUtils::sharedFileUtils()->fullPathForFilename("wall.xml");
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

	rescale=visibleSize.height/(float)height;
	//////////////
	TiXmlElement* dataElement = metaElement->NextSiblingElement();  // data
	TiXmlElement* stoneElement = dataElement->FirstChildElement();  // stone

	hanzilist.clear();
	while (stoneElement) {
		CHanziManage temphanziManage;
		TiXmlElement* typeElement = stoneElement->FirstChildElement();  // type

		string flag1("wordbox");
		string flag2("nouse");
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
			x*=rescale;
			y*=rescale;
			w*=rescale;
			h*=rescale;

			string tempfilename=imgElement->GetText();
			string temphanzi=hanziElement->GetText();
			//CCLog("temphanzi %s",temphanzi.c_str());
			//			string GBKhanzi = UTF8ToGBK::UTF8TOGBK(temphanzi);
			string temppro=proficiencyElement->GetText();

			//stone sprite
			//CCLog("tempfilename %s",tempfilename.c_str());
			CCSprite* pSprite1 = CCSprite::create(tempfilename.c_str());
			pSprite1->setScale(rescale);
			pSprite1->setPosition(ccp(origin.x+x, origin.y+y));
			this->addChild(pSprite1, 1);

			//文本框
			CCLabelTTF* pLabel = CCLabelTTF::create(temphanzi.c_str(), "XingShu", 100);
			pLabel->setPosition(ccp(origin.x + x, origin.y + y));
			this->addChild(pLabel, 2);

			//汉字管理
			temphanziManage.character=hanziElement->GetText();
			temphanziManage.textbox=pLabel;
			temphanziManage.pos=CCPoint(x,y);
			temphanziManage.filename=tempfilename;
			temphanziManage.proficiency=atoi(temppro.c_str());

			vector<CHanziManage>::iterator it = hanzilist.end(); 
			hanzilist.insert(it,temphanziManage);
			stoneElement=stoneElement->NextSiblingElement();

		}
		if(flag3==flag2)
		{
			//nouse
			TiXmlElement* xElement = typeElement->NextSiblingElement();  // x
			TiXmlElement* yElement = xElement->NextSiblingElement();  // y
			TiXmlElement* wElement = yElement->NextSiblingElement();  // w
			TiXmlElement* hElement = wElement->NextSiblingElement();  // h
			TiXmlElement* imgElement = hElement->NextSiblingElement();  // img

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
			x*=rescale;
			y*=rescale;
			w*=rescale;
			h*=rescale;

			//stone sprite
			string tempfilename=imgElement->GetText();
			CCSprite* pSprite2 = CCSprite::create(tempfilename.c_str());
			pSprite2->setScale(rescale);
			pSprite2->setPosition(ccp(origin.x+x, origin.y+y));
			this->addChild(pSprite2, 1);
			/////////////
			stoneElement=stoneElement->NextSiblingElement();
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	CCSprite* pSprite = CCSprite::create("wall.JPG");
	CCSize spriteSize = pSprite->getContentSize();
	pSprite->setPosition(ccp(spriteSize.width/2,spriteSize.height/2));
	pSprite->setScale(15);
	this->addChild(pSprite, 0);

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

void WallScene::onEnter(){
	CCLayer::onEnter();

}

void WallScene::onExit(){
	CCLayer::onExit();
}

// bool  WallScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
// {
// 	popup();
// 	touchbeginpoint = ccp(pTouch->getLocation().x , pTouch->getLocation().y);
// 	touched=true;
// 	beginTime = millisecondNow();
// 	return true;
// }
// void  WallScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
// {
// 
// 	long endTime = millisecondNow();
// 	if (endTime-beginTime < 250 && isMoved==false)
// 	{
// 		//single click
// 		CCPoint touchpoint = pTouch->getLocation();
// 		//CCLog("Touchpoint %f %f",touchpoint.x,touchpoint.y);
// 		for (vector<CHanziManage>::iterator iter = hanzilist.begin();iter!=hanzilist.end();++iter)
// 		{
// 			CCPoint hanziPos = iter->pos;
// 			//CCLog("hanziPos %f %f",hanziPos.x,hanziPos.y);
// 			CCPoint realPos = ccp(hanziPos.x+changepoint.x,hanziPos.y+changepoint.y);
// 			//CCLog("hanziPos %f %f",hanziPos.x,hanziPos.y);
// 			CCRect rect = CCRectMake(realPos.x-100,realPos.y-100,200,200);
// 
// 			if (rect.containsPoint(touchpoint))
// 			{
// 				CCLog(iter->character.c_str());
// 				this->singleClick(iter->character);
// 				return;
// 			}
// 		}
// 	}
// 
// 	if (endTime-beginTime > 3000)
// 	{
// 		popup();
// 	}
// 	touched=false;
// 	isMoved = false;
// }
// void  WallScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
// {
// 	isMoved = true;
// 	CCPoint newpos=ccp(pTouch->getLocation().x , pTouch->getLocation().y);
// 	CCPoint temppoint=ccp(newpos.x-touchbeginpoint.x, newpos.y-touchbeginpoint.y);
// 	changepoint =ccp(changepoint.x+temppoint.x, changepoint.y+temppoint.y);
// 	//CCLog("%f %f",changepoint.x,changepoint.y);
// 	////////////////
// 	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
// 
// 	if(changepoint.y!=0)
// 		changepoint.y=0;
// 
// 	if(changepoint.x>=0)
// 		changepoint.x=0;
// 
// 	if (changepoint.x<=-3000*rescale+visibleSize.width)
// 		changepoint.x=-3000*rescale+visibleSize.width;
// 	////////////////
// 	this->setPosition(changepoint);
// 	touchbeginpoint = newpos;
// }

void WallScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	CCLog("Touches Began~~~");
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();

	touchbeginpoint = ccp(pTouch->getLocation().x , pTouch->getLocation().y);
	prePoint = touchbeginpoint;
	touched=true;

	beginTime = millisecondNow();
	//定时器,直接使用scheduleUpdate无效
	//this->scheduleUpdate();
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(WallScene::longPressUpdate),this,1.5f,false);
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
void WallScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	CCLog("ccTouchesMoved");
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
	if (ccpDistance(prePoint,pTouch->getLocation()) > 50)
	{
		isMoved = true;
	}
	CCPoint newpos=ccp(pTouch->getLocation().x , pTouch->getLocation().y);
	CCPoint temppoint=ccp(newpos.x-touchbeginpoint.x, newpos.y-touchbeginpoint.y);
	changepoint =ccp(changepoint.x+temppoint.x, changepoint.y+temppoint.y);
	//CCLog("%f %f",changepoint.x,changepoint.y);
	////////////////
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	if(changepoint.y!=0)
		changepoint.y=0;

	if(changepoint.x>=0)
		changepoint.x=0;

	if (changepoint.x<=-3000*rescale+visibleSize.width)
		changepoint.x=-3000*rescale+visibleSize.width;
	////////////////
	this->setPosition(changepoint);
	touchbeginpoint = newpos;
}

void WallScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
	long endTime = millisecondNow();
	float length = ccpDistance(prePoint,pTouch->getLocation());
// 	CCLog("length:%f  %f",length,endTime-beginTime);

	if (endTime-beginTime < 1000 && length <= 50)
	{
		//single click
		CCPoint touchpoint = pTouch->getLocation();
		//CCLog("Touchpoint %f %f",touchpoint.x,touchpoint.y);
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

	// 	if (endTime-beginTime > 3000)
	// 	{
	// 		popup();
	// 	}
	touched=false;
	isMoved = false;
	selectedHanzi = "";

	//解除定时器
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(WallScene::longPressUpdate),this);

}

void WallScene::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,1);
}

void WallScene::update(float delta){
	CCLog("update");
}



void WallScene::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
	////
	//this->setTouchEnabled(false);
	//CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	////
	// #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	// 	exit(0);
	// #endif
}

bool WallScene::isInSprite(CCTouch* pTouch){
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
void WallScene::singleClick(string hanzi){
	//解除schedule,不然可能出现不可预测问题。
	
// 	CCDirector::sharedDirector()->replaceScene(lianxi::scene(hanzi));
	if (hanzi != "a")
	{
		this->unscheduleAllSelectors();
		CCDirector::sharedDirector()->getTouchDispatcher()->removeAllDelegates();
		CCDirector::sharedDirector()->replaceScene(LianxiScene::create(hanzi));
	}

// 	CCDirector::sharedDirector()->pushScene(lianxi::scene(hanzi));
// 	CCDirector::sharedDirector()->pushScene(LianxiScene::create(hanzi));
}

void WallScene::popup(string hanzi){
// 	CCLog("popup wall");
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	popL = PopLayer::create(hanzi,"pop/background.png");
	popL->setContentSize(CCSizeMake(winSize.width*0.75,winSize.height*0.75));
	popL->setTitle("test");
	popL->setEditBox();
	popL->setCallBackFunc(this,callfuncN_selector(WallScene::buttonCallBack));
	popL->addButton("sure_up.png","sure_down.png","Y",0);
	popL->addButton("cancer_up.png","cancer_down.png","N",1);
	CCDirector::sharedDirector()->getRunningScene()->addChild(popL,100);
	//解除定时器
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(WallScene::longPressUpdate),this);
}

void WallScene::buttonCallBack(CCNode* pNode){
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
				t->setString(popL->getHanzi());
			}
		}


	}else
	{
		//弹出对话框，取消，什么都不做
	}
}

void WallScene::longPressUpdate(float fDelta){
	CCLog("Update %f",fDelta);

	if (isMoved == false && selectedHanzi.length() > 0)
	{
		popup(selectedHanzi);
	}
	//解除定时器
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(WallScene::longPressUpdate),this);
}

void WallScene::saveToFile(string src,const char* dst){
	int i = 0;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	string myfilename=CCFileUtils::sharedFileUtils()->fullPathForFilename("wall.xml");
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	string myfilename = CCFileUtils::sharedFileUtils()->getWritablePath()+"wall.xml";
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
				// 				string dst_str = GBKToUTF8(dst);
				// 				string dst_ss = DataTool::GB2312ToUTF8(dst);
				// 				CCLog("DST XXXX%s XXXX%s",dst.c_str(),dst_str.c_str());
				// 				const char* temp = dst.c_str();
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

void WallScene::keyBackClicked(){
	CCLog("WallScene::keyBackClicked");
	if (CCDirector::sharedDirector()->getRunningScene()->getChildByTag(TAG_LAYER_EXIT) == NULL) {
		CCLog("WallScene::NULL");
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		PopLayer* exitDialog = PopLayer::create("pop/background.png");
		exitDialog->setContentSize(CCSizeMake(winSize.width*0.8,winSize.height*0.5));
		exitDialog->setTitle("back",50);
		exitDialog->setContentText("back",60,100,150);
		exitDialog->setCallBackFunc(this,callfuncN_selector(WallScene::backtoMainScene));
		exitDialog->addButton("sure_up.png","sure_down.png","Y",0);
		exitDialog->addButton("cancer_up.png","cancer_down.png","N",1);
		CCDirector::sharedDirector()->getRunningScene()->addChild(exitDialog,100,TAG_LAYER_EXIT);
	}
}

void WallScene::backtoMainScene(CCNode* pNode){
	if (pNode->getTag() == 0)
	{
		CCDirector::sharedDirector()->replaceScene(MainScene::scene());
	}else{
		CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(TAG_LAYER_EXIT);
	}
}