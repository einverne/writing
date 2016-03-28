#ifndef __Bujian_H__
#define __Bujian_H__

#include <vector>
#include "Stroke.h"
using namespace std;

class Bujian
{
public:
    Bujian(void);
    ~Bujian(void);
    bool addStroke(Stroke stro);
    int strokeCount;
    vector<Stroke> strokeList;
};

#endif