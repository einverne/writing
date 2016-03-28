//
//  RowStroke.hpp
//  用于表示没有进行笔段分割处理的手写笔画
//
//  Created by Ein Verne on 16/3/16.
//
//

#ifndef RowStroke_hpp
#define RowStroke_hpp

#include <stdio.h>
#include <list>
#include "Stroke.h"
using namespace std;

#include "cocos2d.h"
USING_NS_CC;

class RowStroke : public Stroke{
public:
    
private:
    list<CCPoint> row_list;
};

#endif /* RowStroke_hpp */
