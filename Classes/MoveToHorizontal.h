#ifndef __ACTIONA0001_H__
#define __ACTIONA0001_H__

#include "cocos2d.h"
#include "MultiPointNode.h"
USING_NS_CC;

using namespace std;

class MoveToHorizontal : public CCActionInterval
{
public:
	MoveToHorizontal();
	~MoveToHorizontal();

	/**
	* ����ActionA0001����
	* @param duration ����ʱ��
	* @return
	*/
	static MoveToHorizontal* create(float duration);
	virtual void update(float time);
	bool init(float duration);
	void startWithTarget(CCNode *pTarget);

private:
	vector<float> length;				// ��¼�������ֵ
	vector<float> originY;				// ��¼Yֵ
};


#endif