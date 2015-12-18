#ifndef __CharacterEntity_H__
#define __CharacterEntity_H__

#include <string>

class CharacterEntity
{
public:
	CharacterEntity();
	~CharacterEntity();
    int getID()	const	{ return _id; }
    void setID(const int id) { _id = id; }
    std::string getName() const		{ return _name; }
    void setName(const std::string& name) { _name = name; }
    std::string getXML() const		{ return _xml; }
    void setXML(const std::string& xml)	{ _xml = xml; }
    std::string getRules() const	{ return _rules; }
    void setRules(const std::string& rules)		{ _rules = rules; }
    std::string getSEQ() const { return _strokeIDSeq; }
    void setSEQ(const std::string& strokeIDSeq)			{ _strokeIDSeq = strokeIDSeq; }
    
private:
    int _id;							// id
    std::string _name;					//Character name
    std::string _xml;					//Character xml from sqlite
    std::string _rules;                 //Character rules
    std::string _strokeIDSeq;			//sequence
};

#endif
