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
	void prepareDrawNode();
public:
	Character(void);
	~Character(void);
	int bujianCount;
	int fontSize;
	vector<Bujian> bujianList;
private:
	void transformCoordinate(CCPoint point,float length);

};

