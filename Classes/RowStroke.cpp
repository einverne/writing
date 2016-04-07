//
//  RowStroke.cpp
//  game
//
//  Created by Ein Verne on 16/3/16.
//
//

#include "RowStroke.hpp"


RowStroke::RowStroke()
{
	this->row_list_.clear();
}

RowStroke::~RowStroke()
{
	this->row_list_.clear();
}

RowStroke& RowStroke::operator=(const RowStroke& Right)
{
	this->row_list_.assign(Right.row_list_.begin(),Right.row_list_.end());
	return(*this);
}
RowStroke::RowStroke(const RowStroke& Right)
{
	this->row_list_.assign(Right.row_list_.begin(),Right.row_list_.end());
}

void RowStroke::Init(vector<CCPoint> Right)
{
	this->row_list_=Right;
}


void RowStroke::Clear()
{
	this->row_list_.clear();
}

CCPoint RowStroke::GetPoint(int ind)
{	
	if (ind < 0 || ind > row_list_.size())
	{
		return ccp(0,0);
	}else{
		return row_list_.at(ind);
	}
}

void RowStroke::SetPoint(int ind, CCPoint p)
{
	if (ind >= 0 && ind < row_list_.size())
	{
		row_list_[ind] = p;
	}
}
void RowStroke::SetPointList(vector<CCPoint> ptlist)
{
	if(&ptlist != &(this->row_list_))
	{
		this->row_list_.clear();
		this->row_list_=ptlist;
	}
}

void RowStroke::SetPointList(list<CCPoint> ptlist){
	row_list_.clear();
	for (list<CCPoint>::iterator itr = ptlist.begin();
		itr != ptlist.end(); itr++)
	{
		row_list_.push_back(*itr);
	}
}

vector<CCPoint> RowStroke::GetPointList()
{
	return this->row_list_;
}

list<CCPoint> RowStroke::GetRowPoint(){
	list<CCPoint> list_ret;
	for (vector<CCPoint>::iterator itr = row_list_.begin();
		itr != row_list_.end(); itr++)
	{
		list_ret.push_back(*itr);
	}
	return list_ret;
}