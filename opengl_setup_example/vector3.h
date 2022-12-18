//
//  vector3.h
//
//  Created by Richard Anton on 9/21/21.
//

#ifndef vector3_hpp
#define vector3_hpp

#include <iostream>
#include <cmath>

class Vector3;
double DotProduct( Vector3 a, Vector3 b );
Vector3 CrossProduct( Vector3 a, Vector3 b );

class Matrix;

class Vector3 {
public:  
	double x,y,z;
  
  	// constructors
	Vector3(void);
	Vector3(double inX, double inY, double inZ);
	Vector3(const Vector3& vec);
    
    ~Vector3() = default;
    
    // matrix is expected to be >= 3 rows and 1 col.
    Vector3(const Matrix& m);
    
	void Assign(double inX, double inY, double inZ);

	// magnitude
	double Magnitude(void) const;

    // magnitude squared
	double MagnitudeSquared(void) const;
	
	// return unit vector(noramlized) of this vector
	Vector3 Unit(void) const;

	// normalize
	void Normalize(void);

	// equals operator
	Vector3 operator = (Vector3 vec);

	// scalar multiply
	Vector3 operator * (double n) const;
	// scalar divider
	Vector3 operator / (double n) const;
	// plus
	Vector3 operator + (Vector3 vec) const;
	// minus
	Vector3 operator - (Vector3 vec) const;

	// scalar multiply
	void operator *= (double n);
	// scalar divider
	void operator /= (double n);
	// plus
	void operator += (Vector3 vec);
	// minus
	void operator -= (Vector3 vec);
	
    //  equality (be careful comparing floating points!)
    bool operator == (Vector3 vec) const;
    
    bool operator != (Vector3 vec);
    
	double Dot(Vector3 vec) const
	{
		return DotProduct(*this, vec);
	}
	Vector3 Cross(Vector3 vec) const
	{
		return CrossProduct(*this, vec);
	}

    // std::ostream& operator << (std::ostream& os);
    friend std::ostream& operator<< (std::ostream& out, const Vector3& vec);
    
	void PrettyPrint(std::ostream& out) const;

    // transform to 4x1 matrix.
    void ToMatrix(Matrix& outM) const;
    
    // check for NaN components
    bool IsNan(void) const {
        return isnan(x) || isnan(y) || isnan(z);
    }
    
	// unit test for class
	static bool Test(void);
    
    // static method for parsing from line(separated by whitespace).
    static Vector3 FromLineString(const std::string& s);
};



#endif /* vector3_hpp */

