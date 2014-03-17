#ifndef __StrokeDrawnode_H__
#define __StrokeDrawnode_H__

#include "cocos2d.h"
#include "Stroke.h"
USING_NS_CC;
using namespace std;

class StrokeDrawnode : public CCNode
{
public:
	StrokeDrawnode();
	StrokeDrawnode(Stroke stro);
	~StrokeDrawnode();
	static StrokeDrawnode* create(Stroke stro);
	Stroke stroke;
	ccColor4B mcolor;
	virtual void draw();
	int visibleIndex;
	void addPoint(CCPoint point);
private:

};


#endif