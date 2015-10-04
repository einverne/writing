#include "Character.h"

using namespace std;

/************************************************************************/
/* 读取并解析每一个汉字，笔画xml
/************************************************************************/
class CReadXML
{
public:
	/**
	* get Character from xml file or xml content
	* @return Character
	*/
	Character getCharacter();
public:
	/**
	* use xml content to create a character
	* @param xmlcontent
	* @return
	*/
	CReadXML(const char* xmlcontent);

	/**
	* parse xml to an Object to create a character
	* @param string xmlpath
	* @return
	*/
	CReadXML(string xmlpath);
	~CReadXML(void);
private:
	Character character;
};