#include "WallScene.h"
#include "tinyxml.h" 
#include "UTF8ToGBK.h"
#include "lianxi.h"

USING_NS_CC;

//////////////////////////////////////////
CCScene* WallScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    WallScene *layer = WallScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool WallScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(140,131,122,255)) )
    {
	return false;
    }

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    isMoved = false;
    touched = false;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
	"CloseNormal.png",
	"CloseSelected.png",
	this,
	menu_selector(WallScene::menuCloseCallback));

    pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
	origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 3);

    /////////////////////////////
    // 3. add your codes below...
    //////////////////////////////////////////////////////////////////////////////
    //添加地图：
    //1.读取xml文件，确定缩放比例//
    string myfilename=CCFileUtils::sharedFileUtils()->fullPathForFilename("wall.xml");
    TiXmlDocument* myDocument = new TiXmlDocument(myfilename.c_str());  
    myDocument->LoadFile();  

    TiXmlElement* rootElement = myDocument->RootElement();  // Class
    TiXmlElement* metaElement = rootElement->FirstChildElement();  // meta   
    TiXmlElement* heightElement = metaElement->FirstChildElement(); //获得meta的height元素
    string mheight=heightElement->GetText();
    int height=atoi(mheight.c_str());
    TiXmlElement* widthElement = heightElement->NextSiblingElement(); //获得meta的width元素
    string mwidth=widthElement->GetText(); 
    int width=atoi(mwidth.c_str());

    rescale=visibleSize.height/(float)height;
    //////////////
    TiXmlElement* dataElement = metaElement->NextSiblingElement();  // data
    TiXmlElement* stoneElement = dataElement->FirstChildElement();  // stone

    hanzilist.clear();
    while (stoneElement) {
	CHanziManage temphanziManage;
	TiXmlElement* typeElement = stoneElement->FirstChildElement();  // type

	string flag1("wordbox");
	string flag2("nouse");
	string flag3(typeElement->GetText());
	if(flag3==flag1)
	{
	    //wordbox
	    TiXmlElement* xElement = typeElement->NextSiblingElement();  // x
	    TiXmlElement* yElement = xElement->NextSiblingElement();  // y
	    TiXmlElement* wElement = yElement->NextSiblingElement();  // w
	    TiXmlElement* hElement = wElement->NextSiblingElement();  // h
	    TiXmlElement* imgElement = hElement->NextSiblingElement();  // img
	    TiXmlElement* hanziElement = imgElement->NextSiblingElement();  // hanzi
	    TiXmlElement* proficiencyElement = hanziElement->NextSiblingElement();  // proficiency

	    string tempx=xElement->GetText();
	    int x=atoi((tempx.substr(0,tempx.size()-2)).c_str());
	    string tempy=yElement->GetText();
	    int y=atoi((tempy.substr(0, tempy.size()-2)).c_str());
	    string tempw=wElement->GetText();
	    int w=atoi((tempw.substr(0,tempw.size()-2)).c_str());
	    string temph=hElement->GetText();
	    int h=atoi((temph.substr(0, temph.size()-2)).c_str());

	    //坐标系重定位
	    x=x+w/2;
	    y=height-y-h/2;

	    //缩放；
	    x*=rescale;
	    y*=rescale;
	    w*=rescale;
	    h*=rescale;

	    string tempfilename=imgElement->GetText();
	    string temphanzi=hanziElement->GetText();
	    string GBKhanzi = UTF8ToGBK::UTF8TOGBK(temphanzi);
	    string temppro=proficiencyElement->GetText();

	    //stone sprite
	    CCSprite* pSprite1 = CCSprite::create(tempfilename.c_str());    
	    pSprite1->setScale(rescale);
	    pSprite1->setPosition(ccp(origin.x+x, origin.y+y));
	    this->addChild(pSprite1, 1);

	    //文本框
	    CCLabelTTF* pLabel = CCLabelTTF::create(GBKhanzi.c_str(), "Zapfino", 100);			
	    pLabel->setPosition(ccp(origin.x + x, origin.y + y));
	    this->addChild(pLabel, 2);

	    //汉字管理
	    temphanziManage.character=hanziElement->GetText();
	    temphanziManage.textbox=pLabel;
	    temphanziManage.pos=CCPoint(x,y);
	    temphanziManage.filename=tempfilename;
	    temphanziManage.proficiency=atoi(temppro.c_str());

	    vector<CHanziManage>::const_iterator it = hanzilist.end(); 
	    hanzilist.insert(it,temphanziManage);
	    stoneElement=stoneElement->NextSiblingElement();

	}
	if(flag3==flag2)
	{
	    //nouse
	    TiXmlElement* xElement = typeElement->NextSiblingElement();  // x
	    TiXmlElement* yElement = xElement->NextSiblingElement();  // y
	    TiXmlElement* wElement = yElement->NextSiblingElement();  // w
	    TiXmlElement* hElement = wElement->NextSiblingElement();  // h
	    TiXmlElement* imgElement = hElement->NextSiblingElement();  // img

	    string tempx=xElement->GetText();
	    int x=atoi((tempx.substr(0,tempx.size()-2)).c_str());
	    string tempy=yElement->GetText();
	    int y=atoi((tempy.substr(0, tempy.size()-2)).c_str());
	    string tempw=wElement->GetText();
	    int w=atoi((tempw.substr(0,tempw.size()-2)).c_str());
	    string temph=hElement->GetText();
	    int h=atoi((temph.substr(0, temph.size()-2)).c_str());

	    //坐标系重定位
	    x=x+w/2;
	    y=height-y-h/2;

	    //缩放；
	    x*=rescale;
	    y*=rescale;
	    w*=rescale;
	    h*=rescale;

	    //stone sprite
	    string tempfilename=imgElement->GetText();
	    CCSprite* pSprite2 = CCSprite::create(tempfilename.c_str());    
	    pSprite2->setScale(rescale);
	    pSprite2->setPosition(ccp(origin.x+x, origin.y+y));
	    this->addChild(pSprite2, 1);
	    /////////////
	    stoneElement=stoneElement->NextSiblingElement();
	}		
    }

    ////////////////////////////////////////////////////////////////////////////////
    CCSprite* pSprite = CCSprite::create("wall.JPG");
    CCSize spriteSize = pSprite->getContentSize();
    pSprite->setPosition(ccp(spriteSize.width/2,spriteSize.height/2));
    this->addChild(pSprite, 0);

    //注册触摸事件
    CCPoint changepoint=ccp(0,0);
    touched=false;
    this->setTouchEnabled(true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    return true;
}

void WallScene::onEnter(){


}


bool  WallScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    touchbeginpoint = ccp(pTouch->getLocation().x , pTouch->getLocation().y);
    touched=true;
    beginTime = millisecondNow();
    return true;
}
void  WallScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

    long endTime = millisecondNow();
    if (endTime-beginTime < 250 && isMoved==false)
    {
	//single click
	CCPoint touchpoint = pTouch->getLocation();
	CCLog("Touchpoint %f %f",touchpoint.x,touchpoint.y);
	for (vector<CHanziManage>::iterator iter = hanzilist.begin();iter!=hanzilist.end();++iter)
	{
	    CCPoint hanziPos = iter->pos;
	    //CCLog("hanziPos %f %f",hanziPos.x,hanziPos.y);
	    CCPoint realPos = ccp(hanziPos.x+changepoint.x,hanziPos.y+changepoint.y);
	    //CCLog("hanziPos %f %f",hanziPos.x,hanziPos.y);
	    CCRect rect = CCRectMake(realPos.x-100,realPos.y-100,200,200);
	    if (rect.containsPoint(touchpoint))
	    {
		CCLog(iter->character.c_str());
		this->singleClick(iter->character);
		return;
	    }
	}
    }
    touched=false;
    isMoved = false;
}
void  WallScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    isMoved = true;
    CCPoint newpos=ccp(pTouch->getLocation().x , pTouch->getLocation().y);
    CCPoint temppoint=ccp(newpos.x-touchbeginpoint.x, newpos.y-touchbeginpoint.y);
    changepoint =ccp(changepoint.x+temppoint.x, changepoint.y+temppoint.y);
    CCLog("%f %f",changepoint.x,changepoint.y);
    ////////////////
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    if(changepoint.y!=0)
	changepoint.y=0;

    if(changepoint.x>=0)
	changepoint.x=0;

    if (changepoint.x<=-3000*rescale+visibleSize.width)
	changepoint.x=-3000*rescale+visibleSize.width;
    ////////////////
    this->setPosition(changepoint);
    touchbeginpoint = newpos;
}

void WallScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    ////
    this->setTouchEnabled(false);
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    ////
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool WallScene::isInSprite(CCTouch* pTouch){
    // 返回当前触摸位置在OpenGL坐标 
    CCPoint touchPoint=pTouch->getLocation();
    // 将世界坐标转换为当前父View的本地坐标系

    CCPoint reallyPoint=this->getParent()->convertToNodeSpace(touchPoint);
    // 获取当前基于父view的坐标系

    CCRect rect=this->boundingBox();
    // CCnode->convertToNodeSpace 或者  convertToWorldSpace 是基于当前Node的  与当前Node相关

    if(rect.containsPoint(reallyPoint)){
	return true;
    }
    return false;
}

void WallScene::singleClick(string hanzi){    
    CCDirector::sharedDirector()->replaceScene(lianxi::scene(hanzi));
}