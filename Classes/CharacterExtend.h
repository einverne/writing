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
	std::string getRuleUnit() const		{ return _ruleUnit;	}
	void setRuleUnit(const std::string& ruleUnit)		{ _ruleUnit = ruleUnit; }
	std::string getRuleZi() const	{ return _ruleZi; }
	void setRuleZi(const std::string& ruleZi)  { _ruleZi = ruleZi; }
	std::string getMark() const { return _mark; }
	void setMark(const std::string& m)	{ _mark = m; }

private:
	int _id;							// id
	std::string _name;					//Character name
	std::string _xml;					//Character xml from sqlite
	std::string _ruleLoose;				//Character rules
	std::string _ruleTight;
	std::string _strokeIDSeq;			//sequence
	std::string _ruleUnit;				//character rule unit
	std::string _ruleZi;				//character rule of whole character
	std::string _mark;					// character mark data for segment
};

#endif