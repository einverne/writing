#include "Character.h"

using namespace std;
#pragma once
/************************************************************************/
/* 读取并解析每一个汉字，笔画xml                                                                     */
/************************************************************************/
class CReadXML
{
public:
	Character* getCharacter();
public:
	CReadXML(void);
	CReadXML(string xmlpath);
	~CReadXML(void);
	Character character;
};

