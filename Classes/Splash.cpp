#include "Splash.h"
#include "WallScene.h"
#include "MainScene.h"

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

    CCSprite* bg = CCSprite::create("splash.png");
    CCSize bgSize = bg->getContentSize();
    bg->setPosition(ccp(winSize.width/2,winSize.height/2));
    this->addChild(bg,0);
    bg->setScaleX(winSize.width/bgSize.width);
    bg->setScaleY(winSize.height/bgSize.height);

    //add start Button
//    CCSprite* startButton = CCSprite::create("Button1.png");
//    startButton->setPosition(ccp(winSize.width/2,startButton->getContentSize().height/2+100));
//    this->addChild(startButton);

//    string setting = CCFileUtils::sharedFileUtils()->getWritablePath()+"setting.xml";
//    CCLog("setting %s",setting.c_str());
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	string xmlpath1 = CCFileUtils::sharedFileUtils()->getWritablePath()+"wall.xml";
    CCLog("isexit %d",CCFileUtils::sharedFileUtils()->isFileExist(xmlpath1));
	if(!CCFileUtils::sharedFileUtils()->isFileExist(xmlpath1)){
        string xmlpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("wall.xml");
    	unsigned long int size = 0 ;
		char* pFile = (char*)CCFileUtils::sharedFileUtils()->getFileData(xmlpath.c_str(),"rb",&size);
		xmlpath = CCFileUtils::sharedFileUtils()->getWritablePath()+"wall.xml";
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


		string dbpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("character_info.db");

		char* pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(dbpath.c_str(),"rb",&size);
		CCLog("file %s",pFileContent);
		dbpath = CCFileUtils::sharedFileUtils()->getWritablePath()+"character_info.db";
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

    }
#endif


    return true;
}

void Splash::onEnter(){
    CCLayer::onEnter();
    CCLog("onEnter");
    this->scheduleOnce(schedule_selector(Splash::finishSplash),0.1f);
}

void Splash::onExit(){
    CCLayer::onExit();
    CCLog("onExit");
}

void Splash::onExitTransitionDidStart(){
    CCLayer::onExitTransitionDidStart();
    CCLog("onExitTransitionDidStart");
}

void Splash::finishSplash(float dt){
    
    CCDirector::sharedDirector()->replaceScene(MainScene::scene());
}
