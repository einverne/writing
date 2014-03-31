#include "TcharacterDrawnode.h"
#include "ReadXML.h"

TcharacterDrawnode::TcharacterDrawnode():strokedrawList(NULL)
{
	
}

TcharacterDrawnode::~TcharacterDrawnode()
{
// 	for (int i = 0 ; i < strokedrawList.size(); i++)
// 	{
// 		StrokeDrawnode* temp = strokedrawList.back();
// 		strokedrawList.pop_back();
// 		temp->release();
// 	}
	CCLog("TcharacterDrawnode ref  %d",this->m_uReference);
	CC_SAFE_RELEASE(strokedrawList);
}

bool TcharacterDrawnode::init(string hz,CCSize showrect){
	strokedrawList = CCArray::create();
	strokedrawList->retain();
	string xml = CCFileUtils::sharedFileUtils()->fullPathForFilename("xml/°Ë.xml");
	CReadXML readxml(xml);
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
			// 			strokeDrawnode->retain();
		}
	}
	return true;
}

TcharacterDrawnode* TcharacterDrawnode::create(string hz,CCSize showrect){
	TcharacterDrawnode* pRet = new TcharacterDrawnode();
	if (pRet && pRet->init(hz,showrect))
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
	//CCLog("TcharacterDrawnode::draw");
// 	vector<StrokeDrawnode*>::iterator iter = strokedrawList.begin();
// 	for (iter ; iter != strokedrawList.end(); ++iter)
// 	{
// 		StrokeDrawnode* no = (StrokeDrawnode*)*iter;
// 		no->draw();
// 	}
	CCObject* ob;
// 	strokedrawList->retain();
	CCARRAY_FOREACH(strokedrawList,ob){
		((StrokeDrawnode*)(ob))->draw();
	}
}