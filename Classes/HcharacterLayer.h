#ifndef __Hcharacter_H__
#define __Hcharacter_H__

#include "cocos2d.h"
#include "../CocosWidget/cocos-widget.h"
#include "HcharacterDrawnode.h"
#include "CharacterExtend.h"
#include "JudgeManager.h"
#include <map>

USING_NS_CC;
using namespace cocos2d::cocoswidget;

#include "../rapidjson/document.h"
using namespace rapidjson;

using namespace std;

// writing character Layer to deal with writing action
class HcharacterLayer : public CCLayer
{
public:
	HcharacterLayer();
	~HcharacterLayer();
	static HcharacterLayer* create(string hanzi,CCSprite* tianzige_draw);
	virtual bool init(string hanzi,CCSprite* tianzige_draw);
	virtual void onEnter();
	virtual void onExit();

	void judge();
	bool CharacterSizeJudge();
	bool CharacterPosJudge();

	/**
	* ����lua���ؽ������������ֵ
	* WeeklyMeeting/���������������.md
	* 
	* @param ret ����ֵ string json��ʽ
	* @return
	*/
	void ParseResult(const string ret);

	void reloadChar();				//���¼��غ�����Ϣ
	float getWrongPercent();
	void isJudge(bool isjudge);
	string getPointsOutput() const { return pointsOutput; }
	string getOriginPoints() const { return pointsOrigin; }

	/**
	* clear all writing like function rewrite(CCOject* pSender)
	* @return
	*/
	void clearWriting();

	/**
	* ˮƽƽ��
	* @return
	*/
	void doA0001(multimap<int, float>& points, multimap<int, float>& rightpoints);

	/**
	* �е��з�
	* @return
	*/
	void doA0002(multimap<int, float>& points, multimap<int, float>& rightpoints);

	/**
	* �ƽ�ָ�
	* @return
	*/
	void doA0003(multimap<int, float>& points, multimap<int, float>& rightpoints);

	/**
	* �׵����
	* @return
	*/
	void doA0004(multimap<int, float>& points, multimap<int, float>& rightpoints);

	/**
	* ��ֱ�ȷ�
	* @param points
	* @param rightpoints
	* @return
	*/
	void doA0005(multimap<int, float>& points, multimap<int, float>& rightpoints);

	/**
	* ˮƽ�ȷ�
	* @return
	*/
	void doA0006(multimap<int, float>& points, multimap<int, float>& rightpoints);

	/**
	* ����ƽ��
	* @return
	*/
	void doA0007(multimap<int, float>& points, multimap<int, float>& rightpoints);

	/**
	* ��ֱƽ��
	* @return
	*/
	void doA0008(multimap<int, float>& points, multimap<int, float>& rightpoints);

	/**
	* ͻ������
	* @return
	*/
	void doA0009(multimap<int, float>& points, multimap<int, float>& rightpoints);

	/**
	* ���ȱ���
	* @return
	*/
	void doA0010(multimap<int, float>& points, multimap<int, float>& rightpoints);

	/**
	* �ݺ�ȡ���̫С����̫ƫ
	* @return
	*/
	void doC0001_2_3();

	multimap<int, float> getRightPoints(rapidjson::Document& doc);
	string getErrorType(rapidjson::Document& doc) ;
	multimap<int, float> getErrorStroke(rapidjson::Document& doc); 


	CC_SYNTHESIZE_RETAIN(HcharacterDrawnode*,m_HDrawnode,m_HDrawnode);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, bihuaCountAndTotal,bihuaCountAndTotal);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, scoreLabel, scoreLabel);
	CC_SYNTHESIZE_RETAIN(CCSprite* , m_sprite_draw, Sprite);
	CC_SYNTHESIZE_RETAIN(CCSprite* , m_sprite_info, InfoSprite);
	CharacterExtend* getExChar()	{ return m_exChar; }
	void setExChar(CharacterExtend* exchar) { m_exChar = exchar; }
	
private:
	/**
	* ��д�ص�����
	* @param pSender
	* @return
	*/
	void rewrite(CCObject* pSender);
	/**
	* �Ŵ�ص�����
	* @param pSender
	* @return
	*/
	void zoomin(CCObject* pSender);			//

	/**
	* ��С�ص�����
	* @param pSender
	* @return
	*/
	void zoomout(CCObject* pSender);
	void writeBihuaWrong();
	void writeBihuaRight();
	int writeCount_;				//��д����
	int wrongCount_;				//�������
	string hanzi_;				//����ò�ʹ�ú���
	float scale_;				//���ű���
	bool ijudge_;				//�Ƿ����� false�����У�true����
	string pointsOutput;		//����ַ���
	string pointsOrigin;		//��д����Origin�ַ��� δ��Points �ı���
	int totalBihuaCount;				//�ú����ܹ��еıʻ���
	float score_;				//��ǰ��������
	int curBihuaWrong;			//��ǰ�ʻ�����Ĵ���
	CharacterExtend* m_exChar;	// characterExtend
	JudgeManager _manager;		// judge manager
};

#endif
