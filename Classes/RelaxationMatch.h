// RelaxationMatch.h: interface for the RelaxationMatch class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __RelaxationMatch_H__
#define __RelaxationMatch_H__

// #if _MSC_VER > 1000
// #pragma once
// #endif // _MSC_VER > 1000

#include "TemplateCharacter.hpp"
#include "ScriptCharacter.hpp"

//////////////////////////////////////////////////////////
struct candidate  //用于迭代更新时的临时变量，以及松弛匹配的结果提取
{
	int h;   //手写笔段索引
	int k;   //模板笔段索引
	float probability;
};

struct multicandidate  //用于迭代更新时的临时变量，以及松弛匹配的结果提取
{
	list<int> hlist;   //手写笔段索引
	list<int> klist;   //模板笔段索引
	float probability;
};

class ComponentMatch  //部件匹配情况
{
public:
	list<candidate> havemath;  //已匹配笔段
	list<int> nomatch_model_seg; //未匹配的模板字笔段

	////////////////////////////////////
	//用于包围盒的估计
	list<int>  top_key_segments;
	list<int>  bottom_key_segments;
	list<int>  left_key_segments;
	list<int>  right_key_segments;
	////////////////////////////////////

	ComponentMatch();
	virtual ~ComponentMatch();
	
	///////
	ComponentMatch& operator=(const ComponentMatch& Right);    //赋值
	ComponentMatch(const ComponentMatch& Right); //拷贝构造
};

struct scriptpro
{
	int sh;
	float probability;
};

struct candidateforgreedy
{
	int mk; //模板字笔段
	list<scriptpro> shlist; //手写字笔段待定列表
};

struct conflictforgreedy
{
	candidate can1;  
	candidate can2;
};

struct boundingbox
{
	float m_left;
	float m_right;
	float m_top;
	float m_bottom;
};

////////////////////////////////////////////////////////
class RelaxationMatch  
{
public:
	RelaxationMatch();
	virtual ~RelaxationMatch();
	
	////////////////////////////////
	ScriptCharacter script;  //手写字
	TemplateCharacter model; //模板字

	int iteration_times;     //迭代次数
	float ** probability;    //匹配概率。probability[i][j]表示手写字第i个笔段与模板字第j个笔段的匹配概率.
	                         //          包括无匹配情况在内，j大于等于0，小于等于model_count。
	
	int **  updowncount;     //用于记录在迭代过程中匹配概率的升降计数，用于加速迭代。

	float**** compatibility;  //相容程度。对于compatibility[i][j][h][k]，其中，i、h表示手写字笔段，
	                          //          j、k表示模板字笔段。它表示(i，j)(h，k)同时匹配的相容程度。
	                          //          取值范围是[-1,1]。超出此范围，表示手写字笔段是抖笔，不能参与相容程度的计算。

 	int script_count;        //手写笔段数
	int model_count;         //模板笔段数  
	
	list<candidate> matchresult;  //一对一匹配结果
	list<multicandidate> multimatchresult; //一对多、多对一匹配结果
	////////////////////////////////

	bool Inputcharacters(ScriptCharacter handwritten, TemplateCharacter modelwritten);
	void Initmatchprobability();
	void Initmatchcompatibility();

	void IterateOnce();   //迭代一次，查看结果
	void RelaxationExtraction();  //迭代15次左右后，调用这个函数提取松弛算法的匹配结果。
	void processing_tiny_segments();//由于手写笔画随意性较大，手写字自身的笔段切分算法不能保证绝对正确。
                                    //所以，在松弛匹配后，调用此函数处理一些细碎的未匹配笔段。
                                    
	////////////////////////////////////////////////
	//在松弛匹配算法运行之后，定义一些典型的未匹配情况及其相应的处理规则
	void UnmatchExtension();              //此规则解决如下情况：手写字和模板字中同时含有未匹配笔段(抖笔除外)。

	void ComplexStrokeAdjust();           //此规则解决如下情况：对变形较大、笔段较多的复杂笔画进行调整、确认。
	
	void NoiseExtension_forsingle();      //此规则解决如下情况：模板字中的某个笔段与手写字中的抖笔相对应。
	void FreeExtension();                 //此规则解决如下情况：经过上述处理后，手写字和模板字中仍旧同时含有未匹配笔段。

	void Noturning_Connection_Extension();//此规则解决如下情况：手写字中存在没有拐点的连笔情况。
	void Noturning_Broken_Extension();    //此规则解决如下情况：手写字中存在断笔情况，断开的两笔在模板字中没有拐点
	void NoiseExtension_formulti();

protected:
	void Clearcontext();
	void Allocatecontext();


private:
	//被函数UnmatchExtension()使用	
	void SortandReduce(list<ComponentMatch> &complist);  //对松弛匹配算法后的匹配情况按部件进行排序；	
	CCPoint Evalute_position_forgreedy(int mk, ComponentMatch m_ComponentMatch, boundingbox modelbox,boundingbox sriptbox);//根据已匹配笔段、以及已经估计好的包围盒，为某个模板笔段估计候选手写笔段的中心位置
	float Evalute_length_forgreedy(int mk, boundingbox modelbox,boundingbox sriptbox);//根据已匹配笔段、以及已经估计好的包围盒，为某个模板笔段估计候选手写笔段的长度
	list<candidateforgreedy> probabilityforgreedy(ComponentMatch m_ComponentMatch, list<int> nomatchscript, int passnumber);
	list<scriptpro> Evalute_prob_forgreedy(int model_seg, CCPoint center_pos, float mlen, ComponentMatch m_ComponentMatch,list<int> nomatchscript);
	void Evalute_boundingbox(ComponentMatch m_ComponentMatch,boundingbox &modelbox,boundingbox &scriptbox);
	
	list<conflictforgreedy> conflicttestforgreedy(list<candidateforgreedy> candidatelist);
	list<candidate> GreedyExtraction(list<candidateforgreedy> candidatelist, list<conflictforgreedy> conflictlist);
	bool checkcompatibility_for_Greedy(candidate& m_item, list<candidate>& m_res, list<conflictforgreedy>& conflictlist);


	//被函数ComplexStrokeAdjust()使用
	list<candidate> merge_script_seg_for_multimatch(list<multicandidate> tempmultires);

	void process_stroke_heng_pei_wan_gou();
	void heng_pei_wan_gou_match_1(int start_sh, int start_mk, int sh_count, list<candidate>  &res, list<multicandidate> &tempmultires);
	void heng_pei_wan_gou_match_2(int m_seg1,int s_seg1, int m_seg2,int s_seg2,int m_seg3,int s_seg3,int m_seg4,int s_seg4, list<multicandidate>  &tempmultires);
		
	void process_stroke_wo_gou();
	void wo_gou_match(int mk_wo,int sh_wo, int mk_gou, int sh_gou, list<multicandidate> &tempmultires);

	void process_stroke_wan_gou();
	void wan_gou_match(int mk_wan,int sh_wan, int mk_gou,int sh_gou, list<multicandidate> &tempmultires);

	void process_stroke_xie_gou();
	void xie_gou_match(int mk_xie, int sh_xie, int mk_gou, int sh_gou, list<multicandidate> &tempmultires);

	void process_stroke_heng_zhe_wan_pie();
	void heng_zhe_wan_pie_match_1(int start_sh, int start_mk, int sh_count, list<candidate> &res, list<multicandidate>  &tempmultires);

	//被函数FreeExtension()调用
	list<conflictforgreedy> conflicttestforfree(list<candidateforgreedy> candidatelist);
	list<scriptpro> Evalute_prob_forfree(int model_seg, CCPoint center_pos, float mlen, ComponentMatch m_ComponentMatch, list<int> nomatchscript);


	//匹配结果的查询	
	int Find_Match_for_Model_segment_in_single_result(int k);   //只在matchresult中找
	int Find_Match_for_script_segment_in_single_result(int sh); //只在matchresult中找       

	list<int> Find_Nomatch_normal_Scriptseg_in_both_reslult();  //同时在matchresult和multimatchresult中找
	list<int> find_nomatch_modelseg_in_both_result();           //同时在matchresult和multimatchresult中找

	list<int> find_match_for_modelseg_in_multiresult(int k); //只在multimatchresult中找
	//int find_multimatch_for_script_segment(int sh, list<int> &shlist);


};

#endif // __RelaxationMatch_H__