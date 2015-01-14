#include "TcharacterDrawnode.h"
#include "ReadXML.h"
#include "SqliteHelper.h"
#include "CharacterEntity.h"
#include "SQLiteData.h"

TcharacterDrawnode::TcharacterDrawnode():strokedrawList(NULL),
	visibleIndex(-1)
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

	strokedrawList = CCArray::create();
	strokedrawList->retain();

	CReadXML readxml(p->getXML()->getCString());
	this->m_character = readxml.getCharacter();
	this->showRect = showrect;

	this->m_character.getBox();
	this->m_character.resize(showRect);
	this->m_character.resample();

	vector<Bujian> bujianList = this->m_character.bujianList;
	vector<Bujian>::iterator iter = bujianList.begin();
	for (iter ; iter != bujianList.end() ; ++ iter)
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

/**
* 
* @param hz
* @param showrect
* @param p
* @return
*/
bool TcharacterDrawnode::init(string hz,CCSize showrect,CharacterExtend* p){

	strokedrawList = CCArray::create();
	strokedrawList->retain();

	CReadXML readxml(p->getXML()->getCString());
	this->m_character = readxml.getCharacter();
	myChar = readxml.getCharacter();
	this->showRect = showrect;

	this->m_character.getBox();
	this->m_character.resize(CCSizeMake(500,500));
	this->m_character.resample();

	vector<Bujian> bujianList = this->m_character.bujianList;
	vector<Bujian>::iterator iter = bujianList.begin();
	for (iter ; iter != bujianList.end() ; ++ iter)
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
		((StrokeDrawnode*)(ob))->draw();
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

string TcharacterDrawnode::getCharacterStandardInfo(){
	string ret = "";
	int cout = myChar.getStrokeCount();
	for (int i = 1 ; i <= cout ; i++)
	{
		Stroke stroke = myChar.getStroke(i);
		ret += stroke.sendOutputWithStatus();
	}
	return ret;
}