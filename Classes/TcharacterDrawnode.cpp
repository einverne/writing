#include "TcharacterDrawnode.h"
#include "ReadXML.h"

TcharacterDrawnode::TcharacterDrawnode(string hz,CCSize showrect)
{
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
			StrokeDrawnode* strokeDrawnode = new StrokeDrawnode(strokeList.at(i));
			this->strokedrawList.push_back(strokeDrawnode);
		}
	}
}

TcharacterDrawnode::~TcharacterDrawnode()
{
}

bool TcharacterDrawnode::init(){
	

	return true;
}

TcharacterDrawnode* TcharacterDrawnode::create(string hz,CCSize showrect){
	TcharacterDrawnode* pRet = new TcharacterDrawnode(hz,showrect);
	if (pRet && pRet->init())
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
	vector<StrokeDrawnode*>::iterator iter = strokedrawList.begin();
	for (iter ; iter != strokedrawList.end(); ++iter)
	{
		StrokeDrawnode* no = (StrokeDrawnode*)*iter;
		no->draw();
	}
}