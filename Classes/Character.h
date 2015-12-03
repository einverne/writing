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

private:
	void transformCoordinate(CCPoint point,float length);

};

