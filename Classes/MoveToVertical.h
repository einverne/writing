#ifndef __MoveToVertical_H__
#define __MoveToVertical_H__

#include "cocos2d.h"
#include "MultiPointNode.h"
USING_NS_CC;

using namespace std;

class MoveToVertical : public CCActionInterval
{
public:
	MoveToVertical();
	~MoveToVertical();

	/**
	* ����MoveToVertical����
	* @param duration ����ʱ��
	* @return
	*/
	static MoveToVertical* create(float duration);
	virtual void update(float time);
	bool init(float duration);
	void startWithTarget(CCNode *pTarget);

private:
	vector<float> length;				// ��¼�������ֵ
	vector<float> originX;				// ��¼Xֵ
};


#endif