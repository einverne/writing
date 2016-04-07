#include "TcharacterDrawnode.h"
#include "ReadXML.h"
#include "SqliteHelper.h"
#include "CharacterEntity.h"
#include "SQLiteData.h"

TcharacterDrawnode::TcharacterDrawnode():strokedrawList(NULL),
	SegmentNodeList(NULL),
	visibleIndex(-1),tiangzige(NULL)
{
	
}

TcharacterDrawnode::~TcharacterDrawnode()
{
	CC_SAFE_RELEASE(strokedrawList);
	CC_SAFE_RELEASE(SegmentNodeList);
}

/**
* 
* @param hz 汉字
* @param showrect 显示区域
* @param p 包含数据库中读取的汉字信息XML等等
* @return 初始化是否成功
*/
bool TcharacterDrawnode::init(string hz,CCSize showrect,CharacterEntity* p){

	tiangzige = CCSprite::create("tianzige.png");
	addChild(tiangzige,10);
	tiangzige->setAnchorPoint(ccp(0,0));
	setContentSize(tiangzige->getContentSize());

	strokedrawList = CCArray::create();
	strokedrawList->retain();

	CReadXML readxml(p->getXML().c_str());
	this->m_right_character_ = readxml.getCharacter();
	this->showrect_ = showrect;

	this->m_right_character_.getBox();
	this->m_right_character_.resize(showrect_);
	this->m_right_character_.resample();

	vector<Bujian> bujianList = this->m_right_character_.bujianList;
	vector<Bujian>::iterator iter ;
	for (iter = bujianList.begin(); iter != bujianList.end() ; ++ iter)
	{
		Bujian bujian = (Bujian)*iter;
		vector<Stroke> strokeList = bujian.strokeList;
		for (unsigned int i = 0 ; i < strokeList.size() ; i++)
		{
			this->getstrokedrawList()->addObject(StrokeDrawnode::create(strokeList.at(i)));
		}
	}
	return true;
}

bool TcharacterDrawnode::init(string hz,CCSize showrect,CharacterExtend* p){
	tiangzige = CCSprite::create("tianzige.png");
	addChild(tiangzige,-1);
	tiangzige->setAnchorPoint(ccp(0,0));
	setContentSize(tiangzige->getContentSize());

	strokedrawList = CCArray::create();
	strokedrawList->retain();

	SegmentNodeList = CCArray::create();
	SegmentNodeList->retain();

	// be carefule readxml has two construct functions, one pass a char* and one pass a string file name
	CReadXML readxml(p->getXML().c_str());
	m_right_character_ = readxml.getCharacter();
	m_origin_character_ = readxml.getCharacter();
	showrect_ = showrect;

	// new edit ============
	// 利用 Template Character 重画 
	
	template_character_ = readxml.getTemplateCharacterFromBuffer(p->getXML(),p->getMark());
	template_character_.Normalize(showrect.height, showrect.width);
	template_character_.TransferCoordinate(showrect.height);

	for (list<Stroke>::iterator iter = template_character_.stroke_list_.begin();
		iter != template_character_.stroke_list_.end(); iter++)
	{
		Stroke stroke = *iter;
		this->getstrokedrawList()->addObject(StrokeDrawnode::create(stroke));
	}

	for (list<Segment>::iterator iter = template_character_.segment_list_.begin();
		iter != template_character_.segment_list_.end(); iter++)
	{
		Segment segment = *iter;
		CCLog("Segment name %s",segment.name_.c_str());
		this->getSegmentNodeList()->addObject(SegmentDrawnode::create(segment));
	}


	
	// ==================
// 	this->m_right_character_.getBox();
// 	this->m_right_character_.resize(showrect);
// 	this->m_right_character_.resample();
// 
// 	vector<Bujian> bujianList = this->m_right_character_.bujianList;
// 	vector<Bujian>::iterator iter;
// 	for (iter  = bujianList.begin(); iter != bujianList.end() ; ++ iter)
// 	{
// 		Bujian bujian = (Bujian)*iter;
// 		vector<Stroke> strokeList = bujian.strokeList;
// 		for (int i = 0 ; i < strokeList.size() ; i++)
// 		{
// 			this->getstrokedrawList()->addObject(StrokeDrawnode::create(strokeList.at(i)));
// 		}
// 	}
	return true;
}

TcharacterDrawnode* TcharacterDrawnode::create(string hz,CCSize showrect,CharacterEntity* p){
	TcharacterDrawnode* pRet = new TcharacterDrawnode();
	if (pRet && pRet->init(hz,showrect,p))
	{
		pRet->autorelease();
		return pRet;
	}else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

TcharacterDrawnode* TcharacterDrawnode::create(string hz,CCSize showrect,CharacterExtend* p){
	TcharacterDrawnode* pRet = new TcharacterDrawnode();
	if (pRet && pRet->init(hz,showrect,p))
	{
		pRet->autorelease();
		return pRet;
	}else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

void TcharacterDrawnode::draw(){

	CCLog("TcharDrawnode draw~~~~~~~~~~~");
	
	ccColor4F color = ccc4f(1,0,0,1);
	glLineWidth(8);					//笔画粗细
	ccDrawColor4F(color.r, color.g, color.b, color.a);                     //笔画颜色
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//ccDrawLine(ccp(100,100),ccp(200,200));


// 	CCObject* ob;
// 	CCARRAY_FOREACH(strokedrawList,ob){
// 		StrokeDrawnode* node = (StrokeDrawnode*)ob;
// 		node->color = ccc4f(1,0,0,1);
// 		node->draw();
// 	}


	CCObject* segment_ob;
	CCARRAY_FOREACH(SegmentNodeList, segment_ob){
		SegmentDrawnode* node = (SegmentDrawnode*)segment_ob;
		node->draw();
	}



}

int TcharacterDrawnode::getPointsCount(){
	CCObject* ob;
	int count = 0;
	CCARRAY_FOREACH(strokedrawList,ob){
		count += ((StrokeDrawnode*)(ob))->getStroke().getPointsCount();
	}
	return count;
}

void TcharacterDrawnode::setVisibleIndex(int vi){
	if (vi >= getstrokedrawList()->count())
	{
		this->visibleIndex = getstrokedrawList()->count();
	}else{
		this->visibleIndex = vi;
	}
}

string TcharacterDrawnode::getCharacterStandardInfo() const{
    return m_origin_character_.getCharacterStandardInfo();
}