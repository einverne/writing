#include "Splash.h"
#include "WallScene.h"

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

    string xmlpath = CCFileUtils::sharedFileUtils()->fullPathForFilename("wall.xml");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    unsigned long int size = 0 ;
    char* pFile = (char*)CCFileUtils::sharedFileUtils()->getFileData(xmlpath.c_str(),"rb",&size);
	xmlpath = CCFileUtils::sharedFileUtils()->getWritablePath()+"wall.xml";
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
#endif
    return true;
}

void Splash::onEnter(){
    CCLayer::onEnter();
    CCLog("onEnter");
    this->scheduleOnce(schedule_selector(Splash::finishSplash),2.0f);
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
    
    CCDirector::sharedDirector()->replaceScene(WallScene::scene());
}
