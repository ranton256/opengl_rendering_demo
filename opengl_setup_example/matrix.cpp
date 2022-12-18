//
//  matrix.cpp
//  geomodel
//
//  Created by Richard Anton on 10/22/21.
//

#ifndef __MATRIX__
#include "matrix.h"
#endif

#include "dbgutils.h"
#include <iostream>

// constructors
Matrix::Matrix(void)
{
    mRows = mCols = 0;
    mMat = NULL;
}

Matrix::Matrix(unsigned cols, unsigned rows)
{
    mMat = NULL;
    Alloc(cols,rows);
}

Matrix::Matrix(const Matrix& mat)
{
    mMat = NULL;
    Copy(mat);
}

void Matrix::Alloc(unsigned cols, unsigned rows)
{
    this->mCols = cols;
    this->mRows = rows;
    if(cols && rows)
    {
        mMat = new double[rows*cols];
    }
    else
    {
        mMat = NULL;
    }
}

void Matrix::Free(void)
{
    if(mMat)
    {
        delete[] mMat;
        mMat = NULL;
    }
}

void Matrix::SetSize(unsigned cols, unsigned rows)
{
    if(mRows != rows || mCols != cols || mMat == NULL)
    {
        Free();
        Alloc(cols, rows);
    }
}

void Matrix::Copy(const Matrix& mat)
{
    Free();
    Alloc(mat.mCols, mat.mRows);
    for(unsigned i=0; i<mRows; i++) 
        for(unsigned j=0; j<mCols; j++)
            mMat[i*mCols + j] = mat.mMat[i*mCols + j];
}

void Matrix::Take(Matrix& mat)
{
    Free();
    mRows = mat.mRows;
    mCols = mat.mCols;
    mMat = mat.mMat;
    mat.mMat = NULL;
    mat.mRows = 0;
    mat.mCols = 0;
}

Matrix::~Matrix()
{
    Free();
}

// equals operator
const Matrix& Matrix::operator = (const Matrix& mat)
{
    Copy(mat);
    return *this;
}

double Matrix::Get(unsigned row, unsigned col) const
{
    DbgAssert(row<mRows);
    DbgAssert(col<mCols);
    return mMat[row*mCols + col];
}

void Matrix::Set(unsigned row, unsigned col, double val)
{
    DbgAssert(row<mRows);
    DbgAssert(col<mCols);
    mMat[row*mCols + col] = val;
}

bool Matrix::operator == (const Matrix& mat)
{
    if (mRows != mat.mRows || mCols != mat.mCols)
        return false;

    bool equal = true;
    for(unsigned i=0; i<mRows; i++) 
        for(unsigned j=0; j<mCols; j++)
            if( mMat[i*mCols + j] != mat.mMat[i*mCols + j])
            {
                equal = false;
                break;
            }

    return equal;
}

bool Matrix::operator != (const Matrix& mat)
{
    bool equal = *this == mat;
    return !equal;
}

const double* Matrix::operator [] (unsigned row) const
{
    DbgAssert(row < mRows);
    return mMat + row*mCols;
}

double* Matrix::operator [] (unsigned row)
{
    DbgAssert(row < mRows);
    return mMat + row*mCols;
}

void Matrix::SetRow(unsigned i, const double *row)
{
    DbgAssert(i < mRows);
    DbgAssert(row != NULL);
    for(unsigned j=0; j<mCols; j++)
        mMat[i*mCols + j] = row[j];
}

void Matrix::SetAll(const double* inM)
{
    for(unsigned i=0; i<mRows; i++) {
        for(unsigned j=0; j<mCols; j++) {
            mMat[i*mCols + j] = inM[i*mCols + j];
        }
    }
}

// Set this to zero matrix of a certain size
void Matrix::Zero(unsigned n)
{
    SetSize(n,n);
    for(unsigned i=0; i<mRows; i++) 
        for(unsigned j=0; j<mCols; j++)
            mMat[i*mCols + j] = 0;
}

// Set this to identity matrix of a certain size
void Matrix::Identity(unsigned n)
{
    SetSize(n,n);
    for(unsigned i=0; i<mRows; i++) 
        for(unsigned j=0; j<mCols; j++)
            mMat[i*mCols + j] = (i == j)  ? 1 : 0;
}

// matrix multiply a * b into r.
void Matrix::Multiply(Matrix& r, const Matrix& a, const Matrix& b)
{
    if( a.mCols != b.mRows ) 
        throw new MatrixSizeException();

    // As an improvement we could make this safe when a or b is r.
    unsigned rows = a.mRows;
    unsigned cols = b.mCols;
    r.SetSize(cols, rows);
    for(unsigned i=0; i<rows; i++) 
        for(unsigned j=0; j<cols; j++)
        {
            double sum = 0.0;
            for(unsigned k=0; k < a.mCols; k++) 
            {
                double fa = a[i][k];
                double fb = b[k][j];
                sum += fa * fb;
            }
            r[i][j] = sum;
        }
}

// scalar multiply a * b into r
void Matrix::Multiply(Matrix& r, const Matrix& a, double b)
{
    r.SetSize(a.mCols, a.mRows);
    for(unsigned i=0; i<a.mRows; i++) 
        for(unsigned j=0; j<a.mCols; j++)
            r[i][j] = b * a[i][j];
}

// matrix addition
void Matrix::Add(Matrix& r, const Matrix& a, const Matrix& b)
{
    if(a.mRows != b.mRows || a.mCols != b.mCols)
        throw new MatrixSizeException();
    r.SetSize(a.mCols, a.mRows);
    for(unsigned i=0; i<a.mRows; i++) 
        for(unsigned j=0; j<a.mCols; j++)
            r[i][j] = a[i][j] + b[i][j];
}

// matrix subtraction
void Matrix::Subtract(Matrix& r, const Matrix& a, const Matrix& b)
{
    if(a.mRows != b.mRows || a.mCols != b.mCols)
        throw new MatrixSizeException();
    r.SetSize(a.mCols, a.mRows);
    for(unsigned i=0; i<a.mRows; i++) 
        for(unsigned j=0; j<a.mCols; j++)
        {
            r[i][j] = a[i][j] - b[i][j];
        }
}

Matrix Matrix::operator * (const Matrix& m) const
{
    Matrix r;
    Multiply(r, *this, m);
    return r;
}

Matrix Matrix::operator * (double c) const
{
    Matrix r;
    Multiply(r, *this, c);
    return r;
}

Matrix Matrix::operator / (double c) const
{
    double inv = 1.0 / c;
    Matrix r;
    Multiply(r, *this, inv);
    return r;
}

Matrix Matrix::operator + (Matrix mat) const
{
    Matrix m;
    Add(m, *this, mat);
    return m;
}

Matrix Matrix::operator - (Matrix mat) const
{
    Matrix m;
    Subtract(m, *this, mat);
    return m;
}

void Matrix::operator *= (const Matrix& m)
{
    Matrix r;
    Multiply(r, *this, m);
    Take(r);
}

void Matrix::operator *= (double c)
{
    Matrix m;
    Multiply(m, *this, c);
    Take(m);
}

void Matrix::operator /= (double c)
{
    double inv = 1.0 / c;
    Matrix r;
    Multiply(r, *this, inv);
}

void Matrix::operator += (Matrix mat)
{
    Matrix m;
    Add(m, *this, mat);
    Take(m);
}

void Matrix::operator -= (Matrix mat)
{
    Matrix m;
    Subtract(m, *this, mat);
    Take(m);
}

void Matrix::PrettyPrint(std::ostream& out)
{
    for(unsigned i=0; i<mRows; i++) 
    {
        out << "[";
        for(unsigned j=0; j<mCols; j++)
        {
            if(j>0)
                out << ",";
            out << Get(i,j);
        }
        out << "]" << std::endl;
    }
}

Matrix Matrix::Transpose() const
{
    Matrix tMat(mRows, mRows);
    for(unsigned i=0; i<mRows; i++)
    {
        for(unsigned j=0; j<mCols; j++)
        {
            tMat[j][i] = Get(i,j);
        }
    }
    return tMat;
}


// unit test for class
bool Matrix::Test(void)
{
    Matrix empty;
    DbgAssert(empty.Rows() == 0);
    DbgAssert(empty.Cols() == 0);
    
    Matrix m3x2(3,2);
    DbgAssert(m3x2.Rows() == 2);
    DbgAssert(m3x2.Cols() == 3);
    m3x2.Set(0,0, 1.0);
    m3x2.Set(0,1, 2.0);
    m3x2.Set(0,2, 3.0);
    m3x2.Set(1,0, 2.0);
    m3x2.Set(1,1, 4.0);
    m3x2.Set(1,2, 6.0);

    DbgAssert( m3x2.Get(0,0) ==1.0);
    DbgAssert( m3x2.Get(0,1) ==2.0);
    DbgAssert( m3x2.Get(0,2) ==3.0);
    DbgAssert( m3x2.Get(1,0) ==2.0);
    DbgAssert( m3x2.Get(1,1) ==4.0);
    DbgAssert( m3x2.Get(1,2) ==6.0);

    DbgAssert( m3x2[0][0] ==1.0);
    DbgAssert( m3x2[0][1] ==2.0);
    DbgAssert( m3x2[0][2] ==3.0);
    DbgAssert( m3x2[1][0] ==2.0);
    DbgAssert( m3x2[1][1] ==4.0);
    DbgAssert( m3x2[1][2] ==6.0);
    
    Matrix copy(m3x2);
    DbgAssert(copy.Rows() == 2);
    DbgAssert(copy.Cols() == 3);
    DbgAssert( copy.Get(0,0) ==1.0);
    DbgAssert( copy.Get(0,1) ==2.0);
    DbgAssert( copy.Get(0,2) ==3.0);
    DbgAssert( copy.Get(1,0) ==2.0);
    DbgAssert( copy.Get(1,1) ==4.0);
    DbgAssert( copy.Get(1,2) ==6.0);

    DbgAssert( copy == m3x2 );
    DbgAssert( !(copy != m3x2 ));
    copy.Set(1,2, 7.5);
    DbgAssert( !(copy == m3x2 ) );
    DbgAssert( copy != m3x2 );

    Matrix i;
    i.Identity(2);
    DbgAssert(i.Rows() == 2);
    DbgAssert(i.Cols() == 2);
    DbgAssert(i[0][0] == 1);
    DbgAssert(i[0][1] == 0);
    DbgAssert(i[1][0] == 0);
    DbgAssert(i[1][1] == 1);

    Matrix r;
    i.Identity(3);
    Matrix::Multiply(r, m3x2, i);
    DbgAssert(r==m3x2);

    r.Zero(1);
    r = m3x2 * i;
    DbgAssert(r==m3x2);

    // std::cout << std::endl << "Original:" << std::endl;
    // m3x2.PrettyPrint(std::cout);
#if VERBOSE_DEBUG
    std::cout << std::endl << "Identity:" << std::endl;
    i.PrettyPrint(std::cout);
#endif
    // std::cout << std::endl << "Result:" << std::endl;
    // r.PrettyPrint(std::cout);
    
    i.Identity(2);
    Matrix::Multiply(r, i, 2.0);
    DbgAssert(r[0][0] == 2);
    DbgAssert(r[0][1] == 0);
    DbgAssert(r[1][0] == 0);
    DbgAssert(r[1][1] == 2);


    // test scalar divide
    r = r / 2.0;
    DbgAssert(r == i);
    
    Matrix a(2,2), b(2,2);
    double aRow0[] = {0,1};
    double aRow1[] = {2,3};
    a.SetRow(0, aRow0);
    a.SetRow(1, aRow1);
    DbgAssert(a[0][0] == 0);
    DbgAssert(a[0][1] == 1);
    DbgAssert(a[1][0] == 2);
    DbgAssert(a[1][1] == 3);
    b[0][0] = 3;
    b[0][1] = 2;
    b[1][0] = 1;
    b[1][1] = 0;
    Matrix::Add(r, a, b);
    // std::cout << "addition:" << std::endl;
    // r.PrettyPrint(std::cout);
    DbgAssert(r[0][0] == 3);
    DbgAssert(r[0][1] == 3);
    DbgAssert(r[1][0] == 3);
    DbgAssert(r[1][1] == 3);
    
    Matrix sa(2, 3);
    const double kSetAll[3][2] = {{0,1}, {2,3}, {4,5}};
    sa.SetAll((const double*)kSetAll);
    DbgAssert(sa[0][0] == 0);
    DbgAssert(sa[0][1] == 1);
    DbgAssert(sa[1][0] == 2);
    DbgAssert(sa[1][1] == 3);
    DbgAssert(sa[2][0] == 4);
    DbgAssert(sa[2][1] == 5);
    
    
    Matrix z, r2;
    z.Zero(2);
    Matrix::Subtract(r2, r, z);
    // std::cout << "subtraction:" << std::endl;
    // r2.PrettyPrint(std::cout);
    // std::cout << "matrix a:" << std::endl;
    // a.PrettyPrint(std::cout);

    Matrix::Subtract(r, r, a);
    // std::cout << "subtraction:" << std::endl;
    // r.PrettyPrint(std::cout);
    DbgAssert(r == b);
    
    i.Identity(3);
    r.Zero(3);
    r = r - i;
    r = r * -1.0;
    DbgAssert( r == i );

    r.Zero(3);
    r = r + i;
    DbgAssert( r == i );

    r.SetSize(0,0);
    r.Zero(3);
    r += i;
    DbgAssert( r == i );

    
    Matrix oMat(2,2);
    const double oMatValues[2][2] = {{1,2}, {3,4}};
    oMat.SetAll((const double*)oMatValues);
    DbgAssert(oMat[0][0] == 1);
    DbgAssert(oMat[0][1] == 2);
    DbgAssert(oMat[1][0] == 3);
    DbgAssert(oMat[1][1] == 4);
    Matrix tMat = oMat.Transpose();
    DbgAssert(tMat[0][0] == 1);
    DbgAssert(tMat[0][1] == 3);
    DbgAssert(tMat[1][0] == 2);
    DbgAssert(tMat[1][1] == 4);
    
    
    if (DbgHasAssertFailed()) {
        return false;
    }
    return true;
}

