//
//  FlagMatrix.cpp
//  game
//
//  Created by Ein Verne on 16/3/21.
//
//

#include "FlagMatrix.hpp"

FlagMatrix::FlagMatrix()
{
    m_nNumColumns = 0;
    m_nNumRows = 0;
    m_pData = NULL;
}

FlagMatrix::~FlagMatrix()
{
    if(m_pData)
    {
        delete[] m_pData;
        m_pData = NULL;
    }
}


FlagMatrix::FlagMatrix(int nRows,int nCols)
{
    m_nNumRows = nRows;
    m_nNumColumns = nCols;
    m_pData = NULL;
    Init(m_nNumRows,m_nNumColumns);
}

FlagMatrix::FlagMatrix(int nRows,int nCols,string m_value[])
{
    m_nNumRows = nRows;
    m_nNumColumns = nCols;
    m_pData = NULL;
    Init(m_nNumRows,m_nNumColumns);
    
    SetData(m_value);
}

FlagMatrix::FlagMatrix(const FlagMatrix& other)
{
    m_nNumRows = other.GetNumRows();
    m_nNumColumns = other.GetNumColumns();
    m_pData = NULL;
    
    Init(m_nNumRows,m_nNumColumns);
    //copy the pointer
    for (int row=0;row<m_nNumRows;row++)
    {
        for (int col=0;col<m_nNumColumns;col++)
        {
            m_pData[row*m_nNumColumns+col]=other.m_pData[row*m_nNumColumns+col];
        }
    }
}

bool FlagMatrix::Init(int nRows,int nCols)
{
    if(m_pData)
    {
        delete[] m_pData;
        m_pData = NULL;
    }
    
    m_nNumRows = nRows;
    m_nNumColumns = nCols;
    int nSize = nCols * nRows;
    if(nSize <= 0)
        return false;
    
    //分配内存
    m_pData = new string[nSize];
    
    if(m_pData == NULL)
        return false;
    
    //各个元素置零
    for (int row=0;row<m_nNumRows;row++)
    {
        for (int col=0;col<m_nNumColumns;col++)
        {
            m_pData[row*m_nNumColumns+col]="";
        }
    }
    
    return true;
}

//元素值的获取和设置
void    FlagMatrix::clear()
{
    m_nNumColumns = 0;
    m_nNumRows = 0;
    if(m_pData)
    {
        delete[] m_pData;
        m_pData = NULL;
    }
}

bool    FlagMatrix::SetElement(int nRow,int nCol,string m_value)
{
    //array bounds error
    if(nCol < 0 || nCol >= m_nNumColumns || nRow < 0 || nRow >= m_nNumRows)
        return false;
    if(m_pData == NULL)
        return false;
    m_pData[nCol + nRow * m_nNumColumns] = m_value;
    
    return true;
}

string  FlagMatrix::GetElement(int nRow,int nCol) const
{
    //array bounds error
    //ASSERT(nCol >= 0 && nCol < m_nNumColumns && nRow >= 0 && nRow < m_nNumRows);
    //ASSERT(m_pData);
    return m_pData[nCol + nRow * m_nNumColumns];
}

void FlagMatrix::SetData(string m_value[])
{
    for (int row=0;row<m_nNumRows;row++)
    {
        for (int col=0;col<m_nNumColumns;col++)
        {
            m_pData[row*m_nNumColumns+col]=m_value[row*m_nNumColumns+col];
        }
    }
}

int FlagMatrix::GetNumColumns() const
{
    return m_nNumColumns;
}

int FlagMatrix::GetNumRows() const
{
    return m_nNumRows;
}

int FlagMatrix::GetRowVector(int nRow,string* pVector) const
{
    if(pVector != NULL)
    {
        delete pVector;
        pVector=NULL;
    }
    
    pVector = new string[m_nNumColumns];
    //ASSERT(pVector != NULL);
    
    for(int j = 0; j< m_nNumColumns; ++j)
        pVector[j] = GetElement(nRow,j);
    return m_nNumColumns;
}

int FlagMatrix::GetColVector(int nCol,string* pVector) const
{
    if(pVector != NULL)
    {
        delete pVector;
        pVector=NULL;
    }
    
    pVector = new string[m_nNumRows];
    //ASSERT(pVector != NULL);
    
    for(int i = 0; i< m_nNumRows; ++i)
        pVector[i] = GetElement(i,nCol);
    return m_nNumRows;
}

string* FlagMatrix::GetData() const
{
    return m_pData;
}

//赋值
FlagMatrix& FlagMatrix::operator=(const FlagMatrix& other)
{
    if(& other != this)
    {
        m_nNumColumns = other.GetNumColumns();
        m_nNumRows = other.GetNumRows();
        
        Init(m_nNumRows,m_nNumColumns);
        //copy the pointer
        for (int row=0;row<m_nNumRows;row++)
        {
            for (int col=0;col<m_nNumColumns;col++)
            {
                m_pData[row*m_nNumColumns+col]=other.m_pData[row*m_nNumColumns+col];
            }
        }
    }   
    return *this;
}
