//
//  ScriptCharacter.cpp
//  手写汉字表示
//
//  Created by Ein Verne on 16/3/16.
//
//

#include "ScriptCharacter.hpp"

ScriptCharacter::ScriptCharacter(){
    row_stroke.clear();
    divide = false;
    
    stroke_list.clear();
    segment_list.clear();
    noise_list.clear();
}

ScriptCharacter::ScriptCharacter(const ScriptCharacter& T){
    this->row_stroke = T.row_stroke;
    this->divide = T.divide;
    this->stroke_list = T.stroke_list;
    this->segment_list = T.segment_list;
    this->noise_list = T.noise_list;
}

ScriptCharacter& ScriptCharacter::operator=(const ScriptCharacter& T){
    this->row_stroke = T.row_stroke;
    this->divide = T.divide;
    this->stroke_list = T.stroke_list;
    this->segment_list = T.segment_list;
    this->noise_list = T.noise_list;
    
    return *this;
}

ScriptCharacter::~ScriptCharacter(){
    row_stroke.clear();
    stroke_list.clear();
    segment_list.clear();
    noise_list.clear();

}