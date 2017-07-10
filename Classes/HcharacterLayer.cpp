#include "HcharacterLayer.h"
#include "StrokeDrawnode.h"
#include "Stroke.h"
#include "TcharacterLayer.h"
#include "SimpleAudioEngine.h"
#include "MyToast.h"
#include "tools/DataTool.h"
#include "constants.h"
#include "JudgeScene.h"
#include "Error.h"

//////////////////////////////////////////////
//A0001;A0008;A0009
#include "MultiPointNode.h"
#include "MoveToHorizontal.h"
#include "MoveToVertical.h"
//A0002;A0003
#include "OnePointNode.h"
#include "MoveToRightPlace.h"
//A0004
#include "DotLineNode.h"
//A0005;A0006
#include "UniformSpaceNode.h"
#include "MoveToUniformSpace.h"
//A0010
#include "TwoLineNode.h"
#include "DynamicDrawing.h"
//A0007
#include "BarycentreNode.h"
#include "MoveBarycentre.h"
//C0001_2_3
#include "RectangleNode.h"
#include "MoveRectangleNode.h"
/////////////////////////////////////////////



#include <iomanip>


#include "../rapidjson/document.h"

using namespace rapidjson;
using namespace std;

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

string HcharacterLayer::getErrorType(rapidjson::Document& doc) {
	string errorType = "";
	if (doc.HasMember("error"))
	{
		const Value& errorjson = doc["error"];
		if (errorjson.Size()>0)
		{
			rapidjson::SizeType index = 0;
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

multimap<int, float> HcharacterLayer::getErrorStroke(rapidjson::Document& doc) {
	multimap<int, float> errorPoints;
	if (doc.HasMember("error"))
	{
		const Value& errorjson = doc["error"];
		if (errorjson.Size() > 0)
		{
			rapidjson::SizeType index = 0;
			if (errorjson.Size()>0)
			{
				if(errorjson[index].HasMember("errorstroke")){
					const Value& v = errorjson[index]["errorstroke"];
					for (Value::ConstMemberIterator iter = v.MemberBegin(); iter != v.MemberEnd(); ++iter)
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


multimap<int, float> HcharacterLayer::getRightPoints(rapidjson::Document& doc) {
	multimap<int, float> errorPoints;
	if (doc.HasMember("error"))
	{
		const Value& errorjson = doc["error"];
		if (errorjson.Size() > 0)
		{
			rapidjson::SizeType index = 0;
			if (errorjson.Size()>0)
			{
				if(errorjson[index].HasMember("rightposition")){
					const Value& v = errorjson[index]["rightposition"];
					for (Value::ConstMemberIterator iter = v.MemberBegin(); iter != v.MemberEnd(); ++iter)
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
		return;

	//////////////////////////////////////显示文字提示
	string errortype = "A0001";
	MyToast::showToast(this, DataTool::getChinese(errortype), TOAST_LONG);

	//////////////////////////////////////显示动画提示
	vector<CCPoint> tempPoints = getm_HDrawnode()->GetErrorPoints(points);

	vector<CCPoint> error_points;
	while( tempPoints.size() != 0)
	{
		vector<CCPoint>::iterator it = tempPoints.begin();
		CCPoint curPoint =*it;
		vector<CCPoint>::iterator ita=it;

		for(;it!=tempPoints.end();it++)
		{
			if((*it).x < curPoint.x)
			{
				curPoint = *it;
				ita = it;
			}
		}

		error_points.push_back(curPoint);
		tempPoints.erase(ita);
	}

	if (error_points.size() >= 2)
	{
		MultiPointNode* errorNode = MultiPointNode::create();
		errorNode->setPoint(error_points);
		CCSize content_size = m_sprite_draw->getContentSize();
		errorNode->setPosition(m_sprite_draw->getPosition()- 
			ccp(content_size.width*scale_/2, content_size.height*scale_/2));
		errorNode->setScale(scale_);
		errorNode->setAnchorPoint(ccp(0, 0));
		addChild(errorNode, 1, ACTION_TAG);

		/////////////////////////////////////////////
 		MoveToHorizontal* actionA0001 = MoveToHorizontal::create(2.0);
		errorNode->runAction(actionA0001);
	}
}

void HcharacterLayer::doA0002(multimap<int, float>& points, multimap<int, float>& rightpoints){
	if (points.size() <= 0)
		return;
	
	////////////////////////////////////////////////////////////////////
	string errorType = "A0002";
	MyToast::showToast(this, DataTool::getChinese(errorType), TOAST_LONG);

	vector<CCPoint> errorPoints = getm_HDrawnode()->GetErrorPoints(points);
	vector<CCPoint> midPoints = getm_HDrawnode()->GetErrorPoints(rightpoints);
	
	if (errorPoints.size() >= 1)
	{
		//只选一个出错点即可
		CCPoint errorPoint = errorPoints.at(0);
		CCPoint midPoint = midPoints.at(0);
		OnePointNode* errorNode = OnePointNode::create();
		errorNode->setPoint(errorPoint);
		errorNode->setRightPoint(midPoint);
		CCSize content_size = m_sprite_draw->getContentSize();
		errorNode->setPosition(m_sprite_draw->getPosition()-
			ccp(content_size.width*scale_/2, content_size.height*scale_/2));
		errorNode->setScale(scale_);
		errorNode->setAnchorPoint(ccp(0,0));
		addChild(errorNode, 1, ACTION_TAG);
 
		//////////////////////////////////////////////////
		MoveToRightPlace* action = MoveToRightPlace::create(2.0);
		errorNode->runAction(action);
	}

}

void HcharacterLayer::doA0003(multimap<int, float>& points, multimap<int, float>& rightpoints){
	if (points.size() <= 0)
		return;

	////////////////////////////////////////////////////////////////////
	string errorType = "A0003";
	MyToast::showToast(this, DataTool::getChinese(errorType), TOAST_LONG);

	vector<CCPoint> errorPoints = getm_HDrawnode()->GetErrorPoints(points);
	vector<CCPoint> midPoints = getm_HDrawnode()->GetErrorPoints(rightpoints);

	if (errorPoints.size() >= 1)
	{
		//只选一个出错点即可
		CCPoint errorPoint = errorPoints.at(0);
		CCPoint midPoint = midPoints.at(0);
		OnePointNode* errorNode = OnePointNode::create();
		errorNode->setPoint(errorPoint);
		errorNode->setRightPoint(midPoint);
		CCSize content_size = m_sprite_draw->getContentSize();
		errorNode->setPosition(m_sprite_draw->getPosition()-
			ccp(content_size.width*scale_/2, content_size.height*scale_/2));
		errorNode->setScale(scale_);
		errorNode->setAnchorPoint(ccp(0,0));
		addChild(errorNode, 1, ACTION_TAG);

		//////////////////////////////////////////////////
		MoveToRightPlace* action = MoveToRightPlace::create(2.0);
		errorNode->runAction(action);
	}
}

void HcharacterLayer::doA0004(multimap<int, float>& points, multimap<int, float>& rightpoints){
	if (points.size() <= 0)
		return;

	////////////////////////////////////////////////////////////////////
	string errorType = "A0004";
	MyToast::showToast(this, DataTool::getChinese(errorType), TOAST_LONG);
	
	/////////////////给错误笔画着色
	int markStroke=-1;
	for (multimap<int, float>::iterator it = points.begin(); it != points.end(); it++)
		{ markStroke = it->first;}
	
	if (markStroke != -1)
		{getm_HDrawnode()->markErrorStroke(markStroke);}

	/////////////// 画虚线
	vector<CCPoint> errorPoints = getm_HDrawnode()->GetErrorPoints(points);	
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
		dotlineNode->runAction(blink);
	}
}

void HcharacterLayer::doA0005(multimap<int, float>& points, multimap<int, float>& rightpoints){
	string errotType = "A0005";
	MyToast::showToast(this, DataTool::getChinese(errotType), TOAST_LONG);
	//////////////////////////////////////////////////////////////////////////
	vector<CCPoint> tempPoints = getm_HDrawnode()->GetErrorPoints(points);

	vector<CCPoint> errorPoints;
	while( tempPoints.size() != 0)
	{
		vector<CCPoint>::iterator it = tempPoints.begin();
		CCPoint curPoint =*it;
		vector<CCPoint>::iterator ita=it;

		for(;it!=tempPoints.end();it++)
		{
			if((*it).y < curPoint.y)
			{
				curPoint = *it;
				ita = it;
			}
		}

		errorPoints.push_back(curPoint);
		tempPoints.erase(ita);
	}

	if (errorPoints.size() >= 3)
	{
		UniformSpaceNode* myNode = UniformSpaceNode::create();
		myNode->setPoint(errorPoints);
		myNode->setLength(100);
		myNode->setOrientation(1);

		CCSize size = m_sprite_draw->getContentSize();
		myNode->setPosition(m_sprite_draw->getPosition()-
				ccp(size.width*scale_/2, size.height*scale_/2));
		myNode->setScale(scale_);
		myNode->setAnchorPoint(ccp(0,0));
		addChild(myNode, 1, ACTION_TAG);

		CCBlink* blink = CCBlink::create(1,3);		
		MoveToUniformSpace* moveaction = MoveToUniformSpace::create(2);	
		CCSequence* ccSequence = CCSequence::create(blink, moveaction, NULL);
		myNode->runAction(ccSequence);
	}
}

void HcharacterLayer::doA0006(multimap<int, float>& points, multimap<int, float>& rightpoints){
	string errortype = "A0006";
	MyToast::showToast(this, DataTool::getChinese(errortype), TOAST_LONG);
	/////////////////////////////////////////
	vector<CCPoint> tempPoints = getm_HDrawnode()->GetErrorPoints(points);
	vector<CCPoint> errorPoints;
	while( tempPoints.size() != 0)
	{
		vector<CCPoint>::iterator it = tempPoints.begin();
		CCPoint curPoint =*it;
		vector<CCPoint>::iterator ita=it;

		for(;it!=tempPoints.end();it++)
		{
			if((*it).x < curPoint.x)
			{
				curPoint = *it;
				ita = it;
			}
		}

		errorPoints.push_back(curPoint);
		tempPoints.erase(ita);
	}

	if (errorPoints.size() >= 3)
	{
		UniformSpaceNode* myNode = UniformSpaceNode::create();
		myNode->setPoint(errorPoints);
		myNode->setLength(100);
		myNode->setOrientation(0);

		CCSize size = m_sprite_draw->getContentSize();
		myNode->setPosition(m_sprite_draw->getPosition()-
			ccp(size.width*scale_/2, size.height*scale_/2));
		myNode->setScale(scale_);
		myNode->setAnchorPoint(ccp(0,0));
		addChild(myNode, 1, ACTION_TAG);

		CCBlink* blink = CCBlink::create(1,3);		
		MoveToUniformSpace* moveaction = MoveToUniformSpace::create(2);	
		CCSequence* ccSequence = CCSequence::create(blink, moveaction, NULL);
		myNode->runAction(ccSequence);
	}
}

void HcharacterLayer::doA0007(multimap<int, float>& points, multimap<int, float>& rightpoints){
	///////////////////////////////////////
	string errortype = "A0007";
	MyToast::showToast(this, DataTool::getChinese(errortype), TOAST_LONG);
	
	//////////////////////////////////////
	vector<CCPoint> error_points = getm_HDrawnode()->GetErrorPoints(points);
	if (error_points.size() >= 1)
	{
		BarycentreNode* errorNode = BarycentreNode::create();
		errorNode->setBarycentrePoint(*(error_points.begin()));

		/////////////////////////////////////////////////////
		HcharacterDrawnode* tianzige =getm_HDrawnode();
		int strokecount=tianzige->getStrokeCount();
		float x_min=512;
		float x_max=0;
		float y_min=512;
		float y_max=0;
		for(int i=0;i<strokecount;i++)
		{
			Stroke temp=tianzige->getStroke(i);
			vector<CCPoint> pointlist=temp.getpointList();
			vector<CCPoint>::iterator it=pointlist.begin();
			for (;it!=pointlist.end();it++)
			{
				if((*it).x < x_min)
					x_min=(*it).x;
				if((*it).x > x_max)
					x_max=(*it).x;
				if((*it).y < y_min)
					y_min=(*it).y;
				if((*it).y > y_max)
					y_max=(*it).y;
			}
		}
		errorNode->setLeftPoint(CCPoint(x_min,y_max),CCPoint(x_min,y_min));
		errorNode->setRightPoint(CCPoint(x_max,y_max),CCPoint(x_max,y_min));
		/////////////////////////////////////////////////////
		CCSize content_size = m_sprite_draw->getContentSize();
		errorNode->setPosition(m_sprite_draw->getPosition()- 
			ccp(content_size.width*scale_/2, content_size.height*scale_/2));
		errorNode->setScale(scale_);
		errorNode->setAnchorPoint(ccp(0, 0));
		addChild(errorNode, 1, ACTION_TAG);

		/////////////////////////////////////////////
		MoveBarycentre* action = MoveBarycentre::create(2.0);
		errorNode->runAction(action);
	}
}

void HcharacterLayer::doA0008(multimap<int, float>& points, multimap<int, float>& rightpoints){
	if (points.size() < 2)
		return;

	//////////////////////////////////////显示文字提示
	string errortype = "A0008";
	MyToast::showToast(this, DataTool::getChinese(errortype), TOAST_LONG);

	//////////////////////////////////////显示动画提示
	vector<CCPoint> tempPoints = getm_HDrawnode()->GetErrorPoints(points);

	vector<CCPoint> error_points;
	while( tempPoints.size() != 0)
	{
		vector<CCPoint>::iterator it = tempPoints.begin();
		CCPoint curPoint =*it;
		vector<CCPoint>::iterator ita=it;

		for(;it!=tempPoints.end();it++)
		{
			if((*it).y < curPoint.y)
			{
				curPoint = *it;
				ita = it;
			}
		}

		error_points.push_back(curPoint);
		tempPoints.erase(ita);
	}

	if (error_points.size() >= 2)
	{
		MultiPointNode* errorNode = MultiPointNode::create();
		errorNode->setPoint(error_points);
		CCSize content_size = m_sprite_draw->getContentSize();
		errorNode->setPosition(m_sprite_draw->getPosition()- 
			ccp(content_size.width*scale_/2, content_size.height*scale_/2));
		errorNode->setScale(scale_);
		errorNode->setAnchorPoint(ccp(0, 0));
		addChild(errorNode, 1, ACTION_TAG);

		/////////////////////////////////////////////
		MoveToVertical* actionA0008 = MoveToVertical::create(2.0);
		errorNode->runAction(actionA0008);
	}
}

void HcharacterLayer::doA0009(multimap<int, float>& points, multimap<int, float>& rightpoints){
	if (points.size() <= 0)
		return;

	////////////////////////////////////////////////////////////////////
	string errortype = "A0009";
	MyToast::showToast(this, DataTool::getChinese(errortype), TOAST_LONG);

	///////////////////////////////////////////////////////////////////
	int markStroke=-1;
	for (multimap<int, float>::iterator it = points.begin(); it != points.end(); it++)
	{ markStroke = it->first;}

	if (markStroke != -1)
	{getm_HDrawnode()->markErrorStroke(markStroke);}

	//////////////////////////////////////显示动画提示
	vector<CCPoint> error_points = getm_HDrawnode()->GetErrorPoints(points);
	if (error_points.size() >= 2)
	{
		MultiPointNode* errorNode = MultiPointNode::create();
		errorNode->setPoint(error_points);
		CCSize content_size = m_sprite_draw->getContentSize();
		errorNode->setPosition(m_sprite_draw->getPosition()- 
			ccp(content_size.width*scale_/2, content_size.height*scale_/2));
		errorNode->setScale(scale_);
		errorNode->setAnchorPoint(ccp(0, 0));
		addChild(errorNode, 1, ACTION_TAG);

		/////////////////////////////////////////////
		if(fabs(error_points[0].x-error_points[1].x) > fabs(error_points[0].y-error_points[1].y))
		{
			MoveToHorizontal* action = MoveToHorizontal::create(2.0);
			errorNode->runAction(action);
		}
		else
		{
			MoveToVertical* action = MoveToVertical::create(2.0);
			errorNode->runAction(action);
		}

	}
}

void HcharacterLayer::doA0010(multimap<int, float>& points, multimap<int, float>& rightpoints){
	////////////////////////////////////////////////////////////////////显示文字提示
	string errortype = "A0010";
	MyToast::showToast(this, DataTool::getChinese(errortype), TOAST_LONG);

	////////////////////////////////////////////////////////////////////显示动画提示
	vector<CCPoint> errorPoints = getm_HDrawnode()->GetErrorPoints(points);
	if (errorPoints.size() == 4)
	{
		TwoLineNode* myNode = TwoLineNode::create();
		myNode->setPoint(errorPoints);

		CCSize size = m_sprite_draw->getContentSize();
		myNode->setPosition(m_sprite_draw->getPosition()-
			ccp(size.width*scale_/2, size.height*scale_/2));
		myNode->setScale(scale_);
		myNode->setAnchorPoint(ccp(0,0));
		addChild(myNode, 1, ACTION_TAG);
						
		DynamicDrawing* Drawaction = DynamicDrawing::create(2);	
		CCBlink* blink = CCBlink::create(1,3);
		CCSequence* ccSequence = CCSequence::create(Drawaction, blink, NULL);
		myNode->runAction(ccSequence);
	}
}

void HcharacterLayer::doC0001_2_3(){

	HcharacterDrawnode* tianzige =getm_HDrawnode();
	int strokecount=tianzige->getStrokeCount();

	if (strokecount > 0)
	{
		RectangleNode* errorNode = RectangleNode::create();

		float x_min=512;
		float x_max=0;
		float y_min=512;
		float y_max=0;
		for(int i=0; i<strokecount; i++)
		{
			Stroke temp=tianzige->getStroke(i);
			vector<CCPoint> pointlist=temp.getpointList();
			vector<CCPoint>::iterator it=pointlist.begin();
			for (;it!=pointlist.end();it++)
			{
				if((*it).x < x_min)
					x_min=(*it).x;
				if((*it).x > x_max)
					x_max=(*it).x;
				if((*it).y < y_min)
					y_min=(*it).y;
				if((*it).y > y_max)
					y_max=(*it).y;
			}
		}
		errorNode->setPoint(CCPoint(x_min,y_max), CCPoint(x_min, y_min), CCPoint(x_max,y_max), CCPoint(x_max,y_min));
		CCSize MattSize=this->m_sprite_draw->getContentSize();
		errorNode->setSize(MattSize);

		/////////////////////////////////////////////////////
		CCSize content_size = m_sprite_draw->getContentSize();
		errorNode->setPosition(m_sprite_draw->getPosition()- 
			ccp(content_size.width*scale_/2, content_size.height*scale_/2));
		errorNode->setScale(scale_);
		errorNode->setAnchorPoint(ccp(0, 0));
		addChild(errorNode, 1, ACTION_TAG);

		/////////////////////////////////////////////
		CCBlink* blink1 = CCBlink::create(1,3);
		CCDelayTime* delay1 = CCDelayTime::create(0.4);
		MoveRectangleNode* action = MoveRectangleNode::create(1);	
		CCDelayTime* delay2 = CCDelayTime::create(0.4);
		CCBlink* blink2 = CCBlink::create(1,3);
		CCSequence* ccSequence = CCSequence::create(blink1, delay1,action, delay2,blink2, NULL);
		errorNode->runAction(ccSequence);
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
			//////1.笔画级别的错误处理
			if (retjson.at(0) == '1')
			{
				//笔画正确
				writeBihuaRight();
			}else{
				//笔画错误
				writeBihuaWrong();
				//播放声音，显示提示信息
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(WRONG_EFFECT_FILE);
				MyToast::showToast(this,DataTool::getChinese(errorType),TOAST_LONG);
			}

			//////2.部件级别的错误处理
			if (retjson.at(1) == '1')
			{
				//部件正确：不作处理
				
			}else
			{
				//部件错误：播放声音，显示动画
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(WRONG_EFFECT_FILE);
							
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
					doA0006(points, rightpoints);
				}
				if (errorType == "A0007")
				{
					doA0007(points, rightpoints);					
				}
				if (errorType == "A0008")
				{
					doA0008(points, rightpoints);
				}
				if (errorType == "A0009")
				{
					doA0009(points, rightpoints);					
				}
				if (errorType == "A0010")
				{
					doA0010(points, rightpoints);					
				}
			}

			//////3.整字级别的错误处理
			if (retjson.at(2) == '1')
			{
				//整字正确：不作处理

			} else {
				//播放声音，显示提示
				if (errorType == "C0001")  //纵横比错误
				{
					CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(WRONG_EFFECT_FILE);
					string errortype = "C0001";
					MyToast::showToast(this, DataTool::getChinese(errortype), TOAST_LONG);
					doC0001_2_3();
				}				
			}

			//////4.全部写完，给出最终图标、声音提示
			if(retjson.at(0) == '1' && retjson.at(1) == '1' && retjson.at(2) == '1')
			{				
				int t = getm_HDrawnode()->getStrokeDrawnodeList()->count();
				TcharacterLayer* layer = (TcharacterLayer*)this->getParent()->getChildByTag(kTLayerTag);		//get TcharacterLayer
				if ( t >= layer->getm_TDrawnode()->getCharacter().getStrokeCount())
				{
					//对整字在田字格中的大小、位置进行评判
					string errortype = "right";
					if (CharacterSizeJudge()==false)
						errortype = "C0002";
					if(CharacterPosJudge()==false)
						errortype = "C0003";

					////////////////////////////////////////////////////
					if (errortype == "C0002") //字写得太小
					{
						CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(WRONG_EFFECT_FILE);
						MyToast::showToast(this, DataTool::getChinese(errortype), TOAST_LONG);
						doC0001_2_3();
					}
					else if(errortype == "C0003")//字写得太偏
					{
						CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(WRONG_EFFECT_FILE);
						MyToast::showToast(this, DataTool::getChinese(errortype), TOAST_LONG);
						doC0001_2_3();
					}
					else
					{
						//////////////////////////////////////
						getInfoSprite()->setVisible(true);
						getInfoSprite()->setTexture(CCTextureCache::sharedTextureCache()->addImage("right.png"));
						CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(RIGHT_EFFECT_FILE);
					}
				}				
			}
		} else {
			// 如果 ret 返回长度不等于3 报错
			CCLog("ret length is not 3");
			MyToast::showToast(this, "ret error ret length is not 3", TOAST_LONG);
		}
	}
}

bool HcharacterLayer::CharacterSizeJudge(){
	//田字格尺寸
	CCSize MattSize=this->m_sprite_draw->getContentSize();

	//////////
	//手写字尺寸
	float x_min=1024;
	float x_max=0;
	float y_min=1024;
	float y_max=0;

	CCArray* strokes = m_HDrawnode->getStrokeDrawnodeList();
	CCObject* originob;
	CCARRAY_FOREACH(strokes,originob){
		StrokeDrawnode* node = (StrokeDrawnode*)originob;
		Stroke stro = node->getStroke();
		vector<CCPoint> templist=stro.getpointList();

		for (vector<CCPoint>::const_iterator it = templist.begin(); it != templist.end() ; ++it)
		{
			CCPoint temp = (*it);

			if(temp.x>x_max)
				x_max=temp.x;
			if (temp.x<x_min)
				x_min=temp.x;
			if(temp.y>y_max)
				y_max=temp.y;
			if(temp.y<y_min)
				y_min=temp.y;
		}
	}

	//////////比较
	float width=x_max-x_min;
	float height=y_max-y_min;

	if(width<MattSize.width*0.43 || height<MattSize.height*0.43)
		return false;
	/////////////////////////////////////////
	return true;
}

bool HcharacterLayer::CharacterPosJudge(){
	//田字格尺寸
	CCSize MattSize=this->m_sprite_draw->getContentSize();

	//////////
	//手写字尺寸
	float x_min=1024;
	float x_max=0;
	float y_min=1024;
	float y_max=0;

	CCArray* strokes = m_HDrawnode->getStrokeDrawnodeList();
	CCObject* originob;
	CCARRAY_FOREACH(strokes,originob){
		StrokeDrawnode* node = (StrokeDrawnode*)originob;
		Stroke stro = node->getStroke();
		vector<CCPoint> templist=stro.getpointList();

		for (vector<CCPoint>::const_iterator it = templist.begin(); it != templist.end() ; ++it)
		{
			CCPoint temp = (*it);

			if(temp.x>x_max)
				x_max=temp.x;
			if (temp.x<x_min)
				x_min=temp.x;
			if(temp.y>y_max)
				y_max=temp.y;
			if(temp.y<y_min)
				y_min=temp.y;
		}
	}

	//////////比较
	float dis =sqrtf(((x_max+x_min)/2.0-MattSize.width/2.0)*((x_max+x_min)/2.0-MattSize.width/2.0)+
		             ((y_max+y_min)/2.0-MattSize.height/2.0)*((y_max+y_min)/2.0-MattSize.height/2.0));

	if(dis> MattSize.width*0.16 || dis>MattSize.height*0.16)
		return false;
	else
		return true;
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

	//////////////////////////////////////////////////////////////////
	string funcs = "";
	TcharacterLayer* tlayer = (TcharacterLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(kTLayerTag);
	string points = tlayer->getm_TDrawnode()->getCharacterStandardInfo();		//获取正字信息
    CCLog("output %s",output.c_str());
    CCLog("right Character info %s",points.c_str());
    
	string ret = _manager.getResult(hanzi_,output,points,m_exChar,funcs);
	/////////////////////////////////////////////////////////////////////////////////////////

	//如果不评判则跳过
	if (!ijudge_ || ret.length() <= 0)
	{
		return;
	}
	CCLog("%s\n", ret.c_str());

	//解析评判结果，产生动画反馈！
	ParseResult(ret);
}

void HcharacterLayer::writeBihuaWrong(){

	this->getm_HDrawnode()->removeLastStroke();
	int t = getm_HDrawnode()->getStrokeDrawnodeList()->count();
	string strToshow = DataTool::getChinese("bihuashu")+DataTool::intTostring(t)+"/"+DataTool::intTostring(totalBihuaCount);
	bihuaCountAndTotal->setString(strToshow.c_str());
	//////////////////////////////////////////////////////
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
		pointsOrigin.clear();
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
