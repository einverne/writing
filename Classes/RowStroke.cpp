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
	this->Rowplist.clear();
}

RowStroke::~RowStroke()
{
	this->Rowplist.clear();
}

RowStroke& RowStroke::operator=(const RowStroke& Right)
{
	this->Rowplist.assign(Right.Rowplist.begin(),Right.Rowplist.end());
	return(*this);
}
RowStroke::RowStroke(const RowStroke& Right)
{
	this->Rowplist.assign(Right.Rowplist.begin(),Right.Rowplist.end());
}

void RowStroke::Init(list<CCPoint> Right)
{
	this->Rowplist=Right;
}


void RowStroke::clear()
{
	this->Rowplist.clear();
}

CCPoint RowStroke::getpoint(int ind)
{
	//ASSERT(ind>=0);
	//ASSERT(ind<this->Rowplist.size());
	
	list<CCPoint>::iterator it = this->Rowplist.begin();
	
	for (int x=0;x<ind;x++)
	{
		it++;
	}
	
	return *it;
}

void RowStroke::setpoint(int ind, CCPoint p)
{
	//ASSERT(ind>=0);
	//ASSERT(ind<this->Rowplist.size());
	
	list<CCPoint>::iterator it = this->Rowplist.begin();
	
	for (int x=0;x<ind;x++)
	{
		it++;
	}
	
	*it=p;
}
void RowStroke::setpointlist(list<CCPoint> ptlist)
{
	if(&ptlist != &(this->Rowplist))
	{
		this->Rowplist.clear();
		this->Rowplist=ptlist;
	}
}
	
list<CCPoint> RowStroke::getpointlist()
{
	return this->Rowplist;
}