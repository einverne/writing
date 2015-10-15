#ifndef __CharacterExtend_H__
#define __CharacterExtend_H__

#include <string>

class CharacterExtend
{
public:
	CharacterExtend();
	~CharacterExtend();
	int getID()	const	{ return _id; }
	void setID(const int id) { _id = id; }
	std::string getName() const		{ return _name; }
	void setName(const std::string& name) { _name = name; }  
	std::string getXML() const		{ return _xml; }
	void setXML(const std::string& xml)	{ _xml = xml; }
	std::string getRuleLoose() const	{ return _ruleLoose; }
	void setRuleLoose(const std::string& ruleLoose)		{ _ruleLoose = ruleLoose; }
	std::string getRuleTight() const	{ return _ruleTight; }
	void setRuleTight(const std::string& ruleTight)		{ _ruleTight = ruleTight; }
	std::string getSEQ() const { return _strokeIDSeq; }
	void setSEQ(const std::string& strokeIDSeq)			{ _strokeIDSeq = strokeIDSeq; }

private:
	int _id;							// id
	std::string _name;					//Character name
	std::string _xml;					//Character xml from sqlite
	std::string _ruleLoose;				//Character rules
	std::string _ruleTight;
	std::string _strokeIDSeq;			//sequence
};

#endif