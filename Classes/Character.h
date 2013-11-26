#include "Bujian.h"
#include <vector>

#pragma once

using namespace std;

class Character
{
public:
    Character(void);
    ~Character(void);
    int bujianCount;
    vector<Bujian> bujianList;

};

