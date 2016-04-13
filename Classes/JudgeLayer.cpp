#include "JudgeLayer.h"
#include "JudgeScene.h"
#include "tinyxml.h"
#include "tools/DataTool.h"

JudgeLayer::JudgeLayer()
{
}

JudgeLayer::~JudgeLayer()
{
}

bool JudgeLayer::init(){
	CCLog("JudgeLayer::init()");
	if (CCLayer::init())
	{

		return true;
	}
	return false;
}

void JudgeLayer::onEnter(){
	CCLayer::onEnter();

	BackgroundLayer* backgroundLayer = (BackgroundLayer*)this->getParent()->getChildByTag(kBgLayerTag);
	CCSprite* tianzige = backgroundLayer->tianzige_;
	CCSprite* title_bar = backgroundLayer->title_bar_;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCMenuItemImage* next = CCMenuItemImage::create("strangedesign/Writting_next_button.png",
		"strangedesign/Writting_next_button_down.png",
		this,
		menu_selector(JudgeLayer::menuNext));
	CCPoint nextBtnPosition = ccp(winSize.width - next->getContentSize().width/2-50,tianzige->getPositionY()+tianzige->getContentSize().height/2+20);
	next->setPosition(nextBtnPosition);

	CCMenuItemImage* previous = CCMenuItemImage::create("strangedesign/Writting_pre_button.png",
		"strangedesign/Writting_pre_button_down.png",
		this,
		menu_selector(JudgeLayer::menuPrevious));
	CCPoint previousBtnPosition = ccp(previous->getContentSize().width/2+50 , tianzige->getPositionY()+ tianzige->getContentSize().height/2 + 20);
	previous->setPosition(previousBtnPosition);

	menu_ = CCMenu::create(next, previous, NULL);

	addChild(menu_,10);
	menu_->setPosition(CCPointZero);

	JudgeScene* scene = (JudgeScene*)this->getParent();
	if (!scene->getIsJudge())
	{
		//自由书写 需增加Button 作为保存
		CCMenuItemImage* saveButton = CCMenuItemImage::create("strangedesign/Free_writting_save_button_up.png",
			"strangedesign/Free_writting_save_button_down.png",
			this,
			menu_selector(JudgeLayer::menuSave));
		saveButton->setPosition(ccp(winSize.width/5*4,saveButton->getContentSize().height+5));
		menu_->addChild(saveButton);

		//view button
		CCMenuItemImage* viewBtn = CCMenuItemImage::create("strangedesign/Free_writting_view_button_up.png",
			"strangedesign/Free_writting_view_button_down.png",
			this,
			menu_selector(JudgeLayer::menuView));
		viewBtn->setPosition(ccp(winSize.width-viewBtn->getContentSize().width/2, title_bar->getPositionY()));
		menu_->addChild(viewBtn);
	}
}

void JudgeLayer::menuNext(CCObject* pSender){
	CCLog("next");
	JudgeScene* scene = (JudgeScene*)CCDirector::sharedDirector()->getRunningScene();
	scene->next();
}

void JudgeLayer::menuPrevious(CCObject* pSender){
	//调用上一个
	JudgeScene* scene = (JudgeScene*)CCDirector::sharedDirector()->getRunningScene();
	scene->previous();
}

void JudgeLayer::menuSave(CCObject* pSender){
	JudgeScene* scene = (JudgeScene*)CCDirector::sharedDirector()->getRunningScene();

	//判断是否写完
	if (scene->gettouchLayer()->isTimeSave())
	{
		string unit_id = scene->getUnitID();
		string zi_id = scene->getZiID();
		string note = scene->getHLayer()->GetOriginPoints();			//获取未变形的笔迹

		// TODO 这里添加将书写笔画保存成 手写字 Row xml的格式，兼容后期工作。
		// scene->getHLayer-> GetRowXML()  etc.

		string curChar = scene->getCurChar();

		SQLiteData::insertNote(unit_id,zi_id,note);
		MyToast::showToast(this,DataTool::getChinese("insert_into_sqlite"),TOAST_LONG);

		//delete strokes
		JudgeScene* scene = (JudgeScene*)CCDirector::sharedDirector()->getRunningScene();
		HcharacterLayer* HLayer = scene->getHLayer();
		HLayer->ClearWriting();

		//update sqlite unit 更新数据库中每个汉字练习次数
		vector< vector <string> > groupUnit = SQLiteData::getUnit(unit_id);
		
		for (int i = 0; i < groupUnit.size(); i++)
		{
			if (curChar == groupUnit[i][0])
			{
				string count = DataTool::intTostring(DataTool::stringToInt(groupUnit[i][1])+1);
				groupUnit[i][1]=count;
				break;
			}
		}
		SQLiteData::updateUnit(unit_id,groupUnit);

	}else
	{
		MyToast::showToast(this,DataTool::getChinese("not_finish"),TOAST_LONG);
	}
}

void JudgeLayer::menuView(CCObject* pSender){

//	vector<vector <string> > ret = SQLiteData::getNote("1","32");
	JudgeScene* scene = (JudgeScene*)this->getParent();
	string unitid = scene->getUnitID();
	string ziid = scene->getZiID();
	string curChar = scene->getCurChar();

	CCDirector::sharedDirector()->pushScene(ViewScene::scene(unitid,ziid,curChar));
}

void JudgeLayer::SaveProToFile(float pro){
	//filename hanzi ceshiScene  profic 计算
	JudgeScene* scene = (JudgeScene*)CCDirector::sharedDirector()->getRunningScene();
// 	string wallXMLCurrent = scene->getWallFileName();
	string currhanzi = scene->getCurChar();

	string profic = DataTool::floatToString(pro);

}
