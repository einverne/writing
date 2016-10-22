#include "HcharacterLayer.h"
#include "StrokeDrawnode.h"
#include "Stroke.h"
#include "MoveToRightPlace.h"
#include "TcharacterLayer.h"
#include "MoveToRightPlaceInterval.h"
#include "SimpleAudioEngine.h"
#include "MyToast.h"
#include "tools/DataTool.h"
#include "constants.h"
#include "JudgeScene.h"
#include "Error.h"
#include "ActionA0001.h"
#include "ActionA0003.h"
#include "DotLineNode.h"
#include <iomanip>

#include "../rapidjson/document.h"

using namespace rapidjson;

HcharacterLayer::HcharacterLayer():m_sprite_draw(NULL),
	bihuaCountAndTotal(NULL),
	m_HDrawnode(NULL),
	m_sprite_info(NULL),
	writeCount_(0),
	wrongCount_(0),
	ijudge_(false),
	totalBihuaCount(0),
	scoreLabel(NULL),
	score_(0.0),
	curBihuaWrong(0)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
#define RIGHT_EFFECT_FILE   "right_android.ogg"
#define WRONG_EFFECT_FILE	"wrong_android.ogg"
#elif( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define RIGHT_EFFECT_FILE   "right.wav"
#define WRONG_EFFECT_FILE	"wrong.wav"
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define RIGHT_EFFECT_FILE   "right_android.ogg"
#define WRONG_EFFECT_FILE	"wrong_android.ogg"
#endif
	scale_=1.6f;
}

#define ACTION_TAG 2016

HcharacterLayer::~HcharacterLayer()
{
	CCLog("~HcharacterLayer %d",  this->m_uReference);
	CC_SAFE_RELEASE(m_sprite_draw);
	CC_SAFE_RELEASE(bihuaCountAndTotal);
	CC_SAFE_RELEASE(m_HDrawnode);
	CC_SAFE_RELEASE(m_sprite_info);
	CC_SAFE_RELEASE(scoreLabel);
}

bool HcharacterLayer::init(string hanzi,CCSprite* tianzige_draw){
	if (CCLayer::init())
	{
		this->hanzi_ = hanzi;
		this->setSprite(tianzige_draw);
		this->setm_HDrawnode(HcharacterDrawnode::create());
		getm_HDrawnode()->setPosition(m_sprite_draw->getPosition());
		getm_HDrawnode()->setScale(scale_);
		getm_HDrawnode()->setAnchorPoint(ccp(0.5,0.5));
		addChild(m_HDrawnode);

		_manager.initLuaEngine();
		return true;
	}
	return false;
}

HcharacterLayer* HcharacterLayer::create(string hanzi,CCSprite* tianzige_draw){
	HcharacterLayer* pret = new HcharacterLayer();
	if (pret && pret->init(hanzi,tianzige_draw))
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

void HcharacterLayer::onEnter(){
	CCLayer::onEnter();

	TcharacterLayer* layer = (TcharacterLayer*)this->getParent()->getChildByTag(kTLayerTag);		//get TcharacterLayer
	totalBihuaCount = layer->getm_TDrawnode()->getCharacter().getStrokeCount();

	CCSize visiableSize = CCDirector::sharedDirector()->getVisibleSize();

	JudgeScene* scene = (JudgeScene*)this->getParent();
	BackgroundLayer* backgroundLayer = scene->getbackgroundLayer();
	CCSprite* tianzige = backgroundLayer->tianzige;

	setInfoSprite(CCSprite::create("right.png"));
	addChild(getInfoSprite(),2000);
	getInfoSprite()->setPosition(ccp(visiableSize.width/4*3+50,tianzige->getPositionY()-50));
	getInfoSprite()->setVisible(false);

	CCPoint tianzige_draw_position = getSprite()->getPosition();
	CCSize tianzige_draw_size = getSprite()->getContentSize();

	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(RIGHT_EFFECT_FILE);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(WRONG_EFFECT_FILE);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);

	//放大缩小按钮
	CButton* zoomin = CButton::create("strangedesign/Judge_writting_zoomin_button.png",
		"strangedesign/Judge_writting_zoomin_button_down.png");
	zoomin->setPosition(ccp(visiableSize.width/4,zoomin->getContentSize().height+5));
	zoomin->setOnClickListener(this,ccw_click_selector(HcharacterLayer::zoomin));

	CButton* zoomout = CButton::create("strangedesign/Judge_writting_zoomout_button.png",
		"strangedesign/Judge_writting_zoomout_button_down.png");
	zoomout->setPosition(ccp(visiableSize.width/4*2,zoomout->getContentSize().height+5));
	zoomout->setOnClickListener(this,ccw_click_selector(HcharacterLayer::zoomout));

	CButton* rewriteBtn = CButton::create("strangedesign/Judge_writting_cancel_button.png",
		"strangedesign/Judge_writting_cancel_button_down.png");
	rewriteBtn->setPosition(ccp(visiableSize.width/4*3,rewriteBtn->getContentSize().height+5));
	rewriteBtn->setOnClickListener(this,ccw_click_selector(HcharacterLayer::rewrite));

	if (scene->getIsJudge() == false)
	{
		zoomin->setPosition(ccp(visiableSize.width/5,zoomin->getContentSize().height+5));
		zoomout->setPosition(ccp(visiableSize.width/5*2,zoomout->getContentSize().height+5));
		rewriteBtn->setPosition(ccp(visiableSize.width/5*3,rewriteBtn->getContentSize().height+5));

	}else{
		string countAndTotal = DataTool::getChinese("bihuashu")+"0/"+ DataTool::intTostring(totalBihuaCount);
		setbihuaCountAndTotal(CCLabelTTF::create(countAndTotal.c_str(),"Arial",40));
		getbihuaCountAndTotal()->setColor(ccc3(0,0,0));
		addChild(getbihuaCountAndTotal(),10);
		CCPoint bihuaCountPosition = ccp(visiableSize.width/4-70, tianzige->getPositionY()-50);
		getbihuaCountAndTotal()->setPosition(bihuaCountPosition);

		JudgeScene* scene = (JudgeScene*)this->getParent();
		string unit_id = scene->getUnitID();
		string curChar = scene->getCurChar();
		vector< vector<string> > groupUnit = SQLiteData::getUnit(unit_id);
		string scorestr;
		for (unsigned int i = 0; i < groupUnit.size(); i++)
		{
			if (curChar == groupUnit[i][0])
			{
				scorestr = groupUnit[i][2];
				break;
			}
		}
		float scoretemp = DataTool::stringToFloat(scorestr);
		string scorestring = DataTool::getChinese("defen")+DataTool::to_string_with_precision(scoretemp);
		setscoreLabel(CCLabelTTF::create(scorestring.c_str(),"Arial",40));
		getscoreLabel()->setColor(ccBLACK);
		addChild(getscoreLabel(),10);
		CCPoint scoreLabelPos = ccp(visiableSize.width/4-70, tianzige->getPositionY()+50);
		getscoreLabel()->setPosition(scoreLabelPos);
	}

	CWidgetWindow* m_pWindow = CWidgetWindow::create();
	m_pWindow->setMultiTouchEnabled(true);
	addChild(m_pWindow,10);

	m_pWindow->addChild(zoomin);
	m_pWindow->addChild(zoomout);
	m_pWindow->addChild(rewriteBtn);
}

void HcharacterLayer::onExit(){
	CCLayer::onEnter();
	_manager.exitLuaEngine();
}

/**
* 根据 doc 返回 errorType
* @param doc
* @return
*/
string getErrorType(rapidjson::Document& doc) {
	string errorType = "";
	if (doc.HasMember("error"))
	{
		const Value& errorjson = doc["error"];
		if (errorjson.Size()>0)
		{
			SizeType index = 0;
			if (errorjson[index].HasMember("errortype"))
			{
				errorType = errorjson[index]["errortype"].GetString();
			}
		}
	}
	return errorType;
}


/**
* 从json返回中，获取错误笔画
* @param doc json doc
* @return 返回错误类型的笔画
*/
multimap<int, float> getErrorStroke(rapidjson::Document& doc) {
	multimap<int, float> errorPoints;
	if (doc.HasMember("error"))
	{
		const Value& errorjson = doc["error"];
		if (errorjson.Size() > 0)
		{
			SizeType index = 0;
			if (errorjson.Size()>0)
			{
				if(errorjson[index].HasMember("errorstroke")){
					const Value& v = errorjson[index]["errorstroke"];
					for (Value::ConstMemberIterator iter = v.MemberonBegin(); iter != v.MemberonEnd(); ++iter)
					{
						string key = iter->name.GetString();
						string value = iter->value.GetString();
						int n_key = DataTool::stringToInt(key);

						// 判断 value string 中是否存在 /
						if (DataTool::isExist(value, "/"))
						{
							vector<string> points = DataTool::spliteStringBy(value, "/");	
							for (int i = 0; i < points.size(); i++)
							{
								float f_value = DataTool::stringToFloat(points[i]);
								errorPoints.insert(make_pair(n_key, f_value));
							
							}
						}else{
							float f_value = DataTool::stringToFloat(value);
							errorPoints.insert(make_pair(n_key, f_value));
						}

						//oneError.errorstroke.insert(make_pair(n_key,f_value));
					}
				}

			}
		}
	}
	return errorPoints;
}

multimap<int, float> getRightPoints(rapidjson::Document& doc) {
	multimap<int, float> errorPoints;
	if (doc.HasMember("error"))
	{
		const Value& errorjson = doc["error"];
		if (errorjson.Size() > 0)
		{
			SizeType index = 0;
			if (errorjson.Size()>0)
			{
				if(errorjson[index].HasMember("rightposition")){
					const Value& v = errorjson[index]["rightposition"];
					for (Value::ConstMemberIterator iter = v.MemberonBegin(); iter != v.MemberonEnd(); ++iter)
					{
						string key = iter->name.GetString();
						string value = iter->value.GetString();
						int n_key = DataTool::stringToInt(key);

						// 判断 value string 中是否存在 /
						if (DataTool::isExist(value, "/"))
						{
							vector<string> points = DataTool::spliteStringBy(value, "/");	
							for (int i = 0; i < points.size(); i++)
							{
								float f_value = DataTool::stringToFloat(points[i]);
								errorPoints.insert(make_pair(n_key, f_value));

							}
						}else{
							float f_value = DataTool::stringToFloat(value);
							errorPoints.insert(make_pair(n_key, f_value));
						}

						//oneError.errorstroke.insert(make_pair(n_key,f_value));
					}
				}

			}
		}
	}
	return errorPoints;
}

void HcharacterLayer::doA0001(multimap<int, float>& points,
							  multimap<int, float>& rightpoints) {
	if (points.size() < 2)
	{
		return;
	}
	string errortype = "A0001";
	MyToast::showToast(this, DataTool::getChinese(errortype), TOAST_LONG);

	vector<CCPoint> error_points = getm_HDrawnode()->GetErrorPoints(points);

	if (error_points.size() >= 2)
	{
		CCPoint start_point = error_points.at(0);
		CCPoint end_point = error_points.at(1);

		ShuipingErrorNode* errorNode = ShuipingErrorNode::create();
		errorNode->setPoint(start_point, end_point);
		CCSize content_size = m_sprite_draw->getContentSize();
		errorNode->setPosition(m_sprite_draw->getPosition()- 
			ccp(content_size.width*scale_/2, content_size.height*scale_/2));
		// 					errorNode->setPosition(m_sprite_draw->getPosition());
		errorNode->setScale(scale_);
		errorNode->setAnchorPoint(ccp(0, 0));
		addChild(errorNode, 1, ACTION_TAG);

		CCBlink* blink = CCBlink::create(2,4);
		errorNode->runAction(blink);
		ActionA0001* actionA0001 = ActionA0001::create(2.0);
		errorNode->runAction(actionA0001);
	}
}

void HcharacterLayer::doA0002(multimap<int, float>& points, multimap<int, float>& rightpoints){
	if (points.size() <= 0)
	{
		return;
	}
	string errorType = "A0002";
	MyToast::showToast(this, DataTool::getChinese(errorType), TOAST_LONG);
	vector<CCPoint> errorPoints = getm_HDrawnode()->GetErrorPoints(points);
	multimap<int, float>::iterator pointsBeginIterator = points.begin();
	multimap<int, float> mid;
	mid.insert(make_pair(pointsBeginIterator->first, 0.5));
	CCPoint midPoint = getm_HDrawnode()->GetErrorPoints(mid).at(0);

	if (errorPoints.size() >= 1)
	{
		CCPoint errorPoint = errorPoints.at(0);
		OnePointNode* errorNode = OnePointNode::create();
		errorNode->setPoint(errorPoint);
		errorNode->setRightPoint(midPoint);
		CCSize content_size = m_sprite_draw->getContentSize();
		errorNode->setPosition(m_sprite_draw->getPosition()-
			ccp(content_size.width*scale_/2, content_size.height*scale_/2));
		errorNode->setScale(scale_);
		errorNode->setAnchorPoint(ccp(0,0));
		addChild(errorNode, 1, ACTION_TAG);
		CCBlink* blink = CCBlink::create(2,4);
		errorNode->runAction(blink);
		ActionA0003* action = ActionA0003::create(2.0);
		errorNode->runAction(action);
	}

}

void HcharacterLayer::doA0003(multimap<int, float>& points, multimap<int, float>& rightpoints){
	string errorType = "A0003";
	MyToast::showToast(this, DataTool::getChinese(errorType), TOAST_LONG);
	vector<CCPoint> errorPoints = getm_HDrawnode()->GetErrorPoints(points);

	multimap<int, float>::iterator pointsBeginIterator = points.begin();
	multimap<int, float> mid;
	mid.insert(make_pair(pointsBeginIterator->first, 0.5));
	CCPoint midPoint = getm_HDrawnode()->GetErrorPoints(mid).at(0);


	if (errorPoints.size() >= 1)
	{
		CCPoint centerPoint = errorPoints.at(0);
		OnePointNode* errorNode = OnePointNode::create();
		errorNode->setPoint(centerPoint);
		errorNode->setRightPoint(midPoint);

		CCSize content_size = m_sprite_draw->getContentSize();
		errorNode->setPosition(m_sprite_draw->getPosition()-
			ccp(content_size.width*scale_/2, content_size.height*scale_/2));
		errorNode->setScale(scale_);
		errorNode->setAnchorPoint(ccp(0,0));
		addChild(errorNode, 1, ACTION_TAG);
		CCBlink* blink = CCBlink::create(2,4);
		errorNode->runAction(blink);
		ActionA0003* actionA0003 = ActionA0003::create(2.0);
		errorNode->runAction(actionA0003);
	}
}

void HcharacterLayer::doA0004(multimap<int, float>& points, multimap<int, float>& rightpoints){
	string errorType = "A0004";
	MyToast::showToast(this, DataTool::getChinese(errorType), TOAST_LONG);
	vector<CCPoint> errorPoints = getm_HDrawnode()->GetErrorPoints(points);
	int markStroke;
	for (multimap<int, float>::iterator it = points.begin();
		it != points.end(); it++)
	{
		markStroke = it->first;
	}
	
	if (errorPoints.size() >= 1)
	{
		// 给错误笔画着色
		getm_HDrawnode()->markErrorStroke(markStroke);
	}
	// 画虚线
	vector<CCPoint> rightPoints = getm_HDrawnode()->GetErrorPoints(rightpoints);
	if (!rightPoints.empty())
	{
		DotLineNode* dotlineNode = DotLineNode::create();
		dotlineNode->setCenterPoint(rightPoints[0]);
		dotlineNode->setLength(100);
		CCSize content_size = m_sprite_draw->getContentSize();
		dotlineNode->setPosition(m_sprite_draw->getPosition()-
			ccp(content_size.width*scale_/2, content_size.height*scale_/2));
		dotlineNode->setScale(scale_);
		dotlineNode->setAnchorPoint(ccp(0,0));
		addChild(dotlineNode, 1, ACTION_TAG);

		CCBlink* blink = CCBlink::create(2,4);
		CCSequence* sequence = CCSequence::create(blink, CCRemoveSelf::create(true), NULL);
		dotlineNode->runAction(sequence);
	}
}

void HcharacterLayer::doA0005(multimap<int, float>& points, multimap<int, float>& rightpoints){
	string errotType = "A0005";
	MyToast::showToast(this, DataTool::getChinese(errotType), TOAST_LONG);
	vector<CCPoint> errorPoints = getm_HDrawnode()->GetErrorPoints(points);
	if (errorPoints.size() >= 3)
	{
		for (int i = 0; i < errorPoints.size(); i++)
		{
			DotLineNode* dotlineNode1 = DotLineNode::create();
			dotlineNode1->setCenterPoint(errorPoints.at(i));
			dotlineNode1->setLength(100);
			dotlineNode1->setOrientation(1);
			CCSize size = m_sprite_draw->getContentSize();
			dotlineNode1->setPosition(m_sprite_draw->getPosition()-
				ccp(size.width*scale_/2, size.height*scale_/2));
			dotlineNode1->setScale(scale_);
			dotlineNode1->setAnchorPoint(ccp(0,0));
			addChild(dotlineNode1, 1, ACTION_TAG);

			CCBlink* blink = CCBlink::create(2,4);
			CCSequence* sequence = CCSequence::create(blink, CCRemoveSelf::create(true), NULL);
			dotlineNode1->runAction(sequence);
		}
	}
}

void HcharacterLayer::ParseResult(const string ret) {
	rapidjson::Document doc;
	doc.Parse<kParseDefaultFlags>(ret.c_str());
	vector<Error> errors;

	// 根据 ret 的结果，笔画，部件，整字进行不同的判定
	if(doc.HasMember("ret")){
		string retjson = doc["ret"].GetString();
		string errorType = getErrorType(doc);
		if (retjson.length() == 3)
		{
			if (retjson.at(0) == '1')
			{
				// whether a stroke is right
				writeBihuaRight();
			}else{
				writeBihuaWrong();
				// 写错时的 Toast 根据返回值判断
				MyToast::showToast(this,DataTool::getChinese(errorType),TOAST_LONG);


			}
			if (retjson.at(1) == '1')
			{
				// whether a radical is right
				//MyToast::showToast(this,"radical right",TOAST_LONG);
			}else
			{
				// 当部件错误的时候，显示动画。
				
				multimap<int, float> points = getErrorStroke(doc);
				multimap<int, float> rightpoints = getRightPoints(doc);
				// 水平平齐错误
				if (errorType == "A0001")
				{
					doA0001(points, rightpoints);
				}
				if (errorType == "A0002")
				{
					doA0002(points, rightpoints);
				}
				if (errorType == "A0003")
				{
					doA0003(points, rightpoints);
				}
				if (errorType == "A0004")
				{
					doA0004(points, rightpoints);
				}
				if (errorType == "A0005")
				{
					doA0005(points, rightpoints);
				}
				if (errorType == "A0006")
				{
				}
				if (errorType == "A0007")
				{
				}
				if (errorType == "A0008")
				{
				}
				if (errorType == "A0009")
				{
				}
				if (errorType == "A0010")
				{
				}
			}
			if (retjson.at(2) == '1')
			{
				// 整字是否正确
				//MyToast::showToast(this, "Character right", TOAST_LONG);
			} else {
				MyToast::showToast(this, "Character error", TOAST_LONG);
			}
		} else {
			// 如果 ret 返回长度不等于3 报错
			CCLog("ret length is not 3");
			MyToast::showToast(this, "ret error ret length is not 3", TOAST_LONG);
		}
	}

	// 判断错误类型，根据不同错误类型进行不同操作



	if (doc.HasMember("error"))
	{
		Error oneError;
		const Value& errorjson = doc["error"];
		for (SizeType i = 0 ; i < errorjson.Size(); ++i)
		{
			if(errorjson[i].HasMember("errortype")){
				string errortype = errorjson[i]["errortype"].GetString();
				oneError.errortype = errortype;
			}
			if(errorjson[i].HasMember("errorstroke")){
				const Value& v = errorjson[i]["errorstroke"];
				for (Value::ConstMemberIterator iter = v.MemberonBegin(); iter != v.MemberonEnd(); ++iter)
				{
					string key = iter->name.GetString();
					string value = iter->value.GetString();
					int n_key = DataTool::stringToInt(key);
					float f_value = DataTool::stringToFloat(value);
					oneError.errorstroke.insert(make_pair(n_key,f_value));
				}
			}
			errors.push_back(oneError);
		}
	}

}

void HcharacterLayer::judge(){
	writeCount_ ++;
	CCArray* strokes = m_HDrawnode->getStrokeDrawnodeList();
	string output = "";					// 手写汉字的点集信息

	CCObject* originob;
	CCARRAY_FOREACH(strokes,originob){
		StrokeDrawnode* node = (StrokeDrawnode*)originob;
		Stroke stro = node->getStroke();
		this->pointsOrigin += stro.sendOutput();
	}

	CCObject* ob;
	CCARRAY_FOREACH(strokes,ob){
		StrokeDrawnode* node = (StrokeDrawnode*)ob;
		Stroke stro = node->getStroke();
		vector<CCPoint> points = stro.getpointList();
		stro.convert512(this->m_sprite_draw->getContentSize());
		stro.resample(100);
		output += stro.sendOutput();
	}
	this->pointsOutput=output;

// 	CharacterEntity* p =  ((LianxiScene*)this->getParent())->getCharacterP();
// 	CharacterExtend* p = ((LianxiScene*)this->getParent())->getCharacterExt();
// 	string funcs = ((LianxiScene*)this->getParent())->funcs;
//	string ret = JudgeManager::getResult(hanzi,output,p,funcs);
// 	string funcs = ((LianxiScene*)this->getParent())->funcs;
	string funcs = "";
	TcharacterLayer* tlayer = (TcharacterLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(kTLayerTag);
	string points = tlayer->getm_TDrawnode()->getCharacterStandardInfo();		//获取正字信息
    CCLog("output %s",output.c_str());
    CCLog("right Character info %s",points.c_str());
    
	string ret = _manager.getResult(hanzi_,output,points,m_exChar,funcs);
	if (hanzi_ == DataTool::getChinese("ren") && totalBihuaCount == writeCount_)
	{
		// 人 水平平齐 调试 最后一笔判断
		ret = "{\"error\":[{\"errorstroke\":{\"0\":\"1\",\"1\":\"1\"},\"errortype\":\"A0001\",\"rightposition\":{}}],\"ret\":\"101\"}";
	}

	if (hanzi_ == DataTool::getChinese("da") && totalBihuaCount == writeCount_)
	{
		// 大 中点切分调试 最后一笔
		ret = "{\"error\":[{\"errorstroke\":{\"1\":\"1\",\"2\":\"1\"},\"errortype\":\"A0001\"}],\"ret\":\"101\"}";
	}

	if (hanzi_ == DataTool::getChinese("xia") && totalBihuaCount == writeCount_)
	{
		// 下 字 A0002 调试
		ret = "{\"error\":[{\"errorstroke\":{\"0\":\"0.11\"},\"errortype\":\"A0002\",\"rightposition\":{\"0\":\"0.5\"}}],\"ret\":\"101\"}";
	}

	if (hanzi_ == DataTool::getChinese("shi") && totalBihuaCount == writeCount_)
	{
		// 十 A0002 调试
		ret = "";
	}

	if (hanzi_ == DataTool::getChinese("wen") && totalBihuaCount == writeCount_)
	{
		ret = "{\"error\":[{\"errorstroke\":{\"0\":\"0.2\"},\"errortype\":\"A0004\",\"rightposition\":{\"1\":\"0.5\"}}],\"ret\":\"101\"}";
	} else if (hanzi_ == DataTool::getChinese("wen"))
	{
		ret = "";
	}
	if (hanzi_ == DataTool::getChinese("san") && totalBihuaCount == writeCount_)
	{
		ret = "{\"ret\":\"101\",\"error\":[{\"errortype\":\"A0005\",\"errorstroke\":{\"0\":\"0.5\",\"1\":\"0.5\",\"2\":\"0.5\"},\"rightposition\":{}}]}";
	} else if (hanzi_ == DataTool::getChinese("san"))
	{
		ret = "";
	}

	//CCLog("Hcharacterlay: retstring:%s length:%d",ret.c_str(),ret.length());
	//如果不评判则跳过
	if (!ijudge_ || ret.length() <= 0)
	{
		return;
	}
	CCLog("%s\n", ret.c_str());

	//ret = "{\"ret\":\"111\",\"error\": [] }";
	//ret = "{\"error\":[{\"errortype\":\"B0001\",\"errorstroke\":{}}],\"ret\":\"011\"}";
	//ret = "{\"ret\":\"101\",\"error\":[{\"errortype\":\"A0001\",\"errorstroke\":{\"0\":\"0.2\",\"1\":\"0.3\"}},{\"errortype\":\"A0021\",\"errorstroke\":{\"0\":\"0.2\",\"1\":\"0.3\"}}]}";

	ParseResult(ret);

	//if (ret.length() == 3)
	//{
	//	if (ret.at(0) == '0')
	//	{
	//		//这一笔写错
	//		MyToast::showToast(this,DataTool::getChinese("stroke_wrong"),TOAST_LONG);
	//		
	//		Wrong();
	//		
	//	}else if(ret.at(0) == '1'){
	//		//写对
	//		//MyToast::showToast(this,DataTool::getChinese("stroke_right"),TOAST_LONG);

	//		Right();
	//	}
	//}else if(ret.length() == 4){
	//	if (ret.at(0) == '0')
	//	{
	//		//这一笔写错 或者 位置不对
	//		MyToast::showToast(this,DataTool::getChinese("stroke_wrong"),TOAST_LONG);
	//		Wrong();
	//		
	//	}else if(ret.at(0) == '1' && ret.at(1) == '0'){
	//		MyToast::showToast(this,DataTool::getChinese("position_wrong"),TOAST_LONG);
	//		Wrong();
	//	}else if(ret.at(1) == '1' && ret.at(0) == '1')
	//	{
	//		//MyToast::showToast(this,DataTool::getChinese("position_right"),TOAST_LONG);
	//		Right();
	//	}
	//}
}

void HcharacterLayer::writeBihuaWrong(){

	this->getm_HDrawnode()->removeLastStroke();
	int t = getm_HDrawnode()->getStrokeDrawnodeList()->count();
	string strToshow = DataTool::getChinese("bihuashu")+DataTool::intTostring(t)+"/"+DataTool::intTostring(totalBihuaCount);
	bihuaCountAndTotal->setString(strToshow.c_str());
	//getInfoSprite()->setVisible(true);
	//getInfoSprite()->setTexture(CCTextureCache::sharedTextureCache()->addImage("wrong.png"));
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(WRONG_EFFECT_FILE);
	wrongCount_++;
	curBihuaWrong++;

	string strLabel = DataTool::getChinese("defen")+DataTool::to_string_with_precision(score_);
	getscoreLabel()->setString(strLabel.c_str());

}

void HcharacterLayer::writeBihuaRight(){
	// set stroke count Label
	int t = getm_HDrawnode()->getStrokeDrawnodeList()->count();
	string strToshow = DataTool::getChinese("bihuashu")+DataTool::intTostring(t)+"/"+DataTool::intTostring(totalBihuaCount);
	getbihuaCountAndTotal()->setString(strToshow.c_str());

	// Actions
	TcharacterLayer* layer = (TcharacterLayer*)this->getParent()->getChildByTag(kTLayerTag);		//get TcharacterLayer
	//Stroke temp = layer->getm_TDrawnode()->getCharacter().getStroke(t);								//get No. stroke
	//MoveToRightPlaceInterval* place = MoveToRightPlaceInterval::create(1,t-1,temp);
	//m_HDrawnode->runAction(place);

	// 书写笔画数大于等于正确汉字笔画，设置正确图标，并有声效
	if ( t >= layer->getm_TDrawnode()->getCharacter().getStrokeCount())
	{
		getInfoSprite()->setVisible(true);
		getInfoSprite()->setTexture(CCTextureCache::sharedTextureCache()->addImage("right.png"));
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(RIGHT_EFFECT_FILE);
	}

	// calculate score 计算得分
	switch (curBihuaWrong)
	{
	case 0:
		score_+=10.0/totalBihuaCount;
		break;
	case 1:
		score_+=0.75*10/totalBihuaCount;
		break;
	case 2:
		score_+=0.5*10/totalBihuaCount;
		break;
	case 3:
		score_+=0.25*10/totalBihuaCount;
		break;
	default:
		score_+=0*10/totalBihuaCount;
		break;
	}
	string strLabel = DataTool::getChinese("defen")+DataTool::to_string_with_precision(score_);
	getscoreLabel()->setString(strLabel.c_str());
	curBihuaWrong=0;

	// update Character score into db
	JudgeScene* scene = (JudgeScene*)this->getParent();
	string unit_id = scene->getUnitID();
	string curChar = scene->getCurChar();

	vector<vector<string> > groupUnit = SQLiteData::getUnit(unit_id);
	for (unsigned int i = 0; i < groupUnit.size(); i++)
	{
		if (curChar == groupUnit[i][0])
		{
			string scorestr = DataTool::to_string(score_);
			groupUnit[i][2]=scorestr;
			break;
		}
	}
	SQLiteData::updateUnit(unit_id,groupUnit);

}



void HcharacterLayer::rewrite(CCObject* pSender){
	CCLog("HcharacterLayer::rewrite");
	writeCount_ = 0;
	removeChildByTag(ACTION_TAG, true);			// 清空Action
	if (this->getActionManager()->numberOfRunningActionsInTarget(getm_HDrawnode()) <= 0)
	{
		_manager.exitLuaEngine();
		_manager.initLuaEngine();
		this->getm_HDrawnode()->rewrite();
		JudgeScene* scene = (JudgeScene*)this->getParent();
		if (scene->getIsJudge()==true)
		{
			string strToshow = DataTool::getChinese("bihuashu")+"0/"+ DataTool::intTostring(totalBihuaCount);
			this->getbihuaCountAndTotal()->setString(strToshow.c_str());
			string unit_id = scene->getUnitID();
			string curChar = scene->getCurChar();
			vector< vector<string> > groupUnit = SQLiteData::getUnit(unit_id);
			string scorestr;
			for (unsigned int i = 0; i < groupUnit.size(); i++)
			{
				if (curChar == groupUnit[i][0])
				{
					scorestr = groupUnit[i][2];
					break;
				}
			}
			float scoretemp = DataTool::stringToFloat(scorestr);
			string scorestring = DataTool::getChinese("defen")+DataTool::to_string_with_precision(scoretemp);
			getscoreLabel()->setString(scorestring.c_str());
			score_=0;
			curBihuaWrong=0;
		}
		getInfoSprite()->setVisible(false);
	}
}

void HcharacterLayer::clearWriting(){
	rewrite(this);
}

void HcharacterLayer::zoomin(CCObject* pSender){
	if (scale_ < 1.6)
	{
		scale_ += 0.1f;
		getm_HDrawnode()->setScale(scale_);
	}
}

void HcharacterLayer::zoomout(CCObject* pSender){
	if (scale_ > 0.5)
	{
		scale_ -= 0.1f;
		getm_HDrawnode()->setScale(scale_);
	}
}

void HcharacterLayer::reloadChar(){
	this->removeChild(getm_HDrawnode());
	this->setm_HDrawnode(HcharacterDrawnode::create());
	m_HDrawnode->setPosition(m_sprite_draw->getPosition());
	m_HDrawnode->setAnchorPoint(ccp(0.5,0.5));
	m_HDrawnode->setScale(scale_);
	this->addChild(m_HDrawnode);
	TcharacterLayer* layer = (TcharacterLayer*)this->getParent()->getChildByTag(kTLayerTag);		//get TcharacterLayer
	this->totalBihuaCount = layer->getm_TDrawnode()->getCharacter().getStrokeCount();
	curBihuaWrong=0;
	score_=0;
	rewrite(this);
}

float HcharacterLayer::getWrongPercent(){
	if (writeCount_ == 0)
	{
		return 0;
	}
	return (float)wrongCount_/writeCount_;
}

void HcharacterLayer::isJudge(bool isjudge){
	this->ijudge_ = isjudge;
}
