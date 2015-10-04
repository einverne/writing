#include "ViewScene.h"


ViewScene::ViewScene():pGridView(NULL)
{
}

ViewScene::~ViewScene()
{
}

CCScene* ViewScene::scene(string unit_id, string zi_id, string curChar){
	CCScene* scene = CCScene::create();
	ViewScene* layer = ViewScene::create(unit_id, zi_id, curChar);
	scene->addChild(layer);
	return scene;
}

ViewScene* ViewScene::create(string unit_id, string zi_id,string curChar){
	ViewScene* pret = new ViewScene();
	if (pret && pret->init(unit_id, zi_id, curChar))
	{
		pret->autorelease();
		return pret;
	}else
	{
		delete pret;
		pret = NULL;
		return pret;
	}
}

bool ViewScene::init(string unitid, string ziid, string curChar){
	CCLog("ViewScene init");
	if (!CCLayerColor::initWithColor(ccc4(255,255,255,255)))
	{
		return false;
	}

	this->unit_id = unitid;
	this->zi_id = ziid;
	this->currentChar = curChar;
 	Notes = SQLiteData::getNote(unit_id,zi_id);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSize visiableSize = CCDirector::sharedDirector()->getVisibleSize();

	CCSprite* titlebar = CCSprite::create("strangedesign/title bar_background.png");
	addChild(titlebar);
	titlebar->setPosition(ccp(visiableSize.width/2,visiableSize.height-titlebar->getContentSize().height/2));

	CWidgetWindow* m_pWindow = CWidgetWindow::create();
	m_pWindow->setMultiTouchEnabled(true);
	addChild(m_pWindow);

	CButton* backBtn = CButton::create("strangedesign/back_button.png", "strangedesign/back_button_down.png");
	backBtn->setOnClickListener(this,ccw_click_selector(ViewScene::back));
	backBtn->setPosition(ccp(backBtn->getContentSize().width/2+10, visiableSize.height -backBtn->getContentSize().height/2-50));
	m_pWindow->addChild(backBtn);

	CButton* deleteBtn = CButton::create("strangedesign/Free_writting_delete_button_up.png","strangedesign/Free_writting_delete_button_down.png");
	deleteBtn->setOnClickListener(this, ccw_click_selector(ViewScene::deleteBtnClick));
	deleteBtn->setPosition(ccp(winSize.width-deleteBtn->getContentSize().width/2, winSize.height-deleteBtn->getContentSize().height/2));
	m_pWindow->addChild(deleteBtn);

	CCSize visualSize = CCSizeMake(winSize.width,winSize.height-titlebar->getContentSize().height-10);
	CCSize gridcellSize = CCSizeMake(360 , 350);
	writingCount = Notes.size();
	pGridView = CGridView::create(visualSize,
		gridcellSize,
		writingCount,
		this,
		ccw_datasource_adapter_selector(ViewScene::gridViewDataSource));
	pGridView->setColumns(2);
	pGridView->setPosition(CCSize(winSize.width/2,(winSize.height-titlebar->getContentSize().height)/2));
	pGridView->setAutoRelocate(true);
	pGridView->reloadData();
	m_pWindow->addChild(pGridView);
	return true;
}

CCObject* ViewScene::gridViewDataSource(CCObject* pContentView, unsigned int idx){
	CGridViewCell* pCell = (CGridViewCell*) pContentView;
	CButton* pButton = NULL;
	CCheckBox* pCheckbox = NULL;
	Notes = SQLiteData::getNote(unit_id,zi_id);
	vector<string> oneNote = Notes.at(idx);			//oneNote 中第一个元素为ID，第二个元素为笔画序列

	if (!pCell)
	{
		pCell = new CGridViewCell();
		pCell->autorelease();

		pButton = CButton::create("strangedesign/Dlg_cancel_button.png","strangedesign/Dlg_cancel_button_down.png");
		pButton->setPosition(CCPoint(360/2, 350-pButton->getContentSize().height/2));

		pCheckbox = CCheckBox::create();
		pCheckbox->setNormalImage("ckb_normal_01.png");
		pCheckbox->setNormalPressImage("ckb_normal_02.png");
		pCheckbox->setCheckedImage("ckb_selected_01.png");
		pCheckbox->setCheckedPressImage("ckb_selected_02.png");
		pCheckbox->setDisabledNormalImage("ckb_disable_01.png");
		pCheckbox->setDisabledCheckedImage("ckb_disable_02.png");
		pCheckbox->setPosition(CCPoint(360-pCheckbox->getContentSize().width, 350-pButton->getContentSize().height/2));

		pCell->addChild(pCheckbox,10);
		//pCell->addChild(pButton,10);

		HcharacterDrawnode* handwritingHz = HcharacterDrawnode::create();
		
		vector< vector<CCPoint> > strokesvec = DataTool::spliteString(oneNote.at(1));
		for (unsigned int i = 0; i < strokesvec.size(); i++)
		{
			vector<CCPoint> oneStrokeVec = strokesvec[i];
			Stroke stroke(oneStrokeVec);
			handwritingHz->addStroke(stroke);
		}
		
		handwritingHz->setContentSize(CCSize(320,320));
		CCPoint position = ccp(360/2- handwritingHz->getContentSize().width/2, 350/2- handwritingHz->getContentSize().height/2);
		handwritingHz->setPosition(position);
		pCell->addChild(handwritingHz,1);



	}else
	{
		pButton = CButton::create("strangedesign/Dlg_cancel_button.png","strangedesign/Dlg_cancel_button_down.png");
		pButton->setPosition(CCPoint(360/2, 350-pButton->getContentSize().height/2));

		pCheckbox = CCheckBox::create();
		pCheckbox->setNormalImage("ckb_normal_01.png");
		pCheckbox->setNormalPressImage("ckb_normal_02.png");
		pCheckbox->setCheckedImage("ckb_selected_01.png");
		pCheckbox->setCheckedPressImage("ckb_selected_02.png");
		pCheckbox->setDisabledNormalImage("ckb_disable_01.png");
		pCheckbox->setDisabledCheckedImage("ckb_disable_02.png");
		pCheckbox->setPosition(CCPoint(360-pCheckbox->getContentSize().width, 350-pButton->getContentSize().height/2));

		pCell->addChild(pCheckbox,10);

		//pCell->addChild(pButton,10);

		HcharacterDrawnode* handwritingHz = HcharacterDrawnode::create();
		vector<string> oneNote = Notes.at(idx);			//oneNote 中第一个元素为ID，第二个元素为笔画序列
		vector< vector<CCPoint> > strokesvec = DataTool::spliteString(oneNote.at(1));
		for (unsigned int i = 0; i < strokesvec.size(); i++)
		{
			vector<CCPoint> oneStrokeVec = strokesvec[i];
			Stroke stroke(oneStrokeVec);
			handwritingHz->addStroke(stroke);
		}

 		handwritingHz->setContentSize(CCSize(320,320));
		CCPoint position = ccp(360/2- handwritingHz->getContentSize().width/2, 350/2- handwritingHz->getContentSize().height/2);
		handwritingHz->setPosition(position);
		pCell->addChild(handwritingHz,1);
	}

	char buff[64];
	sprintf(buff, "%u", idx);
	pButton->getLabel()->setString(buff);
	int userTag = DataTool::stringToInt(oneNote.at(0));
	pButton->setUserTag(userTag);
	pCheckbox->setUserTag(userTag);

	pButton->setOnClickListener(this,ccw_click_selector(ViewScene::buttonClick));
	pCheckbox->setOnClickListener(this,ccw_click_selector(ViewScene::checkBoxClick));

	return pCell;
}

void ViewScene::buttonClick(CCObject* pSender){
	CCLog("ViewScene::buttonClick");
	CButton* pBtn = (CButton*)pSender;
	selectDeleteNote = pBtn->getUserTag();

	//delete function
	CCSprite* backgroundImg = CCSprite::create("strangedesign/Dlg_background.png");
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	PopCancelLayer* dialog = PopCancelLayer::create("strangedesign/Dlg_background.png");
	dialog->setContentSize(backgroundImg->getContentSize());
	dialog->addButton("strangedesign/Dlg_delete_button.png","strangedesign/Dlg_delete_button_down.png","Y",0);
	dialog->setCallBackFunc(this,callfuncN_selector(ViewScene::dlgCallback));
	CCDirector::sharedDirector()->getRunningScene()->addChild(dialog,10,1001);


}

void ViewScene::back(CCObject* pSender){
	
	CCDirector::sharedDirector()->popScene();
}

void ViewScene::dlgCallback(CCNode* pNode){
	CCLog("callback");
	int tag = pNode->getTag();
	if (tag == 0)
	{
		// delete writing
		SQLiteData::deleteNote(DataTool::intTostring(selectDeleteNote));
		writingCount--;
		pGridView->setCountOfCell(writingCount);
		pGridView->reloadData();
	}
}

void ViewScene::deleteBtnClick(CCObject* pSender){
	CCLog("delete btn click");
	vector<int>::iterator iter= dltList.begin();
	for (iter ; iter != dltList.end(); iter++)
	{
		int tag = (int)*iter;
		SQLiteData::deleteNote(DataTool::intTostring(tag));
		writingCount--;
	}
	//update sqlite zinote Count
	vector< vector<string> > groupunit = SQLiteData::getUnit(unit_id);
	for (unsigned int i = 0; i < groupunit.size(); i++)
	{
		if (currentChar == groupunit[i][0])
		{
			string count = DataTool::intTostring(writingCount);
			groupunit[i][1]=count;
		}
	}
	SQLiteData::updateUnit(unit_id,groupunit);
	if (dltList.size()>0)
	{
		pGridView->setCountOfCell(writingCount);
		pGridView->reloadData();
		dltList.clear();
	}
}

void ViewScene::checkBoxClick(CCObject* pSender){
	CCheckBox* pCheckBox = (CCheckBox*) pSender;
	int userTag = pCheckBox->getUserTag();
	if (pCheckBox->isChecked())
	{
		dltList.push_back(userTag);
	}else{
		std::vector<int>::iterator pos = std::find(dltList.begin(),dltList.end(),userTag);
		if (pos != dltList.end())
		{
			dltList.erase(pos);
		}
	}
}
