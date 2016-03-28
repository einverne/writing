//
//  Component.hpp
//  game
//
//  Created by Ein Verne on 16/3/21.
//
//

#ifndef Component_hpp
#define Component_hpp

#include <stdio.h>
#include <list>
using namespace std;

class CComponent
{
public:
    CComponent();
    CComponent(const CComponent& other);//拷贝
    CComponent& operator=(const CComponent& other);//赋值
    virtual ~CComponent();
    
    list<int> segments_list;
    list<int> top_key_segments;
    list<int> bottom_key_segments;
    list<int> left_key_segments;
    list<int> right_key_segments;
    
    void setdata(list<int> segments_list, list<int> m_top, list<int> m_bottom, list<int> m_left, list<int> m_right);
};

#endif /* Component_hpp */
