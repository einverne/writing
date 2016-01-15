#ifndef __SPLASH_H__
#define __SPLASH_H__

#include "cocos2d.h"
using namespace std;
USING_NS_CC;

class Splash : public CCLayer
{
public:
    Splash(void);
    ~Splash(void);

    virtual bool init();
    static CCScene* scene();

    CREATE_FUNC(Splash);

    void onEnter();
    void onExit();
    void onExitTransitionDidStart();
private:
    void finishSplash(float dt);

	/**
	* init db in Android and iOS device
	* copy db file from package to writable place
	* @param db_name full name of db file
	* @return
	*/
	void initDB(string db_name);

	/**
	* init xml file 
	* deprecated 
	* @param xmlfile
	* @return
	*/
	void initXML(string xmlfile);
};

#endif
