#ifndef _HcharacterDrawnode_H__
#define _HcharacterDrawnode_H__

#include "cocos2d.h"
#include "StrokeDrawnode.h"
#include "Stroke.h"
USING_NS_CC;

class HcharacterDrawnode : public CCNode
{
public:
	HcharacterDrawnode();
	~HcharacterDrawnode();
	CREATE_FUNC(HcharacterDrawnode);
	virtual bool init();
	virtual void draw();

	//************************************
	// Method:    addPoint 在最后一笔最后添加点
	// FullName:  HcharacterDrawnode::addPoint
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: CCPoint point
	//************************************
	void addPoint(CCPoint point);

	//************************************
	// Method:    addStroke 添加一笔
	// FullName:  HcharacterDrawnode::addStroke
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: Stroke s
	//************************************
	void addStroke(Stroke s);

	//************************************
	// Method:    removeLastStroke 删除最后一笔
	// FullName:  HcharacterDrawnode::removeLastStroke
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void removeLastStroke();

// 	vector<StrokeDrawnode*> getStrokeDrawnodeList(){ return this->strokeDrawlist; }
// 	vector<StrokeDrawnode*> strokeDrawlist;
	CC_SYNTHESIZE_RETAIN(CCArray*,strokeDrawlist,StrokeDrawnodeList);
private:

};



#endif