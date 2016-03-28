//
//  FlagMatrix.hpp
//  game
//
//  Created by Ein Verne on 16/3/21.
//
//

#ifndef FlagMatrix_hpp
#define FlagMatrix_hpp

#include <stdio.h>
#include <string>
#include <list>
using namespace std;

class FlagMatrix
{
protected:
    int m_nNumRows;       //行数
    int m_nNumColumns;    //列数
    string *m_pData;      //数据
    
public:
    //构造和析构
    FlagMatrix();
    FlagMatrix(int nRows,int nCols);
    FlagMatrix(int nRows,int nCols,string m_value[]);
    FlagMatrix(const FlagMatrix& other);
    bool Init(int nRows,int nCols);
    virtual ~FlagMatrix();
    
    //元素值的获取和设置
    void    clear();
    bool    SetElement(int nRow,int nCol,string m_value);
    string  GetElement(int nRow,int nCol) const;
    void    SetData(string m_value[]);
    int     GetNumColumns() const;
    int     GetNumRows() const;
    int     GetRowVector(int nRow,string* pVector) const;
    int     GetColVector(int nCol,string* pVector) const;
    string* GetData() const;
    
    //赋值
    FlagMatrix& operator=(const FlagMatrix& other);
    
};

#endif /* FlagMatrix_hpp */
