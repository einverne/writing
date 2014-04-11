
#include "GameScene.h"
#include "GameOverScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;
GameScene::GameScene():_targets(NULL),_projectiles(NULL),_projectilesDestroyed(0)
{
    _targets = new CCArray();
    _projectiles = new CCArray();
}

GameScene::~GameScene(void)
{
    CC_SAFE_RELEASE_NULL(_targets);
    CC_SAFE_RELEASE_NULL(_projectiles);
}

CCScene* GameScene::scene()
{
    CCScene * scene = CCScene::create();
    GameScene * layer = GameScene::create();

    scene->addChild(layer);
    return scene;
}

bool GameScene::init(){
    if( CCLayerColor::initWithColor(ccc4(255,255,255,255)) ){
	this->setTouchEnabled(true);
	this->schedule( schedule_selector(GameScene::gameLogic ) , 1.0);
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite * player = CCSprite::create( "Player.png" , CCRectMake(0,0,27,40) );

	player->setPosition( ccp(player->getContentSize().width/2 , winSize.height/2 ) );
	this->addChild(player);

	this->scheduleUpdate();
	return true;
    }
    return false;
}

void GameScene::addTarget(){
    CCSprite * target = CCSprite::create(  "Target.png" , CCRectMake(0,0,27,40) );

    //决定怪物出现的XY坐标
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    int minY = target->getContentSize().height/2;
    int maxY = winSize.height - target->getContentSize().height/2;
    int rangeY = maxY-minY;
    int actualY = (rand() % rangeY)+minY;

    // 创建怪物的时候让它稍微出一些屏幕
    // 怪物出现位置的Y坐标是随机的
    target->setPosition( 
	ccp(winSize.width + (target->getContentSize().width/2), 
	actualY) );
    this->addChild(target);

    // 通过设定怪物运动所需时间来决定他的速度
    int minDuration = (int)2.0;
    int maxDuration = (int)4.0;
    int rangeDuration = maxDuration - minDuration;
    // srand( TimGetTicks() );
    int actualDuration = ( rand() % rangeDuration )  + minDuration;

    // 创建action
    CCFiniteTimeAction* actionMove = 
	CCMoveTo::create( (float)actualDuration, 
	ccp(0 - target->getContentSize().width/2, actualY) );
    CCFiniteTimeAction* actionMoveDone =   CCCallFuncN::create( this, callfuncN_selector(GameScene::spriteMoveFinished));
    target->runAction( CCSequence::create(actionMove, 
	actionMoveDone, NULL) );

    target->setTag(1);
    _targets->addObject(target);

}


void GameScene::spriteMoveFinished(CCNode* sender)
{
    CCSprite *sprite = (CCSprite *)sender;
    this->removeChild(sprite, true);
    // 从 tag 判断类型从对应的数组中移除
    if (sprite->getTag() == 1)  
    {
	_targets->removeObject(sprite);
	GameOverScene *gameOverScene = GameOverScene::create();
	gameOverScene->getLayer()->getLabel()->setString("You Lose :[");
	CCDirector::sharedDirector()->replaceScene(gameOverScene);    
    }
    else if (sprite->getTag() == 2) 
    {
	_projectiles->removeObject(sprite);
    }
}

void GameScene::gameLogic(float dt)
{
    this->addTarget();
}

void GameScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    CCLog("Game Scene TouchedEnded ... ");
    // 从触摸点的集合中选取一个点
    
    CCTouch* touch = (CCTouch*)( touches->anyObject() );
    CCPoint location = touch->getLocation();

    // 初始化你的飞镖并且设定一个起点
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite *projectile = CCSprite::create("Projectile.png", CCRectMake(0, 0, 20, 20));
    projectile->setPosition( ccp(20, winSize.height/2) );

    // 计算以下飞镖起点相对于我们触摸点的X、Y坐标的偏移量
    int offX = location.x - projectile->getPosition().x;
    int offY = location.y - projectile->getPosition().y;

    // 我们要往有妖怪的地方打……所以往后打飞镖我们不放出来
    /* 不用担心这里return了之前create的飞镖没有释放会造成
    内存泄漏，使用create方法产生的对象都是自动释放的，
    除非你曾经手动的调用了它的 retain() 方法。*/
    if (offX <= 0) return;

    // 既然它朝着妖怪打，那么就把它加到游戏层里去
    this->addChild(projectile);

    // 计算一下飞镖最终的目的地，我们希望它飞出屏幕再消失
    int realX = winSize.width
	+ (projectile->getContentSize().width/2);
    float ratio = (float)offY / (float)offX;
    int realY = (realX * ratio) + projectile->getPosition().y;
    CCPoint realDest = ccp(realX, realY);

    // 计算一下飞镖要飞多远，因为速度是相同的，要来计算它的时间。
    int offRealX = realX - projectile->getPosition().x;
    int offRealY = realY - projectile->getPosition().y;
    float length = sqrtf((offRealX * offRealX) 
	+ (offRealY*offRealY));
    float velocity = 480/1; // 480 速度是 pixel/ 1 second 
    float realMoveDuration = length/velocity;

    // 用MoveTO动作让飞镖用同样的速度飞到不同的目的地，同样执行结束
    // 之后用调用 spriteMoveFinished 方法。
    projectile->runAction( CCSequence::create(
	CCMoveTo::create(realMoveDuration, realDest),
	CCCallFuncN::create(this, 
	callfuncN_selector(GameScene::spriteMoveFinished)), 
	NULL) );

    projectile->setTag(2);
    _projectiles->addObject(projectile);
}


void GameScene::update(float dt)
{
    CCArray* projectilesToDelete = new CCArray();
    CCObject* pobject;

    // cocos2d定义的宏，提供方便的只读遍历CCARRAY写法
    CCARRAY_FOREACH(_projectiles, pobject)
    {
	CCSprite* projectile = (CCSprite*)pobject;
	CCRect pRect = CCRect(projectile->getPosition().x - projectile->getContentSize().width/2, 
	    projectile->getPosition().y - projectile->getContentSize().height/2, 
	    projectile->getContentSize().width, 
	    projectile->getContentSize().height);     

	CCArray* targetsToDelete = new CCArray();
	CCObject* tobject;
	CCARRAY_FOREACH(_targets, tobject)
	{
	    CCSprite* target = (CCSprite*)tobject;
	    CCRect tRect = CCRect(target->getPosition().x - target->getContentSize().width/2, 
		target->getPosition().y - target->getContentSize().height/2, 
		target->getContentSize().width, 
		target->getContentSize().height);

	    // 碰撞测试
	    if( pRect.intersectsRect( tRect ) )
	    {
		targetsToDelete->addObject( target );
		// 计数干掉的妖怪数量，如果超过30就跳转到游戏结束场景
		_projectilesDestroyed++;                       
		if (_projectilesDestroyed > 30)
		{
		    GameOverScene *gameOverScene = GameOverScene::create();
		    gameOverScene->getLayer()->getLabel()->setString("You Win!");
		    CCDirector::sharedDirector()->replaceScene(gameOverScene);
		}
	    }
	}

	// 移除被击中的目标
	CCARRAY_FOREACH(targetsToDelete, tobject)
	{
	    CCSprite* target = (CCSprite*)tobject;
	    _targets->removeObject(target);
	    this->removeChild(target, true);
	}

	// 记录击中目标的子弹
	if(targetsToDelete->count() > 0)
	{
	    targetsToDelete->addObject(projectile);
	}

	/* 由于我们是用的 new CCArray() 而非 CCArray::create()
	获得的数组对象，所以需要手动调用release */
	targetsToDelete->release();
    }

    // 移除击中目标的子弹
    CCARRAY_FOREACH(projectilesToDelete, pobject)
    {
	CCSprite* projectile = (CCSprite*)pobject;
	_projectiles->removeObject(projectile, true);
	this->removeChild(projectile, true);
    }

    projectilesToDelete->release();
}    

void GameScene::keyBackClicked(){
    //游戏场景后退按钮
    CCLog("GameScene back key pressed...");
    CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());

}
