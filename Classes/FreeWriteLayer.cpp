#include "FreeWriteLayer.h"
#include "JudgeScene.h"
#include "tinyxml.h"
#include "RelaxationMatch.h"
#include "tools/DataTool.h"

FreeWriteLayer::FreeWriteLayer()
{
}

FreeWriteLayer::~FreeWriteLayer()
{
}

bool FreeWriteLayer::init(){
	CCLog("JudgeLayer::init()");
	if (CCLayer::init())
	{

		return true;
	}
	return false;
}

void FreeWriteLayer::onEnter(){
	CCLayer::onEnter();

	BackgroundLayer* backgroundLayer = (BackgroundLayer*)this->getParent()->getChildByTag(kBgLayerTag);
	CCSprite* tianzige = backgroundLayer->tianzige_;
	CCSprite* title_bar = backgroundLayer->title_bar_;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCMenuItemImage* next = CCMenuItemImage::create("strangedesign/Writting_next_button.png",
		"strangedesign/Writting_next_button_down.png",
		this,
		menu_selector(FreeWriteLayer::menuNext));
	CCPoint nextBtnPosition = ccp(winSize.width - next->getContentSize().width/2-50,tianzige->getPositionY()+tianzige->getContentSize().height/2+20);
	next->setPosition(nextBtnPosition);

	CCMenuItemImage* previous = CCMenuItemImage::create("strangedesign/Writting_pre_button.png",
		"strangedesign/Writting_pre_button_down.png",
		this,
		menu_selector(FreeWriteLayer::menuPrevious));
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
			menu_selector(FreeWriteLayer::menuSave));
		saveButton->setPosition(ccp(winSize.width/5*4,saveButton->getContentSize().height+5));
		menu_->addChild(saveButton);

		//view button
		CCMenuItemImage* viewBtn = CCMenuItemImage::create("strangedesign/Free_writting_view_button_up.png",
			"strangedesign/Free_writting_view_button_down.png",
			this,
			menu_selector(FreeWriteLayer::menuView));
		viewBtn->setPosition(ccp(winSize.width-viewBtn->getContentSize().width/2, title_bar->getPositionY()));
		menu_->addChild(viewBtn);

		//match button 松弛匹配Btn
		// TODO 更换松弛匹配素材
		CCMenuItemImage* matchBtn = CCMenuItemImage::create("strangedesign/Free_writting_view_button_up.png",
			"strangedesign/Free_writting_view_button_down.png",
			this,
			menu_selector(FreeWriteLayer::menuMatch));
		matchBtn->setPosition(ccp(winSize.width-matchBtn->getContentSize().width/2*3, title_bar->getPositionY()));
		menu_->addChild(matchBtn);
	}
}

void FreeWriteLayer::menuNext(CCObject* pSender){
	CCLog("next");
	JudgeScene* scene = (JudgeScene*)CCDirector::sharedDirector()->getRunningScene();
	scene->next();
}

void FreeWriteLayer::menuPrevious(CCObject* pSender){
	//调用上一个
	JudgeScene* scene = (JudgeScene*)CCDirector::sharedDirector()->getRunningScene();
	scene->previous();
}

void FreeWriteLayer::menuSave(CCObject* pSender){
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
		
		for (unsigned int i = 0; i < groupUnit.size(); i++)
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

void FreeWriteLayer::menuView(CCObject* pSender){

//	vector<vector <string> > ret = SQLiteData::getNote("1","32");
	JudgeScene* scene = (JudgeScene*)this->getParent();
	string unitid = scene->getUnitID();
	string ziid = scene->getZiID();
	string curChar = scene->getCurChar();

	CCDirector::sharedDirector()->pushScene(ViewScene::scene(unitid,ziid,curChar));
}

void updateSegmentColor(){

}

void FreeWriteLayer::menuMatch(CCObject* pSender){
	// TODO 松弛匹配
	JudgeScene* scene = (JudgeScene*)this->getParent();
	HcharacterLayer* hlayer = (HcharacterLayer*)scene->getHLayer();
	ScriptCharacter scriptCharacter = hlayer->getm_HDrawnode()->GetScriptCharacter();
	CCSprite* tianzige = hlayer->getm_HDrawnode()->tianzige_;
	hlayer->getm_HDrawnode()->script_char_.Normalize(tianzige->getContentSize().height,
		tianzige->getContentSize().width);			// 标准化 512
	hlayer->getm_HDrawnode()->script_char_.divideSegment2();	// 切分笔段
	hlayer->getm_HDrawnode()->script_char_.new_IdentifynoiseSegment();

	hlayer->getm_HDrawnode()->GetScriptCharacterSegmentDrawnodeReady();

	TcharacterLayer* tlayer = (TcharacterLayer*)scene->getTLayer();
	TemplateCharacter templateChar = tlayer->getm_TDrawnode()->template_character_;


	scriptCharacter.Normalize(512,512);
	templateChar.Normalize(512,512);

//  	hlayer->getm_HDrawnode()->script_char_.TransferCoordinateToFour();
//  	tlayer->getm_TDrawnode()->template_character_.TransferCoordinateToFour();

	if (scriptCharacter.normal_size_ == templateChar.normal_size_)
	{
		CCLog("normal_size same");
		// 
		hlayer->getm_HDrawnode()->script_char_.clearSegmentColor(ccc4f(255,0,0,1));
		tlayer->getm_TDrawnode()->template_character_.clearSegmentColor(ccc4f(255,0,0,1));
		
		hlayer->getm_HDrawnode()->GetScriptCharacterSegmentDrawnodeReady();
		tlayer->getm_TDrawnode()->GetTemplateCharacterSegmentDrawnodeReady();

		RelaxationMatch match;
		bool yes = match.Inputcharacters(hlayer->getm_HDrawnode()->script_char_,
			tlayer->getm_TDrawnode()->template_character_);
		int m_iterationtimes;
		if (yes)
		{
			match.Initmatchprobability();
			match.Initmatchcompatibility();
			m_iterationtimes=0;

			for (int i = 0; i < 15; i++)
			{
				match.IterateOnce();
				m_iterationtimes++;
			}

			match.RelaxationExtraction();
			match.processing_tiny_segments();

			hlayer->getm_HDrawnode()->script_char_ = match.script;
			tlayer->getm_TDrawnode()->template_character_ = match.model;
			hlayer->getm_HDrawnode()->GetScriptCharacterSegmentDrawnodeReady();
			tlayer->getm_TDrawnode()->GetTemplateCharacterSegmentDrawnodeReady();

			
		}

		updateSegmentColor();
		list<ccColor4F> colorlist;
		colorlist.push_back(ccc4f(0,0,255,1));
		colorlist.push_back(ccc4f(0,255,0,1));
		colorlist.push_back(ccc4f(128,128,128,1));
		colorlist.push_back(ccc4f(0,0,128,1));
		colorlist.push_back(ccc4f(255,128,255,1));
		colorlist.push_back(ccc4f(128,128,255,1));
		colorlist.push_back(ccc4f(0,0,225,1));
		colorlist.push_back(ccc4f(128,128,0,1));
		colorlist.push_back(ccc4f(255,128,0,1));
		colorlist.push_back(ccc4f(255,255,0,1));
		colorlist.push_back(ccc4f(255,255,128,1));
		colorlist.push_back(ccc4f(255,0,255,1));

		// 一对一匹配
		list<candidate>::iterator itmatch = match.matchresult.begin();
		list<ccColor4F>::iterator itcolor = colorlist.begin();
		while(itmatch != match.matchresult.end())
		{
			if(itcolor==colorlist.end())
				itcolor=colorlist.begin();

			//////////////
			candidate temp=*itmatch;
			hlayer->getm_HDrawnode()->script_char_.SetSegmentColor(temp.h,*itcolor);
			tlayer->getm_TDrawnode()->template_character_.SetSegmentColor(temp.k,*itcolor);
			//////////////
			itmatch++;
			itcolor++;
		}

		// 一对多、或多对一匹配
		list<multicandidate>::iterator itmulti = match.multimatchresult.begin();
		while(itmulti != match.multimatchresult.end())
		{
			if(itcolor==colorlist.end())
				itcolor=colorlist.begin();

			//////////////
			multicandidate temp=*itmulti;
			list<int>::iterator ith=temp.hlist.begin();
			list<int>::iterator itk=temp.klist.begin();

			for(;ith!=temp.hlist.end();ith++)
				hlayer->getm_HDrawnode()->script_char_.SetSegmentColor(*ith,*itcolor);
			for(;itk!=temp.klist.end();itk++)
				tlayer->getm_TDrawnode()->template_character_.SetSegmentColor(*itk,*itcolor);
			//////////////
			itmulti++;
			itcolor++;
		}
		hlayer->getm_HDrawnode()->GetScriptCharacterSegmentDrawnodeReady();
		tlayer->getm_TDrawnode()->GetTemplateCharacterSegmentDrawnodeReady();

	}
	

}

void FreeWriteLayer::SaveProToFile(float pro){
	//filename hanzi ceshiScene  profic 计算
	JudgeScene* scene = (JudgeScene*)CCDirector::sharedDirector()->getRunningScene();
// 	string wallXMLCurrent = scene->getWallFileName();
	string currhanzi = scene->getCurChar();

	string profic = DataTool::floatToString(pro);

}
