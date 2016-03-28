//
//  Component.cpp
//  game
//
//  Created by Ein Verne on 16/3/21.
//
//

#include "Component.hpp"
#include <algorithm>

CComponent::CComponent()
{
    segments_list.clear();
    top_key_segments.clear();
    bottom_key_segments.clear();
    left_key_segments.clear();
    right_key_segments.clear();
}

CComponent::~CComponent()
{
    segments_list.clear();
    top_key_segments.clear();
    bottom_key_segments.clear();
    left_key_segments.clear();
    right_key_segments.clear();
}

CComponent::CComponent(const CComponent& other)//拷贝
{
    this->segments_list=other.segments_list;
    this->top_key_segments=other.top_key_segments;
    this->bottom_key_segments=other.bottom_key_segments;
    this->left_key_segments=other.left_key_segments;
    this->right_key_segments=other.right_key_segments;
}

CComponent& CComponent::operator=(const CComponent& other)//赋值
{
    if(this!=&other)
    {
        this->segments_list=other.segments_list;
        this->top_key_segments=other.top_key_segments;
        this->bottom_key_segments=other.bottom_key_segments;
        this->left_key_segments=other.left_key_segments;
        this->right_key_segments=other.right_key_segments;
    }
    return (*this);
}

void CComponent::setdata(list<int> segments_list, list<int> m_top, list<int> m_bottom, list<int> m_left, list<int> m_right)
{
    list<int>::iterator it;
    
    it=m_top.begin();
    while(it!=m_top.end())
    {
        list<int>::iterator ita = find( segments_list.begin(), segments_list.end(), *it );
        if ( ita == segments_list.end() )
            return;
        else
            it++;
    }
    
    it=m_bottom.begin();
    while(it!=m_bottom.end())
    {
        list<int>::iterator ita = find( segments_list.begin(), segments_list.end(), *it );
        if ( ita == segments_list.end() )
            return;
        else
            it++;
    }
    
    it=m_left.begin();
    while(it!=m_left.end())
    {
        list<int>::iterator ita = find( segments_list.begin(), segments_list.end(), *it );
        if ( ita == segments_list.end() )
            return;
        else
            it++;
    }
    
    it=m_right.begin();
    while(it!=m_right.end())
    {
        list<int>::iterator ita = find( segments_list.begin(), segments_list.end(), *it );
        if ( ita == segments_list.end() )
            return;
        else
            it++;
    } 
    
    /////////////////////////////////
    this->segments_list=segments_list;
    this->top_key_segments=m_top;
    this->bottom_key_segments=m_bottom;
    this->left_key_segments=m_left;
    this->right_key_segments=m_right;
}