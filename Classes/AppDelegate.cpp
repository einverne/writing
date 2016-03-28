#include "AppDelegate.h"
#include "Splash.h"
#include "WallScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
    CCLog("AppDelegate()");
}

AppDelegate::~AppDelegate() 
{
    CCLog("AppDelegate() ~()");
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    CCSize screenSize = pEGLView->getFrameSize();
    CCSize designSize = CCSizeMake(720, 1280);
    CCLog("screen size: %f %f",screenSize.width, screenSize.height);
    
    pDirector->setOpenGLView(pEGLView);
    CCSize winSize = pDirector->getWinSize();
    CCLog("screen size: %f %f",winSize.width, winSize.height);
    pEGLView->setDesignResolutionSize(designSize.width,designSize.height,kResolutionExactFit);
    
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    // set search path
//    int i = 0;
//    CCFileUtils* fileUtils = CCFileUtils::sharedFileUtils();
//    vector<string> searchPaths = fileUtils->getSearchPaths();
//    searchPaths.insert(searchPaths.begin()+ i++, "lua_ext");
//    fileUtils->setSearchPaths(searchPaths);

    // create a scene. it's an autorelease object
    CCScene* pScene = Splash::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
// 当应用程序不再活动时，会调用此方法。当手机接到电话时，它也会被调用
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
// 当应用程序重新活动时，会调用此方法
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
