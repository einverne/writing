//
//  ScriptCharacter.hpp
//  game
//
//  Created by Ein Verne on 16/3/16.
//
//

#ifndef ScriptCharacter_hpp
#define ScriptCharacter_hpp

#include <stdio.h>
#include <list>
#include "RowStroke.hpp"
#include "Stroke.h"
#include "Segment.hpp"

class ScriptCharacter{
public:
    list<RowStroke> row_stroke;
    bool divide;
    
    list<Stroke> stroke_list;
    list<Segment> segment_list;
    
    list<int> noise_list;               //疑似抖笔笔段列表。怀疑是抖笔的笔段索引号。
    
    ScriptCharacter();
    ScriptCharacter(const ScriptCharacter& T);
    ScriptCharacter& operator=(const ScriptCharacter& T);    //赋值
    ~ScriptCharacter();
};

#endif /* ScriptCharacter_hpp */
