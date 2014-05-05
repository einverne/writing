#include "TcharacterDrawnode.h"
#include "ReadXML.h"
#include "SqliteHelper.h"
#include "CharacterEntity.h"

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
// 	CCLog("TcharacterDrawnode ref  %d",this->m_uReference);
	CC_SAFE_RELEASE(strokedrawList);
}

bool TcharacterDrawnode::init(string hz,CCSize showrect){
// 	CCLog("TcharacterDrawnode init()");
	string dbpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("test.db");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	unsigned long int size = 0;
	char* pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(dbpath.c_str(),"rb",&size);
	CCLog("file %s",pFileContent);
	dbpath = CCFileUtils::sharedFileUtils()->getWritablePath()+"test.db";
	FILE* file = fopen(dbpath.c_str(),"w");
	if (file != NULL)
	{
		CCLog("file not NULL");
		file = fopen(dbpath.c_str(),"wb");
		fwrite(pFileContent,size,1,file);
		CC_SAFE_DELETE_ARRAY(pFileContent);
	}else{
		CCLog("file NULL");
	}
	fclose(file);
#endif
	SqliteHelper::initDB(dbpath.c_str());
// 	string createsql = "create table user (id integer,username test,password text,word text)";
// 	SqliteHelper::createTable(createsql,"user");

// 	string insertsql = "insert into table_test values(4,'"+UTF8ToGBK::UTF8TOGBK("บร")+"')";
// 	SqliteHelper::insertData(insertsql);

	string sql = "select * from table_test where name='"+hz+"'";
	CharacterEntity* p = new CharacterEntity();
	SqliteHelper::getDataInfo(sql,p);
	SqliteHelper::closeDB();
	strokedrawList = CCArray::create();
	strokedrawList->retain();
// 	stringstream ss;
// 	ss << p->getID()->getValue();
// 	string filepath = "lua/ZiList/"+ss.str()+"/xml.xml";
// 	string xml = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
// 	string xml(p->getXML()->getCString());
/*	CReadXML readxml(xml);*/
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
	CCARRAY_FOREACH(strokedrawList,ob){
		((StrokeDrawnode*)(ob))->draw();
	}
}
