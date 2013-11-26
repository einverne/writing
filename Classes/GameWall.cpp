#include "GameWall.h" 
#include "HelloWorldScene.h" 
#include "lianxi.h"

USING_NS_CC;
using namespace std;

GameWall::GameWall(){


}

GameWall::~GameWall()
{

}

void GameWall::onEnter(){
    CCLog("GameWall onEnter");
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize(); 
    CCSize visiableSize = CCDirector::sharedDirector()->getVisibleSize();
    CCSize originSize = CCDirector::sharedDirector()->getVisibleOrigin();

    CCDictionary* strings = CCDictionary::createWithContentsOfFile("chinese_character.xml");
    const char* hello = ((CCString*)strings->objectForKey("Hello"))->m_sString.c_str();
    const char* info = ((CCString*)strings->objectForKey("Info"))->m_sString.c_str();

    tagMap = 1;
    tagLabel = 2;
    sprites = CCArray::create();
    map = CCTMXTiledMap::create("stonemap.tmx");
    this->addChild(map,0,tagMap);
    CCSize mapSize = map->getMapSize();
    CCTMXLayer* background = map->layerNamed("background");

    CCSize layerSize = background->getLayerSize();

    CCTMXObjectGroup* objGroup = map->objectGroupNamed("zi");

    characters = objGroup->getObjects();
    unsigned int count = characters->count();
    CCObject* obj;
    CCARRAY_FOREACH(characters,obj){
	CCDictionary* dic = (CCDictionary*)obj;
	float x = dic->valueForKey("x")->floatValue();
	float y = dic->valueForKey("y")->floatValue();
	float tiled_width = dic->valueForKey("width")->floatValue();
	float tiled_height = dic->valueForKey("height")->floatValue();

	CCLabelTTF* pic = CCLabelTTF::create(hello,"Arial",50);
	this->addChild(pic,1,tagLabel);
	pic->setPosition(ccp(x+tiled_width/2,y+tiled_height/2));
	sprites->addObject(pic);
    }

    //     CCDictionary* testZi = objGroup->objectNamed("test");
    //     float x = testZi->valueForKey("x")->floatValue();
    //     float y = testZi->valueForKey("y")->floatValue();
    //     oneCharacter = CCSprite::create("Player.png");
    //     oneCharacter->setPosition(ccp(x,y));
    //     this->addChild(oneCharacter);

    //add ceshi logo 
    CCSprite* ceshi_logo = CCSprite::create("ceshi_logo.png");
    ceshi_logo->setPosition(ccp(winSize.width-ceshi_logo->getContentSize().width/2-40,ceshi_logo->getContentSize().height/2+40));
    this->addChild(ceshi_logo);


    

    //add wall-head
    CCSprite* wall_head = CCSprite::create("wall_head.png");
    this->addChild(wall_head);
    wall_head->setPosition(ccp(visiableSize.width/2,visiableSize.height-wall_head->getContentSize().height/2));
    wall_head->setScaleX(visiableSize.width/wall_head->getContentSize().width);

    //add ceshi tail on the bottom
    CCSprite* wall_tail = CCSprite::create("wall_tail.png");
    this->addChild(wall_tail);
    wall_tail->setPosition(ccp(visiableSize.width/2,wall_tail->getContentSize().height/2));
    wall_tail->setScaleX(visiableSize.width/wall_tail->getContentSize().width);

    //add ceshi_label
    CCLabelTTF* ceshi = CCLabelTTF::create("ceshi","Arial",40);
    CCMenuItemLabel* ceshi_Lebal = CCMenuItemLabel::create(ceshi,this,menu_selector(GameWall::menuCallCeshiButton));
    ceshi_Lebal->setPosition(ccp(visiableSize.width-ceshi_Lebal->getContentSize().width/2-100,wall_tail->getContentSize().height/2));

    //add CCMenuItemLabel
    CCLabelTTF* test = CCLabelTTF::create("tesetdd","Arial",40);
    CCMenuItemLabel* menuLabel = CCMenuItemLabel::create(test,this,menu_selector(GameWall::menuCall));
    menuLabel->setPosition(ccp(winSize.width/2,winSize.height/2));
    CCMenu* menu = CCMenu::create(menuLabel,ceshi_Lebal,NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu,1);

    this->setTouchEnabled(true);
    registerWithTouchDispatcher();
    menu->registerWithTouchDispatcher();

}

void GameWall::onExit(){
    CCLog("GameWall onExit");
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
}

void GameWall::registerWithTouchDispatcher(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,kCCMenuHandlerPriority,true);
}

bool GameWall::init()
{
    if ( !CCLayer::init() )
    {
	return false;
    }
    CCLog("Game Wall init()");

    return true;
}

CCScene* GameWall::scene(){
    CCScene* scene = CCScene::create();
    GameWall * layer = GameWall::create();
    scene->addChild(layer);
    return scene;
}

void GameWall::menuCloseCallback(CCObject* pSender){

}

void GameWall::menuCallCeshiButton(CCObject* pSender){
    CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

void GameWall::keyBackClicked(){
    CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());

}

// void GameWall::ccTouchesBegan (CCSet *pTouches, CCEvent *pEvent){
// 
// }
// void GameWall::ccTouchesMoved (CCSet *pTouches, CCEvent *pEvent){
//     if (pTouches->count() == 1)
//     {
// 	
// 	CCTouch* touch = (CCTouch*)pTouches->anyObject();
// 	CCPoint diff = touch->getDelta();
// 	CCNode* node = this->getChildByTag(tagMap);
// 	CCPoint curPos = node->getPosition();
// 	CCLog("%f XXXXXXXXXXXXXXX",curPos.x);
// 	float map_width = map->getMapSize().width*map->getTileSize().width;
// 	float visiable_width = CCDirector::sharedDirector()->getVisibleSize().width;
// 	
// 	if (curPos.x+diff.x <= 0 && curPos.x+diff.x >= visiable_width-map_width)
// 	{
// 	    node->setPosition(curPos.x+diff.x,curPos.y);
// 	    //map->objectGroupNamed("zi")->setPositionOffset(diff);
// // 	    CCARRAY_FOREACH(characters,obj){
// // 		CCDictionary* dic = (CCDictionary*)obj;
// // 		float x = dic->valueForKey("x")->floatValue();
// // 		CCLog("%f ",x);
// // 		CCFloat* ccfx = CCFloat::create(x+diff.x);
// // 		dic->setObject(ccfx,"x");
// // 	    }
// 	}
// 	//this->update(diff);
// 
//     }
// }

// void GameWall::ccTouchesEnded (CCSet *pTouches, CCEvent *pEvent){
// 
// }

bool GameWall::ccTouchBegan (CCTouch *pTouch, CCEvent *pEvent){
    return true;
}
void GameWall::ccTouchMoved (CCTouch *pTouch, CCEvent *pEvent){
    CCTouch* touch = pTouch;
    CCPoint diff = touch->getDelta();
    CCNode* node = this->getChildByTag(tagMap);
    CCPoint curPos = node->getPosition();
    CCLog("%f XXXXXXXXXXXXXXX",curPos.x);
    float map_width = map->getMapSize().width*map->getTileSize().width;
    float visiable_width = CCDirector::sharedDirector()->getVisibleSize().width;

    if (curPos.x+diff.x <= 0 && curPos.x+diff.x >= visiable_width-map_width)
    {
	node->setPosition(curPos.x+diff.x,curPos.y);
	//map->objectGroupNamed("zi")->setPositionOffset(diff);
	// 	    CCARRAY_FOREACH(characters,obj){
	// 		CCDictionary* dic = (CCDictionary*)obj;
	// 		float x = dic->valueForKey("x")->floatValue();
	// 		CCLog("%f ",x);
	// 		CCFloat* ccfx = CCFloat::create(x+diff.x);
	// 		dic->setObject(ccfx,"x");
	// 	    }
    }
    //this->update(diff);

}
void GameWall::ccTouchEnded (CCTouch *pTouch, CCEvent *pEvent){

}



void GameWall::update(CCPoint diff){
    float x = diff.x;
    CCObject* obj;
    CCARRAY_FOREACH(sprites,obj){
	CCSprite* sprite = (CCSprite*)obj;
	sprite->setPosition(ccp(sprite->getPositionX()+x,sprite->getPositionY()));
    }
}

void GameWall::menuCall(CCObject* pSender){
    CCDirector::sharedDirector()->replaceScene(lianxi::scene(string("A")));
}