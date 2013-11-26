#include "SecondScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace std;
SecondScene::SecondScene(void)
{
}


SecondScene::~SecondScene(void)
{
}

bool SecondScene::init(){
    if (CCLayerColor::initWithColor( ccc4(1,1,1,255) ) )
    {
	this->setTouchEnabled(true);	    //设置可触摸
	this->setKeypadEnabled(true);	    //设置
	CCLabelTTF* pLabel = CCLabelTTF::create("Draw","Arial",24);

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	pLabel->setPosition(ccp(pLabel->getContentSize().width/2+10,size.height-pLabel->getContentSize().height/2 - 10));
	this->addChild(pLabel);

	return true;
    }
    return false;
}

CCScene * SecondScene::scene(){
    CCScene * scene = CCScene::create();
    SecondScene * layer = SecondScene::create();

    scene->addChild(layer);
    return scene;
}

/*
bool SecondScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint location = pTouch->getLocation();
    CCDrawNode * dot = CCDrawNode::create();
    dot->drawDot(location,5.0,ccc4f(1,1,1, 1));

    this->addChild(dot );

    
    return true;
}

void SecondScene::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint location = pTouch->getLocation();
    CCDrawNode * dot = CCDrawNode::create();
    

    dot->drawDot(location,5.0,ccc4f(188, 188, 188, 1));

    this->addChild(dot );
}
*/


void SecondScene::ccTouchesBegan(cocos2d::CCSet* pTouches,cocos2d::CCEvent* pEvent){
    // 从触摸点的集合中选取一个点
    if (!pTouches || !pTouches->count())
    {
	return;
    }
    stroke = "[[";
    //CCTouch* touch = (CCTouch*)( pTouches->anyObject() );
    for (CCSetIterator iterTouch = pTouches->begin(); iterTouch!= pTouches->end(); iterTouch++)
    {
	CCTouch *touch = (CCTouch * )(*iterTouch);
	CCPoint location = touch->getLocation();
	location = CCDirector::sharedDirector()->convertToGL(location);
	
	CCDrawNode * dot = CCDrawNode::create();
	dot->drawDot(location,5.0,ccc4f(188, 188, 188, 1));

	this->addChild(dot );
    }
}

void SecondScene::ccTouchesMoved(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent){
    //     if (pTouches->count() > 1)
    //     {
    // 	CCTouch* touch = (CCTouch*) (pTouches->anyObject() );
    // 	CCPoint positon = touch->getLocation();
    // 	CCDrawNode * dot = CCDrawNode::create();
    // 	dot->drawDot(positon , 5.0 , ccc4f(1,1,255,188));
    //     }
    //     if (!pTouches || !pTouches->count())
    //     {
    // 	return;
    //     }
    if (pTouches->count() == 1)
    {
	//single touch 
	//CCTouch* touch = (CCTouch*)( pTouches->anyObject() );
	for (CCSetIterator iterTouch = pTouches->begin(); iterTouch!= pTouches->end(); iterTouch++)
	{
	    CCTouch *touch = (CCTouch * )(*iterTouch);
	    CCPoint location = touch->getLocation();

	    
	    stroke += location.x ;
	    stroke += "/";
	    stroke += location.y ;
	    stroke += "/";
	    CCDrawNode * dot = CCDrawNode::create();
	    dot->drawDot(location,5.0,ccc4f(188, 188, 188, 1));

	    this->addChild(dot );
	}
    }else
    {

    }

}

void SecondScene::ccTouchesEnded(cocos2d::CCSet *pTouches,cocos2d::CCEvent* pEvent){

    stroke += "]]";
    CCLog(stroke.c_str());
    // 从触摸点的集合中选取一个点
    //     CCTouch* touch = (CCTouch*)( pTouches->anyObject() );
    //     CCPoint location = touch->getLocation();
    // 
    //     CCLog("XXX");
    //     CCDrawNode * dot = CCDrawNode::create();
    //     dot->drawDot(location,5.0,ccc4f(155,155, 155, 200));
    // 
    //     this->addChild(dot );
}

void SecondScene::keyBackClicked(){
    CCLog("SecondScene key back clicked~");
    CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}