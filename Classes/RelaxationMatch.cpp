// RelaxationMatch.cpp: implementation of the RelaxationMatch class.
//
//////////////////////////////////////////////////////////////////////

#include "RelaxationMatch.h"
#include "GeometryTool.hpp"
#include <math.h>
#include <algorithm>

/*#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif*/

ComponentMatch::ComponentMatch()
{
	havemath.clear(); 
	nomatch_model_seg.clear();

	top_key_segments.clear();
	bottom_key_segments.clear();
	left_key_segments.clear();
	right_key_segments.clear();
}
ComponentMatch::~ComponentMatch()
{
	havemath.clear(); 
	nomatch_model_seg.clear();

	top_key_segments.clear();
	bottom_key_segments.clear();
	left_key_segments.clear();
	right_key_segments.clear();
}

///////
ComponentMatch& ComponentMatch::operator=(const ComponentMatch& Right)
{
	this->havemath.assign(Right.havemath.begin(),Right.havemath.end());
	this->nomatch_model_seg.assign(Right.nomatch_model_seg.begin(),Right.nomatch_model_seg.end());

	this->top_key_segments.assign(Right.top_key_segments.begin(),Right.top_key_segments.end());
	this->bottom_key_segments.assign(Right.bottom_key_segments.begin(),Right.bottom_key_segments.end());
	this->left_key_segments.assign(Right.left_key_segments.begin(),Right.left_key_segments.end());
	this->right_key_segments.assign(Right.right_key_segments.begin(),Right.right_key_segments.end());

	return (*this);
}

ComponentMatch::ComponentMatch(const ComponentMatch& Right)
{
	this->havemath.assign(Right.havemath.begin(),Right.havemath.end());
	this->nomatch_model_seg.assign(Right.nomatch_model_seg.begin(),Right.nomatch_model_seg.end());

	this->top_key_segments.assign(Right.top_key_segments.begin(),Right.top_key_segments.end());
	this->bottom_key_segments.assign(Right.bottom_key_segments.begin(),Right.bottom_key_segments.end());
	this->left_key_segments.assign(Right.left_key_segments.begin(),Right.left_key_segments.end());
	this->right_key_segments.assign(Right.right_key_segments.begin(),Right.right_key_segments.end());
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


RelaxationMatch::RelaxationMatch()
{
	iteration_times=0;
	script_count=0; 
	model_count=0;
	probability=NULL;
	compatibility=NULL;
	updowncount=NULL;	
}

RelaxationMatch::~RelaxationMatch()
{
	for(int a=0;a<script_count;a++) 
	{
		delete[] probability[a];  
		delete[] updowncount[a];  
	}
	delete[] probability;
	delete[] updowncount;

	/////////////
	for (int i=0;i<script_count;i++)
	{
		for (int j=0;j<model_count;j++)
		{
			for (int h=0;h<script_count;h++)
			{
				delete[] compatibility[i][j][h];
			}
			delete[] compatibility[i][j];
		}
		delete[] compatibility[i];
	}
	delete[] compatibility;
}

//提供两个待匹配的字
bool RelaxationMatch::Inputcharacters(ScriptCharacter handwritten, TemplateCharacter modelwritten)
{
	if (handwritten.segment_list_.size()==0 || modelwritten.segment_list_.size()==0)
	{
		return false;
	}
	Clearcontext();
	//
	this->model=modelwritten;
	this->script=handwritten;
	//
	this->model_count=model.segment_list_.size();
	this->script_count=script.segment_list_.size();
	//
	Allocatecontext();
	//
	return true;
}

//迭代之初的匹配概率
void RelaxationMatch::Initmatchprobability()
{
	GeometryTool gt;
	//计算手写字笔段与模板字笔段，任意两个组合的匹配概率
	for (int si=0;si<script_count;si++)
	{
		for (int mj=0;mj<model_count;mj++)
		{
			//1.如果si被估计为抖笔，那么所有匹配概率为0
			if (script.IsNoiseSegment(si))
			{
				probability[si][mj]=0.0;
				continue;
			}

			//2.如果si没被估计为抖笔，那么计算匹配概率。
			Segment s=this->script.getsegment(si);
			Segment m=this->model.getsegment(mj);
			
			//根据mj是否为稳定笔段，进行不同的概率计算
			if (model.isinstablesegment(mj))
			{
				probability[si][mj]=gt.PathSimilarity(s,m,model.normal_size_,false, model_count>10);
			}
			else
			{
				probability[si][mj]=gt.PathSimilarity(s,m,model.normal_size_,true, model_count>10);
			}			
		}
	}

	//计算未匹配概率
	for (unsigned int si=0;si<script_count;si++)
	{
		float maxmatch=0.0;
		for (int mj=0;mj<model_count;mj++)
		{
			if(maxmatch<probability[si][mj])
				maxmatch=probability[si][mj];
		}
		probability[si][model_count]=1.0-maxmatch;
	}

	//对匹配概率进行归一化处理
	for (unsigned int si=0;si<script_count;si++)
	{
		float sum=0.0;
		for (int mj=0;mj<=model_count;mj++)
		{
			sum+=probability[si][mj];
		}
		for (unsigned int mj=0;mj<=model_count;mj++)
		{
			probability[si][mj]=probability[si][mj]/sum;
		}
	}

	this->iteration_times=0;
}

//初始化相容程度
void RelaxationMatch::Initmatchcompatibility()
{
	for (int i=0;i<script_count;i++)
		for (int j=0;j<model_count;j++)
			for (int h=0;h<script_count;h++)
				for (int k=0;k<model_count;k++)
					compatibility[i][j][h][k]=-100;

	////////////////////////////
	GeometryTool gt;
	Segment pi,pj,ph,pk;
	for (unsigned int i=0;i<script_count;i++)
	{
		for (int j=0;j<model_count;j++)
		{
			for (int h=i+1;h<script_count;h++)
			{
				for (int k=0;k<model_count;k++)
				{
					//i,h表示手写笔段；j,k表示模板笔段
					//1.忽略抖笔
					if (script.IsNoiseSegment(i) || script.IsNoiseSegment(h))
						continue;
					
					//2.忽略自身重复
					if(j==k)
						continue;

					//3.忽略没有匹配可能性的情况
					if(probability[i][j]<0.00001 || probability[h][k]<0.00001)
						continue;
					
					//4.分情况计算相容性
					bool Insamecompnent=model.IsShareOnecomponent(j,k);
					bool instable=model.isinstablesegment(j) || model.isinstablesegment(k);
					string relation=model.GetRelation(j,k);
							
					pi=this->script.getsegment(i);
					pj=this->model.getsegment(j);
					ph=this->script.getsegment(h);
					pk=this->model.getsegment(k);

					compatibility[i][j][h][k]=gt.computecompatibility(pi,pj,ph,pk,Insamecompnent,instable,relation,
						                                              model.normal_size_, model_count>10);
					compatibility[h][k][i][j]=compatibility[i][j][h][k];
				}
			}
		}
	}
}

void RelaxationMatch::Clearcontext()
{
	for(int a=0;a<script_count;a++) 
	{
		delete[] probability[a];  
		delete[] updowncount[a];
	}
	delete[] probability;
	delete[] updowncount;

	////////////////////
	for (int i=0;i<script_count;i++)
	{
		for (int j=0;j<model_count;j++)
		{
			for (int h=0;h<script_count;h++)
			{
				delete[] compatibility[i][j][h];
			}
			delete[] compatibility[i][j];
		}
		delete[] compatibility[i];
	}
	delete[] compatibility;

	////////////////////
	script_count=0; 
	model_count=0;
	iteration_times=0;
	probability=NULL;
	updowncount=NULL;
	compatibility=NULL;
}

void RelaxationMatch::Allocatecontext()
{
	//1.匹配概率
	//1.1分配空间
	probability=new float* [script_count];
	updowncount=new int* [script_count];
	for(int a=0;a<script_count;a++) 
	{
		probability[a]=new float[model_count+1]; 
		updowncount[a]=new int[model_count+1]; 
	}	
	//1.2初始化
	for (int i=0;i<script_count;i++)
		for (int j=0;j<=model_count;j++)
		{
			probability[i][j]=0.0;
			updowncount[i][j]=0;
		}
	/////////////////////////////////////
	//2.相容程度
	//2.1分配空间
	compatibility=new float***[script_count];
	for (unsigned int i=0;i<script_count;i++)
	{
		compatibility[i]=new float**[model_count];
		for (int j=0;j<model_count;j++)
		{
			compatibility[i][j]=new float*[script_count];
			for (int h=0;h<script_count;h++)
			{
				compatibility[i][j][h]=new float[model_count];
			}			
		}		
	}
	//2.2初始化
	for (unsigned int i=0;i<script_count;i++)
		for (int j=0;j<model_count;j++)
			for (int h=0;h<script_count;h++)
				for (int k=0;k<model_count;k++)
					compatibility[i][j][h][k]=0.0;

}



void RelaxationMatch::IterateOnce()   //迭代一次，查看结果
{	
	//暂存迭代之前的匹配概率
	list<candidate> m_candidate;
	m_candidate.clear();
	candidate temp;
	//////	
	for (int h=0;h<script_count;h++)
	{
		for (int k=0;k<model_count;k++)
		{
			if(probability[h][k]>=0.00001)
			{				
				temp.h=h;
				temp.k=k;
				temp.probability=probability[h][k];
				m_candidate.push_back(temp);
			}
		}
	}

	//计算一个除数finalcount
	int finalcount=0;
	float finalevaluate=0;
	for (int mh=0;mh<script_count;mh++)
	{
		float tempmax=0.0;
		for (int mk=0;mk<model_count;mk++)
		{
			if(probability[mh][mk]>tempmax)
				tempmax=probability[mh][mk];
		}
		finalevaluate+=tempmax;
	}
	
	finalcount=(int)(finalevaluate+1.0);
	//对每个匹配概率不为0的(i,j)，使用相容值对其进行更新
	for (int i=0;i<script_count;i++)
	{
		for (int j=0;j<model_count;j++)
		{			
			//1.如果ij之间不存在匹配概率，那么就不进行更新
			if(probability[i][j]<=0.00001)
				continue;

			//2.迭代循环，每次都找到最大可能支持(i，j)的(h，k)，并计算支撑值
			int count=0;  //用于求平均值的计数器
			float support_sum=0.0;
			list<candidate> iteration_can;
			std::copy(m_candidate.begin(), m_candidate.end(), std::back_inserter(iteration_can));  
			
			//2.1删除和当前笔段重复的候选内容
			list<candidate>::iterator it=iteration_can.begin();
			while(it!=iteration_can.end())
			{
				if((*it).h==i || (*it).k==j)
					it=iteration_can.erase(it);
				else
					it++;
			}
			
			//2.2找最大支撑值
			while(iteration_can.size()>0)
			{
				float tempmax=-100;
				int temph;
				int tempk;
				list<candidate>::iterator ita=iteration_can.begin();
				while(ita!=iteration_can.end())
				{
					float cur_support=(compatibility[i][j][ita->h][ita->k])*(ita->probability);
					if(cur_support>=tempmax)
					{
						tempmax=cur_support;
						temph=ita->h;
						tempk=ita->k;
					}
					ita++;
				}
				count++;
				support_sum+=tempmax;

				//2.3删除无效项
				ita=iteration_can.begin();
				while(ita!=iteration_can.end())
				{
					if((*ita).h==temph || (*ita).k==tempk)
						ita=iteration_can.erase(ita);
					else
						ita++;
				}
			}
			
			//3.更新
			if(count>0)
			{
				//probability[i][j]=probability[i][j]*(1.0+support_sum/count);
				float m_res=support_sum/finalcount;				
				/*if(this->iteration_times>5)
				{
					if(m_res>=0.0)
						m_res=pow(m_res,2.0);
					else
						m_res=-pow(-m_res,2.0);
				}	*/			
				probability[i][j]=probability[i][j]*(1.0+m_res);  //非线性放大效果:幂
			}
		}
	}

	////////////////////
	//归一化——对匹配概率进行归一化处理
	for (int si=0;si<script_count;si++)
	{
		float sum=0.0;
		for (int mj=0;mj<=model_count;mj++)
		{
			sum+=probability[si][mj];
		}

		//ASSERT(sum>0.0);

		for (unsigned int mj=0;mj<=model_count;mj++)
		{
			probability[si][mj]=probability[si][mj]/sum;
		}
	}

	//清除特别小的垃圾数据，再次归一化
	//1.竖直清除(利用连续升降计数加速)
	//1.1更新计数
	list<candidate>::iterator it=m_candidate.begin();
	while(it!=m_candidate.end())
	{
		if(probability[it->h][it->k] - it->probability > 0.005 || probability[it->h][it->k]>0.9999)  //增加计数或清零
		{
			if(updowncount[it->h][it->k]>=0)
				updowncount[it->h][it->k]++;
			else
				updowncount[it->h][it->k]=0;
		}
		else if(it->probability - probability[it->h][it->k] > 0.002)  //减少计数或清零
		{
			if(updowncount[it->h][it->k]<=0)
				updowncount[it->h][it->k]--;
			else
				updowncount[it->h][it->k]=0;
		}
		else  //清零
		{
			updowncount[it->h][it->k]=0;
		}

		it++;
	}
	//1.2清除
	for (int mj=0;mj<model_count;mj++)
	{
		bool yes=false;
		for (int si=0;si<script_count;si++)
		{  
			if (probability[si][mj]>0.63 && updowncount[si][mj] > 8)
			{
				yes=true;
				break;
			}
		}

		if (yes)
		{
			for (int si=0;si<script_count;si++)
			{
				if (probability[si][mj]<0.37 && updowncount[si][mj] < -8)
				{
					probability[si][mj]=0.0;
				}
			}
		}
	}


	//2.水平清除，并归一化
	for (unsigned int si=0;si<script_count;si++)
	{
		float sum=0.0;
		for (int mj=0;mj<=model_count;mj++)
		{
			if(probability[si][mj]<0.09)
				probability[si][mj]=0.0;
			sum+=probability[si][mj];
		}
		
		//ASSERT(sum>0.0);
		
		for (unsigned int mj=0;mj<=model_count;mj++)
		{
			probability[si][mj]=probability[si][mj]/sum;
		}
	}

	//增加迭代计数
	this->iteration_times++;
}

void RelaxationMatch::RelaxationExtraction()
{
	matchresult.clear();
	multimatchresult.clear();
	if(iteration_times<=0)
		return;
	
	//第一轮检测
	for (int mi=0;mi<script_count;mi++)
	{
		//找到这一行的最大匹配值
		float pr=probability[mi][0];
		int temph=mi;
		int tempk=0;
		for (int mj=0;mj<model_count;mj++)
		{
			if(probability[mi][mj]>pr)
			{
				pr=probability[mi][mj];
				tempk=mj;
			}

		}

		//检查这一列是否有干扰项
		if(pr>=0.89)  //高概率
		{
			bool harm=true;
			for (int mh=0;mh<script_count;mh++)
			{
				if(probability[mh][tempk]>0.281 && mh!=temph)
				{
					harm=false;
					break;
				}
			}

			if(harm)
			{
				candidate temp;
				temp.h=temph;
				temp.k=tempk;
				temp.probability=pr;
				matchresult.push_back(temp);
			}
		}
		else if (pr>=0.82)  //中概率
		{
			bool harm=true;
			for (int mh=0;mh<script_count;mh++)
			{
				if(probability[mh][tempk]>0.19 && mh!=temph)
				{
					harm=false;
					break;
				}
			}
			
			if(harm)
			{
				candidate temp;
				temp.h=temph;
				temp.k=tempk;
				temp.probability=pr;
				matchresult.push_back(temp);
			}
		}
		else if(pr>=0.73)  //低概率
		{
			bool harm=true;
			for (int mh=0;mh<script_count;mh++)
			{
				if(probability[mh][tempk]>0.133 && mh!=temph)
				{
					harm=false;
					break;
				}
			}
			
			if(harm)
			{
				candidate temp;
				temp.h=temph;
				temp.k=tempk;
				temp.probability=pr;
				matchresult.push_back(temp);
			}
		}
		else if(pr>=0.61) //小概率
		{
			bool harm=true;
			for (int mh=0;mh<script_count;mh++)
			{
				if(probability[mh][tempk]>0.02 && mh!=temph)
				{
					harm=false;
					break;
				}
			}
			
			if(harm)
			{
				candidate temp;
				temp.h=temph;
				temp.k=tempk;
				temp.probability=pr;
				matchresult.push_back(temp);
			}
		}
	}

	//第二轮检测
	while(true)
	{
		int newcount=0;  //是否找到了新的匹配结果，用于退出循环
		/////
		for (int mi=0;mi<script_count;mi++)
		{
			//是否已检测完
			bool f=false;
			list<candidate>::iterator it=matchresult.begin();
			while(it!=matchresult.end())
			{
				if(it->h==mi)
				{
					f=true;
					break;
				}
				it++;
			}

			if(f)
				continue;
			//继续检测,找到这一行的最大匹配值
			float pr=probability[mi][0];
			int temph=mi;
			int tempk=0;
			for (int mj=0;mj<model_count;mj++)
			{
				if(probability[mi][mj]>pr)
				{
					pr=probability[mi][mj];
					tempk=mj;
				}
				
			}
			//////
			if(pr>=0.67)
			{
				//找到这一列所有大于0.1的值
				list<int> con_mh;
				con_mh.clear();
				for (int mh=0;mh<script_count;mh++)
				{
					if(probability[mh][tempk]>0.1 && mh!=temph)
						con_mh.push_back(mh);
				}

				//逐一排除con_mh
				list<int>::iterator ith=con_mh.begin();
				while(ith!=con_mh.end())
				{
					bool f=false;
					list<candidate>::iterator it=matchresult.begin();
					while(it!=matchresult.end())
					{
						if(it->h==*ith)
						{
							f=true;
							break;
						}
						it++;
					}
					
					if(f)
						ith=con_mh.erase(ith);
					else
						break;
				}
				//
				if(con_mh.size()==0)
				{
					candidate temp;
					temp.h=temph;
					temp.k=tempk;
					temp.probability=pr;
					matchresult.push_back(temp);
					newcount++;
				}					
			}
		}
		/////
		if(newcount==0)
			break;
	}
}

/*由于手写笔画随意性较大，手写字自身的笔段切分算法不能保证绝对正确。
  所以，在松弛匹配后，调用此函数处理一些细碎的未匹配笔段。
  主要原理是：1.如果手写字中ABC属于同一笔画、AC已匹配、匹配笔段相接、B的长度很小且未匹配(判断夹角)。
              那么，将B均分后给AC。
			  2.如果手写字中AB属于同一笔画、A已匹配、B未匹配、AB夹角很小，合并后匹配效果无明显变化。
			  那么，将AB合并。
			  3.如果手写字中AB属于同一笔画、B已匹配、A未匹配、AB夹角很小，合并后匹配效果无明显变化。
			  那么，将AB合并。
  先调用RelaxationExtraction()，再调用此函数。
*/
void RelaxationMatch::processing_tiny_segments()
{
	if(matchresult.size()<2)
		return;

	//第1种情况
	/////////////////////////	
	for(list<candidate>::iterator ita=matchresult.begin(); ita!=matchresult.end(); ita++)
	{
		list<candidate>::iterator itb=ita;
		itb++;

		for (;itb!=matchresult.end();itb++)
		{
			int temph1=min(ita->h,itb->h);
			int tempk1=ita->k;
			int temph2=max(ita->h,itb->h);
			int tempk2=itb->k;
			
			string m_r=model.GetRelation(tempk1,tempk2);

			if (m_r[0]=='C' && (m_r[1]=='B' || m_r[1]=='E' )  && (m_r[2]=='B' ||m_r[2]=='E' ) &&
				script.InsameStroke(temph1,temph2)==true && temph2-temph1==2)
			{
				int temph=temph1+1;
				float len=script.getsegment(temph).len_;
				
				////////////////////////////////////////
				bool m_yes=false;
				list<candidate>::iterator itm=matchresult.begin();
				while(itm!=matchresult.end())
				{
					if (itm->h == temph)
					{
						m_yes=true;
						break;
					}
					itm++;
				}

				if(m_yes)
					continue;
				////////////////////////////////////////
				if (len<script.normal_size_/8.53) //归到一边
				{
					//处理手写字
					Segment h1=script.getsegment(temph1);
					Segment h2=script.getsegment(temph2);
					Segment h=script.getsegment(temph);
					int count= h.GetPointList().size();
					//////////////////////////////////////					
					float dis11=h.dirangle_ - (h1.dirangle_ + 180);
					float dis12=(h1.dirangle_ + 180) - h.dirangle_;
					while(dis11<0.0)
						dis11+=360.0;
					while(dis11>=360.0)
						dis11-=360.0;
					while(dis12<0.0)
						dis12+=360.0;
					while(dis12>=360.0)
						dis12-=360.0;
					float dis1 =min(dis11,dis12);

					float dis21=h2.dirangle_ - (h.dirangle_ + 180);
					float dis22=(h.dirangle_ + 180) - h2.dirangle_;
					while(dis21<0.0)
						dis21+=360.0;
					while(dis21>=360.0)
						dis21-=360.0;
					while(dis22<0.0)
						dis22+=360.0;
					while(dis22>=360.0)
						dis22-=360.0;
					float dis2 =min(dis21,dis22);

					if(h1.dirangle_<120 && h1.dirangle_>70  && (h.dirangle_<15 || h.dirangle_>345)   && h2.dirangle_<120 && h2.dirangle_>70)
					{
						for (int i=count-2; i>=0;i--)
						{
							h2.point_list_.push_front(h.GetPoint(i));
						}
					}
					else if (dis1>dis2)
					{
						for (int i=1;i<count;i++)
						{
							h1.point_list_.push_back(h.GetPoint(i));
						}
					} 
					else
					{
						for (int i=count-2; i>=0;i--)
						{
							h2.point_list_.push_front(h.GetPoint(i));
						}
					}					
					h1.UpdateAttribute();					
					h2.UpdateAttribute();
					///////////////////////////////////////
					script.Replacesegment(temph1,h1);
					script.Replacesegment(temph2,h2);
					script.removesegment(temph);
					
					//处理匹配结果
					list<candidate>::iterator itm=matchresult.begin();
					while(itm!=matchresult.end())
					{
						if (itm->h > temph)
						{
							itm->h -= 1;
						}
						itm++;
					}
				}
				else   if (len<script.normal_size_/6.8)//均分
				{
					//处理手写字
					Segment h1=script.getsegment(temph1);
					Segment h2=script.getsegment(temph2);
					Segment h=script.getsegment(temph);
					int count= h.GetPointList().size();

					GeometryTool gt;
					CCPoint tempp1=h1.GetPoint(h1.GetPointList().size()-1);
					CCPoint tempp2=h1.GetPoint(h1.GetPointList().size()-3);
					CCPoint tempp3=h.GetPoint(0);
					CCPoint tempp4=h.GetPoint(2);					
					float m_a1=gt.AngleInDegrees(tempp2,tempp1,true);
					float m_a2=gt.AngleInDegrees(tempp3,tempp4,true);
					float m_dis_a=max(m_a1,m_a2)-min(m_a1,m_a2);
					if(m_dis_a>180)
						m_dis_a=360-m_dis_a;

					tempp1=h.GetPoint(h.GetPointList().size()-1);
					tempp2=h.GetPoint(h.GetPointList().size()-3);
					tempp3=h2.GetPoint(0);
					tempp4=h2.GetPoint(2);					
					float m_b1=gt.AngleInDegrees(tempp2,tempp1,true);
					float m_b2=gt.AngleInDegrees(tempp3,tempp4,true);
					float m_dis_b=max(m_b1,m_b2)-min(m_b1,m_b2);
					if(m_dis_b>180)
						m_dis_b=360-m_dis_b;
					
					if(max(m_dis_a,m_dis_b)<33 && min(m_dis_a,m_dis_b)<23)
					{					
						//////////////
						for (int i=1;i<count/2;i++)
						{
							h1.point_list_.push_back(h.GetPoint(i));
						}
						for (unsigned int i=count-2; i>=count/2-1;i--)
						{
							h2.point_list_.push_front(h.GetPoint(i));
						}
						h1.UpdateAttribute();					
						h2.UpdateAttribute();
						///////////////////////////////////////
						script.Replacesegment(temph1,h1);
						script.Replacesegment(temph2,h2);
						script.removesegment(temph);
						
						//处理匹配结果
						list<candidate>::iterator itm=matchresult.begin();
						while(itm!=matchresult.end())
						{
							if (itm->h > temph)
							{
								itm->h -= 1;
							}
							itm++;
						}
					}
				}
			}
		}
	}
	
	///////////第2种情况
	for(list<candidate>::iterator ita=matchresult.begin(); ita!=matchresult.end(); ita++)
	{
		int temph1=ita->h;
		int tempk1=ita->k;
		int temph =temph1-1;
		
		if(temph<0 || temph>=script.segment_list_.size())
			continue;
		////////////////////////////////////////
		bool m_yes=false;
		list<candidate>::iterator itm=matchresult.begin();
		while(itm!=matchresult.end())
		{
			if (itm->h == temph)
			{
				m_yes=true;
				break;
			}
			itm++;
		}
		
		if(m_yes)
			continue;
		////////////////////////////////////////
		if(script.InsameStroke(temph1,temph)==true && script.IsNoiseSegment(temph)==false)
		{
			//夹角是否很小  
			Segment h1=script.getsegment(temph1);
			Segment h=script.getsegment(temph);
			Segment k1=model.getsegment(tempk1);

			GeometryTool gt;
			CCPoint tempp1=h.GetPoint(h.GetPointList().size()-1);
			CCPoint tempp2=h.GetPoint(h.GetPointList().size()>=7? h.GetPointList().size()-7:h.GetPointList().size()-3);
			CCPoint tempp3=h1.GetPoint(0);
			CCPoint tempp4=h1.GetPoint(h1.GetPointList().size()>=7?6:2);					
			float m_a1=gt.AngleInDegrees(tempp2,tempp1,true);
			float m_a2=gt.AngleInDegrees(tempp3,tempp4,true);
			float m_dis_a=max(m_a1,m_a2)-min(m_a1,m_a2);
			if(m_dis_a>180)
						m_dis_a=360-m_dis_a;

			//合并后是否匹配
			int count= h1.GetPointList().size();
			for (int i=1;i<count;i++)
			{
				h.point_list_.push_back(h1.GetPoint(i));
			}
			h.UpdateAttribute();

			float s1=gt.PathSimilarity(h1,k1,model.normal_size_,!model.isinstablesegment(tempk1),model_count>10);
			float s2=gt.PathSimilarity(h ,k1,model.normal_size_,!model.isinstablesegment(tempk1),model_count>10);
			if(m_dis_a<28 && s2>=s1)
			{
				//////////////////////////////////////
				script.Replacesegment(temph1,h);
				script.removesegment(temph);
				
				//处理匹配结果
				list<candidate>::iterator itm=matchresult.begin();
				while(itm!=matchresult.end())
				{
					if (itm->h > temph)
					{
						itm->h -= 1;
					}
					itm++;
				}
			}
		}
	}

	///////////第3种情况
	for(list<candidate>::iterator ita=matchresult.begin(); ita!=matchresult.end(); ita++)
	{
		int temph1=ita->h;
		int tempk1=ita->k;
		int temph =temph1+1;
		
		if(temph<0 || temph>=script.segment_list_.size())
			continue;
		////////////////////////////////////////
		bool m_yes=false;
		list<candidate>::iterator itm=matchresult.begin();
		while(itm!=matchresult.end())
		{
			if (itm->h == temph)
			{
				m_yes=true;
				break;
			}
			itm++;
		}
		
		if(m_yes)
			continue;
		////////////////////////////////////////
		////////////////////////////////////////
		if(script.InsameStroke(temph1,temph)==true && script.IsNoiseSegment(temph)==false)
		{
			//夹角是否很小  
			Segment h1=script.getsegment(temph1);
			Segment h=script.getsegment(temph);
			Segment k1=model.getsegment(tempk1);
			
			GeometryTool gt;
			CCPoint tempp1=h1.GetPoint(h1.GetPointList().size()-1);
			CCPoint tempp2=h1.GetPoint(h1.GetPointList().size()-3);
			CCPoint tempp3=h.GetPoint(0);
			CCPoint tempp4=h.GetPoint(2);					
			float m_a1=gt.AngleInDegrees(tempp2,tempp1,true);
			float m_a2=gt.AngleInDegrees(tempp3,tempp4,true);
			float m_dis_a=max(m_a1,m_a2)-min(m_a1,m_a2);
			if(m_dis_a>180)
						m_dis_a=360-m_dis_a;

			//合并后是否匹配
			int count= h1.GetPointList().size();
			for (int i=count-2;i>=0;i--)
			{
				h.point_list_.push_front(h1.GetPoint(i));
			}
			h.UpdateAttribute();
			
			float s1=gt.PathSimilarity(h1,k1,model.normal_size_,!model.isinstablesegment(tempk1),model_count>10);
			float s2=gt.PathSimilarity(h ,k1,model.normal_size_,!model.isinstablesegment(tempk1),model_count>10);

			if((m_dis_a<20 && s2>=s1) || (m_dis_a<29 && s2>=2*s1))
			{
				//////////////////////////////////////
				script.Replacesegment(temph1,h);
				script.removesegment(temph);
				
				//处理匹配结果
				list<candidate>::iterator itm=matchresult.begin();
				while(itm!=matchresult.end())
				{
					if (itm->h > temph)
					{
						itm->h -= 1;
					}
					itm++;
				}
			}
		}
	}
}


list<int> RelaxationMatch::find_nomatch_modelseg_in_both_result()
{
	list<int> m_res;
	///////////
	for (int k=0;k<model.segment_list_.size();k++)
	{
		bool yes1=true;
		bool yes2=true;
		/////////////////
		list<candidate>::iterator it=matchresult.begin();
		for(;it!=matchresult.end();it++)
		{
			if(it->k==k)
			{
				yes1=false;
				break;
			}
		}

		list<multicandidate>::iterator itm=multimatchresult.begin();
		for (;itm!=multimatchresult.end();itm++)
		{
			list<int>::iterator itk=itm->klist.begin();
			for (;itk!=itm->klist.end();itk++)
			{
				if(*itk == k)
				{
					yes2=false;
					break;
				}
			}
			if(itk!=itm->klist.end())
				break;
		}
		/////////////////
		if (yes1 && yes2)
		{
			m_res.push_back(k);
		}
	}
	///////////
	return m_res;
}

//在获得松弛匹配结果后，如果手写字中存在没有拐点的连笔情况，再调用此函数
void RelaxationMatch::Noturning_Connection_Extension()
{
	//1找到模板字中的未匹配笔段a，如果为空就返回
	list<int> nomatch_klist=find_nomatch_modelseg_in_both_result();
	if(nomatch_klist.size()==0)
		return;

	list<int>::iterator itmk=nomatch_klist.begin();
	for (;itmk!=nomatch_klist.end();itmk++)
	{
		int nomatch_k=*itmk;
		//2找到可能与其形成无拐点连笔的笔段b，如果没有就返回
		int tempk=model.find_noturning_connecttion_segment(nomatch_k);
		if(tempk==-1)
			continue;

		//3找到它所对应的手写笔段c,如果没有就返回
		int temph=Find_Match_for_Model_segment_in_single_result(tempk);
		if(temph==-1)
			continue;

		//4.对连笔的可能性进行判断		
		bool yes=false;
		
		////与b相离、与a不相离的笔段，是否与c不相离
		int count_ok=0;
		int count_no=0;

		list<int> connect_klist=model.find_first_apart_second_notapart_relation_segment(tempk,nomatch_k);
		for (list<int>::iterator itkl=connect_klist.begin(); itkl!=connect_klist.end();itkl++)
		{
			int connecth=Find_Match_for_Model_segment_in_single_result(*itkl);
			if(connecth!=-1)
			{
				if(script.is_apart_relation(temph,connecth))
					count_no+=1;
				else
					count_ok+=1;
			}			
		}
		
		if(count_no==0 && count_ok>0)
			yes=true;
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//补充判断
		bool yes1=false;
		int common_connect_k=model.find_common_connect_relation_segment(tempk,nomatch_k);
		int common_connect_h=Find_Match_for_Model_segment_in_single_result(common_connect_k);
		if(common_connect_h!=-1)
		{
			Segment h1=script.getsegment(temph);
			Segment h2=script.getsegment(common_connect_h);
			CCPoint pt1s=h1.GetPoint(0);
			CCPoint pt1e=h1.GetPoint(h1.point_list_.size()-1);
			CCPoint pt2s=h2.GetPoint(0);
			CCPoint pt2e=h2.GetPoint(h2.point_list_.size()-1);

			GeometryTool gt;
			bool inter=gt.judge_intersection(pt1s,pt1e,pt2s,pt2e);
			if (inter==true)//相交
			{
				//计算交点
				CCPoint interp=gt.segment_interpoint(pt1s,pt1e,pt2s,pt2e);
				float dis1=gt.pointDistance(pt1s,interp);
				float dis2=gt.pointDistance(pt1e,interp);
				float r=max(dis1,dis2)/min(dis1,dis2);
				if(r<1.4)
					yes1=true;
			}
			else
			{
				float disa=gt.pointToSegment(pt1s,pt1e,pt2s);
				float disb=gt.pointToSegment(pt1s,pt1e,pt2e);
				if (disa<script.normal_size_/34.0)
				{
					float dis1=gt.pointDistance(pt1s,pt2s);
					float dis2=gt.pointDistance(pt1e,pt2s);
					float r=max(dis1,dis2)/min(dis1,dis2);
					if(r<1.4)
						yes1=true;
				}
				else if(disb<script.normal_size_/34.0)
				{
					float dis1=gt.pointDistance(pt1s,pt2e);
					float dis2=gt.pointDistance(pt1e,pt2e);
					float r=max(dis1,dis2)/min(dis1,dis2);
					if(r<1.4)
						yes1=true;
				}
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		if(yes || yes1)//5.调整匹配结果
		{
			list<candidate>::iterator itr=matchresult.begin();
			for (;itr!=matchresult.end();itr++)
			{
				if(itr->k==tempk && itr->h==temph)
				{
					matchresult.erase(itr);
					break;
				}
			}
			////////////////
			multicandidate tempmulti;
			tempmulti.hlist.push_back(temph);
			tempmulti.klist.push_back(tempk);
			tempmulti.klist.push_back(nomatch_k);
			multimatchresult.push_back(tempmulti);
		}
	}
}

//在获得松弛匹配结果后，如果手写字中存在断笔情况，且断开的两笔在模板字中没有拐点，再调用此函数
void RelaxationMatch::Noturning_Broken_Extension()
{
	//1找到手写字中的未匹配笔段a，如果为空就返回
	list<int> nomatch_hlist=Find_Nomatch_normal_Scriptseg_in_both_reslult();
	if(nomatch_hlist.size()==0)
		return;

	list<int>::iterator itsh=nomatch_hlist.begin();
	for (;itsh!=nomatch_hlist.end();itsh++)
	{
		int nomatch_h=*itsh;
		//2找到可能与其形成无拐点断笔的笔段b，如果没有就返回
		int temph=script.find_noturning_connection_segment(nomatch_h);
		if(temph==-1)
			continue;

		//3找到它所对应的模板笔段c，如果没有就返回
		int tempk=Find_Match_for_script_segment_in_single_result(temph);
		if(tempk==-1)
			continue;

		//4.对断笔的可能性进行判断
		////是否存在一条与c相交的笔段，它所对应手写段与ab同时相接
		bool yes=false;
		list<int>  connect_klist=model.find_intersect_relation_segment(tempk);
		for (list<int>::iterator itk=connect_klist.begin();itk!=connect_klist.end();itk++)
		{
			int connecth=Find_Match_for_Model_segment_in_single_result(*itk);
			if(connecth!=-1 && !script.is_apart_relation(temph,connecth) && !script.is_apart_relation(nomatch_h,connecth))
			{
				yes=true;
				break;
			}
		}


		if(yes)//5.调整匹配结果
		{
			list<candidate>::iterator itr=matchresult.begin();
			for (;itr!=matchresult.end();itr++)
			{
				if(itr->k==tempk && itr->h==temph)
				{
					matchresult.erase(itr);
					break;
				}
			}
			////////////////
			multicandidate tempmulti;
			tempmulti.hlist.push_back(temph);
			tempmulti.hlist.push_back(nomatch_h);
			tempmulti.klist.push_back(tempk);
			multimatchresult.push_back(tempmulti);
		}
	}
}

list<int> RelaxationMatch::find_match_for_modelseg_in_multiresult(int k)
{
	list<int> m_res;
	////////////////
	list<multicandidate>::iterator itm=multimatchresult.begin();
	for (;itm!=multimatchresult.end();itm++)
	{
		list<int>::iterator itk=itm->klist.begin();
		if(itm->klist.size()==1 && *itk==k)
		{
			m_res.assign(itm->hlist.begin(),itm->hlist.end());
			break;
		}
	}
	////////////////
	return m_res;
}



//在获得松弛匹配结果后，如果需要将模板字中的某个未匹配笔段与手写字中的抖笔对应，再调用此函数
void RelaxationMatch::NoiseExtension_forsingle()
{
	/////////////
	list<int>  nomatchmodel =find_nomatch_modelseg_in_both_result();
	list<int>::iterator it=nomatchmodel.begin();
	for (;it!=nomatchmodel.end();it++)
	{
		//是否为模板字某个笔画的最后一个笔段，该笔画最少包含两段
		if(!model.Is_Last_segment_of_OneStroke(*it))
			continue;
		if(!model.InsameStroke(*it,*it-1))
			continue;		
		if(model.getsegment(*it).name_!="钩" && model.getsegment(*it).name_!="提")
			continue;

		//它的前一笔段已经与手写字中的一个笔段匹配
		int sh=Find_Match_for_Model_segment_in_single_result(*it-1);
		if(sh!=-1 )			                                                                     
		{		
			float sh_dir=script.getsegment(sh).dirangle_;
			float mk_dir=model.getsegment(*it-1).dirangle_;

			float dis=max(sh_dir,mk_dir)-min(sh_dir,mk_dir);
			if(dis>180)
				dis=360-dis;

			////////////
			if(dis<90 && sh+1>=script_count)
				continue;
			if(dis>90 && sh-1<0)
				continue;

			////////////
			if(dis<90 && script.InsameStroke(sh,sh+1) && script.IsNoiseSegment(sh+1))//相匹配的手写段与其下一段处于同一笔画，而且下一段为抖笔
			{
				//建立匹配，删除抖笔
				candidate temp;
				temp.h=sh+1;
				temp.k=*it;
				matchresult.push_back(temp);
				
				script.remove_from_noistlist(sh+1);
				continue;
			}
			else if(dis>90 && script.InsameStroke(sh,sh-1) && script.IsNoiseSegment(sh-1))
			{
				//建立匹配，删除抖笔
				candidate temp;
				temp.h=sh-1;
				temp.k=*it;
				matchresult.push_back(temp);
				
				script.remove_from_noistlist(sh-1);
				continue;
			}			
		}		
	}
	/////////////
}

void RelaxationMatch::NoiseExtension_formulti()
{
	/////////////
	list<int>  nomatchmodel =find_nomatch_modelseg_in_both_result();
	list<int>::iterator it=nomatchmodel.begin();
	for (;it!=nomatchmodel.end();it++)
	{
		//是否为模板字某个笔画的最后一个笔段，该笔画最少包含两段
		if(!model.Is_Last_segment_of_OneStroke(*it))
			continue;
		if(!model.InsameStroke(*it,*it-1))
			continue;		
		if(model.getsegment(*it).name_!="钩")
			continue;
		
		//它的前一笔段已经与手写字中的一组笔段匹配			
		list<int> shlist=find_match_for_modelseg_in_multiresult(*it-1);
		if(shlist.size()==0)
			continue;
		
		////判断方向
		int sh_end;
		list<int>::iterator itx=shlist.end();
		itx--;
		sh_end=*itx;
		int sh_beg=*(shlist.begin());

		float dir1=model.getsegment(*it-1).dirangle_;
		GeometryTool gt;
		float dir2=gt.AngleInDegrees(script.getsegment(sh_beg).GetPoint(0),
			                         script.getsegment(sh_end).GetPoint(script.getsegment(sh_end).point_list_.size()-1),true);
		float dis=max(dir1,dir2)-min(dir1,dir2);
		if (dis>180)
			dis=360-dis;		
				
		////////////
		if(dis<90 && sh_end+1>=script_count)
			continue;
		if(dis>90 && sh_beg-1<0)
			continue;
		
		////////////

		if (dis<90 && script.InsameStroke(sh_end,sh_end+1) && script.IsNoiseSegment(sh_end+1))
		{
			//建立匹配，删除抖笔
			candidate temp;
			temp.h=sh_end+1;
			temp.k=*it;
			matchresult.push_back(temp);
			
			script.remove_from_noistlist(sh_end+1);
			continue;
		}		
		else if(dis>90 && script.InsameStroke(sh_beg,sh_beg-1) && script.IsNoiseSegment(sh_beg-1))
		{
			//建立匹配，删除抖笔
			candidate temp;
			temp.h=sh_beg-1;
			temp.k=*it;
			matchresult.push_back(temp);
			
			script.remove_from_noistlist(sh_beg-1);
			continue;
		}
	}
	/////////////
}

void RelaxationMatch::wan_gou_match(int mk_wan,int sh_wan,int mk_gou,int sh_gou, list<multicandidate> &tempmultires)
{
	tempmultires.clear();
	if(sh_wan==-1 && sh_gou==-1)
		return;
	if (sh_wan==-1 && sh_gou!=-1)
	{
		multicandidate gou;
		gou.klist.push_back(mk_gou);
		gou.hlist.push_back(sh_gou);
		tempmultires.push_back(gou);
		return;
	}

	//对“弯”进行扩展
	//处理“勾”时，不考虑抖笔。
	////////////////////////////////////////////////////////////////////
	multicandidate tempm;
	tempm.klist.push_back(mk_wan);
	tempm.hlist.push_back(sh_wan);
	
	float sh_wan_dir=script.getsegment(sh_wan).dirangle_;	
	///////////////向模板字笔画的起点扩展

	int cur_h=sh_wan;
	bool m_yes=true;
	while(m_yes)
	{	
		m_yes=false;		
		if (sh_wan_dir<180)
		{
			cur_h--;
			
			if(cur_h<0 || cur_h>=script_count)
				continue;
			//查找
			bool same=script.InsameStroke(sh_wan,cur_h) & !(script.IsNoiseSegment(cur_h));		
			int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);			
			
			////
			if(same && cur_mk==-1)
			{
				float cur_dir=script.getsegment(cur_h).dirangle_;
				float cur_len=script.getsegment(cur_h).len_;
				if ((cur_dir<90 && cur_dir>20) || ((cur_dir<15 || cur_dir>350) && cur_len<50))
				{
					tempm.hlist.push_front(cur_h);
					m_yes=true;
				}				
			}
		}
		else if(sh_wan_dir>180)
		{
			cur_h++;
			if(cur_h<0 || cur_h>=script_count)
				continue;

			//查找
			bool same=script.InsameStroke(sh_wan,cur_h) & !(script.IsNoiseSegment(cur_h));			
			int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
			
			////
			if(same && cur_mk==-1)
			{
				float cur_dir=script.getsegment(cur_h).dirangle_;
				if (cur_dir<270 && cur_dir>200)
				{
					tempm.hlist.push_back(cur_h);
					m_yes=true;
				}				
			}
		}
	}

	//////////////向模板字笔画的终点扩展
	bool processed_gou=false;
	m_yes=true;
	cur_h=sh_wan;
	while(m_yes)
	{	
		m_yes=false;
		if (sh_wan_dir<180)
		{
			cur_h++;
			if(cur_h<0 || cur_h>=script_count)
				continue;
			//查找
			bool same=script.InsameStroke(sh_wan,cur_h) & !(script.IsNoiseSegment(cur_h));				
			int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
			
			////
			if(same && cur_mk==-1)
			{
				float cur_dir=script.getsegment(cur_h).dirangle_;
				if (cur_dir<160 && cur_dir>90)
				{
					tempm.hlist.push_back(cur_h);	
					m_yes=true;
				}				
				else if (cur_dir>=200)
				{
					multicandidate gou;
					gou.klist.push_back(mk_gou);
					gou.hlist.push_back(cur_h);
					tempmultires.push_back(gou);
					processed_gou=true;
				}				
				else if(cur_dir<200 && cur_dir>=160)
				{
					int next_cur_h=cur_h+1;

					bool next_same;
					int next_cur_mk;
					float next_dir;

					if(next_cur_h>=script_count)
						next_same=false;
					else
					{
						next_same=script.InsameStroke(sh_wan,next_cur_h) & !(script.IsNoiseSegment(next_cur_h));				
					    next_cur_mk=Find_Match_for_script_segment_in_single_result(next_cur_h);
						next_dir=script.getsegment(next_cur_h).dirangle_;
					}

					if(next_same && next_cur_mk==-1 && next_dir>180)
					{
						tempm.hlist.push_back(cur_h);
						multicandidate gou;
						gou.klist.push_back(mk_gou);
						gou.hlist.push_back(next_cur_h);
						tempmultires.push_back(gou);
						processed_gou=true;
					}
					else
					{
						multicandidate gou;
						gou.klist.push_back(mk_gou);
						gou.hlist.push_back(cur_h);
						tempmultires.push_back(gou);
						processed_gou=true;
					}
				}
			}
		}
		else if(sh_wan_dir>180)
		{
			cur_h--;
			if(cur_h<0 || cur_h>=script_count)
				continue;
			//查找
			bool same=script.InsameStroke(sh_wan,cur_h) & !(script.IsNoiseSegment(cur_h));			
			int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
			
			////
			if(same && cur_mk==-1)
			{
				float cur_dir=script.getsegment(cur_h).dirangle_;

				if(cur_dir>250 && cur_dir<340)
				{
					tempm.hlist.push_front(cur_h);
					m_yes=true;
				}
				else if(cur_dir>=340  || cur_dir<100)
				{
					multicandidate gou;
					gou.klist.push_back(mk_gou);
					gou.hlist.push_back(cur_h);
					tempmultires.push_back(gou);
					processed_gou=true;
				}
			}
		}		
	}

	//////////////////////////////
	if(!processed_gou && sh_gou!=-1)
	{
		list<int>::iterator it=tempm.hlist.begin();
		for (;it!=tempm.hlist.end();it++)
		{
			if(*it==sh_gou)
				break;
		}
		if (it==tempm.hlist.end())
		{
			multicandidate gou;
			gou.klist.push_back(mk_gou);
			gou.hlist.push_back(sh_gou);
			tempmultires.push_back(gou);
		}
	}
	
	//////////////////////////////
	tempmultires.push_back(tempm);
}



void RelaxationMatch::wo_gou_match(int mk_wo,int sh_wo, int mk_gou, int sh_gou, list<multicandidate> &tempmultires)
{
	tempmultires.clear();
	if(sh_wo==-1 && sh_gou==-1)
		return;

	if(sh_wo==-1 && sh_gou!=-1)
	{
		multicandidate gou;
		gou.klist.push_back(mk_gou);
		gou.hlist.push_back(sh_gou);
		tempmultires.push_back(gou);
		return;
	}
	//对“卧”进行扩展
	//处理“勾”时，不考虑抖笔。
	////////////////////////////////////////////////////////////////////
	multicandidate tempm;
	tempm.klist.push_back(mk_wo);
	tempm.hlist.push_back(sh_wo);
	
	float sh_wo_dir=script.getsegment(sh_wo).dirangle_;
	///////////////向模板字的起点扩展
	int cur_h=sh_wo;
	bool m_yes=true;
	while(m_yes)
	{	
		m_yes=false;		
		if (sh_wo_dir<90 || sh_wo_dir>270)
		{
			cur_h--;
			if(cur_h<0 || cur_h>=script_count)
				continue;
			//查找
			bool same=script.InsameStroke(sh_wo,cur_h) & !(script.IsNoiseSegment(cur_h));			
					
			////
			if(same)
			{
				//转角是否满足要求
				float cur_ang=script.getsegment(cur_h).dirangle_;				
				
				//合并后方向是否满足要求
				CCPoint p1=script.getsegment(cur_h).GetPoint(0);
				CCPoint p2=script.getsegment(sh_wo).GetPoint(script.getsegment(sh_wo).GetPointList().size()-1);
				GeometryTool gt;
				float sumdir1=gt.AngleInDegrees(p1,p2,true);
								
				if(cur_ang<160 && (sumdir1<60  || sumdir1>300))
				{
					int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
					if (cur_mk!=-1)
					{
						list<candidate>::iterator itr=matchresult.begin();
						for (;itr!=matchresult.end();itr++)
						{
							if(itr->h==cur_h)
							{
								matchresult.erase(itr);	
								break;
							}
						}
					}
					tempm.hlist.push_front(cur_h);
					m_yes=true;
				}
			}
		}
		else if(sh_wo_dir>90 && sh_wo_dir<270)
		{
			cur_h++;
			if(cur_h<0 || cur_h>=script_count)
				continue;
			//查找
			bool same=script.InsameStroke(sh_wo,cur_h) & !(script.IsNoiseSegment(cur_h));			
		
			////
			if(same)
			{
				//转角是否满足要求
				float cur_ang=script.getsegment(cur_h).dirangle_;				
				
				//合并后方向是否满足要求
				CCPoint p2=script.getsegment(sh_wo).GetPoint(0);
				CCPoint p1=script.getsegment(cur_h).GetPoint(script.getsegment(cur_h).GetPointList().size()-1);
				GeometryTool gt;
				float sumdir1=gt.AngleInDegrees(p1,p2,true);

				if((cur_ang<340 && cur_ang>180) && (sumdir1<60  || sumdir1>300))
				{
					int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
					if (cur_mk!=-1)
					{
						list<candidate>::iterator itr=matchresult.begin();
						for (;itr!=matchresult.end();itr++)
						{
							if(itr->h==cur_h)
							{
								matchresult.erase(itr);	
								break;
							}
						}
					}
					tempm.hlist.push_back(cur_h);
					m_yes=true;
				}
			}
		}
	}

	///////////////向模板字的终点扩展
	bool processed_gou=false;
	m_yes=true;
	cur_h=sh_wo;
	while(m_yes)
	{	
		m_yes=false;
		if (sh_wo_dir<90 || sh_wo_dir>270)
		{
			cur_h++;
			if(cur_h<0 || cur_h>=script_count)
				continue;
			//查找
			bool same=script.InsameStroke(sh_wo,cur_h) & !(script.IsNoiseSegment(cur_h));
			////
			if(same)
			{
				float cur_ang=script.getsegment(cur_h).dirangle_;
				if (cur_ang>180 && cur_ang<270)
				{
					int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
					if (cur_mk!=-1)
					{
						list<candidate>::iterator itr=matchresult.begin();
						for (;itr!=matchresult.end();itr++)
						{
							if(itr->h==cur_h)
							{
								matchresult.erase(itr);	
								break;
							}
						}
					}
					multicandidate gou;
					gou.klist.push_back(mk_gou);
					gou.hlist.push_back(cur_h);
					tempmultires.push_back(gou);
					processed_gou=true;
				}
				else if(cur_ang>0 && cur_ang<30)
				{
					int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
					if (cur_mk!=-1)
					{
						list<candidate>::iterator itr=matchresult.begin();
						for (;itr!=matchresult.end();itr++)
						{
							if(itr->h==cur_h)
							{
								matchresult.erase(itr);	
								break;
							}
						}
					}
					tempm.hlist.push_back(cur_h);
					m_yes=true;
				}
				else if(cur_ang>=270 && cur_ang<=360)
				{
					int next_cur_h=cur_h+1;
					bool next_same;
					float next_dir;
					if(next_cur_h>=script_count)
						next_same=false;
					else
					{
						next_same=script.InsameStroke(sh_wo,next_cur_h) & !(script.IsNoiseSegment(next_cur_h));
						next_dir=script.getsegment(next_cur_h).dirangle_;
					}

					if (next_same && (next_dir>180 && next_dir<=cur_ang))
					{
						int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
						if (cur_mk!=-1)
						{
							list<candidate>::iterator itr=matchresult.begin();
							for (;itr!=matchresult.end();itr++)
							{
								if(itr->h==cur_h)
								{
									matchresult.erase(itr);	
									break;
								}
							}
						}
						int next_mk=Find_Match_for_script_segment_in_single_result(next_cur_h);
						if (next_mk!=-1)
						{
							list<candidate>::iterator itr=matchresult.begin();
							for (;itr!=matchresult.end();itr++)
							{
								if(itr->h==next_cur_h)
								{
									matchresult.erase(itr);	
									break;
								}
							}
						}
						
						tempm.hlist.push_back(cur_h);
						multicandidate gou;
						gou.klist.push_back(mk_gou);
						gou.hlist.push_back(next_cur_h);
						tempmultires.push_back(gou);
						processed_gou=true;
					}
					else
					{
						int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
						if (cur_mk!=-1)
						{
							list<candidate>::iterator itr=matchresult.begin();
							for (;itr!=matchresult.end();itr++)
							{
								if(itr->h==cur_h)
								{
									matchresult.erase(itr);	
									break;
								}
							}
						}
						multicandidate gou;
						gou.klist.push_back(mk_gou);
						gou.hlist.push_back(cur_h);
						tempmultires.push_back(gou);
						processed_gou=true;
					}
				}
			}
		}
		else if(sh_wo_dir>90 && sh_wo_dir<270)
		{
			cur_h--;
			if(cur_h<0 || cur_h>=script_count)
				continue;
			//查找
			bool same=script.InsameStroke(sh_wo,cur_h) & !(script.IsNoiseSegment(cur_h));
			////
			if(same)
			{
				float cur_ang=script.getsegment(cur_h).dirangle_;
				if (cur_ang>0 && cur_ang<90)
				{
					int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
					if (cur_mk!=-1)
					{
						list<candidate>::iterator itr=matchresult.begin();
						for (;itr!=matchresult.end();itr++)
						{
							if(itr->h==cur_h)
							{
								matchresult.erase(itr);	
								break;
							}
						}
					}
					multicandidate gou;
					gou.klist.push_back(mk_gou);
					gou.hlist.push_back(cur_h);
					tempmultires.push_back(gou);
					processed_gou=true;
				} 
				else if(cur_ang>180 && cur_ang<210)
				{
					int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
					if (cur_mk!=-1)
					{
						list<candidate>::iterator itr=matchresult.begin();
						for (;itr!=matchresult.end();itr++)
						{
							if(itr->h==cur_h)
							{
								matchresult.erase(itr);	
								break;
							}
						}
					}
					tempm.hlist.push_front(cur_h);
					m_yes=true;
				}
				else if(cur_ang>90 && cur_ang<180)
				{
					int next_cur_h=cur_h-1;
					bool next_same;
					float next_dir;
					if(next_cur_h<0)
						next_same=false;
					else
					{
						next_same=script.InsameStroke(sh_wo,next_cur_h) & !(script.IsNoiseSegment(next_cur_h));
						next_dir=script.getsegment(next_cur_h).dirangle_;
					}

					if (next_same && (next_dir>0 && next_dir<=cur_ang))
					{
						int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
						if (cur_mk!=-1)
						{
							list<candidate>::iterator itr=matchresult.begin();
							for (;itr!=matchresult.end();itr++)
							{
								if(itr->h==cur_h)
								{
									matchresult.erase(itr);	
									break;
								}
							}
						}
						int next_mk=Find_Match_for_script_segment_in_single_result(next_cur_h);
						if (next_mk!=-1)
						{
							list<candidate>::iterator itr=matchresult.begin();
							for (;itr!=matchresult.end();itr++)
							{
								if(itr->h==next_cur_h)
								{
									matchresult.erase(itr);	
									break;
								}
							}
						}
						
						tempm.hlist.push_front(cur_h);
						multicandidate gou;
						gou.klist.push_back(mk_gou);
						gou.hlist.push_back(next_cur_h);
						tempmultires.push_back(gou);
						processed_gou=true;
					} 
					else
					{
						int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
						if (cur_mk!=-1)
						{
							list<candidate>::iterator itr=matchresult.begin();
							for (;itr!=matchresult.end();itr++)
							{
								if(itr->h==cur_h)
								{
									matchresult.erase(itr);	
									break;
								}
							}
						}
						multicandidate gou;
						gou.klist.push_back(mk_gou);
						gou.hlist.push_back(cur_h);
						tempmultires.push_back(gou);
						processed_gou=true;
					}
				}	
			}
		}
	}

	////////////////
	if(!processed_gou && sh_gou!=-1)
	{
		list<int>::iterator it=tempm.hlist.begin();
		for (;it!=tempm.hlist.end();it++)
		{
			if(*it==sh_gou)
				break;
		}
		if (it==tempm.hlist.end())
		{
			multicandidate gou;
			gou.klist.push_back(mk_gou);
			gou.hlist.push_back(sh_gou);
			tempmultires.push_back(gou);
		}
	}
	////////////////
	tempmultires.push_back(tempm);
}

void RelaxationMatch::xie_gou_match(int mk_xie, int sh_xie, int mk_gou, int sh_gou, list<multicandidate> &tempmultires)
{
	tempmultires.clear();
	if(sh_xie==-1 && sh_gou==-1)
		return;

	if(sh_xie==-1 && sh_gou!=-1)
	{
		multicandidate gou;
		gou.klist.push_back(mk_gou);
		gou.hlist.push_back(sh_gou);
		tempmultires.push_back(gou);
		return;
	}

	//对“斜”进行扩展
	//处理“勾”时，不考虑抖笔。
	////////////////////////////////////////////////////////////////////
	multicandidate tempm;
	tempm.klist.push_back(mk_xie);
	tempm.hlist.push_back(sh_xie);
	
	float sh_xie_dir=script.getsegment(sh_xie).dirangle_;

	///////////////向模板字笔画的起点扩展
	int cur_h=sh_xie;
	bool m_yes=true;
	while(m_yes)
	{	
		m_yes=false;
		if (sh_xie_dir<135 || sh_xie_dir>315)//正向书写
		{
			cur_h--;
			if(cur_h<0 || cur_h>=script_count)
				continue;
			//查找
			bool same=script.InsameStroke(sh_xie,cur_h) & !(script.IsNoiseSegment(cur_h));			
			if(Find_Match_for_script_segment_in_single_result(cur_h)!=-1)
				same=false;
			////
			if(same)
			{
				//转角是否满足要求
				float cur_ang=script.getsegment(cur_h).dirangle_;				
				
				//合并后方向是否满足要求
				CCPoint p1=script.getsegment(cur_h).GetPoint(0);
				CCPoint p2=script.getsegment(sh_xie).GetPoint(script.getsegment(sh_xie).GetPointList().size()-1);
				GeometryTool gt;
				float sumdir1=gt.AngleInDegrees(p1,p2,true);
				
				if((cur_ang<120 && cur_ang>sh_xie_dir) && (sumdir1<90  && sumdir1>20))
				{					
					tempm.hlist.push_front(cur_h);
					m_yes=true;
				}
			}
		}
		else if(sh_xie_dir>135 && sh_xie_dir<315)  //逆向书写
		{
			cur_h++;
			if(cur_h<0 || cur_h>=script_count)
				continue;
			//查找
			bool same=script.InsameStroke(sh_xie,cur_h) & !(script.IsNoiseSegment(cur_h));			
			if(Find_Match_for_script_segment_in_single_result(cur_h)!=-1)
				same=false;

			////
			if(same)
			{
			    //转角是否满足要求
			    float cur_ang=script.getsegment(cur_h).dirangle_;				
			  
			    //合并后方向是否满足要求
				CCPoint p2=script.getsegment(sh_xie).GetPoint(0);
				CCPoint p1=script.getsegment(cur_h).GetPoint(script.getsegment(cur_h).GetPointList().size()-1);
				GeometryTool gt;
				float sumdir1=gt.AngleInDegrees(p1,p2,true);
				
				if((cur_ang<300 && cur_ang>sh_xie_dir) && (sumdir1<90  && sumdir1>20))
				{				  
					tempm.hlist.push_back(cur_h);
					m_yes=true;
				}
			}			
		}
	}

	///////////////向模板字笔画的终点扩展
	bool processed_gou=false;
	m_yes=true;
	cur_h=sh_xie;
	while(m_yes)
	{	
		m_yes=false;
		if (sh_xie_dir<135 || sh_xie_dir>315)////正向书写
		{
			cur_h++;			
			if(cur_h<0 || cur_h>=script_count)
				continue;
			//查找
			bool same=script.InsameStroke(sh_xie,cur_h) & !(script.IsNoiseSegment(cur_h));
			////
			if(same)
			{
				float cur_ang=script.getsegment(cur_h).dirangle_;
				if (cur_ang>180 && cur_ang<300)
				{
					int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
					if (cur_mk!=-1)
					{
						list<candidate>::iterator itr=matchresult.begin();
						for (;itr!=matchresult.end();itr++)
						{
							if(itr->h==cur_h)
							{
								matchresult.erase(itr);	
								break;
							}
						}
					}
					multicandidate gou;
					gou.klist.push_back(mk_gou);
					gou.hlist.push_back(cur_h);
					tempmultires.push_back(gou);
					processed_gou=true;
				}
				else if(cur_ang>40 && cur_ang<90)
				{
					int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
					if (cur_mk!=-1)
					{
						list<candidate>::iterator itr=matchresult.begin();
						for (;itr!=matchresult.end();itr++)
						{
							if(itr->h==cur_h)
							{
								matchresult.erase(itr);	
								break;
							}
						}
					}
					tempm.hlist.push_back(cur_h);
					m_yes=true;
				}
				else if(cur_ang<=40 || cur_ang>=300)
				{
					int next_cur_h=cur_h+1;
					bool next_same;
					float next_dir;
					if(next_cur_h>=script_count)
						next_same=false;
					else
					{
						next_same=script.InsameStroke(sh_xie,next_cur_h) & !(script.IsNoiseSegment(next_cur_h));
						next_dir=script.getsegment(next_cur_h).dirangle_;
					}

					if (next_same && (next_dir>180 && next_dir<=330))
					{
						int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
						if (cur_mk!=-1)
						{
							list<candidate>::iterator itr=matchresult.begin();
							for (;itr!=matchresult.end();itr++)
							{
								if(itr->h==cur_h)
								{
									matchresult.erase(itr);	
									break;
								}
							}
						}
						int next_mk=Find_Match_for_script_segment_in_single_result(next_cur_h);
						if (next_mk!=-1)
						{
							list<candidate>::iterator itr=matchresult.begin();
							for (;itr!=matchresult.end();itr++)
							{
								if(itr->h==next_cur_h)
								{
									matchresult.erase(itr);	
									break;
								}
							}
						}
						
						tempm.hlist.push_back(cur_h);
						multicandidate gou;
						gou.klist.push_back(mk_gou);
						gou.hlist.push_back(next_cur_h);
						tempmultires.push_back(gou);
						processed_gou=true;
					}
					else
					{
						int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
						if (cur_mk!=-1)
						{
							list<candidate>::iterator itr=matchresult.begin();
							for (;itr!=matchresult.end();itr++)
							{
								if(itr->h==cur_h)
								{
									matchresult.erase(itr);	
									break;
								}
							}
						}
						multicandidate gou;
						gou.klist.push_back(mk_gou);
						gou.hlist.push_back(cur_h);
						tempmultires.push_back(gou);
						processed_gou=true;
					}
				}
			}
			
		}
		else if(sh_xie_dir>135 && sh_xie_dir<315)  //逆向书写
		{
			cur_h--;
			if(cur_h<0 || cur_h>=script_count)
				continue;
			//查找
			bool same=script.InsameStroke(sh_xie,cur_h) & !(script.IsNoiseSegment(cur_h));
			////
			if(same)
			{
				float cur_ang=script.getsegment(cur_h).dirangle_;
				if (cur_ang>0 && cur_ang<120)
				{
					int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
					if (cur_mk!=-1)
					{
						list<candidate>::iterator itr=matchresult.begin();
						for (;itr!=matchresult.end();itr++)
						{
							if(itr->h==cur_h)
							{
								matchresult.erase(itr);	
								break;
							}
						}
					}
					multicandidate gou;
					gou.klist.push_back(mk_gou);
					gou.hlist.push_back(cur_h);
					tempmultires.push_back(gou);
					processed_gou=true;
				} 
				else if(cur_ang>220 && cur_ang<270)
				{
					int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
					if (cur_mk!=-1)
					{
						list<candidate>::iterator itr=matchresult.begin();
						for (;itr!=matchresult.end();itr++)
						{
							if(itr->h==cur_h)
							{
								matchresult.erase(itr);	
								break;
							}
						}
					}
					tempm.hlist.push_front(cur_h);
					m_yes=true;
				}
				else if(cur_ang>120 && cur_ang<220)
				{
					int next_cur_h=cur_h-1;
					bool next_same;
					float next_dir;
					if(next_cur_h<0)
						next_same=false;
					else
					{
						next_same=script.InsameStroke(sh_xie,next_cur_h) & !(script.IsNoiseSegment(next_cur_h));
						next_dir=script.getsegment(next_cur_h).dirangle_;
					}

					if (next_same && (next_dir>0 && next_dir<=cur_ang))
					{
						int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
						if (cur_mk!=-1)
						{
							list<candidate>::iterator itr=matchresult.begin();
							for (;itr!=matchresult.end();itr++)
							{
								if(itr->h==cur_h)
								{
									matchresult.erase(itr);	
									break;
								}
							}
						}
						int next_mk=Find_Match_for_script_segment_in_single_result(next_cur_h);
						if (next_mk!=-1)
						{
							list<candidate>::iterator itr=matchresult.begin();
							for (;itr!=matchresult.end();itr++)
							{
								if(itr->h==next_cur_h)
								{
									matchresult.erase(itr);	
									break;
								}
							}
						}
						
						tempm.hlist.push_front(cur_h);
						multicandidate gou;
						gou.klist.push_back(mk_gou);
						gou.hlist.push_back(next_cur_h);
						tempmultires.push_back(gou);
						processed_gou=true;
					} 
					else
					{
						int cur_mk=Find_Match_for_script_segment_in_single_result(cur_h);
						if (cur_mk!=-1)
						{
							list<candidate>::iterator itr=matchresult.begin();
							for (;itr!=matchresult.end();itr++)
							{
								if(itr->h==cur_h)
								{
									matchresult.erase(itr);	
									break;
								}
							}
						}
						multicandidate gou;
						gou.klist.push_back(mk_gou);
						gou.hlist.push_back(cur_h);
						tempmultires.push_back(gou);
						processed_gou=true;
					}
				}	
			}
			
		}
	}

	////////////////
	if(!processed_gou && sh_gou!=-1)
	{
		list<int>::iterator it=tempm.hlist.begin();
		for (;it!=tempm.hlist.end();it++)
		{
			if(*it==sh_gou)
				break;
		}
		if (it==tempm.hlist.end())
		{
			multicandidate gou;
			gou.klist.push_back(mk_gou);
			gou.hlist.push_back(sh_gou);
			tempmultires.push_back(gou);
		}
	}
	////////////////
	tempmultires.push_back(tempm);
}

//处理“弯钩”
void RelaxationMatch::process_stroke_wan_gou()
{
	//1.判断模板字中是否存在弯钩，如果存在进行调整.如果不存在，返回.
	list<Stroke>::iterator it=model.stroke_list_.begin();
	Stroke wan_gou;
	while(it!=model.stroke_list_.end())
	{
		if(it->name_=="弯钩")
		{
			wan_gou=*it;
			break;
		}
		it++;
	}
	if (it==model.stroke_list_.end())
		return;
	
	//2.找到初始匹配
	int mk_wan=wan_gou.start_index_;
	int sh_wan=Find_Match_for_Model_segment_in_single_result(mk_wan);
	int sh_gou=Find_Match_for_Model_segment_in_single_result(mk_wan+1);
	
	//3.删除
	if (sh_wan!=-1)
	{
		for (int x=mk_wan; x<mk_wan+2;x++)
		{
			list<candidate>::iterator itr=matchresult.begin();
			for (;itr!=matchresult.end();itr++)
			{
				if(itr->k==x)
				{
					matchresult.erase(itr);	
					break;
				}
			}
		}
	}
	else
		return;
	
	//4.扩展
	list<multicandidate>  tempmultires;
	wan_gou_match(mk_wan,sh_wan, mk_wan+1, sh_gou, tempmultires);
	
	
	//5.合并
	list<candidate>  tempres=merge_script_seg_for_multimatch(tempmultires);		
	//////////////////////////
	if (tempres.size()!=0)
	{
		list<candidate>::iterator it=tempres.begin();
		while(it!=tempres.end())
		{
			matchresult.push_back(*it);
			it++;
		}
	}
}

//处理“卧勾”
void RelaxationMatch::process_stroke_wo_gou()
{	
	//1.判断模板字中是否存在卧勾，如果存在进行调整.如果不存在，返回.
	list<Stroke>::iterator it=model.stroke_list_.begin();
	Stroke wo_gou;
	while(it!=model.stroke_list_.end())
	{
		if(it->name_=="卧钩")
		{
			wo_gou=*it;
			break;
		}
		it++;
	}
	if (it==model.stroke_list_.end())
		return;

	//2.找到初始匹配
	int mk_wo=wo_gou.start_index_;
	int sh_wo=Find_Match_for_Model_segment_in_single_result(mk_wo);
	int sh_gou=Find_Match_for_Model_segment_in_single_result(mk_wo+1);

	//3.删除
	if (sh_wo!=-1)
	{
		for (int x=mk_wo; x<mk_wo+2;x++)
		{
			list<candidate>::iterator itr=matchresult.begin();
			for (;itr!=matchresult.end();itr++)
			{
				if(itr->k==x)
				{
					matchresult.erase(itr);	
					break;
				}
			}
		}
	}
	else
		return;

	//4.扩展
	list<multicandidate>  tempmultires;
	wo_gou_match(mk_wo,sh_wo, mk_wo+1, sh_gou, tempmultires);
	
	
	//5.合并
	list<candidate>  tempres=merge_script_seg_for_multimatch(tempmultires);		
	//////////////////////////
	if (tempres.size()!=0)
	{
		list<candidate>::iterator it=tempres.begin();
		while(it!=tempres.end())
		{
			matchresult.push_back(*it);
			it++;
		}
	}

}

//处理斜勾
void RelaxationMatch::process_stroke_xie_gou()
{
	//1.判断模板字中是否存在斜钩，如果存在进行调整.如果不存在，返回.
	list<Stroke>::iterator it=model.stroke_list_.begin();
	Stroke xie_gou;
	while(it!=model.stroke_list_.end())
	{
		if(it->name_=="斜钩")
		{
			xie_gou=*it;
			break;
		}
		it++;
	}
	if (it==model.stroke_list_.end())
		return;

	//2.找到初始匹配
	int mk_xie=xie_gou.start_index_;
	int sh_xie=Find_Match_for_Model_segment_in_single_result(mk_xie);
	int sh_gou=Find_Match_for_Model_segment_in_single_result(mk_xie+1);

	//3.删除
	if (sh_xie!=-1)
	{
		for (int x=mk_xie; x<mk_xie+2;x++)
		{
			list<candidate>::iterator itr=matchresult.begin();
			for (;itr!=matchresult.end();itr++)
			{
				if(itr->k==x)
				{
					matchresult.erase(itr);	
					break;
				}
			}
		}
	}
	else
		return;

	//4.扩展
	list<multicandidate>  tempmultires;
	xie_gou_match(mk_xie,sh_xie, mk_xie+1, sh_gou, tempmultires);

	//5.合并
	list<candidate>  tempres=merge_script_seg_for_multimatch(tempmultires);		
	//////////////////////////
	if (tempres.size()!=0)
	{
		list<candidate>::iterator it=tempres.begin();
		while(it!=tempres.end())
		{
			matchresult.push_back(*it);
			it++;
		}
	}

}

//处理横折弯撇"走之旁"
void RelaxationMatch::process_stroke_heng_zhe_wan_pie()
{
	//1.判断模板字中是否存在横折弯撇"走之旁"，如果存在进行调整.如果不存在，返回.
	list<Stroke>::iterator it=model.stroke_list_.begin();
	Stroke heng_zhe_wan_pie;
	while(it!=model.stroke_list_.end())
	{
		if(it->name_=="横折弯撇")
		{
			heng_zhe_wan_pie=*it;
			break;
		}
		it++;
	}
	if (it==model.stroke_list_.end())
		return;

	////////////////////////////////////////////////
	//2.检查条件：a模板笔画“横折弯撇”中所有已匹配笔段都来自同一手写笔画,
	//            b应该匹配至少2段，第一段必须匹配；
	//            c第一笔在手写字中为起笔。
	//            d手写字中，从第一匹配笔段开始，该笔画还有至少三个连续笔段。
	//            e这些段不与其他模板笔段匹配。

	
	bool m_same=true;
	int m_seg1=heng_zhe_wan_pie.start_index_;
	int m_seg2=heng_zhe_wan_pie.start_index_+1;
	int m_seg3=heng_zhe_wan_pie.start_index_+2;
	int m_seg4=heng_zhe_wan_pie.start_index_+3;
	int s_seg1=Find_Match_for_Model_segment_in_single_result(m_seg1);
	int s_seg2=Find_Match_for_Model_segment_in_single_result(m_seg2);
	int s_seg3=Find_Match_for_Model_segment_in_single_result(m_seg3);
	int s_seg4=Find_Match_for_Model_segment_in_single_result(m_seg4);
	
	//2.0如果没有一个匹配笔段，则返回。
	if(s_seg1==-1 && s_seg2 ==-1 && s_seg3==-1 && s_seg4 ==-1)
		return;
	
	//2.1 模板笔画“横折弯撇”中所有已匹配笔段都来自同一手写笔画
	if (s_seg1!=-1 && s_seg2 !=-1)	
		m_same &= script.InsameStroke(s_seg1,s_seg2);
	if (s_seg1!=-1 && s_seg3 !=-1)	
		m_same &= script.InsameStroke(s_seg1,s_seg3);
	if (s_seg1!=-1 && s_seg4 !=-1)	
		m_same &= script.InsameStroke(s_seg1,s_seg4);
	if (s_seg2!=-1 && s_seg3 !=-1)	
		m_same &= script.InsameStroke(s_seg2,s_seg3);
	if (s_seg2!=-1 && s_seg4 !=-1)	
		m_same &= script.InsameStroke(s_seg2,s_seg4);
	if (s_seg3!=-1 && s_seg4 !=-1)	
		m_same &= script.InsameStroke(s_seg3,s_seg4);
	
	//2.2 总体匹配至少2段，第一段必须匹配
	if (s_seg1 == -1 || (s_seg2==-1 && s_seg3 ==-1 && s_seg4 ==-1))
		m_same=false;
	
	
	//2.3第一笔在手写字中应该为起笔
	if((s_seg2!=-1 && s_seg1>s_seg2) || (s_seg3!=-1 && s_seg1>s_seg3) || (s_seg4!=-1 && s_seg1>s_seg4))
		m_same=false;
	
	int m_segcount=0;
	if(m_same)
	{
		int tempcount=0;
		//2.4手写字中，从第一匹配笔段开始，该笔画还有至少三个笔段。
		list<Stroke>::iterator its=script.stroke_list_.begin();
		for(;its!=script.stroke_list_.end();its++)
		{
			if(its->start_index_ <= s_seg1 && its->start_index_+its->seg_count_ -3>=s_seg1 )
			{
				tempcount=its->start_index_+its->seg_count_ -s_seg1;
				break;
			}
			
		}
		
		if (its==script.stroke_list_.end())
			m_same=false;		
		//2.5这些段涵盖了s_seg1——s_seg4,且应该不与其他模板笔段匹配。
		else
		{
			int w;
			for (w=0; w<tempcount; w++)
			{
				int tempk=Find_Match_for_script_segment_in_single_result(s_seg1+w);
				if(tempk!=-1 && (tempk<m_seg1 || tempk>m_seg4))
				{
					m_segcount=w;
					break;
				}
			}
			if(w == tempcount)
				m_segcount=tempcount;
			///////////////////////////////////////
			if(m_segcount<3 || s_seg1+m_segcount <= max(max(s_seg1,s_seg2),max(s_seg3,s_seg4)))
				m_same=false;
			else
			{			
				///////////////////////////////////
				for (int x=s_seg1; x<s_seg1+m_segcount;x++)
				{
					list<candidate>::iterator itr=matchresult.begin();
					for (;itr!=matchresult.end();itr++)
					{
						if(itr->h==x)
						{
							matchresult.erase(itr);	
							break;
						}
					}
				}
			}
		}
	}

	//3.满足整体调整条件。开始匹配
	if(m_same)
	{
		//3.1删除已有匹配结果
		for (int mk=0;mk<4;mk++)
		{
			list<candidate>::iterator itr=matchresult.begin();
			for (;itr!=matchresult.end();itr++)
			{
				if(itr->k==m_seg1+mk)
				{
					matchresult.erase(itr);	
					break;
				}
			}
		}
		//3.2整体遍历（不包括最末尾的抖笔）
		list<candidate>  tempres;
		list<multicandidate>  tempmultires;
		heng_zhe_wan_pie_match_1(s_seg1, m_seg1, m_segcount, tempres, tempmultires);
		
		//3.3笔段合并
		if (tempres.size()!=0)
		{
			list<candidate>::iterator it=tempres.begin();
			while(it!=tempres.end())
			{
				matchresult.push_back(*it);
				it++;
			}
		}
		///////////////////////////////
		tempres.clear();
		tempres=merge_script_seg_for_multimatch(tempmultires);		
		//////////////////////////
		if (tempres.size()!=0)
		{
			list<candidate>::iterator it=tempres.begin();
			while(it!=tempres.end())
			{
				matchresult.push_back(*it);
				it++;
			}
		}
	}
}

//在获得松弛匹配结果后，如果存在耳刀旁、斜勾、卧勾等复杂笔画时，再调用此函数进行调整
void RelaxationMatch::ComplexStrokeAdjust()
{
	process_stroke_heng_pei_wan_gou();
	process_stroke_heng_zhe_wan_pie();
	process_stroke_wo_gou();
	process_stroke_xie_gou();
	process_stroke_wan_gou();
	//以后可以附加：对其他复杂笔画进行调整
}


//如果“横撇弯钩”是一笔写成的，那么整体调整。如果不是一笔写成的，那么分段扩展。
void RelaxationMatch::process_stroke_heng_pei_wan_gou()
{
	//1.判断模板字中是否存在耳刀旁，如果存在进行调整.如果不存在，返回.
	list<Stroke>::iterator it=model.stroke_list_.begin();
	Stroke heng_pei_wan_gou;
	while(it!=model.stroke_list_.end())
	{
		if(it->name_=="横撇弯钩")
		{
			heng_pei_wan_gou=*it;
			break;
		}
		it++;
	}
	if (it==model.stroke_list_.end())
		return;

	////////////////////////////////////////////////
	//2.检查条件：a模板笔画“横撇弯钩”中所有已匹配笔段都来自同一手写笔画,b应该匹配至少2段，第一段必须匹配；c第一笔在手写字中为起笔。
	//            d手写字中，从第一匹配笔段开始，该笔画还有至少四个连续笔段。e这些段不与其他模板笔段匹配。
	bool m_same=true;
	int m_seg1=heng_pei_wan_gou.start_index_;
	int m_seg2=heng_pei_wan_gou.start_index_+1;
	int m_seg3=heng_pei_wan_gou.start_index_+2;
	int m_seg4=heng_pei_wan_gou.start_index_+3;
	int s_seg1=Find_Match_for_Model_segment_in_single_result(m_seg1);
	int s_seg2=Find_Match_for_Model_segment_in_single_result(m_seg2);
	int s_seg3=Find_Match_for_Model_segment_in_single_result(m_seg3);
	int s_seg4=Find_Match_for_Model_segment_in_single_result(m_seg4);
	
	//2.0如果没有一个匹配笔段，则返回。
	if(s_seg1==-1 && s_seg2 ==-1 && s_seg3==-1 && s_seg4 ==-1)
		return;

	//2.1 模板笔画“横撇弯钩”中所有已匹配笔段都来自同一手写笔画
	if (s_seg1!=-1 && s_seg2 !=-1)	
		m_same &= script.InsameStroke(s_seg1,s_seg2);
	if (s_seg1!=-1 && s_seg3 !=-1)	
		m_same &= script.InsameStroke(s_seg1,s_seg3);
	if (s_seg1!=-1 && s_seg4 !=-1)	
		m_same &= script.InsameStroke(s_seg1,s_seg4);
	if (s_seg2!=-1 && s_seg3 !=-1)	
		m_same &= script.InsameStroke(s_seg2,s_seg3);
	if (s_seg2!=-1 && s_seg4 !=-1)	
		m_same &= script.InsameStroke(s_seg2,s_seg4);
	if (s_seg3!=-1 && s_seg4 !=-1)	
		m_same &= script.InsameStroke(s_seg3,s_seg4);
	
	//2.2 总体匹配至少2段，第一段必须匹配
	if (s_seg1 == -1 || (s_seg2==-1 && s_seg3 ==-1 && s_seg4 ==-1))
		m_same=false;

	
	//2.3第一笔在手写字中应该为起笔
	if((s_seg2!=-1 && s_seg1>s_seg2) || (s_seg3!=-1 && s_seg1>s_seg3) || (s_seg4!=-1 && s_seg1>s_seg4))
		m_same=false;

	int m_segcount=0;
	if(m_same)
	{
		int tempcount=0;
		//2.4手写字中，从第一匹配笔段开始，该笔画还有至少四个笔段。
		list<Stroke>::iterator its=script.stroke_list_.begin();
		for(;its!=script.stroke_list_.end();its++)
		{
			if(its->start_index_ <= s_seg1 && its->start_index_+its->seg_count_-4>=s_seg1 )
			{
				tempcount=its->start_index_+its->seg_count_-s_seg1;
				break;
			}

		}

		if (its==script.stroke_list_.end())
			m_same=false;		
		//2.5这些段涵盖了s_seg1——s_seg4,且应该不与其他模板笔段匹配。
		else
		{
			int w;
			for (w=0; w<tempcount; w++)
			{
				int tempk=Find_Match_for_script_segment_in_single_result(s_seg1+w);
				if(tempk!=-1 && (tempk<m_seg1 || tempk>m_seg4))
				{
					m_segcount=w;
					break;
				}
			}
			if(w==tempcount)
				m_segcount=tempcount;
			///////////////////////////////////////
			if(m_segcount<4 || s_seg1+m_segcount <= max(max(s_seg1,s_seg2),max(s_seg3,s_seg4)))
				m_same=false;
			else
			{			
				///////////////////////////////////
				for (int x=s_seg1; x<s_seg1+m_segcount;x++)
				{
					list<candidate>::iterator itr=matchresult.begin();
					for (;itr!=matchresult.end();itr++)
					{
						if(itr->h==x)
						{
							matchresult.erase(itr);	
							break;
						}
					}
				}
			}
		}
	}

	//3.满足整体调整条件。
	if(m_same)
	{
		//3.1删除已有匹配结果
		for (int mk=0;mk<4;mk++)
		{
			list<candidate>::iterator itr=matchresult.begin();
			for (;itr!=matchresult.end();itr++)
			{
				if(itr->k==m_seg1+mk)
				{
					matchresult.erase(itr);	
					break;
				}
			}
		}
		//3.2整体遍历（不包括最末尾的抖笔）
		list<candidate>  tempres;
		list<multicandidate>  tempmultires;
		heng_pei_wan_gou_match_1(s_seg1, m_seg1, m_segcount, tempres, tempmultires);

		//3.3笔段合并
		if (tempres.size()!=0)
		{
			list<candidate>::iterator it=tempres.begin();
			while(it!=tempres.end())
			{
				matchresult.push_back(*it);
				it++;
			}
		}
		///////////////////////////////
		tempres.clear();
		tempres=merge_script_seg_for_multimatch(tempmultires);		
		//////////////////////////
		if (tempres.size()!=0)
		{
			list<candidate>::iterator it=tempres.begin();
			while(it!=tempres.end())
			{
				matchresult.push_back(*it);
				it++;
			}
		}
	}
	//4.不满足整体调整条件。
	else
	{
		//4.1删除已有匹配结果
		for (int mk=0;mk<4;mk++)
		{
			list<candidate>::iterator itr=matchresult.begin();
			for (;itr!=matchresult.end();itr++)
			{
				if(itr->k==m_seg1+mk)
				{
					matchresult.erase(itr);	
					break;
				}
			}
		}
		//4.2对各个笔段进行两端扩展。(不包括最末尾的抖笔)
		list<multicandidate>  tempmultires;
		heng_pei_wan_gou_match_2(m_seg1,s_seg1,m_seg2,s_seg2,m_seg3,s_seg3,m_seg4,s_seg4,tempmultires);

		//4.3 笔段合并		
		list<candidate>  tempres=merge_script_seg_for_multimatch(tempmultires);		
		//////////////////////////
		if (tempres.size()!=0)
		{
			list<candidate>::iterator it=tempres.begin();
			while(it!=tempres.end())
			{
				matchresult.push_back(*it);
				it++;
			}
		}
	}

}

list<candidate> RelaxationMatch::merge_script_seg_for_multimatch(list<multicandidate> tempmultires)
{
	//（调整：手写字对象的笔画、笔段、抖笔列表；匹配结果，临时存储）
	list<candidate> m_res;

	/////////////////////
	list<multicandidate>::iterator itm=tempmultires.begin();
	for (;itm!=tempmultires.end();itm++)
	{		
		while(itm->hlist.size()!=1)
		{
			list<int>::iterator ith=itm->hlist.begin();
			int h1=*ith;
			ith++;
			int h2=*ith;
			//ASSERT(script.InsameStroke(h1,h2));
			//ASSERT(h2!=h1);

			int min_h, max_h;
			if(h1>h2)
			{
				min_h=h2;
				max_h=h1;
				ith--;
			}
			else
			{
				min_h=h1;
				max_h=h2;
			}
			//ASSERT(max_h-min_h==1);
			
			Segment temph1=script.getsegment(min_h);
			Segment temph2=script.getsegment(max_h);


			for (int i=1;i<temph2.point_list_.size();i++)
			{
				temph1.point_list_.push_back(temph2.GetPoint(i));
			}
							
			temph1.UpdateAttribute();		
			///////////////////////////////////////
			script.Replacesegment(min_h,temph1);
			script.removesegment(max_h);
			itm->hlist.erase(ith);
			////////////////////已有匹配结果
			list<candidate>::iterator itx=matchresult.begin();
			while(itx!=matchresult.end())
			{
				if (itx->h > max_h)
				{
					itx->h -= 1;
				}
				itx++;
			}
			
			///////////////////临时存储
			for (list<multicandidate>::iterator ittemp=tempmultires.begin(); ittemp!=tempmultires.end(); ittemp++)
			{
				list<int>::iterator ithtemp=ittemp->hlist.begin();
				for(;ithtemp!=ittemp->hlist.end();ithtemp++)
				{
					if ((*ithtemp)>max_h)
						(*ithtemp)=(*ithtemp)-1;
				}
			}
		}
			
		
	}

	////////////////////////////////////
	for (itm=tempmultires.begin(); itm!=tempmultires.end(); itm++)
	{
		candidate m_merge;
		m_merge.h=*(itm->hlist.begin());
		m_merge.k=*(itm->klist.begin());
		m_res.push_back(m_merge);
	}

	return m_res;
}

void RelaxationMatch::heng_pei_wan_gou_match_2(int k1,int h1,int k2,int h2,int k3,int h3,int k4,int h4, list<multicandidate>  &tempmultires)
{
	tempmultires.clear();
	
	///////////////////////
	list<int> tempsave;
	tempsave.clear();
	
	if(h1!=-1)
		tempsave.push_back(h1);
	if(h2!=-1)
		tempsave.push_back(h2);
	if(h3!=-1)
		tempsave.push_back(h3);
	if(h4!=-1)
		tempsave.push_back(h4);

	///////////////////////
	//横		
	if(h1!=-1)
	{
		bool yes1=true;	
		bool yes2=true;
		multicandidate tempm;
		tempm.klist.push_back(k1);
		tempm.hlist.push_back(h1);
		
		int cur_h=h1;
		while(yes1)
		{
			yes1=false;
			cur_h++;
			//是否属于同一笔画
			bool same=script.InsameStroke(h1,cur_h);
			if(same)
				same &=!(script.IsNoiseSegment(cur_h));

			if(same)
			{
				//查找
				int mat=Find_Match_for_script_segment_in_single_result(cur_h);
				list<int>::iterator it=tempsave.begin();
				for (;it!=tempsave.end();it++)
				{
					if(*it==cur_h)
						break;
				}

				//如果还没匹配,查看夹角
				if(mat==-1 && it==tempsave.end())
				{					
					float cur_dir=script.getsegment(cur_h).dirangle_;
					float mdir=script.getsegment(h1).dirangle_;

					float dis=fabs(cur_dir-mdir);
					if(dis>180)
						dis=360-180;
					//存入待匹配
					if(dis<30.0)
					{
						tempm.hlist.push_back(cur_h);
						tempsave.push_back(cur_h);
						yes1=true;
					}
				}
			}
		}

		cur_h=h1;
		while(yes2)
		{
			yes2=false;
			cur_h--;
			//是否属于同一笔画
			bool same=script.InsameStroke(h1,cur_h);
			if(same)
				same &=!(script.IsNoiseSegment(cur_h));
			if(same)
			{
				//查找
				int mat=Find_Match_for_script_segment_in_single_result(cur_h);
				list<int>::iterator it=tempsave.begin();
				for (;it!=tempsave.end();it++)
				{
					if(*it==cur_h)
						break;
				}
				
				//如果还没匹配,查看夹角
				if(mat==-1 && it==tempsave.end())
				{					
					float cur_dir=script.getsegment(cur_h).dirangle_;
					float mdir=script.getsegment(h1).dirangle_;
					
					float dis=fabs(cur_dir-mdir);
					if(dis>180)
						dis=360-180;
					//存入待匹配
					if(dis<30.0)
					{
						tempm.hlist.push_front(cur_h);
						tempsave.push_back(cur_h);
						yes2=true;
					}
				}
			}
		}

		tempmultires.push_back(tempm);
	}

	//撇
	if(h2!=-1)
	{
		bool yes1=true;	
		bool yes2=true;
		multicandidate tempm;
		tempm.klist.push_back(k2);
		tempm.hlist.push_back(h2);

		int cur_h=h2;
		while(yes1)
		{
			yes1=false;
			cur_h++;
			float h2_dir=script.getsegment(h2).dirangle_;

			//是否属于同一笔画
			bool same=script.InsameStroke(h2,cur_h);
			if(same)
				same &=!(script.IsNoiseSegment(cur_h));
			if(same)
			{
				//查找
				int mat=Find_Match_for_script_segment_in_single_result(cur_h);
				list<int>::iterator it=tempsave.begin();
				for (;it!=tempsave.end();it++)
				{
					if(*it==cur_h)
						break;
				}

				//如果还没匹配,查看角度
				if(mat==-1 && it==tempsave.end())
				{
					float cur_dir=script.getsegment(cur_h).dirangle_;
					if(h2_dir<210 && h2_dir>50 && cur_dir>120 && cur_dir<270)
					{
						tempm.hlist.push_back(cur_h);
						tempsave.push_back(cur_h);
						yes2=true;
					}
					else if( (h2_dir>270 || h2_dir<50) && (cur_dir>225 || cur_dir<45))
					{
						tempm.hlist.push_back(cur_h);
						tempsave.push_back(cur_h);
						yes2=true;
					}
				}
			}
		}

		cur_h=h2;
		while(yes2)
		{
			yes2=false;
			cur_h--;
			float h2_dir=script.getsegment(h2).dirangle_;
			
			//是否属于同一笔画
			bool same=script.InsameStroke(h2,cur_h);
			if(same)
				same &=!(script.IsNoiseSegment(cur_h));
			if(same)
			{
				//查找
				int mat=Find_Match_for_script_segment_in_single_result(cur_h);
				list<int>::iterator it=tempsave.begin();
				for (;it!=tempsave.end();it++)
				{
					if(*it==cur_h)
						break;
				}

				//如果还没匹配,查看角度
				if(mat==-1 && it==tempsave.end())
				{
					float cur_dir=script.getsegment(cur_h).dirangle_;
					if(h2_dir<210 && h2_dir>50 && cur_dir<130 && cur_dir>30)
					{
						tempm.hlist.push_front(cur_h);
						tempsave.push_back(cur_h);
						yes2=true;
					}
					else if( (h2_dir>270 || h2_dir<50) && (cur_dir>305 || cur_dir<90))
					{
						tempm.hlist.push_front(cur_h);
						tempsave.push_back(cur_h);
						yes2=true;
					}
				}
			}
		}
		tempmultires.push_back(tempm);
	}

	//弯
	if(h3!=-1)
	{
		bool yes1=true;	
		bool yes2=true;
		multicandidate tempm;
		tempm.klist.push_back(k3);
		tempm.hlist.push_back(h3);
		
		int cur_h=h3;
		while(yes1)
		{
			yes1=false;
			cur_h++;
			float h3_dir=script.getsegment(h3).dirangle_;
			
			//是否属于同一笔画
			bool same=script.InsameStroke(h3,cur_h);
			if(same)
				same &=!(script.IsNoiseSegment(cur_h));
			if(same)
			{
				//查找
				int mat=Find_Match_for_script_segment_in_single_result(cur_h);
				list<int>::iterator it=tempsave.begin();
				for (;it!=tempsave.end();it++)
				{
					if(*it==cur_h)
						break;
				}
				
				//如果还没匹配,查看角度
				if(mat==-1 && it==tempsave.end())
				{
					float cur_dir=script.getsegment(cur_h).dirangle_;
					if(h3_dir<180 && (cur_dir<160  && cur_dir> h3_dir ))
					{
						tempm.hlist.push_back(cur_h);
						tempsave.push_back(cur_h);
						yes2=true;
					}
					else if(h3_dir>180 && (cur_dir<300 && cur_dir>120))
					{
						tempm.hlist.push_back(cur_h);
						tempsave.push_back(cur_h);
						yes2=true;
					}
				}
			}
		}

		cur_h=h3;
		while(yes2)
		{
			yes2=false;
			cur_h--;
			float h3_dir=script.getsegment(h3).dirangle_;
			
			//是否属于同一笔画
			bool same=script.InsameStroke(h3,cur_h);
			if(same)
				same &=!(script.IsNoiseSegment(cur_h));
			if(same)
			{
				//查找
				int mat=Find_Match_for_script_segment_in_single_result(cur_h);
				list<int>::iterator it=tempsave.begin();
				for (;it!=tempsave.end();it++)
				{
					if(*it==cur_h)
						break;
				}
				
				//如果还没匹配,查看角度
				if(mat==-1 && it==tempsave.end())
				{
					float cur_dir=script.getsegment(cur_h).dirangle_;
					if(h3_dir<180 && (cur_dir>270  || cur_dir< h3_dir ))
					{
						tempm.hlist.push_front(cur_h);
						tempsave.push_back(cur_h);
						yes2=true;
					}
					else if(h3_dir>180 && (cur_dir>h3_dir && cur_dir<340))
					{
						tempm.hlist.push_front(cur_h);
						tempsave.push_back(cur_h);
						yes2=true;
					}
				}
			}
		}
		tempmultires.push_back(tempm);
	}

	//勾
	if(h4!=-1)
	{
		bool yes1=true;	
		bool yes2=true;
		multicandidate tempm;
		tempm.klist.push_back(k4);
		tempm.hlist.push_back(h4);
		
		int cur_h=h4;
		while(yes1)
		{
			yes1=false;
			cur_h++;
			float h4_dir=script.getsegment(h4).dirangle_;
			
			//是否属于同一笔画
			bool same=script.InsameStroke(h4,cur_h);
			if(same)
				same &=!(script.IsNoiseSegment(cur_h));
			if(same)
			{
				//查找
				int mat=Find_Match_for_script_segment_in_single_result(cur_h);
				list<int>::iterator it=tempsave.begin();
				for (;it!=tempsave.end();it++)
				{
					if(*it==cur_h)
						break;
				}
				
				//如果还没匹配,查看角度
				if(mat==-1 && it==tempsave.end())
				{
					float cur_dir=script.getsegment(cur_h).dirangle_;
					if(h4_dir>180 && cur_dir>h4_dir)
					{
						tempm.hlist.push_back(cur_h);
						tempsave.push_back(cur_h);
						yes2=true;
					}
					else if(h4_dir<180 && cur_dir<h4_dir)
					{
						tempm.hlist.push_back(cur_h);
						tempsave.push_back(cur_h);
						yes2=true;
					}
				}
			}
		}
		
		cur_h=h4;
		while(yes2)
		{
			yes2=false;
			cur_h--;
			float h4_dir=script.getsegment(h4).dirangle_;

			//是否属于同一笔画
			bool same=script.InsameStroke(h4,cur_h);
			if(same)
				same &=!(script.IsNoiseSegment(cur_h));
			if(same)
			{
				//查找
				int mat=Find_Match_for_script_segment_in_single_result(cur_h);
				list<int>::iterator it=tempsave.begin();
				for (;it!=tempsave.end();it++)
				{
					if(*it==cur_h)
						break;
				}
				
				//如果还没匹配,查看角度
				if(mat==-1 && it==tempsave.end())
				{
					float cur_dir=script.getsegment(cur_h).dirangle_;
					if(h4_dir>180 && cur_dir>180)
					{
						tempm.hlist.push_front(cur_h);
						tempsave.push_back(cur_h);
						yes2=true;
					}					
				}
			}
		}
		tempmultires.push_back(tempm);
	}

}

void RelaxationMatch::heng_zhe_wan_pie_match_1(int start_sh, int start_mk, int sh_count, list<candidate> &res, list<multicandidate>  &tempmultires)
{
	res.clear();
	tempmultires.clear();
	
	int sh=0;
	int flag=0;
	/////////////////////////////////////
	while(sh<sh_count)  //横
	{		
		Segment t=script.getsegment(start_sh+sh);
		
		if(!script.IsNoiseSegment(start_sh+sh) && (t.dirangle_>290 || t.dirangle_<60))
			sh++;
		else
			break;
	}	
	
	if(sh-flag==1)
	{
		candidate temp;
		temp.h=start_sh+sh-1;
		temp.k=start_mk;
		res.push_back(temp);
		flag=sh;
	}
	else if( sh-flag > 1)
	{
		multicandidate temp;
		temp.klist.push_back(start_mk);
		for (int x=flag; x<sh;x++)
		{
			temp.hlist.push_back(start_sh+x);
		}
		tempmultires.push_back(temp);
		flag=sh;
	}
	else
		return;

	////////////////////////
	float standard=script.getsegment(start_sh+sh).dirangle_;
	if (standard>=180 || standard<=60)
	{
		standard=130.0;
	}
	
	while(sh<sh_count)  //折
	{
		Segment t=script.getsegment(start_sh+sh);
		float deta=fabs(t.dirangle_-standard);
		if (deta>=180.0)
			deta=360-deta;
		
		float deta1=t.dirangle_-standard;
		if(!script.IsNoiseSegment(start_sh+sh) && (deta<=30.0 || (t.dirangle_>130.0 && t.dirangle_<260.0)) && deta1>-5.0)
		{
			standard=t.dirangle_;
			sh++;
		}
		else
			break;
	}
	
	if(sh-flag==1)
	{
		candidate temp;
		temp.h=start_sh+sh-1;
		temp.k=start_mk+1;
		res.push_back(temp);
		flag=sh;
	}
	else if( sh-flag > 1)
	{
		multicandidate temp;
		temp.klist.push_back(start_mk+1);
		for (int x=flag; x<sh;x++)
		{
			temp.hlist.push_back(start_sh+x);
		}
		tempmultires.push_back(temp);
		flag=sh;
	}
	else
		return;

	////////////////////////
	bool ti=true;
	while(sh<sh_count)  //弯
	{
		Segment t=script.getsegment(start_sh+sh);
		
		if(!script.IsNoiseSegment(start_sh+sh) && t.dirangle_>290 && ti)
			sh++;
		else if(!script.IsNoiseSegment(start_sh+sh) && t.dirangle_<45)
		{
			ti=false;
			sh++;
		}
		else
			break;
	}
	
	if(sh-flag==1)
	{
		candidate temp;
		temp.h=start_sh+sh-1;
		temp.k=start_mk+2;
		res.push_back(temp);
		flag=sh;
	}
	else if( sh-flag > 1)
	{
		multicandidate temp;
		temp.klist.push_back(start_mk+2);
		for (int x=flag; x<sh;x++)
		{
			temp.hlist.push_back(start_sh+x);
		}
		tempmultires.push_back(temp);
		flag=sh;
	}

	//////////////////////////////////////////
	while(sh<sh_count)  //撇
	{
		Segment t=script.getsegment(start_sh+sh);
		
		if(!script.IsNoiseSegment(start_sh+sh) && t.dirangle_>=45 && t.dirangle_<160)
			sh++;
		else
			break;
	}
	if(sh-flag==1)
	{
		candidate temp;
		temp.h=start_sh+sh-1;
		temp.k=start_mk+3;
		res.push_back(temp);
		flag=sh;
	}
	else if( sh-flag > 1)
	{
		multicandidate temp;
		temp.klist.push_back(start_mk+3);
		for (int x=flag; x<sh;x++)
		{
			temp.hlist.push_back(start_sh+x);
		}
		tempmultires.push_back(temp);
		flag=sh;
	}
	else
		return;
}

void RelaxationMatch::heng_pei_wan_gou_match_1(int start_sh, int start_mk, int sh_count, list<candidate> &res, list<multicandidate>  &tempmultires)
{
	res.clear();
	tempmultires.clear();
	
	int sh=0;
	int flag=0;
	/////////////////////////////////////
	while(sh<sh_count)  //横
	{		
		Segment t=script.getsegment(start_sh+sh);

		if(!script.IsNoiseSegment(start_sh+sh) && (t.dirangle_>290 || t.dirangle_<60))
			sh++;
		else
			break;
	}	

	if(sh-flag==1)
	{
		candidate temp;
		temp.h=start_sh+sh-1;
		temp.k=start_mk;
		res.push_back(temp);
		flag=sh;
	}
	else if( sh-flag > 1)
	{
		multicandidate temp;
		temp.klist.push_back(start_mk);
		for (int x=flag; x<sh;x++)
		{
			temp.hlist.push_back(start_sh+x);
		}
		tempmultires.push_back(temp);
		flag=sh;
	}
	else
		return;

	////////////////////////
	float standard=script.getsegment(start_sh+sh).dirangle_;
	if (standard>=180 || standard<=60)
	{
		standard=130.0;
	}

	while(sh<sh_count)  //撇
	{
		Segment t=script.getsegment(start_sh+sh);
		float deta=fabs(t.dirangle_-standard);
		if (deta>=180.0)
			deta=360-deta;
		
		float deta1=t.dirangle_-standard;
		if(!script.IsNoiseSegment(start_sh+sh) && (deta<=30.0 || (t.dirangle_>130.0 && t.dirangle_<260.0)) && deta1>-15.0)
		{
			standard=t.dirangle_;
			sh++;
		}
		else
			break;
	}

	if(sh-flag==1)
	{
		candidate temp;
		temp.h=start_sh+sh-1;
		temp.k=start_mk+1;
		res.push_back(temp);
		flag=sh;
	}
	else if( sh-flag > 1)
	{
		multicandidate temp;
		temp.klist.push_back(start_mk+1);
		for (int x=flag; x<sh;x++)
		{
			temp.hlist.push_back(start_sh+x);
		}
		tempmultires.push_back(temp);
		flag=sh;
	}
	else
		return;

	////////////////////////
	bool ti=true;
	while(sh<sh_count)  //弯
	{
		Segment t=script.getsegment(start_sh+sh);
		
		if(!script.IsNoiseSegment(start_sh+sh) && t.dirangle_>290 && ti)
			sh++;
		else if(!script.IsNoiseSegment(start_sh+sh) && t.dirangle_<160)
		{
			ti=false;
			sh++;
		}
		else
			break;
	}
	
	if(sh-flag==1)
	{
		candidate temp;
		temp.h=start_sh+sh-1;
		temp.k=start_mk+2;
		res.push_back(temp);
		flag=sh;
	}
	else if( sh-flag > 1)
	{
		multicandidate temp;
		temp.klist.push_back(start_mk+2);
		for (int x=flag; x<sh;x++)
		{
			temp.hlist.push_back(start_sh+x);
		}
		tempmultires.push_back(temp);
		flag=sh;
	}
	else
		return;

	////////////////////////
	while(sh<sh_count)  //勾
	{
		Segment t=script.getsegment(start_sh+sh);
		
		if(!script.IsNoiseSegment(start_sh+sh) && t.dirangle_>=160)
			sh++;
		else
			break;
	}
	if(sh-flag==1)
	{
		candidate temp;
		temp.h=start_sh+sh-1;
		temp.k=start_mk+3;
		res.push_back(temp);
		flag=sh;
	}
	else if( sh-flag > 1)
	{
		multicandidate temp;
		temp.klist.push_back(start_mk+3);
		for (int x=flag; x<sh;x++)
		{
			temp.hlist.push_back(start_sh+x);
		}
		tempmultires.push_back(temp);
		flag=sh;
	}
	else
		return;
}



list<conflictforgreedy> RelaxationMatch::conflicttestforfree(list<candidateforgreedy> candidatelist)
{
	list<conflictforgreedy> m_res;
	GeometryTool gt;
	///////////////////////////////
	list<candidateforgreedy>::iterator it1=candidatelist.begin();
	list<candidateforgreedy>::iterator it2;
	
	int sha, mka, shb, mkb;
	for (;it1!=candidatelist.end();it1++)
	{
		it2=it1;
		it2++;
		for(;it2!=candidatelist.end();it2++)
		{
			mka=it1->mk;
			mkb=it2->mk;
			
			list<scriptpro>::iterator it3=it1->shlist.begin();
			for (;it3!=it1->shlist.end();it3++)
			{
				list<scriptpro>::iterator it4=it2->shlist.begin();
				for (;it4!=it2->shlist.end();it4++)
				{
					sha=it3->sh;
					shb=it4->sh;
					//是否重复
					if(sha==shb)
					{
						conflictforgreedy temp;
						temp.can1.h=sha;
						temp.can1.k=mka;
						temp.can2.h=shb;
						temp.can2.k=mkb;
						m_res.push_back(temp);
					}				
				}
			}
		}
	}
	
	////////////////
	return m_res;
}

//最后一次查漏补缺。尽可能建立匹配。
void RelaxationMatch::FreeExtension()
{
	if(matchresult.size()==0)  
		return;
	
	if(matchresult.size()==model_count)
		return;
	
	int normalcount=script_count - script.noise_list_.size();
	if(matchresult.size()==normalcount)
		return;

	/////////////////////////
	//1.按部件整理匹配结果
	list<ComponentMatch>  compoint_list_;
	compoint_list_.clear();
	list<CComponent>::iterator it=model.m_components_.begin();

	while(it!=model.m_components_.end())
	{
		ComponentMatch temp;
		list<int>::iterator itk=it->segments_list.begin();
		while(itk != it->segments_list.end())
		{
			int h=Find_Match_for_Model_segment_in_single_result(*itk);
			if(h!=-1)
			{
				candidate ca;
				ca.h=h;
				ca.k=*itk;
				temp.havemath.push_back(ca);
			}
			else
				temp.nomatch_model_seg.push_back(*itk);
			itk++;
		}
		////
		temp.top_key_segments=it->top_key_segments;
		temp.bottom_key_segments=it->bottom_key_segments;
		temp.left_key_segments=it->left_key_segments;
		temp.right_key_segments=it->right_key_segments;
		////
		compoint_list_.push_back(temp);
		it++;
	}
	//2.按未匹配笔段个数从少到多，对部件进行排序。并删除满足如下条件的部件：没有未匹配的模板字笔段
	SortandReduce(compoint_list_);
	
	//3.按部件顺序处理：
	list<candidateforgreedy> totallist;
	totallist.clear();
	
	//3.0收集未匹配的、非抖笔的手写字笔段
	list<int>  nomatchscript=Find_Nomatch_normal_Scriptseg_in_both_reslult();
	
	//3.1为该部件中未匹配的每一个模板字笔段估计候选手写字笔段
	list<ComponentMatch>::iterator itcom=compoint_list_.begin();
	for(;itcom!=compoint_list_.end();itcom++)
	{		
		list<candidateforgreedy> candidatelist;
		candidatelist.clear();
		candidatelist=probabilityforgreedy(*itcom,nomatchscript,2);		
		
		list<candidateforgreedy>::iterator itt=candidatelist.begin();
		for (;itt!=candidatelist.end();itt++)
		{
			totallist.push_back(*itt);
		}		
	}
	
	//3.2兼容性检测
	list<conflictforgreedy> conflictlist;
	conflictlist=conflicttestforfree(totallist);
	
	//3.3贪心算法求解
	list<candidate> m_res=GreedyExtraction(totallist, conflictlist);
	if (m_res.size()!=0)
	{
		list<candidate>::iterator it=m_res.begin();
		while(it!=m_res.end())
		{
			matchresult.push_back(*it);
			it++;
		}
	}

}

//在获得松弛匹配结果后，如果还存在未匹配笔段时，再调用此函数.
//此函数遍历每一个部件，对手写字和模板字中同时存在的未匹配笔段进行处理。
void RelaxationMatch::UnmatchExtension()
{
	
	if(matchresult.size()==0)  
		return;
	
	if(matchresult.size()==model_count)
		return;

	int normalcount=script_count - script.noise_list_.size();
	if(matchresult.size()==normalcount)
		return;
	/////////////////////////
	//1.按部件整理匹配结果
	list<ComponentMatch>  compoint_list_;
	compoint_list_.clear();
	list<CComponent>::iterator it=model.m_components_.begin();

	while(it!=model.m_components_.end())
	{
		ComponentMatch temp;
		list<int>::iterator itk=it->segments_list.begin();
		while(itk != it->segments_list.end())
		{
			int h=Find_Match_for_Model_segment_in_single_result(*itk);
			if(h!=-1)
			{
				candidate ca;
				ca.h=h;
				ca.k=*itk;
				temp.havemath.push_back(ca);
			}
			else
				temp.nomatch_model_seg.push_back(*itk);
			itk++;
		}
		////
		temp.top_key_segments=it->top_key_segments;
		temp.bottom_key_segments=it->bottom_key_segments;
		temp.left_key_segments=it->left_key_segments;
		temp.right_key_segments=it->right_key_segments;
		////
		compoint_list_.push_back(temp);
		it++;
	}
	//2.按未匹配笔段个数从少到多，对部件进行排序。并删除满足如下条件的部件：没有未匹配的模板字笔段
	SortandReduce(compoint_list_);
	
	//3.按部件顺序处理：
	list<candidateforgreedy> totallist;
	totallist.clear();

	//3.0收集未匹配的、非抖笔的手写字笔段
	list<int>  nomatchscript=Find_Nomatch_normal_Scriptseg_in_both_reslult();

	//3.1为该部件中未匹配的每一个模板字笔段估计候选手写字笔段
	list<ComponentMatch>::iterator itcom=compoint_list_.begin();
	for(;itcom!=compoint_list_.end();itcom++)
	{		
		list<candidateforgreedy> candidatelist;
		candidatelist.clear();
		candidatelist=probabilityforgreedy(*itcom,nomatchscript,1);		
		
		list<candidateforgreedy>::iterator itt=candidatelist.begin();
		for (;itt!=candidatelist.end();itt++)
		{
			totallist.push_back(*itt);
		}		
	}

	//3.2兼容性检测
	list<conflictforgreedy> conflictlist;
	conflictlist=conflicttestforgreedy(totallist);
	
	//3.3贪心算法求解
	list<candidate> m_res;
	m_res.clear();
	if(totallist.size()<10)
		m_res=GreedyExtraction(totallist, conflictlist);
	if (m_res.size()!=0)
	{
		list<candidate>::iterator it=m_res.begin();
		while(it!=m_res.end())
		{
			matchresult.push_back(*it);
			it++;
		}
	}
	/*
	list<ComponentMatch>::iterator itcom=compoint_list_.begin();
	for(;itcom!=compoint_list_.end();itcom++)
	{
		//3.0收集未匹配的、非抖笔的手写字笔段
		list<int>  nomatchscript=Find_Nomatch_normal_Scriptseg_in_both_reslult();
		if(nomatchscript.size()==0)
			break;

		//3.1为该部件中未匹配的每一个模板字笔段估计候选手写字笔段
		list<candidateforgreedy> candidatelist;
		candidatelist=probabilityforgreedy(*itcom,nomatchscript);

		//3.2兼容性检测
		list<conflictforgreedy> conflictlist;
		conflictlist=conflicttestforgreedy(candidatelist);

		//3.2贪心算法求解
		list<candidate> m_res=GreedyExtraction(candidatelist, conflictlist);
		if (m_res.size()!=0)
		{
			list<candidate>::iterator it=m_res.begin();
			while(it!=m_res.end())
			{
				matchresult.push_back(*it);
				it++;
			}
		}
		
	}
	*/
}

list<candidate> RelaxationMatch::GreedyExtraction(list<candidateforgreedy> candidatelist, list<conflictforgreedy> conflictlist)
{
	list<candidate> m_res;
	m_res.clear();
	float proflag=-1;

	//首先保证匹配数量最大，然后保证相似度最高
	list<candidate> temp_res;
	temp_res.clear();
	float tempflag=0.0;
	
	if(candidatelist.size()==0)
		return m_res;
	///////////////////////辅助信息
	int* mark=new int[candidatelist.size()];
	for (int i=0;i<candidatelist.size();i++)
		mark[i]=0;	

	while(true)
	{
		//穷举
		tempflag=0.0;
		temp_res.clear();
		for (int x=0;x<candidatelist.size();x++)
		{
			list<candidateforgreedy>::iterator it=candidatelist.begin();
			for (int y=0;y<x;y++)
			{
				it++;
			}
			list<scriptpro>::iterator ita=it->shlist.begin();
			for(int z=0;z<mark[x];z++)
			{
				ita++;
			}
			if(ita!=it->shlist.end())
			{
				candidate temp;
				temp.probability=ita->probability;
				temp.h=ita->sh;
				temp.k=it->mk;
				
				if(checkcompatibility_for_Greedy(temp,temp_res,conflictlist))
				{
					temp_res.push_back(temp);
					tempflag+=temp.probability;
				}
			}

			//预测截止:数量
			if(temp_res.size()<m_res.size()  && candidatelist.size()-x-1 < m_res.size()-temp_res.size())
				break;			
		}
		/////////////////////
		//判断
		if(m_res.size()<temp_res.size())
		{
			m_res.clear();
			m_res.assign(temp_res.begin(),temp_res.end());
			proflag=tempflag;
		}
		else if(m_res.size()==temp_res.size() && tempflag>proflag)
		{
			m_res.clear();
			m_res.assign(temp_res.begin(),temp_res.end());
			proflag=tempflag;
		}
		/////////////////////
		//迭代
		mark[candidatelist.size()-1]++;
		for (unsigned int x=candidatelist.size()-1;x>=0;x--)
		{
			list<candidateforgreedy>::iterator it=candidatelist.begin();
			for (int y=0;y<x;y++)
			{
				it++;
			}
			if(mark[x] > it->shlist.size())
			{
				mark[x]=0;
				mark[x-1]++;
			}
		}
		//结束
		bool yes=true;
		for (unsigned int x=0;x<candidatelist.size();x++)
		{
			list<candidateforgreedy>::iterator it=candidatelist.begin();
			for (int y=0;y<x;y++)
			{
				it++;
			}
			if(mark[x] < it->shlist.size())
			{
				yes=false;
				break;
			}
		}
		if(yes)
			break;
	}
	///////////
	delete[] mark;
	return m_res;
}

bool RelaxationMatch::checkcompatibility_for_Greedy(candidate& m_item, list<candidate>& m_res, list<conflictforgreedy>& conflictlist)
{
	bool yes=true;
	//////////////
	list<candidate>::iterator it=m_res.begin();
	list<conflictforgreedy>::iterator itc;
	for (;it!=m_res.end();it++)
	{
		if(m_item.h==it->h || m_item.k==it->k)
		{
			yes=false;
			break;
		}
		
		itc=conflictlist.begin();
		for(;itc!=conflictlist.end();itc++)
		{
			if(m_item.h==itc->can1.h && m_item.k==itc->can1.k && it->h==itc->can2.h && it->k==itc->can2.k)
			{
				yes=false;
				break;
			}
			else if(m_item.h==itc->can2.h && m_item.k==itc->can2.k && it->h==itc->can1.h && it->k==itc->can1.k)
			{
				yes=false;
				break;
			}
		}
		
		if(!yes)
			break;
	}

	//////////////
	return yes;
}

list<conflictforgreedy> RelaxationMatch::conflicttestforgreedy(list<candidateforgreedy> candidatelist)
{
	list<conflictforgreedy> m_res;
	GeometryTool gt;
	///////////////////////////////
	list<candidateforgreedy>::iterator it1=candidatelist.begin();
	list<candidateforgreedy>::iterator it2;

	int sha, mka, shb, mkb;
	for (;it1!=candidatelist.end();it1++)
	{
		it2=it1;
		it2++;
		for(;it2!=candidatelist.end();it2++)
		{
			mka=it1->mk;
			mkb=it2->mk;

			list<scriptpro>::iterator it3=it1->shlist.begin();
			for (;it3!=it1->shlist.end();it3++)
			{
				list<scriptpro>::iterator it4=it2->shlist.begin();
				for (;it4!=it2->shlist.end();it4++)
				{
					sha=it3->sh;
					shb=it4->sh;
					//是否重复
					if(sha==shb)
					{
						conflictforgreedy temp;
						temp.can1.h=sha;
						temp.can1.k=mka;
						temp.can2.h=shb;
						temp.can2.k=mkb;
						m_res.push_back(temp);
					}
					else//方向性
					{
						string rel=model.GetRelation(mka,mkb);
						if(rel!=R_INTERSECT)
						{
							CCPoint pti=script.getsegment(sha).mid_point_;
							CCPoint pth=script.getsegment(shb).mid_point_;
							CCPoint ptj=model.getsegment(mka).mid_point_;
							CCPoint ptk=model.getsegment(mkb).mid_point_;
							
							float angle1=gt.AngleInDegrees(pti,pth,true);
							float angle2=gt.AngleInDegrees(ptj,ptk,true);
							
							float maxang=max(angle1,angle2);
							float minang=min(angle1,angle2);
														
							float diff_dir=maxang-minang;
							if(diff_dir>180.0)
								diff_dir=360-diff_dir;

							if(fabs(angle2-0)<3.0 || fabs(angle2-90)<3.0 || fabs(angle2-180)<3.0 || fabs(angle2-270)<3.0 || fabs(angle2-360)<3.0)
							{
								if(diff_dir>80.0)
								{							
									conflictforgreedy temp;
									temp.can1.h=sha;
									temp.can1.k=mka;
									temp.can2.h=shb;
									temp.can2.k=mkb;
									m_res.push_back(temp);
								}
							}
							else if(diff_dir>77.9 && rel[0]=='C')
							{
								conflictforgreedy temp;
								temp.can1.h=sha;
								temp.can1.k=mka;
								temp.can2.h=shb;
								temp.can2.k=mkb;
								m_res.push_back(temp);
							}
							else if(diff_dir>68.0 && rel[0]!='C')
							{
								conflictforgreedy temp;
								temp.can1.h=sha;
								temp.can1.k=mka;
								temp.can2.h=shb;
								temp.can2.k=mkb;
								m_res.push_back(temp);
							}
						}
					}
				}
			}
		}
	}

	////////////////
	return m_res;
}

void RelaxationMatch::Evalute_boundingbox(ComponentMatch m_ComponentMatch,boundingbox &modelbox,boundingbox &scriptbox)
{
	float m_left=512000;
	float m_right=0;
	float m_top=512000;
	float m_bottom=0;
	
	/////////模板字包围盒
	list<int>::iterator it=m_ComponentMatch.nomatch_model_seg.begin();
	for(;it!=m_ComponentMatch.nomatch_model_seg.end();it++)
	{
		Segment mseg=model.getsegment(*it);
		CCPoint p1=mseg.GetPoint(0);

		if(p1.x<m_left)
			m_left=p1.x;
		if(p1.x>m_right)
			m_right=p1.x;
		if(p1.y<m_top)
			m_top=p1.y;
		if(p1.y>m_bottom)
			m_bottom=p1.y;

		CCPoint p2=mseg.GetPoint(mseg.point_list_.size()-1);
		if(p2.x<m_left)
			m_left=p2.x;
		if(p2.x>m_right)
			m_right=p2.x;
		if(p2.y<m_top)
			m_top=p2.y;
		if(p2.y>m_bottom)
			m_bottom=p2.y;
	}

	list<candidate>::iterator ita=m_ComponentMatch.havemath.begin();
	for(;ita!=m_ComponentMatch.havemath.end();ita++)
	{
		Segment mseg=model.getsegment(ita->k);
		CCPoint p1=mseg.GetPoint(0);
		
		if(p1.x<m_left)
			m_left=p1.x;
		if(p1.x>m_right)
			m_right=p1.x;
		if(p1.y<m_top)
			m_top=p1.y;
		if(p1.y>m_bottom)
			m_bottom=p1.y;
		
		CCPoint p2=mseg.GetPoint(mseg.point_list_.size()-1);
		if(p2.x<m_left)
			m_left=p2.x;
		if(p2.x>m_right)
			m_right=p2.x;
		if(p2.y<m_top)
			m_top=p2.y;
		if(p2.y>m_bottom)
			m_bottom=p2.y;
	}

	////////////////////////
	//手写字关键笔段
	float m_width=m_right-m_left;
	float m_height=m_bottom-m_top;

	list<int> s_top_key;
	list<int> s_bottom_key;
	list<int> s_left_key;
	list<int> s_right_key;	
	
	for(ita=m_ComponentMatch.havemath.begin(); ita!=m_ComponentMatch.havemath.end(); ita++)
	{
		if (find (m_ComponentMatch.top_key_segments.begin(), m_ComponentMatch.top_key_segments.end(), ita->k)
			!=m_ComponentMatch.top_key_segments.end())
		{
			s_top_key.push_back(ita->h);
		}
		if (find (m_ComponentMatch.bottom_key_segments.begin(), m_ComponentMatch.bottom_key_segments.end(), ita->k)
			!=m_ComponentMatch.bottom_key_segments.end())
		{
			s_bottom_key.push_back(ita->h);
		}
		if (find (m_ComponentMatch.left_key_segments.begin(), m_ComponentMatch.left_key_segments.end(), ita->k)
			!=m_ComponentMatch.left_key_segments.end())
		{
			s_left_key.push_back(ita->h);
		}

		if (find (m_ComponentMatch.right_key_segments.begin(), m_ComponentMatch.right_key_segments.end(), ita->k)
			!=m_ComponentMatch.right_key_segments.end())
		{
			s_right_key.push_back(ita->h);
		}
	}

	////////////////
	float s_left=512000;
	float s_right=0;
	float s_top=512000;
	float s_bottom=0;

	//16 种情况
	if(s_left_key.size()==0 && s_right_key.size()==0 && s_top_key.size()==0 && s_bottom_key.size()==0)  //全无
	{
		s_left=m_left; s_right=m_right; s_top=m_top; s_bottom=m_bottom;
	}
	else if(s_left_key.size()!=0 && s_right_key.size()!=0 && s_top_key.size()!=0 && s_bottom_key.size()!=0) //全有
	{
		//left
		list<int>::iterator it=s_left_key.begin();
		for(;it!=s_left_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x < s_left)
				s_left=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x < s_left)
				s_left=p2.x;
		}
		//right
		it=s_right_key.begin();
		for(;it!=s_right_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x > s_right)
				s_right=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x > s_right)
				s_right=p2.x;
		}
		//top
		it=s_top_key.begin();
		for(;it!=s_top_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y < s_top)
				s_top=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y < s_top)
				s_top=p2.y;
		}
		//bottom
		it=s_bottom_key.begin();
		for(;it!=s_bottom_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y > s_bottom)
				s_bottom=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y > s_bottom)
				s_bottom=p2.y;
		}
	}
	else if(s_left_key.size()!=0 && s_right_key.size()==0 && s_top_key.size()==0 && s_bottom_key.size()==0) //一侧
	{
		//left
		list<int>::iterator it=s_left_key.begin();
		for(;it!=s_left_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x < s_left)
				s_left=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x < s_left)
				s_left=p2.x;
		}
		s_right=s_left+m_width;
		s_top=m_top;
		s_bottom=m_bottom;
	}
	else if(s_left_key.size()==0 && s_right_key.size()!=0 && s_top_key.size()==0 && s_bottom_key.size()==0)//一侧
	{
		//right
		list<int>::iterator it=s_right_key.begin();
		for(;it!=s_right_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x > s_right)
				s_right=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x > s_right)
				s_right=p2.x;
		}
		s_left=s_right-m_width;
		s_top=m_top;
		s_bottom=m_bottom;
	}
	else if(s_left_key.size()==0 && s_right_key.size()==0 && s_top_key.size()!=0 && s_bottom_key.size()==0)//一侧
	{
		list<int>::iterator it=s_top_key.begin();
		for(;it!=s_top_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y < s_top)
				s_top=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y < s_top)
				s_top=p2.y;
		}
		s_left=m_left;
		s_right=m_right;		
		s_bottom=s_top+m_height;
	}
	else if(s_left_key.size()==0 && s_right_key.size()==0 && s_top_key.size()==0 && s_bottom_key.size()!=0)//一侧
	{
		//bottom
		it=s_bottom_key.begin();
		for(;it!=s_bottom_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y > s_bottom)
				s_bottom=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y > s_bottom)
				s_bottom=p2.y;
		}
		s_left=m_left;
		s_right=m_right;		
		s_top=s_bottom-m_height;
	}
	else if(s_left_key.size()!=0 && s_right_key.size()!=0 && s_top_key.size()!=0 && s_bottom_key.size()==0)//三侧
	{
		//left
		list<int>::iterator it=s_left_key.begin();
		for(;it!=s_left_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x < s_left)
				s_left=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x < s_left)
				s_left=p2.x;
		}
		//right
		it=s_right_key.begin();
		for(;it!=s_right_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x > s_right)
				s_right=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x > s_right)
				s_right=p2.x;
		}
		//top
		it=s_top_key.begin();
		for(;it!=s_top_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y < s_top)
				s_top=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y < s_top)
				s_top=p2.y;
		}
		s_bottom=s_top+m_height;
		
	}
	else if(s_left_key.size()!=0 && s_right_key.size()!=0 && s_top_key.size()==0 && s_bottom_key.size()!=0)//三侧
	{
		//left
		list<int>::iterator it=s_left_key.begin();
		for(;it!=s_left_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x < s_left)
				s_left=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x < s_left)
				s_left=p2.x;
		}
		//right
		it=s_right_key.begin();
		for(;it!=s_right_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x > s_right)
				s_right=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x > s_right)
				s_right=p2.x;
		}
		//bottom
		it=s_bottom_key.begin();
		for(;it!=s_bottom_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y > s_bottom)
				s_bottom=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y > s_bottom)
				s_bottom=p2.y;
		}
		s_top=s_bottom-m_height;
	}
	else if(s_left_key.size()!=0 && s_right_key.size()==0 && s_top_key.size()!=0 && s_bottom_key.size()!=0)//三侧
	{
		//left
		list<int>::iterator it=s_left_key.begin();
		for(;it!=s_left_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x < s_left)
				s_left=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x < s_left)
				s_left=p2.x;
		}		
		//top
		it=s_top_key.begin();
		for(;it!=s_top_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y < s_top)
				s_top=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y < s_top)
				s_top=p2.y;
		}
		//bottom
		it=s_bottom_key.begin();
		for(;it!=s_bottom_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y > s_bottom)
				s_bottom=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y > s_bottom)
				s_bottom=p2.y;
		}
		s_right=s_left+m_width;
	}
	else if(s_left_key.size()==0 && s_right_key.size()!=0 && s_top_key.size()!=0 && s_bottom_key.size()!=0)//三侧
	{
		//right
		it=s_right_key.begin();
		for(;it!=s_right_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x > s_right)
				s_right=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x > s_right)
				s_right=p2.x;
		}
		//top
		it=s_top_key.begin();
		for(;it!=s_top_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y < s_top)
				s_top=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y < s_top)
				s_top=p2.y;
		}
		//bottom
		it=s_bottom_key.begin();
		for(;it!=s_bottom_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y > s_bottom)
				s_bottom=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y > s_bottom)
				s_bottom=p2.y;
		}
		s_left=s_right-m_width;
	}
	else if(s_left_key.size()==0 && s_right_key.size()==0 && s_top_key.size()!=0 && s_bottom_key.size()!=0)//两侧
	{
		//top
		it=s_top_key.begin();
		for(;it!=s_top_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y < s_top)
				s_top=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y < s_top)
				s_top=p2.y;
		}
		//bottom
		it=s_bottom_key.begin();
		for(;it!=s_bottom_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y > s_bottom)
				s_bottom=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y > s_bottom)
				s_bottom=p2.y;
		}
		s_left=m_left;
		s_right=m_right;
	}
	else if(s_left_key.size()==0 && s_right_key.size()!=0 && s_top_key.size()==0 && s_bottom_key.size()!=0)//两侧
	{		
		//right
		it=s_right_key.begin();
		for(;it!=s_right_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x > s_right)
				s_right=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x > s_right)
				s_right=p2.x;
		}		
		//bottom
		it=s_bottom_key.begin();
		for(;it!=s_bottom_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y > s_bottom)
				s_bottom=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y > s_bottom)
				s_bottom=p2.y;
		}
		s_left=s_right-m_width;
		s_top=s_bottom-m_height;
	}
	else if(s_left_key.size()==0 && s_right_key.size()!=0 && s_top_key.size()!=0 && s_bottom_key.size()==0)//两侧
	{
		//right
		it=s_right_key.begin();
		for(;it!=s_right_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x > s_right)
				s_right=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x > s_right)
				s_right=p2.x;
		}
		//top
		it=s_top_key.begin();
		for(;it!=s_top_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y < s_top)
				s_top=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y < s_top)
				s_top=p2.y;
		}
		s_left=s_right-m_width;
		s_bottom=s_top+m_height;
	}
	else if(s_left_key.size()!=0 && s_right_key.size()==0 && s_top_key.size()==0 && s_bottom_key.size()!=0)//两侧
	{
		//left
		list<int>::iterator it=s_left_key.begin();
		for(;it!=s_left_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x < s_left)
				s_left=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x < s_left)
				s_left=p2.x;
		}
		//bottom
		it=s_bottom_key.begin();
		for(;it!=s_bottom_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y > s_bottom)
				s_bottom=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y > s_bottom)
				s_bottom=p2.y;
		}
		s_right=s_left+m_width;
		s_top=s_bottom-m_height;
	}
	else if(s_left_key.size()!=0 && s_right_key.size()==0 && s_top_key.size()!=0 && s_bottom_key.size()==0)//两侧
	{
		//left
		list<int>::iterator it=s_left_key.begin();
		for(;it!=s_left_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x < s_left)
				s_left=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x < s_left)
				s_left=p2.x;
		}		
		//top
		it=s_top_key.begin();
		for(;it!=s_top_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.y < s_top)
				s_top=p1.y;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.y < s_top)
				s_top=p2.y;
		}
		s_right=s_left+m_width;
		s_bottom=s_top+m_height;
	}
	else if(s_left_key.size()!=0 && s_right_key.size()!=0 && s_top_key.size()==0 && s_bottom_key.size()==0)//两侧
	{
		//left
		list<int>::iterator it=s_left_key.begin();
		for(;it!=s_left_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x < s_left)
				s_left=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x < s_left)
				s_left=p2.x;
		}
		//right
		it=s_right_key.begin();
		for(;it!=s_right_key.end();it++)
		{
			Segment sseg=script.getsegment(*it);
			CCPoint p1=sseg.GetPoint(0);
			if(p1.x > s_right)
				s_right=p1.x;
			CCPoint p2=sseg.GetPoint(sseg.point_list_.size()-1);
			if(p2.x > s_right)
				s_right=p2.x;
		}
		s_top=m_top;
		s_bottom=m_bottom;
	}

	//调整
	if(s_left<0)
		s_left=0;
	if(s_top<0)
		s_top=0;
	if(s_right>model.normal_size_)
		s_right=model.normal_size_;
	if(s_bottom>model.normal_size_)
		s_bottom=model.normal_size_;
	
	//微调
	if (fabs(m_top-s_top)>model.normal_size_/4.65)
	{
		s_top += (m_top-s_top)/3.0;
	}

	if (fabs(m_bottom-s_bottom)>model.normal_size_/4.65)
	{
		s_bottom += (m_bottom-s_bottom)/3.0;
	}

	if (fabs(m_left-s_left)>model.normal_size_/4.65)
	{
		s_left += (m_left-s_left)/3.0;
	}

	if (fabs(m_right-s_right)>model.normal_size_/4.65)
	{
		s_right += (m_right-s_right)/3.0;
	}
	
	//微调
	if(model.m_struct_name_=="上下" || model.m_struct_name_=="上中下")
	{
		if(fabs(m_top-0.0)<2.0  && s_top>80)
			s_top -= s_top/2;
		if(fabs(m_bottom-model.normal_size_)<2.0 && script.normal_size_-s_bottom>80)
			s_bottom +=(script.normal_size_-s_bottom)/2;	
	}
	if(model.m_struct_name_=="左右" || model.m_struct_name_=="左中右")
	{
		if(fabs(m_left-0.0)<2.0 && s_left>80)
			s_left -= s_left/2;
		if(fabs(m_right-model.normal_size_)<2.0 && script.normal_size_-s_right>80)
			s_right +=(script.normal_size_-s_right)/2;
	}
	//返回
	modelbox.m_top=m_top;
	modelbox.m_bottom=m_bottom;
	modelbox.m_left=m_left;
	modelbox.m_right=m_right;

	scriptbox.m_top=s_top;
	scriptbox.m_bottom=s_bottom;
	scriptbox.m_left=s_left;
	scriptbox.m_right=s_right;	
}

//根据已匹配笔段、以及包围盒，估计候选手写笔段的中心位置
CCPoint RelaxationMatch::Evalute_position_forgreedy(int mk, ComponentMatch m_ComponentMatch, boundingbox modelbox,boundingbox sriptbox)
{
	float x_p=0;
	float y_p=0;
	int m_count=0;

	CCPoint mseg_need=model.getsegment(mk).mid_point_;

	///////////////////////////////////////////////////	
	x_p=(mseg_need.x-modelbox.m_left)/(modelbox.m_right-modelbox.m_left)*(sriptbox.m_right-sriptbox.m_left)+sriptbox.m_left;
	y_p=(mseg_need.y-modelbox.m_top)/(modelbox.m_bottom-modelbox.m_top)*(sriptbox.m_bottom-sriptbox.m_top)+sriptbox.m_top;
	m_count+=1;

	///////////////////////////////////////////////////
	list<candidate>::iterator itc=m_ComponentMatch.havemath.begin();
	for (; itc!=m_ComponentMatch.havemath.end();itc++)
	{
		int Ref_k=itc->k;
		int Ref_h=itc->h;
		
		CCPoint ref_kp=model.getsegment(Ref_k).mid_point_;
		CCPoint ref_hp=script.getsegment(Ref_h).mid_point_;

		if (ref_kp.x<mseg_need.x)
			x_p+=(mseg_need.x-ref_kp.x)/(modelbox.m_right-ref_kp.x)*(sriptbox.m_right-ref_hp.x)+ref_hp.x;
		else if(ref_kp.x>mseg_need.x)
			x_p+=-(ref_kp.x-mseg_need.x)/(ref_kp.x-modelbox.m_left)*(ref_hp.x-sriptbox.m_left)+ref_hp.x;
		else
			x_p+=ref_hp.x;
		
		if (ref_kp.y < mseg_need.y)
			y_p+=(mseg_need.y-ref_kp.y)/(modelbox.m_bottom-ref_kp.y)*(sriptbox.m_bottom-ref_hp.y)+ref_hp.y;
		else if((ref_kp.y > mseg_need.y))
			y_p+=-(ref_kp.y-mseg_need.y)/(ref_kp.y-modelbox.m_top)*(ref_hp.y-sriptbox.m_top)+ref_hp.y;
		else
			y_p+=ref_hp.y;
		m_count++;
	}
	
	///////////////////////////
	x_p /=m_count;
	y_p /=m_count;

	return CCPoint(x_p,y_p);
}

float RelaxationMatch::Evalute_length_forgreedy(int mk, boundingbox modelbox,boundingbox sriptbox)
{
	float mang=model.getsegment(mk).dirangle_;
	float mlen=model.getsegment(mk).len_;

	float xlen=mlen*fabs(cos(mang/180.0*3.14159));
	float ylen=mlen*fabs(sin(mang/180.0*3.14159));
	
	float newxlen=xlen/(modelbox.m_right-modelbox.m_left)*(sriptbox.m_right-sriptbox.m_left);
	float newylen=ylen/(modelbox.m_bottom-modelbox.m_top)*(sriptbox.m_bottom-sriptbox.m_top);

	return sqrt(newxlen*newxlen+newylen*newylen);
}

//根据如下参数计算匹配概率：位置(紧)、方向(稍微松)、长度(很松)、与已匹配笔段的交搭关系(紧)
list<candidateforgreedy> RelaxationMatch::probabilityforgreedy(ComponentMatch m_ComponentMatch, list<int> nomatchscript, int passnumber)
{
	list<candidateforgreedy> m_res;
	//1.计算该部件的包围盒
	boundingbox modelbox;
	boundingbox sriptbox;
	Evalute_boundingbox(m_ComponentMatch,modelbox,sriptbox);

	//2.为该部件的每个未匹配模板字笔段估计候选手写笔段
	list<int>::iterator it=m_ComponentMatch.nomatch_model_seg.begin();
	for(;it!=m_ComponentMatch.nomatch_model_seg.end();it++)
	{
		candidateforgreedy temp;
		temp.mk=*it;

		//2.1根据已匹配笔段、以及包围盒，估计候选手写笔段的中心位置、长度
		CCPoint mp=Evalute_position_forgreedy(*it, m_ComponentMatch, modelbox,sriptbox);
		float mlen=Evalute_length_forgreedy(*it, modelbox,sriptbox);

		//2.2寻找候选笔段及其匹配概率
		list<scriptpro> scan;
		if(passnumber==1)
			scan=Evalute_prob_forgreedy(*it, mp, mlen, m_ComponentMatch, nomatchscript);
		else if(passnumber==2)
			scan=Evalute_prob_forfree(*it, mp, mlen, m_ComponentMatch, nomatchscript);

		//2.3为了提高效率，剔除一些候选
		while(scan.size()>4)
		{
			float min_pro=100;
			list<scriptpro>::iterator min_it;

			for (list<scriptpro>::iterator itx=scan.begin(); itx!=scan.end();itx++)
			{
				if(itx->probability<min_pro)
				{
					min_pro=itx->probability;
					min_it=itx;
				}
			}			
			scan.erase(min_it);
		}

		//2.4保存
		if(scan.size()>0)
		{
			temp.shlist.assign(scan.begin(),scan.end());
			m_res.push_back(temp);
		}		
	}
	return m_res;
}

list<scriptpro> RelaxationMatch::Evalute_prob_forfree(int model_seg, CCPoint center_pos, float mlen, ComponentMatch m_ComponentMatch, list<int> nomatchscript)
{
	list<scriptpro> m_res;
	//////////////////////////////
	GeometryTool gt;
	Segment mseg=model.getsegment(model_seg);
	list<int>::iterator it=nomatchscript.begin();
	for (;it!=nomatchscript.end();it++)
	{
		Segment sseg=script.getsegment(*it);
		CCPoint segmid=sseg.mid_point_;

		//1距离——不用区分笔画稳定性
		float c3;
		if(model_count<10 && gt.pointDistance(segmid,center_pos)<=0.44*script.normal_size_)
			c3=0.01;
		else if(model_count>=10 && gt.pointDistance(segmid,center_pos)<=0.258*script.normal_size_)
			c3=0.01;
		else
			c3=100;
		
		//2缩放——需要区分稳定性
		float c2;
		float slen=sseg.len_;
		//float mlen=mseg.len;

		if(model.isinstablesegment(model_seg))
		{
			if(slen/mlen>3.2 || mlen/slen>7)
				c2=100;
			else
				c2=0.09;
		}
		else
		{
			if(max(slen,mlen)/min(slen,mlen)>5.4)
				c2=100;
			else if(max(slen,mlen)/min(slen,mlen)>4.2)
				c2=0.999;
			else
				c2=0.09;
		}

		//3旋转——需要区分稳定性
		float c1=0.0;
		float anglediff=0.0;

		if(!model.isinstablesegment(model_seg))
		{
			float angle1=sseg.dirangle_;
			float angle2=mseg.dirangle_;

			float temp1=max(angle2,angle1)-min(angle2,angle1);
			float temp2=min(angle2,angle1)-max(angle2,angle1)+360;
			float temp3=max(max(angle2,angle1)-180,min(angle2,angle1))-min(max(angle2,angle1)-180,min(angle2,angle1));
			
			anglediff=min(min(temp1,temp2),temp3);
			
			if(fabs(angle2-0)<3.0 || fabs(angle2-90)<3.0 || fabs(angle2-180)<3.0 || fabs(angle2-270)<3.0)
			{
				if(anglediff<=15)
					c1=0.0;
				else if(anglediff<=59.0)
					c1=0.04;
				else 
					c1=100;
			}
			else
			{
				if(anglediff<=20)
					c1=0.0;
				else if(anglediff<=58.0)
					c1=0.04;
				else 
					c1=100;
			}
		}

		//综合
		if(c1<1.0 && c2<1.0 && c3<1.0)
		{
			float similarity=1.0/(1.0+c1*anglediff+c2*max(slen,mlen)/min(slen,mlen)+
				                  c3*gt.pointDistance(segmid,center_pos)*512.0/script.normal_size_);
			scriptpro temp;
			temp.sh=*it;
			temp.probability=similarity;
			m_res.push_back(temp);
		}		
	}

	return m_res;
}

list<scriptpro> RelaxationMatch::Evalute_prob_forgreedy(int model_seg, CCPoint center_pos, float mlen, ComponentMatch m_ComponentMatch, list<int> nomatchscript)
{
	list<scriptpro> m_res;
	//////////////////////////////
	GeometryTool gt;
	Segment mseg=model.getsegment(model_seg);
	list<int>::iterator it=nomatchscript.begin();
	for (;it!=nomatchscript.end();it++)
	{
		Segment sseg=script.getsegment(*it);
		CCPoint segmid=sseg.mid_point_;

		//1距离——不用区分笔画稳定性
		float c3;
		if(gt.pointDistance(segmid,center_pos)<=0.32*script.normal_size_)
			c3=0.013;
		else
			c3=100;
		
		//2缩放——需要区分稳定性
		float c2;
		float slen=sseg.len_;		

		if(model.isinstablesegment(model_seg))
		{
			if(slen/mlen>3.2 || mlen/slen>7)
				c2=100;
			else
				c2=0.09;
		}
		else
		{
			if(max(slen,mlen)/min(slen,mlen)>4.2)
				c2=100;
			else
				c2=0.09;
		}

		//3旋转——需要区分稳定性
		float c1=0.0;
		float anglediff=0.0;

		if(!model.isinstablesegment(model_seg))
		{
			float angle1=sseg.dirangle_;
			float angle2=mseg.dirangle_;

			float temp1=max(angle2,angle1)-min(angle2,angle1);
			float temp2=min(angle2,angle1)-max(angle2,angle1)+360;
			float temp3=max(max(angle2,angle1)-180,min(angle2,angle1))-min(max(angle2,angle1)-180,min(angle2,angle1));
			
			anglediff=min(min(temp1,temp2),temp3);
			
			if(fabs(angle2-0)<3.0 || fabs(angle2-90)<3.0 || fabs(angle2-180)<3.0 || fabs(angle2-270)<3.0 || fabs(angle2-360)<3.0)
			{
				if(anglediff<=15)
					c1=0.0;
				else if(anglediff<=59.0)
					c1=0.033;
				else 
					c1=100;
			}
			else
			{
				if(anglediff<=23.3)
					c1=0.0;
				else if(anglediff<=58.0)
					c1=0.033;
				else 
					c1=100;
			}

			if(temp1>142 && temp2>142)
			{
				if(c1<0.01)
					c1+=0.044;
				else
					c1+=0.011;
			}
		}

		//4.交搭关系
		float c4=1.0;
		if(c1<1.0 && c2<1.0 && c3<1.0)
		{
			int m_count=1;
			list<candidate>::iterator it=m_ComponentMatch.havemath.begin();
			for (; it!=m_ComponentMatch.havemath.end();it++)
			{
				Segment temph=script.getsegment(it->h);
				Segment tempk=model.getsegment(it->k);

				float tempf=gt.Evaluate_relation_similarity(sseg,mseg,temph,tempk,model.GetRelation(model_seg,it->k), model.normal_size_, model_count>10);
				if(tempf<1.00001)
				{
					c4+=tempf;
					m_count++;
				}
				
			}
			//c4 /=m_count;
			c4 /= m_ComponentMatch.havemath.size()+1.0;
		}

		//综合
		if(c1<1.0 && c2<1.0 && c3<1.0 && c4>0.0)
		{				
			float show1;
			if(c1>=0.0439)
				show1=c1*max(fabs(anglediff-15.0),12.0)*0.5;
			else
				show1=c1*fabs(anglediff-15.0)*0.5;
			float show2=c2*max(slen,mlen)/min(slen,mlen)*1.15;
			float show3=gt.pointDistance(segmid,center_pos);
			float show4=c3*gt.pointDistance(segmid,center_pos)*0.5*512.0/script.normal_size_;
			float similarity=c4*1.0/(1.0+show1+c2*max(slen,mlen)/min(slen,mlen)*1.15+
				                  c3*gt.pointDistance(segmid,center_pos)*0.5*512.0/script.normal_size_);
			scriptpro temp;
			temp.sh=*it;
			temp.probability=similarity;
			m_res.push_back(temp);
		}		
	}

	return m_res;
}

/*
CCPoint RelaxationMatch::Evalute_script_seg_pos(int mseg, candidate ref_match)
{
	CCPoint mseg_ref_mid=model.getsegment(ref_match.k).mid_point_;
	CCPoint sseg_ref_mid=script.getsegment(ref_match.h).mid_point_;
	
	CCPoint mseg_need=model.getsegment(mseg).mid_point_;

	float x=sseg_ref_mid.getx()+(mseg_need.getx()-mseg_ref_mid.getx());
	float y=sseg_ref_mid.gety()+(mseg_need.gety()-mseg_ref_mid.gety());

	CCPoint m_res(x,y);
	return m_res;
}
*/

list<int> RelaxationMatch::Find_Nomatch_normal_Scriptseg_in_both_reslult()
{
	list<int> m_res;
	for (int h=0;h<script.segment_list_.size();h++)
	{
		if(!script.IsNoiseSegment(h))
		{
			bool yes1=true;
			bool yes2=true;
			////////////////////////////
			list<candidate>::iterator it=matchresult.begin();
			while(it!=matchresult.end())
			{
				if(it->h==h)
				{
					yes1=false;
					break;
				}
				else
					it++;
			}
			//////////////////////////
			list<multicandidate>::iterator itm=multimatchresult.begin();
			for (;itm!=multimatchresult.end(); itm++)
			{
				list<int>::iterator ith=itm->hlist.begin();
				for (;ith!=itm->hlist.end();ith++)
				{
					if(*ith == h)
					{
						yes2=false;
						break;
					}
				}
				if(ith!=itm->hlist.end())
					break;
			}

			//////////////////////////
			if(yes1 && yes2)
				m_res.push_back(h);
		}
	}

	return m_res;
}

void RelaxationMatch::SortandReduce(list<ComponentMatch> &compoint_list_)
{
	//删除满足如下条件的部件：没有未匹配的模板字笔段
	list<ComponentMatch>::iterator it=compoint_list_.begin();
	while(it!=compoint_list_.end())
	{
		if(it->nomatch_model_seg.size()==0)
			it=compoint_list_.erase(it);
		else
			it++;
	}

	//排序
	it=compoint_list_.begin();
	while(it!=compoint_list_.end())
	{
		list<ComponentMatch>::iterator ita=it;
		ita++;
		while(ita!=compoint_list_.end())
		{
			if(ita->nomatch_model_seg.size() < it->nomatch_model_seg.size())
				swap(*ita,*it);
			else if(ita->nomatch_model_seg.size() == it->nomatch_model_seg.size() &&
				    ita->havemath.size() > it->havemath.size())
					swap(*ita,*it);
			ita++;
		}
		it++;
	}
}

int RelaxationMatch::Find_Match_for_Model_segment_in_single_result(int segind)
{
	int m_res=-1;
	list<candidate>::iterator it=matchresult.begin();

	while(it!=matchresult.end())
	{
		if(it->k==segind)
		{
			m_res=it->h;
			break;
		}
		else
			it++;
	}

	return m_res;
}


int RelaxationMatch::Find_Match_for_script_segment_in_single_result(int sh)
{
	int m_res=-1;
	list<candidate>::iterator it=matchresult.begin();
	while(it!=matchresult.end())
	{
		if(it->h==sh)
		{
			m_res=it->k;
			break;
		}
		else
			it++;
	}
	
	return m_res;
}

/*
int RelaxationMatch::find_multimatch_for_script_segment(int sh, list<int> &shlist)
{
	int m_res=-1;
	///////////////////////////////////
	list<multicandidate>::iterator it=multimatchresult.begin();
	while(it!=multimatchresult.end())
	{
		if(it->klist.size()==1)
		{
			list<int>::iterator FindIterator;
			for(FindIterator=it->hlist.begin(); FindIterator!=it->hlist.end();FindIterator++)
			{
				if(*FindIterator == sh)
					break;
			}	
			if (FindIterator != it->hlist.end())
			{
				shlist.clear();
				shlist.assign(it->hlist.begin(),it->hlist.end());
				m_res=*(it->klist.begin());
				break;
			}
		}

		it++;
	}

	return m_res;
}
*/