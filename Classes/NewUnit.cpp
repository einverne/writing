#include "NewUnit.h"

#define TAG_LAYER_EXIT 1001

NewUnitLayer::NewUnitLayer(string unitID):tag(0){
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

	//注册触摸事件
	CCPoint changepoint=ccp(0,0);
//	this->setTouchEnabled(true);
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

// 	CCSprite* selectionMode = CCSprite::create("strangedesign/Page_selectionmode_character.png");
// 	addChild(selectionMode,11);
// 	selectionMode->setPosition(titlebar->getPosition());

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
		CCMenuItemImage* updateBtn = CCMenuItemImage::create("strangedesign/Edit_Finish_button.png",
			"strangedesign/Edit_Finish_button_down.png",
			this,
			menu_selector(NewUnitLayer::updateUnit));
		CCPoint updateBtnPosition = ccp(visibleSize.width, visibleSize.height) - ccp(updateBtn->getContentSize().width/2,updateBtn->getContentSize().height/2);
		updateBtn->setPosition(updateBtnPosition);
		menu->addChild(updateBtn);
	}else
	{
		CCMenuItemImage* finishBtn = CCMenuItemImage::create("main_button.png",
			"mail_button.png",
			this,
			menu_selector(NewUnitLayer::finishAddUnit));
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

	TiXmlElement* dataElement = metaElement->NextSiblingElement();  // data
	TiXmlElement* stoneElement = dataElement->FirstChildElement();  // stone

	hanzilist.clear();
	int indexOfCharacter = 0;
	while (stoneElement) {
		CHanziManage temphanziManage;
		TiXmlElement* typeElement = stoneElement->FirstChildElement();  // type

		string flag1("wordbox");
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

			CCSize originsize = pSprite1->getContentSize();
			CCRect fullRect = CCRectMake(0,0,originsize.width,originsize.height);
			CCRect insetRect = fullRect;
			CCScale9Sprite* scalesprite = CCScale9Sprite::create(tempfilename.c_str(),fullRect, insetRect);

			CCSize editBoxSize = ccp(scalesprite->getContentSize().width*width_rescale, scalesprite->getContentSize().height*rescale);
			CCEditBox* editboxtemp = CCEditBox::create(editBoxSize,scalesprite);
			editboxtemp->setPosition(ccp(origin.x+x, origin.y+y));
			addChild(editboxtemp, 1);

			hanzis.push_back(groupCharacter.at(indexOfCharacter).at(0));

			//添加汉字
			editboxtemp->setPlaceHolder("Input Chinese Character:");
			editboxtemp->setPlaceholderFontColor(ccBLACK);
			editboxtemp->setPlaceholderFont("Arial",25);
// 			editboxtemp->setPlaceholderFontSize(100);

			editboxtemp->setText(groupCharacter.at(indexOfCharacter).at(0).c_str());
			editboxtemp->setFontName("Arial");
			editboxtemp->setFontSize(100);
			editboxtemp->setFontColor(ccBLACK);
			editboxtemp->setMaxLength(1);
			editboxtemp->setInputMode(kEditBoxInputModeSingleLine);

			editboxtemp->setDelegate(this);
			editboxtemp->setTag(tag);
			tag++;

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
// 			temphanziManage.textbox=pLabel;
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
// 	CCLog("WallSingleScene::onExit");
// 	removeAllChildren();
// 	hanzis.clear();
}

void NewUnitLayer::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,1);
}

void NewUnitLayer::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
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

void NewUnitLayer::finishAddUnit(CCObject* pSender){
	CCLog("finish");
	SQLiteData::insertUnit(groupCharacter);
	CCDirector::sharedDirector()->replaceScene(MainScene::scene());
}

void NewUnitLayer::deleteUnit(CCObject* pSender){
	//delete unit with unitID method has deprecated
	SQLiteData::deleteUnit(unitID);
	CCDirector::sharedDirector()->replaceScene(MainScene::scene());
}

void NewUnitLayer::updateUnit(CCObject* pSender){
	CCLog("updateUnit");
	SQLiteData::updateUnit(unitID,groupCharacter);
	MyToast::showToast(this,DataTool::getChinese("update_unit"),TOAST_LONG);
	CCDirector::sharedDirector()->replaceScene(MainScene::scene());
}

void NewUnitLayer::editBoxEditingDidBegin(CCEditBox* editBox){
	CCLog("editbox begin");

}

void NewUnitLayer::editBoxEditingDidEnd(CCEditBox* editBox){
	CCLog("editbox end");

}

void NewUnitLayer::editBoxTextChanged(CCEditBox* editBox, const std::string& text){
	CCLog("editbox changed");

}

void NewUnitLayer::editBoxReturn(CCEditBox* editBox){
	CCLog("editbox return");
	int i = editBox->getTag();
	vector<string> newSingle;
	newSingle.push_back(string(editBox->getText()));
	newSingle.push_back(DataTool::intTostring(0));
	newSingle.push_back(DataTool::intTostring(2));
	groupCharacter[i] = newSingle;
}