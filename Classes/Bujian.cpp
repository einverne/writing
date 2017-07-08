#include "Bujian.h"


Bujian::Bujian(void)
{
	strokeCount = 0;
}


Bujian::~Bujian(void)
{
	strokeList.clear();
}

bool Bujian::addStroke(Stroke stro){
	vector<Stroke>::iterator it = strokeList.end();
	strokeList.insert(it,stro);
	strokeCount++;
	return true;
}