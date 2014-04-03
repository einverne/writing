#include "Bujian.h"
#include <vector>

#pragma once

using namespace std;
USING_NS_CC;

class Character
{
public:
	bool addBujian(Bujian bujian);
	//************************************
	// Method:    getBox 获取包围盒
	// FullName:  Character::getBox
	// Access:    public 
	// Returns:   cocos2d::CCSize
	// Qualifier:
	//************************************
	CCSize getBox();
	//************************************
	// Method:    getStrokeCount 获取字的笔画数
	// FullName:  Character::getStrokeCount
	// Access:    public 
	// Returns:   int
	// Qualifier:
	//************************************
	int getStrokeCount();
	void resize(CCSize size);
	void resample();
	void prepareDrawNode();
	//************************************
	// Method:    getStroke 获取第no笔，笔画从1开始
	// FullName:  Character::getStroke
	// Access:    public 
	// Returns:   Stroke
	// Qualifier:
	// Parameter: int no
	//************************************
	Stroke getStroke(int no);
public:
	Character(void);
	~Character(void);
	int bujianCount;
	int fontSize;		//汉字大小，默认512
	vector<Bujian> bujianList;
private:
	void transformCoordinate(CCPoint point,float length);

};

