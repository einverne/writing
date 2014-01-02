#include "lianxi.h"
#include "UTF8ToGBK.h"
#include "WallScene.h"
#include "Tianzige.h"
#include "tinyxml.h"
#include "ReadXML.h"
#include <sstream>
// #include "Stroke.h"
#include "StrokeNode.h"
#include "tools/DataTool.h"

#define tianzige_draw_tag 1001

lianxi::lianxi(){
	
	
}

lianxi::~lianxi(){

}

bool lianxi::init(){
	if (CCLayer::init())
	{
		this->setKeypadEnabled(true);
		this->setTouchEnabled(true);
		output = "";
		current_writing_stroke = 1;
		CCSize winSize = CCDirector::sharedDirector()->getWinSize(); 
		CCSize visiableSize = CCDirector::sharedDirector()->getVisibleSize();
		CCSize originSize = CCDirector::sharedDirector()->getVisibleOrigin();

		///////////////////////
		// add a dog sprite
		CCPoint ptCenter = CCPointMake(winSize.width/2,winSize.height/2);
		CCSprite* dog = CCSprite::create("dog.png");
		dog->setPosition(ptCenter);
		this->addChild(dog,1000,1000);
		CCMoveTo* moveTo = CCMoveTo::create(2,CCPointMake(300,500));
		dog->runAction(moveTo);

		//add wall-head
		CCSprite* wall_head = CCSprite::create("wall_head.png");
		this->addChild(wall_head,2);
		CCSize headSize = wall_head->getContentSize();
		wall_head->setPosition(ccp(visiableSize.width/2,visiableSize.height-wall_head->getContentSize().height/2));
		wall_head->setScaleX(visiableSize.width/wall_head->getContentSize().width);

		//add ceshi tail on the bottom
		CCSprite* wall_tail = CCSprite::create("wall_tail.png");
		this->addChild(wall_tail,2);
		CCSize tailSize = wall_tail->getContentSize();
		wall_tail->setPosition(ccp(visiableSize.width/2,wall_tail->getContentSize().height/2));
		wall_tail->setScaleX(visiableSize.width/wall_tail->getContentSize().width);

		//add tianzige
		tianzige = CCSprite::create("tianzige.png");
		this->addChild(tianzige,1);
		CCSize tianzigeSize = tianzige->getContentSize();
		tianzige->setPosition(ccp(visiableSize.width/2,visiableSize.height-headSize.height-tianzigeSize.height/2-50));


		tianzige_draw = CCSprite::create("tianzige.png");
		this->addChild(tianzige_draw,1,tianzige_draw_tag);
		CCSize tianzigeDrawSize = tianzige_draw->getContentSize();
		tianzige_draw->setPosition(ccp(visiableSize.width/2,tailSize.height+50+tianzigeDrawSize.height/2));

		CCSprite* bg = CCSprite::create("lianxi_background.png");
		this->addChild(bg,0);
		bg->setPosition(ccp(visiableSize.width/2,visiableSize.height/2));
		bg->setScaleX(visiableSize.width/bg->getContentSize().width);
		bg->setScaleY((visiableSize.height-headSize.height-tailSize.height)/bg->getContentSize().height);

		//     CCSprite* bg_top = CCSprite::create("lianxi_top.png");
		//     this->addChild(bg_top,0);
		//     bg_top->setPosition(ccp(visiableSize.width/2,-headSize.height+visiableSize.height-bg_top->getContentSize().height/2));
		//     bg_top->setScaleX(visiableSize.width/bg_top->getContentSize().width);
		//     float scale_y = visiableSize.height/(visiableSize.height-headSize.height*2);
		//     bg_top->setScaleY(scale_y);
		// 
		//     CCSprite* bg_bottom = CCSprite::create("lianxi_bottom.png");
		//     this->addChild(bg_bottom,0);
		//     bg_bottom->setPosition(ccp(visiableSize.width/2,tailSize.height+bg_bottom->getContentSize().height/2));
		//     bg_bottom->setScale(visiableSize.width/bg_bottom->getContentSize().width);
		//     bg_bottom->setScaleY(scale_y);
		// 
		//     CCSprite* bg_separ = CCSprite::create("lianxi_separate.png");
		//     this->addChild(bg_separ,0);
		//     bg_separ->setPosition(ccp(visiableSize.width/2,visiableSize.height/2));
		//     bg_separ->setScaleX(visiableSize.width/bg_separ->getContentSize().width);

		CCLabelTTF* back = CCLabelTTF::create("BACK","Arial",25);
		CCMenuItemLabel* menuLabel = CCMenuItemLabel::create(back,this,menu_selector(lianxi::menuBack));
		CCMenu* menu = CCMenu::create(menuLabel,NULL);
		this->addChild(menu,20);
		menuLabel->setPosition(ccp(back->getContentSize().width/2+100,back->getContentSize().height+30));
		menu->setPosition(CCPointZero);
		/*menu->registerWithTouchDispatcher();*/

		CCLabelTTF* bihuaCount = CCLabelTTF::create(UTF8ToGBK::UTF8TOGBK(string("笔画数")).c_str(),"Arial",40);
		addChild(bihuaCount);
		bihuaCount->setPosition(tianzige->getPosition()+ccp(-tianzigeSize.width/2-bihuaCount->getContentSize().width/2-50,tianzigeSize.height/2));
		CCLabelTTF* bihuashuzi = CCLabelTTF::create();
		addChild(bihuashuzi);
		bihuashuzi->setPosition(bihuaCount->getPosition()-ccp(0,2*bihuaCount->getContentSize().height));


		CCLog(hanzi.c_str());
		//luanma
		CCLabelTTF* pHanzi = CCLabelTTF::create(UTF8ToGBK::UTF8TOGBK(hanzi).c_str(),"Arial", 100);
		this->addChild(pHanzi,10);
		pHanzi->setPosition(tianzige->getPosition());

		//read xml show animation of character
		string xml = CCFileUtils::sharedFileUtils()->fullPathForFilename("xml/八.xml");
		CReadXML readxml(xml);
		charac = readxml.getCharacter();
		int coun = charac.getStrokeCount();
		string str_coun = std::to_string(coun);
		bihuashuzi->initWithString(str_coun.c_str(),"Arial",40);
		// 	this->addChild(readxml.getCharacter(),10);
		// 	readxml.getCharacter()->setPosition(ccp(visiableSize.width/2,visiableSize.height/2));


		//画正字
		charac.getBox();
		charac.resize(tianzige->getContentSize());
		//重采样
		charac.resample();

		charac.prepareDrawNode();
		vector<Bujian> bujianList = charac.bujianList;
		vector<Bujian>::iterator iter = bujianList.begin();
		for (iter;iter != bujianList.end(); ++iter)
		{
			Bujian bujian = (Bujian)*iter;
			vector<Stroke> strokeList = bujian.strokeList;
			vector<Stroke>::iterator stro_iter = strokeList.begin();
			for (stro_iter; stro_iter != strokeList.end(); ++stro_iter)
			{
				Stroke stroke = (Stroke)*stro_iter;
				vector<CCDrawNode*> nodeList = stroke.nodeList;
				for (vector<CCDrawNode*>::iterator nodeIter = nodeList.begin(); nodeIter != nodeList.end(); ++nodeIter)
				{
					tianzige->addChild(*nodeIter,100);
				}
			}
		}

		//初始化writingPoints
		writing_points_node = CCNode::create();
		this->addChild(writing_points_node,20);
		writing_points_node->setPosition(ccp(visiableSize.width/2 - 0.5*tianzige->getContentSize().width, visiableSize.height-headSize.height-tianzigeSize.height/2-50 - 0.5*tianzigeSize.height/2));
		writing_points_node->setContentSize(tianzigeSize);
// 		writing_points_node->set


// 		vector<CCDrawNode*> nodeList = charac.getStroke(1).nodeList;
// 		CCMoveTo* actionTo = CCMoveTo::create(2,ccp(visiableSize.width/2,visiableSize.height-headSize.height-tianzigeSize.height/2-50));
// 		for (vector<CCDrawNode*>::const_iterator it = nodeList.begin(); it != nodeList.end(); ++it)
// 		{
// 			(*it)->runAction(actionBy);
// 		}

		Originccccc = readxml.getCharacter();


		return true;
	}
	return false;
}

void lianxi::onEnter(){
	CCLayer::onEnter();
	CCLog("onEnter lixian");

	//将构造的正确汉字放置到手写字区域，并且不显示
	string xml = CCFileUtils::sharedFileUtils()->fullPathForFilename("xml/八.xml");
	CReadXML readxml(xml);
	newCharac = readxml.getCharacter();
	newCharac.getBox();
	newCharac.resize(tianzige->getContentSize());
	newCharac.resample();
	newCharac.prepareDrawNode();

	Stroke stro = newCharac.getStroke(current_writing_stroke);
// 	vector<CCDrawNode*> nList = stro.nodeList;
// 	for (vector<CCDrawNode*>::iterator it = nList.begin(); it != nList.end() ;++it)
// 	{
// 		writing_points_node->addChild(*it,50);
// 		(*it)->setVisible(true);
// 	}


	//测试写好的StrokeNode类
	/////////////////////////////////////


	for (int i = 0 ; i < newCharac.getStrokeCount(); ++i)
	{
		Stroke stroi = newCharac.getStroke(i+1);			
		my_strokenode = StrokeNode::getStrokeNode(stroi);
		my_strokenode->setVisible(false);
		strokeNodes.push_back(my_strokenode);
		this->addChild(my_strokenode,1000);
		my_strokenode->setPosition(tianzige->getPosition()-ccp(tianzige->getContentSize().width/2,tianzige->getContentSize().height/2));

	}
	strokeNodes[0]->setVisible(true);
	//my_strokenode->runAction(CCSequence::create(mb,rb,NULL));



}

void lianxi::onExit(){
	CCLayer::onExit();
}

void lianxi::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}

/************************************************************************/
/* hanzi 需要练习的汉字                                                                     */
/************************************************************************/
CCScene* lianxi::scene(string hanzi){
	CCScene *scene = CCScene::create();
	lianxi *layer = lianxi::create();
	layer->hanzi = hanzi;
	scene->addChild(layer);
	return scene;
}

// bool  lianxi::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
// 	prePoint = pTouch->getLocation();
// 	CCSprite* tianzige = (CCSprite*)this->getChildByTag(tianzige_draw_tag);
// 	CCRect rect = tianzige->boundingBox();
// 	if (rect.containsPoint(prePoint))
// 	{
// 		output += "[[";
// 		string temp = convertToString(prePoint.x) + "/" + convertToString(prePoint.y) + ",";
// 		output += temp;
// 	}
// 	return true;
// }

// void  lianxi::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
// 	CCPoint location = pTouch->getLocation();
// 	CCSprite* tian = (CCSprite*)getChildByTag(tianzige_draw_tag);
// 	CCRect rect = tian->boundingBox();
// 	if (rect.containsPoint(location))
// 	{
// 		string temp = convertToString(location.x) + "/" + convertToString(location.y);
// 		output += temp;
// 		output += "]]";
// 	}
// 	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(lianxi::clearStroke),this,1.0,false);
// 
// 	//获取手写出来笔画的中心点 prePoint location
// 	centerPoint = centerP(prePoint,location);
// }

// void  lianxi::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
// 	CCPoint location = pTouch->getLocation();
// 
// 	CCSprite* tianzige = (CCSprite*)this->getChildByTag(tianzige_draw_tag);
// 	CCRect rect = tianzige->boundingBox();
// 	if (rect.containsPoint(location))
// 	{
// 		// 采集点信息
// 		string temp = convertToString(location.x) + "/" + convertToString(location.y) + ",";
// 		output += temp;
// 		// 	CCDrawNode * dot = CCDrawNode::create();
// 		// 	dot->drawDot(location,5.0,ccc4f(188, 188, 188, 120));
// 		// 	//this->addChild(dot,10);
// 		// 	tianzige->addChild(dot,10);
// 		CCDrawNode* node = CCDrawNode::create();
// 		writing_stroke.push_back(node);
// 		addChild(node,10);
// 		node->drawSegment(prePoint,location,5.0,ccc4f(180,180,180,100));
// 		prePoint = location;
// 	}
// }


void lianxi::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	if (current_writing_stroke > charac.getStrokeCount())
	{
		return;
	}
	writing_prePoint = ((CCTouch*)pTouches->anyObject())->getLocation();
	writing_points.push_back(writing_prePoint);
	CCSprite* tianzige = (CCSprite*)this->getChildByTag(tianzige_draw_tag);
	CCRect rect = tianzige->boundingBox();
	output = "";
	if (rect.containsPoint(writing_prePoint))
	{
		CCPoint tmpp = tianzige_draw->convertToNodeSpace(writing_prePoint);
		tmpp = convert512(tmpp);
		//output += "[[";
		string temp = convertToString(ceil(tmpp.x)) + "/" + convertToString(ceil(tmpp.y)) + "/";	
		output += temp;
	}
}

void lianxi::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	if (current_writing_stroke > charac.getStrokeCount())
	{
		return;
	}
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
	CCPoint location = pTouch->getLocation();
	writing_points.push_back(location);
	CCSprite* tianzige = (CCSprite*)this->getChildByTag(tianzige_draw_tag);
	CCRect rect = tianzige->boundingBox();
	if (rect.containsPoint(location))
	{
		CCPoint tmpp = tianzige_draw->convertToNodeSpace(location);
		tmpp = convert512(tmpp);
		// 采集点信息
		string temp = convertToString(ceil(tmpp.x)) + "/" + convertToString(ceil(tmpp.y)) + "/";
		output += temp;
		// 	CCDrawNode * dot = CCDrawNode::create();
		// 	dot->drawDot(location,5.0,ccc4f(188, 188, 188, 120));
		// 	//this->addChild(dot,10);
		// 	tianzige->addChild(dot,10);
		CCDrawNode* node = CCDrawNode::create();
		writing_stroke.push_back(node);
		addChild(node,10);
		node->drawSegment(writing_prePoint,location,5.0,ccc4f(180,180,180,100));
		writing_prePoint = location;
	}
}

void lianxi::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	if (current_writing_stroke > charac.getStrokeCount())
	{
		return;
	}
	CCPoint location = ((CCTouch*)pTouches->anyObject())->getLocation();
	writing_points.push_back(location);	//获取手写出来笔画的中心点 prePoint location

	centerPoint = ccpMidpoint(writing_points[0],location);	//centerP(writing_prePoint,location);

	CCSprite* tian = (CCSprite*)getChildByTag(tianzige_draw_tag);
	CCRect rect = tian->boundingBox();
	if (rect.containsPoint(location))
	{
		CCPoint tmpp = tianzige_draw->convertToNodeSpace(location);
		tmpp = convert512(tmpp);
		string temp = convertToString(ceil(tmpp.x)) + "/" + convertToString(ceil(tmpp.y));
		output += temp;
		output += "/@";
		//output += "]]";
	}
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(lianxi::clearStroke),this,1.0,false);

	float w = writing_points[0].x - location.x;
	float h = writing_points[0].y - location.y;

	CCSize writing_size = CCSizeMake(w,h);
	CCSize z_size = newCharac.getStroke(1).getRotateAng();
	

	float angle = ccpAngle(ccpFromSize(writing_size),ccpFromSize(z_size));
	angle = angle*180/3.1415;
	CCRotateBy* rotateBy = CCRotateBy::create(1,-angle);
//	writing_points_node->runAction(CCSequence::create(move_down,rotateBy,NULL));



	my_strokenode = strokeNodes[current_writing_stroke-1];
	// StrokeNode 实现
	CCPoint zhengziMidPoint = my_strokenode->convertToWorldSpace(my_strokenode->getMidStrokePoint());
	CCPoint writingMidPoint = centerPoint;
	CCPoint dp = writingMidPoint - zhengziMidPoint;
	CCMoveBy* mb = CCMoveBy::create(1,dp);
	
	CCMoveTo* mt = CCMoveTo::create(1,tianzige_draw->getPosition()-ccp(tianzige_draw->getContentSize().width/2,tianzige_draw->getContentSize().height/2));
	






	//////////////////////////////////////////

	string filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/WriteZiInfo.lua");
	string basepath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/BaseLib.lua");
	string gpath = string("lua/ZiList/") + hanzi + string("/funcs.txt");			//字符串拼接 调试技能 ,s8
	string Globalpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/ZiList/八/funcs.txt");
	string apipath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/RunAPI.lua");
	string rulespath = CCFileUtils::sharedFileUtils()->fullPathForFilename("lua/ZiList/八/rules.txt");

	//产生output

	int curr = 1;
	string toutput;
	if (current_writing_stroke > 1)
	{
		toutput = output;
		output = "";
	}
	while (curr < current_writing_stroke)
	{
		output += Originccccc.getStroke(curr).sendOutput();
		curr ++;
	}
	output += toutput;
	CCLog("output %s",output.c_str());


	CLuaScriptReader gReader;
	gReader.InitLuaScriptReader();
	char * temp = new char[output.length() + 1];
	strcpy(temp,output.c_str());
	gReader.GetWriteZiInfo(temp);			//手写字坐标
	delete [] temp;
	char * retStr = new char[50];
	gReader.GetZiName(hanzi);
	gReader.SetRulesFunc(rulespath.c_str());
	gReader.RunScriptFile(filepath.c_str(),"WriteZiInfo.lua");
	gReader.SetGlobalFunc(Globalpath.c_str());
	gReader.RunMixedFile(basepath.c_str(),"BaseLib.lua");
	gReader.RunScriptFile(apipath.c_str(),retStr,"RunAPI.lua");
	gReader.ExitLuaScriptReader();		//退出Lua_State

	int t = atoi(retStr);
	if (1 == t)
	{
		CCLog("suitable to lua ");
		my_strokenode->runAction(CCSequence::create(mb,mt,NULL));

		//当前笔画往后+1
		CCLog("current_writing_stroke %d",current_writing_stroke);
		current_writing_stroke++;
		if (current_writing_stroke <= charac.getStrokeCount())
		{
			strokeNodes[current_writing_stroke-1]->setVisible(true);
		}
	}else{
		CCLog("current_writing_stroke %d",current_writing_stroke);
	}



}


void lianxi::keyBackClicked(){

// 	CCDirector::sharedDirector()->replaceScene(WallScene::scene());
	CCDirector::sharedDirector()->popScene();
}

void lianxi::menuBack(CCObject* pSender){
// 	CCDirector::sharedDirector()->replaceScene(WallScene::scene());
	CCDirector::sharedDirector()->popScene();
}

// void lianxi::draw(){
// 
// }

string lianxi::convertToString(float f){
	ostringstream buff;
	buff<<f;
	return buff.str();
}

void lianxi::clearStroke(float dt){
	for (vector<CCDrawNode*>::const_iterator it = writing_stroke.begin(); it != writing_stroke.end(); ++it )
	{
		removeChild(*it);
	}
	writing_stroke.clear();
	
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(lianxi::clearStroke),this);

}

void lianxi::DoAnimation(){
	//run action
	Stroke stro = newCharac.getStroke(1);
	vector<CCDrawNode*> nList = stro.nodeList;
	for (vector<CCDrawNode*>::iterator it = nList.begin(); it != nList.end() ;++it)
	{
		(*it)->setVisible(true);
		CCMoveBy* actionBy = CCMoveBy::create(2,ccp(50,50));
		(*it)->runAction(actionBy);
	}
}

CCPoint lianxi::convert512(CCPoint p){
	float tx = p.x;
	float ty = p.y;
	CCSize tianzigeSize = tianzige_draw->getContentSize();

	float fx = tx * 512 / tianzigeSize.width;
	float fy = ty * 512 / tianzigeSize.height;
	return ccp(fx,-(fy-512));
}

