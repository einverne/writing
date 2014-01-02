#include <vector>
#include "Stroke.h"
using namespace std;

#pragma once
class Bujian
{
public:
	bool addStroke(Stroke stro);
public:
    Bujian(void);
    ~Bujian(void);
    int strokeCount; 
    vector<Stroke> strokeList;
};

