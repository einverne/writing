#ifndef __CREADXML_H__
#define __CREADXML_H__

#include "Character.h"
#include "tinyxml.h"
#include "TemplateCharacter.hpp"
#include "ScriptCharacter.hpp"
#include "Segment.hpp"
using namespace std;

// 读取并解析每一个汉字，笔画xml
class CReadXML
{
public:
	/**
	* get Character from xml file or xml content
	* @return Character
	*/
	Character getCharacter() const;

	/**
	* use xml content to create a character, need a char*
	* @param xmlcontent
	* @return
	*/
	CReadXML(const char* xmlcontent);

	/**
	* parse xml file path to an Object to create a character
	* @param string xmlpath
	* @return
	*/
	CReadXML(string xmlpath);
    
    
    TemplateCharacter getTemplateCharacterFromBuffer(string xmlcontent, string mark_buffer);
    ScriptCharacter getScriptCharacterFromBuffer(string xmlcontent, string mark_buffer);
	~CReadXML(void);
private:
    void parseXML(TiXmlDocument* document);
	Character character;
};
#endif
