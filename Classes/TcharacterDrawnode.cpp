#include "TcharacterDrawnode.h"
#include "ReadXML.h"
#include "SqliteHelper.h"
#include "CharacterEntity.h"
#include "SQLiteData.h"

TcharacterDrawnode::TcharacterDrawnode():strokedrawList(NULL),
	visibleIndex(-1),tiangzige(NULL)
{
	
}

TcharacterDrawnode::~TcharacterDrawnode()
{
	CC_SAFE_RELEASE(strokedrawList);
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
	this->m_right_character = readxml.getCharacter();
	this->showRect = showrect;

	this->m_right_character.getBox();
	this->m_right_character.resize(showRect);
	this->m_right_character.resample();

	vector<Bujian> bujianList = this->m_right_character.bujianList;
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

	// be carefule readxml has two construct functions, one pass a char* and one pass a string file name
	CReadXML readxml(p->getXML().c_str());
	m_right_character = readxml.getCharacter();
	m_char = readxml.getCharacter();
	showRect = showrect;

	// new edit ============
	/*
	TemplateCharacter tc = readxml.getTemplateCharacterFromBuffer(p->getXML(),p->getMark());
	tc.normalize(showrect.height, showrect.width);

	for (list<Stroke>::iterator iter = tc.stroke_list.begin();
		iter != tc.stroke_list.end(); iter++)
	{
		this->getstrokedrawList()->addObject(StrokeDrawnode::create((Stroke)*iter));
	}
	*/
	// ==================
	this->m_right_character.getBox();
	this->m_right_character.resize(showrect);
	this->m_right_character.resample();

	vector<Bujian> bujianList = this->m_right_character.bujianList;
	vector<Bujian>::iterator iter;
	for (iter  = bujianList.begin(); iter != bujianList.end() ; ++ iter)
	{
		Bujian bujian = (Bujian)*iter;
		vector<Stroke> strokeList = bujian.strokeList;
		for (int i = 0 ; i < strokeList.size() ; i++)
		{
			this->getstrokedrawList()->addObject(StrokeDrawnode::create(strokeList.at(i)));
		}
	}
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
	
	CCObject* ob;
	CCARRAY_FOREACH(strokedrawList,ob){
		StrokeDrawnode* node = (StrokeDrawnode*)ob;
		node->color = ccc4f(1,0,0,1);
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
    return m_char.getCharacterStandardInfo();
}