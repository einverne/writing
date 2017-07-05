#ifndef __MoveToUniformSpace_H__
#define __MoveToUniformSpace_H__

#include "cocos2d.h"
#include "UniformSpaceNode.h"
USING_NS_CC;

using namespace std;

class MoveToUniformSpace : public CCActionInterval
{
public:
	MoveToUniformSpace();
	~MoveToUniformSpace();

	/**
	* ���췽��
	* @param duration ����ʱ��
	* @return
	*/
	static MoveToUniformSpace* create(float duration);
	virtual void update(float time);
	bool init(float duration);
	void startWithTarget(CCNode *pTarget);

private:
	vector<float> length;				// ��¼�������ֵ
	vector<float> origin;				// ��¼Yֵ
	int flag;
};


#endif