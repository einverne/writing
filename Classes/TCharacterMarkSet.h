#ifndef __TCharacterMarkSet_H__
#define __TCharacterMarkSet_H__

#include <string>
using namespace std;

class TCharacterMarkSet
{
public:
	TCharacterMarkSet(void);
	~TCharacterMarkSet(void);
	string m_zi;				// character name
	string m_code;				// character number, code
	string m_mark;				// character mark data
	/* °×
	<instablesegments index="" />
	<segmentrelation>
	<relation segment="0" relationship="N/CEB/CEB/A/A/A" />
	<relation segment="1" relationship="CBE/N/CBB/A/CNB/CEB" />
	<relation segment="2" relationship="CBE/CBB/N/CEB/A/A" />
	<relation segment="3" relationship="A/A/CBE/N/CNE/CEE" />
	<relation segment="4" relationship="A/CBN/A/CEN/N/A" />
	<relation segment="5" relationship="A/CBE/A/CEE/A/N" />
	</segmentrelation>
	<structbox><boundingbox struct="0/1/2/3/4/5" top="0" bottom="1/3/5" left="1/2/4/5" right="2/3/4/5" />
	</structbox>
	*/
};


#endif
