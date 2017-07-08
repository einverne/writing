#include "ReadXML.h"
#include "Bujian.h"
#include "Stroke.h"
#include "Character.h"

USING_NS_CC;
using namespace std;

CReadXML::CReadXML(const char* xmlcontent)
{
	TiXmlDocument* document = new TiXmlDocument();
	document->Parse(xmlcontent,0,TIXML_ENCODING_UTF8);
    parseXML(document);
}


CReadXML::CReadXML(string xmlpath)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	unsigned long size = 0;
	char* pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(xmlpath.c_str(),"r",&size);
	TiXmlDocument* document = new TiXmlDocument();
	document->Parse(pFileContent,0,TIXML_ENCODING_UTF8);
#endif
#if CC_TARGET_PLATFORM ==  CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	TiXmlDocument* document = new TiXmlDocument(xmlpath.c_str());
	document->LoadFile();
#endif
	parseXML(document);
}


CReadXML::~CReadXML(void)
{
}

Character CReadXML::getCharacter() const{
	return character;
}

void CReadXML::parseXML(TiXmlDocument* document){
    TiXmlElement* rootElement = document->RootElement();
	const char* name = rootElement->Attribute("name");
	character.setName(name);
	const char* seq = rootElement->Attribute("stroke_seq_id");
	character.setStrokeSeq(seq);
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
            const char* statusPoint = trunpoint->Attribute("status");
            stroke.addStatus(statusPoint);
            stroke.addPoint(ccp(atof(x),atof(y)));
        }
        bujian.addStroke(stroke);
    }
    character.addBujian(bujian);
}