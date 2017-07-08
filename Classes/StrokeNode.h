#ifndef __StrokeNode_H__
#define __StrokeNode_H__

#include "cocos2d.h"
#include "Stroke.h"
USING_NS_CC;

#pragma once
class StrokeNode : public CCNode
{
public:
	StrokeNode(Stroke stroke);
	~StrokeNode(void);

	virtual bool init();
	virtual void draw(void);
	static StrokeNode* getStrokeNode(Stroke stroke);
	CCPoint getMidStrokePoint();
	void setAnchorMidStroke();
private:
	Stroke n_stroke;
};

#endif