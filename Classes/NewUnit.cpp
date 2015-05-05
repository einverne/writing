#include "NewUnit.h"

#define TAG_LAYER_EXIT 1001

NewUnitLayer::NewUnitLayer(string unitID){
	isLongPressAllow = false;		//允许长按响应
	this->unitID = unitID;
}

NewUnitLayer::~NewUnitLayer(){

}

CCScene* NewUnitLayer::scene(string unitID)
{
	CCScene *scene = CCScene::create();
	NewUnitLayer *layer = NewUnitLayer::create(unitID);
	scene->addChild(layer);
	return scene;
}

NewUnitLayer* NewUnitLayer::create(string unitID){
	NewUnitLayer* pret = new NewUnitLayer(unitID);
	if (pret && pret->init())
	{
		pret->autorelease();
		return pret;
	}else{
		delete pret;
		pret = NULL;
		return pret;
	}
}

bool NewUnitLayer::init()
{
	if ( !CCLayerColor::initWithColor(ccc4(255,255,255,255)) )
	{
		return false;
	}
	CCLog("NewUnitLayer::init");

	isMoved = false;
	touched = false;

	//注册触摸事件
	CCPoint changepoint=ccp(0,0);
	touched=false;
	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);			//android back key

	return true;
}

void NewUnitLayer::onEnter(){
	CCLayer::onEnter();
	CCLog("NewUnitLayer::onEnter()");
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCSprite* titlebar = CCSprite::create("strangedesign/title bar_background.png");
	addChild(titlebar,10);
	titlebar->setPosition(ccp(visibleSize.width/2,visibleSize.height-titlebar->getContentSize().height/2));

	CCSprite* selectionMode = CCSprite::create("strangedesign/Page_selectionmode_character.png");
	addChild(selectionMode,11);
	selectionMode->setPosition(titlebar->getPosition());

	CCSprite* wall_tail = CCSprite::create("strangedesign/title bar_background.png");
	addChild(wall_tail,2);
	CCSize tailSize = wall_tail->getContentSize();
	wall_tail->setPosition(ccp(visibleSize.width/2,wall_tail->getContentSize().height/2));
	wall_tail->setScaleX(visibleSize.width/wall_tail->getContentSize().width);

	CCMenuItemImage* back_button = CCMenuItemImage::create("strangedesign/back_button.png",
		"strangedesign/back_button_down.png",
		this,
		menu_selector(NewUnitLayer::back));
	back_button->setPosition(ccp(back_button->getContentSize().width/2+10, visibleSize.height-back_button->getContentSize().height/2-50));

	CCMenu* menu = CCMenu::create(back_button, NULL);
	this->addChild(menu,20);
	menu->setPosition(CCPointZero);

	if (unitID != "add")
	{
		CCMenuItemImage* deleteBtn = CCMenuItemImage::create("strangedesign/Dlg_delete_button.png",
			"strangedesign/Dlg_delete_button_down.png",
			this,
			menu_selector(NewUnitLayer::del));
		deleteBtn->setPosition(wall_tail->getPosition()-ccp(wall_tail->getPositionX()/2,0));
		CCMenuItemImage* updateBtn = CCMenuItemImage::create("strangedesign/Dlg_edit_button.png",
			"strangedesign/Dlg_edit_button_down.png",
			this,
			menu_selector(NewUnitLayer::updateUnit));
		updateBtn->setPosition(wall_tail->getPosition()+ccp(wall_tail->getPositionX()/2,0));
		menu->addChild(deleteBtn);
		menu->addChild(updateBtn);
	}else
	{
		CCMenuItemImage* finishBtn = CCMenuItemImage::create("main_button.png",
			"mail_button.png",
			this,
			menu_selector(NewUnitLayer::finish));
		finishBtn->setPosition(wall_tail->getPosition());
		menu->addChild(finishBtn);
	}
	


	if (unitID != "add")
	{
		groupCharacter = SQLiteData::getUnit(unitID);
	}else
	{
		for (int i = 0; i< 16; i++)
		{
			vector<string> single;
			single.push_back(DataTool::intTostring(i));
			single.push_back(DataTool::intTostring(0));
			single.push_back(DataTool::intTostring(0));
			groupCharacter.push_back(single);
		}
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	string str_filename = "strangedesign/scoretable.xml";
	string myfilename=CCFileUtils::sharedFileUtils()->fullPathForFilename(str_filename.c_str());
	unsigned long size = 0;
	char* pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(myfilename.c_str(),"r",&size);
	TiXmlDocument* myDocument = new TiXmlDocument();
	myDocument->Parse(pFileContent,0,TIXML_ENCODING_UTF8);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
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
			TiXmlElement* characterposx = imgElement->NextSiblingElement();	//
			TiXmlElement* characterposy = characterposx->NextSiblingElement();
			TiXmlElement* scoreposx = characterposy->NextSiblingElement();
			TiXmlElement* scoreposy = scoreposx->NextSiblingElement();
			TiXmlElement* timesposx = scoreposy->NextSiblingElement();
			TiXmlElement* timesposy = timesposx->NextSiblingElement();

			string tempx=xElement->GetText();
			int x=atoi((tempx.substr(0,tempx.size()-2)).c_str());
			string tempy=yElement->GetText();
			int y=atoi((tempy.substr(0, tempy.size()-2)).c_str());
			string tempw=wElement->GetText();
			int w=atoi((tempw.substr(0,tempw.size()-2)).c_str());
			string temph=hElement->GetText();
			int h=atoi((temph.substr(0, temph.size()-2)).c_str());

			string strcharacterposx=characterposx->GetText();
			int intcharacterposx=atoi((strcharacterposx.substr(0,strcharacterposx.size())).c_str());
			string strcharacterposy=characterposy->GetText();
			int intcharacterposy=atoi((strcharacterposy.substr(0, strcharacterposy.size())).c_str());
			string strscoreposx=scoreposx->GetText();
			int intscoreposx=atoi((strscoreposx.substr(0,strscoreposx.size())).c_str());
			string strscoreposy=scoreposy->GetText();
			int intscoreposy=atoi((strscoreposy.substr(0,strscoreposy.size())).c_str());
			string strtimesposx=timesposx->GetText();
			int inttimesposx=atoi((strtimesposx.substr(0,strtimesposx.size())).c_str());
			string strtimesposy=timesposy->GetText();
			int inttimesposy=atoi((strtimesposy.substr(0, strtimesposy.size())).c_str());


			//坐标系重定位 将XML中第四象限坐标转成第一象限
			x+=w/2;
			y=height-y-h/2;


			intcharacterposx=height-intcharacterposx;
			intscoreposy=height-intscoreposy;
			inttimesposy=height-inttimesposy;


			//缩放
			x*=width_rescale;
			y*=rescale;

			intcharacterposx*=width_rescale;
			intcharacterposy*=rescale;
			intscoreposx*=width_rescale;
			intscoreposy*=rescale;
			inttimesposx*=width_rescale;
			inttimesposy*=rescale;

			w*=width_rescale;
			h*=rescale;
			y += wall_tail->getContentSize().height-20;

			intcharacterposy += wall_tail->getContentSize().height-20;
			intscoreposy += wall_tail->getContentSize().height-20;
			inttimesposy += wall_tail->getContentSize().height-20;

			string tempfilename=imgElement->GetText();

			//添加田字格背景图
			CCSprite* pSprite1 = CCSprite::create(tempfilename.c_str());
			pSprite1->setScaleY(rescale);
			pSprite1->setScaleX(width_rescale);
			pSprite1->setPosition(ccp(origin.x+x, origin.y+y));
			addChild(pSprite1, 1);

			hanzis.push_back(groupCharacter.at(indexOfCharacter).at(0));

			//添加汉字
			CCLabelTTF* pLabel = CCLabelTTF::create(groupCharacter.at(indexOfCharacter).at(0).c_str(), "Arial", 100);
			pLabel->setPosition(ccp(origin.x + x, origin.y + y));
			pLabel->setColor(ccc3(0,0,0));
			this->addChild(pLabel, 2);
			//添加次数
			CCLabelTTF* timesLabel = CCLabelTTF::create(groupCharacter.at(indexOfCharacter).at(1).c_str(),"Arial",35);
			timesLabel->setPosition(ccp(origin.x+inttimesposx,origin.y+inttimesposy));
			timesLabel->setColor(ccc3(0,0,0));
			addChild(timesLabel,2);
			//添加评判最高分
			CCLabelTTF* scoreLabel = CCLabelTTF::create(groupCharacter.at(indexOfCharacter).at(2).c_str(),"Arial",35);
			scoreLabel->setPosition(ccp(origin.x+intscoreposx,origin.y+intscoreposy));
			scoreLabel->setColor(ccc3(0,0,0));
			addChild(scoreLabel,2);

			//汉字管理
			temphanziManage.character=groupCharacter.at(indexOfCharacter).at(0);
			temphanziManage.textbox=pLabel;
			temphanziManage.pos=CCPoint(x,y);
			temphanziManage.filename=tempfilename;
// 			temphanziManage.proficiency=atoi(temppro.c_str());

			vector<CHanziManage>::iterator it = hanzilist.end(); 
			hanzilist.insert(it,temphanziManage);
			stoneElement=stoneElement->NextSiblingElement();

			indexOfCharacter++;
		}
	}
}

void NewUnitLayer::onExit(){
	CCLayer::onExit();
	CCLog("WallSingleScene::onExit");
	removeAllChildren();
	hanzis.clear();
}

void NewUnitLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	CCLog("Touches Began~~~");
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();

	touchbeginpoint = ccp(pTouch->getLocation().x , pTouch->getLocation().y);
	prePoint = touchbeginpoint;
	touched=true;

	beginTime = millisecondNow();
	//定时器,直接使用scheduleUpdate无效
	//this->scheduleUpdate();
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(NewUnitLayer::longPressUpdate),this,1.5f,false);
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
void NewUnitLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
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

void NewUnitLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
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
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NewUnitLayer::longPressUpdate),this);

}

void NewUnitLayer::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,1);
}

void NewUnitLayer::update(float delta){
	CCLog("update");
}

void NewUnitLayer::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
}

bool NewUnitLayer::isInSprite(CCTouch* pTouch){
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

void NewUnitLayer::singleClick(string hanzi){
	CCLog("singleClick");
	if (isMoved == false && selectedHanzi.length() > 0)
	{
		popup(selectedHanzi);
	}
}

void NewUnitLayer::popup(string hanzi){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	popL = PopLayer::create(hanzi,"pop/background.png");
	popL->setContentSize(CCSizeMake(winSize.width*0.75,winSize.height*0.75));
	popL->setTitle("Modify Unit");
	popL->setEditBox();
	popL->setCallBackFunc(this,callfuncN_selector(NewUnitLayer::buttonCallBack));
	popL->addButton("sure_up.png","sure_down.png","Y",0);
	popL->addButton("cancer_up.png","cancer_down.png","N",1);
	CCDirector::sharedDirector()->getRunningScene()->addChild(popL,100);
	//解除定时器
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NewUnitLayer::longPressUpdate),this);
}

void NewUnitLayer::buttonCallBack(CCNode* pNode){
	CCLog("button call back. tag: %d", pNode->getTag());
	if (pNode->getTag() == 0)
	{
		//弹出对话框，确认，将汉字写到对应位置
		for (vector<CHanziManage>::iterator iter = hanzilist.begin(); iter != hanzilist.end(); ++iter)
		{
			if (iter == selectedCHanziManageIter)
			{
				// 将改动的汉字
				const char* h = popL->getHanzi();			//改动的汉字

				//将汉字更新到groupCharacter
				for (int i = 0 ;i < groupCharacter.size(); ++i)
				{
					if (iter->character == groupCharacter.at(i).at(0))
					{
						vector<string> newSingle;
						newSingle.push_back(string(h));
						newSingle.push_back(DataTool::intTostring(0));
						newSingle.push_back(DataTool::intTostring(2));
						groupCharacter[i] = newSingle;
						break;
					}
				}
				//SQLiteData::updateUnit(unitID,groupCharacter);

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

void NewUnitLayer::longPressUpdate(float fDelta){
	CCLog("longPressUpdate %f",fDelta);
	if (isLongPressAllow == false){
		//解除定时器
		CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NewUnitLayer::longPressUpdate),this);
		return;
	}
	if (isMoved == false && selectedHanzi.length() > 0)
	{
		popup(selectedHanzi);
	}
	//解除定时器
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NewUnitLayer::longPressUpdate),this);
}

void NewUnitLayer::keyBackClicked(){
	CCLog("WallSingleScene::keyBackClicked");
	if (CCDirector::sharedDirector()->getRunningScene()->getChildByTag(TAG_LAYER_EXIT) == NULL) {
		CCLog("WallSingleScene::NULL");
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		PopLayer* exitDialog = PopLayer::create("pop/background.png");
		exitDialog->setContentSize(CCSizeMake(winSize.width*0.8,winSize.height*0.5));
		exitDialog->setTitle("back",50);
		exitDialog->setContentText("back",60,100,150);
		exitDialog->setCallBackFunc(this,callfuncN_selector(NewUnitLayer::backtoMainScene));
		exitDialog->addButton("sure_up.png","sure_down.png","Y",0);
		exitDialog->addButton("cancer_up.png","cancer_down.png","N",1);
		CCDirector::sharedDirector()->getRunningScene()->addChild(exitDialog,100,TAG_LAYER_EXIT);
	}
}

void NewUnitLayer::backtoMainScene(CCNode* pNode){
	if (pNode->getTag() == 0)
	{
		CCDirector::sharedDirector()->replaceScene(MainScene::scene());
	}else{
		CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(TAG_LAYER_EXIT);
	}
}

void NewUnitLayer::back(CCObject* pSender){
	CCDirector::sharedDirector()->replaceScene(MainScene::scene());
}

void NewUnitLayer::finish(CCObject* pSender){
	CCLog("finish");
	SQLiteData::insertUnit(groupCharacter);
	CCDirector::sharedDirector()->replaceScene(MainScene::scene());
}

void NewUnitLayer::del(CCObject* pSender){
	CCLog("del");
	SQLiteData::deleteUnit(unitID);
	CCDirector::sharedDirector()->replaceScene(MainScene::scene());
}

void NewUnitLayer::updateUnit(CCObject* pSender){
	CCLog("updateUnit");
	SQLiteData::updateUnit(unitID,groupCharacter);
	MyToast::showToast(this,DataTool::getChinese("update_unit"),TOAST_LONG);
//	CCDirector::sharedDirector()->replaceScene(MainScene::scene());
}
