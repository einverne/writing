#include "HcharacterLayer.h"

HcharacterLayer::HcharacterLayer()
{
}

HcharacterLayer::~HcharacterLayer()
{
}

bool HcharacterLayer::init(){
	if (CCLayer::init())
	{

		return true;
	}
	return false;
}