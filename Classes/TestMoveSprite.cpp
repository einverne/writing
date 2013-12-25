#include "TestMoveSprite.h"
#include "HelloWorldScene.h"
#include "PopLayer.h"


#define COCOS2D_DEBUG 1
USING_NS_CC;
TestMoveSprite::TestMoveSprite(void)
{
	is_selected = false;
}


TestMoveSprite::~TestMoveSprite(void)
{
}

bool TestMoveSprite::init(){
	if (CCLayerColor::initWithColor( ccc4(255,255,255,255) ))
	{
		this->setTouchEnabled(true);
		this->setKeypadEnabled(true);
		// add a dog sprite
		CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint ptCenter = CCPointMake(winSize.width/2,winSize.height/2);
		CCSprite* dog = CCSprite::create("dog.png");
		dog->setPosition(ptCenter);
		this->addChild(dog,0,1000);
		CCMoveTo* moveTo = CCMoveTo::create(2,CCPointMake(300,500));
		dog->runAction(moveTo);

		// 1.生成动画需要数据类
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("action_people.png");
		CCSpriteFrame* frame0 = CCSpriteFrame::createWithTexture(texture , CCRectMake(32*0 , 48*0 , 32 , 48));
		CCSpriteFrame* frame1 = CCSpriteFrame::createWithTexture(texture , CCRectMake(32*1 , 48*0 , 32 , 48));
		CCSpriteFrame* frame2 = CCSpriteFrame::createWithTexture(texture , CCRectMake(32*2 , 48*0 , 32 , 48));
		CCSpriteFrame* frame3 = CCSpriteFrame::createWithTexture(texture , CCRectMake(32*3 , 48*0 , 32 , 48));

		CCArray* animFrames = CCArray::create();
		animFrames->addObject(frame0);
		animFrames->addObject(frame1);
		animFrames->addObject(frame2);
		animFrames->addObject(frame3);

		CCAnimation* animation =CCAnimation::createWithSpriteFrames(animFrames, 0.2f );
		animFrames->release();

		// 2.初始化并设置Sprite
		CCSprite* sprite = CCSprite::createWithSpriteFrame(frame0);
		sprite->setPosition( ccp( sprite->getContentSize().width+50 , winSize.height/2 ) );
		addChild(sprite);

		// 2.使用animation生成一个动画动作animation
		CCAnimate* animate = CCAnimate::create(animation);
		sprite->runAction(CCRepeatForever::create(animate));

		return true;
	}
	return false;
}

CCScene* TestMoveSprite::scene(){
	CCScene* scene = CCScene::create();
	TestMoveSprite * layer = TestMoveSprite::create();
	scene->addChild(layer);
	return scene;
}

void TestMoveSprite::menuCloseCallback(CCObject* pSender){

}

void TestMoveSprite::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	popup();
	if (pTouches->count()>=1)
	{
		CCTouch *touch_position = (CCTouch*)pTouches->anyObject();
		CCPoint pt = touch_position->getLocation();
		CCNode* pNode = this->getChildByTag(1000);
		if (pNode->boundingBox().containsPoint(pt))
		{
			is_selected = true;
		}else{
			is_selected = false;
		}

	}

}

void TestMoveSprite::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	if (is_selected)
	{
		CCPoint pt_diff = ((CCTouch*)pTouches->anyObject())->getDelta();
		CCNode* pNode = this->getChildByTag(1000);
		CCPoint pt_Current = pNode->getPosition();
		pNode->setPosition(ccpAdd(pt_Current,pt_diff));

	}
}

void TestMoveSprite::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	is_selected = false;
}

// void TestMoveSprite::onEnter(){
//     CCLog("TestMoveSprite onEnter .. ");
//     CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);
// }
// 
// void TestMoveSprite::onExit(){
//     CCLog("TestMoveSprite onExit ... ");
//     CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
// }

// bool TestMoveSprite::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//     CCLog("TestMoveSprite Touch Began ... ");
//     CCPoint touchPoint = pTouch->getLocationInView();

//     touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

// 
//     return true;
// }
// void TestMoveSprite::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//     //CCLog("Touch Moved ... ");
// 
// }
// void TestMoveSprite::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

//     CCLog("TestMoveSprite Touch Ended ... ");

// }


void TestMoveSprite::keyBackClicked(){
	CCLog("TestMoveSprite key back clicked~");
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}
void TestMoveSprite::popup(){
// 	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
// 	PopLayer* popL = PopLayer::create("pop/background.png");
// 	popL->setContentSize(CCSizeMake(400,400));
// 	popL->setTitle("test");
// 	popL->setCallBackFunc(this,callfuncN_selector(TestMoveSprite::buttonCallback));
// 	popL->addButton("Button1.png","Button1.png","Y",0);
// 	popL->addButton("Button2.png","Button2.png","N",1);
// 	this->addChild(popL,100);
}

void TestMoveSprite::buttonCallback(CCNode* pSender){
	CCLog("button call back. tag: %d", pSender->getTag());
}