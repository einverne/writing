#ifndef _SegmentDrawnode_H__
#define _SegmentDrawnode_H__

#include "cocos2d.h"
#include "Segment.hpp"
USING_NS_CC;

class SegmentDrawnode :
	public CCNode
{
public:
	SegmentDrawnode(Segment segment);
	~SegmentDrawnode(void);
	static SegmentDrawnode* create(Segment segment);
	void draw();

	ccColor4F color_;
public:
	Segment segment_;
};

#endif