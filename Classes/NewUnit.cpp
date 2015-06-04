#include "NewUnit.h"

#define TAG_LAYER_EXIT 1001

NewUnitLayer::NewUnitLayer(string unitID):tag(0),
	m_editBox(NULL),m_TextList(NULL)
{
	this->unitID = unitID;
}

NewUnitLayer::~NewUnitLayer(){
	CC_SAFE_RELEASE(m_editBox);
	CC_SAFE_RELEASE(m_TextList);
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
		CC_SAFE_DELETE(pret);
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
	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);			//android back key

	m_TextList = CCArray::create();
	m_TextList->retain();
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
		CCMenuItemImage* finishBtn = CCMenuItemImage::create("strangedesign/Edit_Finish_button.png",
			"strangedesign/Edit_Finish_button_down.png",
			this,
			menu_selector(NewUnitLayer::finishAddUnit));
		CCPoint finishBtnPosition = ccp(visibleSize.width, visibleSize.height) - ccp(finishBtn->getContentSize().width/2,finishBtn->getContentSize().height/2);
		finishBtn->setPosition(finishBtnPosition);
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
		CCLog("New Unit stone");

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
			addChild(pSprite1,1);

			CCSize originsize = pSprite1->getContentSize();
			CCRect fullRect = CCRectMake(0,0,originsize.width,originsize.height);
			CCRect insetRect = fullRect;
			CCScale9Sprite* scalesprite = CCScale9Sprite::create(tempfilename.c_str(),fullRect, insetRect);

			CCLog("add editbox");

			string placeholder = groupCharacter.at(indexOfCharacter).at(0);
			CCTextFieldTTF*	m_TextFieldTTF = CCTextFieldTTF::textFieldWithPlaceHolder(placeholder.c_str(), "Arial", 80);
			m_TextFieldTTF->setPosition(ccp(origin.x+x, origin.y+y));
			addChild(m_TextFieldTTF, 1);

			m_TextFieldTTF->setColor(ccBLACK);

			m_TextFieldTTF->setDelegate(this);
			
			m_TextList->addObject(m_TextFieldTTF);

// 			CCSize editBoxSize = ccp(scalesprite->getContentSize().width*width_rescale, scalesprite->getContentSize().height*rescale);
// 			m_editBox = CCEditBox::create(editBoxSize,scalesprite);
// 			m_editBox->setPosition(ccp(origin.x+x, origin.y+y));
// 			addChild(m_editBox, 1);

			hanzis.push_back(groupCharacter.at(indexOfCharacter).at(0));
// 
// 			//添加汉字
// 			m_editBox->setPlaceHolder("Input Chinese Character:");
// 			m_editBox->setPlaceholderFontColor(ccBLACK);
// 
// 			m_editBox->setText(groupCharacter.at(indexOfCharacter).at(0).c_str());
// 			m_editBox->setFontName("Arial");
// 			m_editBox->setFontSize(100);
// 			m_editBox->setFontColor(ccBLACK);
// 			m_editBox->setMaxLength(1);
// 			m_editBox->setInputMode(kEditBoxInputModeSingleLine);
// 
// 			m_editBox->setDelegate(this);
// 			m_editBox->setTag(tag);

			m_TextFieldTTF->setTag(tag);
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

void NewUnitLayer::onExit(){
	CCLayer::onExit();
}

void NewUnitLayer::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,1,false);
}

void NewUnitLayer::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
}

void NewUnitLayer::keyBackClicked(){
	CCLog("WallSingleScene::keyBackClicked");
	CCDirector::sharedDirector()->replaceScene(MainScene::scene());
// 	if (CCDirector::sharedDirector()->getRunningScene()->getChildByTag(TAG_LAYER_EXIT) == NULL) {
// 		CCLog("WallSingleScene::NULL");
// 		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
// 		PopLayer* exitDialog = PopLayer::create("pop/background.png");
// 		exitDialog->setContentSize(CCSizeMake(winSize.width*0.8,winSize.height*0.5));
// 		exitDialog->setTitle("back",50);
// 		exitDialog->setContentText("back",60,100,150);
// 		exitDialog->setCallBackFunc(this,callfuncN_selector(NewUnitLayer::backtoMainScene));
// 		exitDialog->addButton("sure_up.png","sure_down.png","Y",0);
// 		exitDialog->addButton("cancer_up.png","cancer_down.png","N",1);
// 		CCDirector::sharedDirector()->getRunningScene()->addChild(exitDialog,100,TAG_LAYER_EXIT);
// 	}
}

// void NewUnitLayer::backtoMainScene(CCNode* pNode){
// 	if (pNode->getTag() == 0)
// 	{
// 		CCDirector::sharedDirector()->replaceScene(MainScene::scene());
// 	}else{
// 		CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(TAG_LAYER_EXIT);
// 	}
// }

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

bool NewUnitLayer::ccTouchBegan(CCTouch* touch, CCEvent* event){
	CCLog("touch began");
	return true;
}

void NewUnitLayer::ccTouchMoved(CCTouch* touch, CCEvent* event){
	CCLog("touch moved");
}

void NewUnitLayer::ccTouchEnded(CCTouch* touch, CCEvent* event){
	CCLog("touch ended");
	CCPoint pos = touch->getLocation();

	CCObject* it = NULL;
	CCARRAY_FOREACH(m_TextList,it){
		CCTextFieldTTF* textField = dynamic_cast<CCTextFieldTTF*>(it);
		float x = textField->getPositionX() - textField->getContentSize().width/2;
		float y = textField->getPositionY() - textField->getContentSize().height/2;
		float width = textField->getContentSize().width;
		float height = textField->getContentSize().height;
		CCRect rect = CCRectMake(x, y, width, height);
		//判断触点是否触摸到编辑框内部
		if( rect.containsPoint(pos) ) {
			CCLOG("attachWithIME");
			textField->attachWithIME(); //开启虚拟键盘
			break;
		}else {
			CCLOG("detachWithIME");
			textField->detachWithIME(); //关闭虚拟键盘
		}
	}
}

// void NewUnitLayer::editBoxEditingDidBegin(CCEditBox* editBox){
// 	CCLog("editbox begin");
// 
// }
// 
// void NewUnitLayer::editBoxEditingDidEnd(CCEditBox* editBox){
// 	CCLog("editbox end");
// 
// }
// 
// void NewUnitLayer::editBoxTextChanged(CCEditBox* editBox, const std::string& text){
// 	CCLog("editbox changed");
// }
// 
// void NewUnitLayer::editBoxReturn(CCEditBox* returnEditBox){
// 	CCLog("editbox return");
// 	int i = returnEditBox->getTag();
// 	vector<string> newSingle;
// 	newSingle.push_back(string(returnEditBox->getText()));
// 	newSingle.push_back(DataTool::intTostring(0));
// 	newSingle.push_back(DataTool::intTostring(2));
// 	groupCharacter[i] = newSingle;
// }


//当用户启动虚拟键盘时的回调函数
//启用键盘false; 不启用键盘true
bool NewUnitLayer::onTextFieldAttachWithIME(CCTextFieldTTF* sender){
	sender->setFontSize(100);
	sender->setColorSpaceHolder(ccRED);
	sender->setColor(ccRED);
	return false;
}

//当用户关闭虚拟键盘时的回调函数
//关闭键盘false; 不关闭键盘true
bool NewUnitLayer::onTextFieldDetachWithIME(CCTextFieldTTF* sender){
	CCLog("editbox return");
	
	sender->setFontSize(80);
	sender->setColorSpaceHolder(ccBLACK);
	sender->setColor(ccBLACK);

	int i = sender->getTag();
	vector<string> newSingle;
	newSingle.push_back(sender->getString());
	newSingle.push_back(DataTool::intTostring(0));
	newSingle.push_back(DataTool::intTostring(0));
	groupCharacter[i] = newSingle;
	return false;
}

//当用户输入时的回调函数
//允许输入字符false; 不允许输入字符true
bool NewUnitLayer::onTextFieldInsertText(CCTextFieldTTF* sender, const char* text, int nLen){
	//if insert enter, treat as default to detach with ime
    CCLOG("%d",nLen);//当前输入的单个字符长度
   
    //空格和\n作为输入结束符
    if (*text==' '||'\n' == *text)
    {
        sender->detachWithIME(); //关闭输入 隐藏键盘
        return true;
    }

    //中文的nlen是3  数字和字母的是1
    //如果输入是中文  则接受输入的内容
    if (nLen==3)
    {
        return false;//true 则不接受输入的内容 但是可以继续输入
    }
   
    //判断是否数字或者字符,和下划线_
    //不接受数字和英文大小写字符的输入
    if((*text>='0'&& *text<='9')||((*text>='a'&&*text<='z')||((*text>='A')&&(*text<='Z')))||*text>='_')
    {
		MyToast::showToast(this, DataTool::getChinese("only_chinese"),TOAST_LONG);
		return true;
    }
    else
    {
        return false;
    }

	return false;
}

//当用户删除文字时的回调函数
//允许删除字符false; 不允许删除字符true
bool NewUnitLayer::onTextFieldDeleteBackward(CCTextFieldTTF* sender, const char* delText, int nLen){
	return false;
}
