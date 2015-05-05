#include "Bujian.h"
#include <vector>

#pragma once

using namespace std;
USING_NS_CC;

class Character
{
public:
	bool addBujian(Bujian bujian);

	/**
	* getBox 获取包围盒
	* @return
	*/
	CCSize getBox();

	/**
	* getStrokeCount 获取字的笔画数
	* @return
	*/
	int getStrokeCount();
	void resize(CCSize size);
	void resample();
	void prepareDrawNode();

	/**
	* getStroke 获取第no笔，笔画从1开始
	* @param no
	* @return
	*/
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

