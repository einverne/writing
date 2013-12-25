#include "Bujian.h"
#include <vector>

#pragma once

using namespace std;
USING_NS_CC;

class Character
{
public:
	bool addBujian(Bujian bujian);
	CCSize getBox();
	int getStrokeCount();
	void resize(CCSize size);
	void resample();
	void prepareDrawNode();
	Stroke getStroke(int no);			//获取第几笔
public:
	Character(void);
	~Character(void);
	int bujianCount;
	int fontSize;		//汉字大小，默认512
	vector<Bujian> bujianList;
private:
	void transformCoordinate(CCPoint point,float length);

};

