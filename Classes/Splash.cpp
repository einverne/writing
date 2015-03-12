#include "Splash.h"
#include "WallScene.h"
#include "MainScene.h"
#include "tools/DataTool.h"

USING_NS_CC;
Splash::Splash(void)
{
}


Splash::~Splash(void)
{
}

CCScene* Splash::scene(){
    CCScene* scene = CCScene::create();
    Splash* layer = Splash::create();
    scene->addChild(layer);
    return scene;
}

bool Splash::init(){
    if (!CCLayer::init())
    {
		return false;
    }

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    CCSprite* bg = CCSprite::create("strangedesign\\splash.jpg");
    CCSize bgSize = bg->getContentSize();
    bg->setPosition(ccp(winSize.width/2,winSize.height/2));
    this->addChild(bg,0);
    bg->setScaleX(winSize.width/bgSize.width);
    bg->setScaleY(winSize.height/bgSize.height);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    for(int i = 1; i<=20; i++){
//    	string str_i = DataTool::intTostring(i);
//    	initXML("wall_"+str_i+".xml");
//    }
    string dbpat = CCFileUtils::sharedFileUtils()->getWritablePath()+"character_info.db";
	if (!CCFileUtils::sharedFileUtils()->isFileExist(dbpat))
	{
		initDB("character_info.db");
		initDB("character_judge.db");
	}
	string settingxml = CCFileUtils::sharedFileUtils()->getWritablePath()+"setting.xml";
	if (!CCFileUtils::sharedFileUtils()->isFileExist(settingxml))
	{
		CCLog("first time init setting.xml");
		DataTool::storeToFile("1","setting.xml");
	}
#endif
    return true;
}

void Splash::initXML(string xmlfile){
	string xmlpath1 = CCFileUtils::sharedFileUtils()->getWritablePath()+xmlfile;
    CCLog("isexit %d",CCFileUtils::sharedFileUtils()->isFileExist(xmlpath1));
	if(!CCFileUtils::sharedFileUtils()->isFileExist(xmlpath1)){
		string s = "wall/";
		string fullpath = s+xmlfile;
        string xmlpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fullpath.c_str());
    	unsigned long int size = 0 ;
		char* pFile = (char*)CCFileUtils::sharedFileUtils()->getFileData(xmlpath.c_str(),"rb",&size);
		xmlpath = CCFileUtils::sharedFileUtils()->getWritablePath()+xmlfile;
		CCLog("xmlpath %s",xmlpath.c_str());

		FILE* file = fopen(xmlpath.c_str(),"w");
		if (file != NULL) {
			CCLog("XMLFile not NULL");
			file = fopen(xmlpath.c_str(),"wb");
			fwrite(pFile,size,1,file);
			CC_SAFE_DELETE_ARRAY(pFile);
		}else {
			CCLog("XMLFile NULL");
		}
		fclose(file);


    }
}

void Splash::initDB(string db_name){
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	string dbpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(db_name.c_str());
	unsigned long int size = 0 ;
	char* pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(dbpath.c_str(),"rb",&size);
	CCLog("file %s",pFileContent);
	dbpath = CCFileUtils::sharedFileUtils()->getWritablePath()+db_name;
	FILE* file1 = fopen(dbpath.c_str(),"w");
	if (file1 != NULL)
	{
		CCLog("file not NULL");
		file1 = fopen(dbpath.c_str(),"wb");
		fwrite(pFileContent,size,1,file1);
		CC_SAFE_DELETE_ARRAY(pFileContent);
	}else{
		CCLog("file NULL");
	}
	fclose(file1);
#endif
}

void Splash::onEnter(){
    CCLayer::onEnter();
    this->scheduleOnce(schedule_selector(Splash::finishSplash),0.8f);
}

void Splash::onExit(){
    CCLayer::onExit();
}

void Splash::onExitTransitionDidStart(){
    CCLayer::onExitTransitionDidStart();
    CCLog("onExitTransitionDidStart");
}

void Splash::finishSplash(float dt){
    
    CCDirector::sharedDirector()->replaceScene(MainScene::scene());
}
