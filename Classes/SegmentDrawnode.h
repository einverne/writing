#ifndef _SegmentDrawnode_H__
#define _SegmentDrawnode_H__

#include "cocos2d.h"
#include "Segment.hpp"
#include "tools\DataTool.h"
USING_NS_CC;

class SegmentDrawnode :
	public CCNode
{
public:
	SegmentDrawnode(Segment segment);
	~SegmentDrawnode(void);
	static SegmentDrawnode* create(Segment segment);
	void draw();

public:
	Segment segment_;
};

#endif