#ifndef __Character_H__
#define __Character_H__

#include "Bujian.h"
#include <vector>

using namespace std;
USING_NS_CC;

// Character class include Bujians and Strokes
class Character
{
public:
    Character(void);
    ~Character(void);
    int bujianCount;
    int fontSize;		//汉字大小，默认512
    vector<Bujian> bujianList;
    
	bool addBujian(Bujian bujian);

	/**
	* getBox 获取包围盒
	* @return
	*/
	CCSize getBox();

	/**
	* getStrokeCount 获取字的笔画数
	* @return
	*/
	int getStrokeCount();
    
    /**
     resize Character size
     @param size
     @returns void
     */
	void resize(CCSize size);
    
    /**
     重采样
     @param
     @returns
     */
	void resample();
    
	void prepareDrawNode();

	/**
	* getStroke 获取第no笔，笔画从1开始
	* @param no
	* @return
	*/
	Stroke getStroke(int no);

	
	/**
	* set Stroke sequence read from xml
	* @param seq
	* @return
	*/
	void setStrokeSeq(const string seq)	{
		_stroke_seq = seq;
	}

	/**
	* get Stroke sequence read from xml
	* @return
	*/
	string getStrokeSeq() { return _stroke_seq; }

	/**
	* set character name
	* @param na
	* @return
	*/
	void setName(const string na) {
		_name = na;
	}

	/**
	* get character Name
	* @return
	*/
	string getName() { return _name; }

	/**
	* set radical seq
	* @param seq
	* @return
	*/
	void setRadicalSeq(const string seq){
		_radical_seq = seq;
	}

	/**
	* return radical seq
	* @return
	*/
	vector<string> getRadicalSeq() const {
		return DataTool::spliteStringBy(_radical_seq,"-");
	}

private:
	void transformCoordinate(CCPoint point,float length);
	string _stroke_seq;
	string _name;
	string _radical_seq;
};

#endif