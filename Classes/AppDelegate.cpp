#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "Splash.h"
#include "GameWall.h"
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

    pDirector->setOpenGLView(pEGLView);
    pEGLView->setDesignResolutionSize(720,1280,kResolutionExactFit);

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    //CCScene *pScene = HelloWorld::scene();
//     CCScene* pScene = Splash::scene();
    CCScene* pScene = WallScene::scene();

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
