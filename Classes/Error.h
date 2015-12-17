#ifndef __ERROR_H__
#define __ERROR_H__

#include <string>
#include <map>
using namespace std;

struct Error
{
	string errortype;
	map<int,float> errorstroke;
};

#endif