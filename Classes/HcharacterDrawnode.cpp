#include "HcharacterDrawnode.h"

HcharacterDrawnode::HcharacterDrawnode()
{
}

HcharacterDrawnode::~HcharacterDrawnode()
{
}

bool HcharacterDrawnode::init(){
	return true;
}

void HcharacterDrawnode::draw(){
	vector<StrokeDrawnode*>::iterator iter = strokeDrawlist.begin();
	for (iter ; iter != strokeDrawlist.end() ; ++iter)
	{
		(*iter)->draw();
	}
}

void HcharacterDrawnode::addPoint(CCPoint point){
	(strokeDrawlist.at(strokeDrawlist.size()-1))->addPoint(point);
}

void HcharacterDrawnode::addStroke(Stroke s){
	StrokeDrawnode* drawnode = StrokeDrawnode::create(s);
	this->strokeDrawlist.push_back(drawnode);
	drawnode->retain();
}

void HcharacterDrawnode::removeLastStroke(){
	(strokeDrawlist.at(strokeDrawlist.size()-1))->release();
	this->strokeDrawlist.pop_back();
}