#include "ReadXML.h"
#include "tinyxml.h"
#include "Bujian.h"
#include "Stroke.h"
#include "Character.h"

USING_NS_CC;
using namespace std;

CReadXML::CReadXML(void)
{
}


CReadXML::CReadXML(string xmlpath)
{
	TiXmlDocument* document = new TiXmlDocument(xmlpath.c_str());
	document->LoadFile();
	TiXmlElement* rootElement = document->RootElement();
	TiXmlElement* outlineElement = rootElement->FirstChildElement();
	TiXmlElement* strokeElement = outlineElement->FirstChildElement();
	Bujian bujian;
	for (strokeElement;strokeElement;strokeElement = strokeElement->NextSiblingElement())
	{
		TiXmlElement* trunpoint = strokeElement->FirstChildElement();
		Stroke stroke;
		for (trunpoint;trunpoint;trunpoint = trunpoint->NextSiblingElement())
		{
			const char* x = trunpoint->Attribute("x");
			const char* y = trunpoint->Attribute("y");
			stroke.addPoint(ccp(atof(x),atof(y)));
		}
		bujian.addStroke(stroke);
	}
	character.addBujian(bujian);
}


CReadXML::~CReadXML(void)
{
}

Character CReadXML::getCharacter(){
	return character;
}