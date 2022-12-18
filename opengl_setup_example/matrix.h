//
//  matrix.h
//  geomodel
//
//  Created by Richard Anton on 10/22/21.
//

#ifndef __MATRIX__
#define __MATRIX__ 


#include <iostream>
#include <exception>

class MatrixSizeException : public std::exception {

    virtual const char *what() const throw()
    {
        return "Invalid size matrix for operaton";
    }
};

class Matrix {
private: 
    unsigned mCols, mRows;
    double *mMat;
protected:
    void Alloc(unsigned cols, unsigned rows);
    void Free(void);
    void Copy(const Matrix& mat);
    void Take(Matrix& mat);

public:  
	Matrix(void);
	Matrix(unsigned cols, unsigned rows);
	Matrix(const Matrix& mat);
	
	virtual ~Matrix();

	const Matrix& operator = (const Matrix& mat);

    void SetSize(unsigned cols, unsigned rows);

    unsigned Cols()  const { return mCols; }
    unsigned Rows() const { return mRows; }

    double Get(unsigned row, unsigned col) const;
    void Set(unsigned row, unsigned col, double val);
    
	bool operator == (const Matrix& mat);
	bool operator != (const Matrix& mat);

    const double* operator [] (unsigned row) const;
    double* operator [] (unsigned row);

    void PrettyPrint(std::ostream& out);

    void SetRow(unsigned i, const double *row);

    void SetAll(const double* inM);
    
    // Set this to zero matrix of a certain size
    void Zero(unsigned n);

    // Set this to identity matrix of a certain size
    void Identity(unsigned n);

    // matrix multiply a * b into r.
    static void Multiply(Matrix& r, const Matrix& a, const Matrix& b);

    // scalar multiply a * b into r
    static void Multiply(Matrix& r, const Matrix& a, double b);
    
    // matrix addition
    static void Add(Matrix& r, const Matrix& a, const Matrix& b);

    // matrix subtraction
    static void Subtract(Matrix& r, const Matrix& a, const Matrix& b);

    Matrix Transpose() const;
    
    // matrix multiply
	Matrix operator * (const Matrix& m) const;

	// scalar multiply
	Matrix operator * (double c) const;

	// scalar divider
	Matrix operator / (double c) const;

	// plus
	Matrix operator + (Matrix mat) const;
	// minus
	Matrix operator - (Matrix mat) const;

    // matrix multiply
    void operator *= (const Matrix& m);

	// scalar multiply
	void operator *= (double n);
	// scalar divider
	void operator /= (double n);

	// plus
	void operator += (Matrix mat);
	// minus
	void operator -= (Matrix mat);

	// unit test for class
	static bool Test(void);
};


#endif

