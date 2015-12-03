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

