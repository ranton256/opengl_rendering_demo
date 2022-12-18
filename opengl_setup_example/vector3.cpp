//
//  vector3.cpp
//
//  Created by Richard Anton on 9/21/21.
//


#include "vector3.h"
#include "dbgutils.h"

#include <sstream>


#include "matrix.h"

using namespace std;

// constructors
Vector3::Vector3(void)
{
    // do nothing
}

Vector3::Vector3(double inX, double inY, double inZ)
{
    x = inX;
    y = inY;
    z = inZ;
}

void Vector3::Assign(double inX, double inY, double inZ)
{
    x = inX;
    y = inY;
    z = inZ;
}

Vector3::Vector3(const Vector3& vec)
{
    x = vec.x;
    y = vec.y;
    z = vec.z;
}

// matrix is expected to be >= 3 rows and 1 col.
Vector3::Vector3(const Matrix& m)
{
    DbgAssert(m.Cols() >= 1);
    DbgAssert(m.Rows() >= 3);
    
    if(m.Rows() >= 3) {
        x = m[0][0];
        y = m[1][0];
        z = m[2][0];
    }
}



double Vector3::MagnitudeSquared(void) const
{
    double result = x*x;
    result += y*y;
    result += z*z;
    return result;
}

// magnitude
double Vector3::Magnitude(void) const
{
    double mag = sqrt(MagnitudeSquared());
    return mag;
}

// return unit vector(noramlized) of this vector
Vector3 Vector3::Unit(void) const
{
    Vector3 v( *this );
    v.Normalize();
    return v;
}

// normalize
void Vector3::Normalize(void)
{
    double mag = Magnitude();
    if(mag != 0.0)
        (*this) /= mag;
}

// equals operator
Vector3 Vector3::operator = (Vector3 vec)
{
    x = vec.x;
    y = vec.y;
    z = vec.z;
    return *this;
}


// scalar multiply
Vector3 Vector3::operator * (double n) const
{
    Vector3 vec = *this;
    vec *= n;
    return vec;
}

// scalar divider
Vector3 Vector3::operator / (double n) const
{
    Vector3 vec = *this;
    vec /= n;
    return vec;
}

// plus
Vector3 Vector3::operator + (Vector3 vec) const
{
    Vector3 temp = vec;
    temp += *this;
    return temp;
}

// minus
Vector3 Vector3::operator - (Vector3 vec) const
{
    Vector3 temp = *this;
    temp -= vec;
    return temp;
}


// scalar multiply
void Vector3::operator *= (double n)
{
    x *= n;
    y *= n;
    z *= n;
}

// scalar divider
void Vector3::operator /= (double n)
{
    DbgAssert(n != 0.0 );
    double inverse = 1.0 / n;
    x *= inverse;
    y *= inverse;
    z *= inverse;
}

// plus
void Vector3::operator += (Vector3 vec)
{
    x += vec.x;
    y += vec.y;
    z += vec.z;
}

// minus
void Vector3::operator -= (Vector3 vec)
{
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
}

//  equality (be careful comparing floating points!)
bool Vector3::operator == (Vector3 vec) const
{
    return (x == vec.x && y == vec.y && z == vec.z);
}

bool Vector3::operator != (Vector3 vec)
{
    return (x != vec.x || y != vec.y || z != vec.z);
}

double DotProduct( Vector3 a, Vector3 b )
{
    double dot;
    dot = a.x * b.x;
    dot += a.y * b.y;
    dot += a.z * b.z;
    return dot;
}

Vector3 CrossProduct( Vector3 a, Vector3 b )
{
    // U x V = x = UyVz - Uz*Vy, y = -(Ux*Vz - Uz*Vx), z = Ux*Vy - Uy*Vx
    
    Vector3 c;
    c.x =   a.y*b.z - a.z*b.y;
    c.y = -(a.x*b.z - a.z*b.x);
    c.z =   a.x*b.y - a.y*b.x;
    
    return c;
}

void Vector3::PrettyPrint(std::ostream& out) const
{
    out << *this << std::endl;
}


std::ostream& operator<< (std::ostream& os, const Vector3& vec)
{
    os << "[";
    os << vec.x << ", ";
    os << vec.y << ", ";
    os << vec.z;
    os << "]";
    
    return os;
}


Vector3 Vector3::FromLineString(const std::string& s) {
    stringstream ss(s);
    double x, y, z;
    ss >> x;
    ss >> y;
    ss >> z;
    
    return Vector3(x,y,z);
}

void Vector3::ToMatrix(Matrix& outM) const
{
    outM.SetSize(1,4);
    outM.Set(0, 0, x);
    outM.Set(1, 0, y);
    outM.Set(2, 0, z);
    outM.Set(3, 0, 1.0);
}



bool Vector3::Test(void)
{
    // test assignment, addition, subtraction, multiplication,
    Vector3 one23( 1, 2, 3);
    Vector3 cv( one23 );
    Vector3 times;
    times = one23 * 4;
    
    DbgAssert( times.x == 4 );
    DbgAssert( times.y == 8 );
    DbgAssert( times.z == 12 );
    
    // test equal/not-equal
    DbgAssert(Vector3(1,2,3) == Vector3(1.0,2.0,3.0));
    
    DbgAssert(Vector3(1,2,3) != Vector3(2.0,2.0,3.0));
    DbgAssert(Vector3(1,2,3) != Vector3(1.0,5.0,3.0));
    DbgAssert(Vector3(1,2,3) != Vector3(1.0,2.0,4.0));
    
    Vector3 unit = times;
    unit.Normalize();
    DbgAssert( FCompare( unit.Magnitude(), 1.0 ));
    // test dot product
    Vector3 a( 1, 1, 0 );
    a.Normalize();
    Vector3 b = a;
    b.z = a.Magnitude();
    double dc = a.Dot( b );
    double oneOverSqrt2 = 1.0 / sqrt(2.0);
    double cos45 = dc / b.Magnitude(); // a mag is 1 anyway
    DbgAssert( FCompare( cos45, oneOverSqrt2  ) );
#if VERBOSE_DEBUG
    cout << "cos45 is " << cos45 << " and 1/sqrt(2) is " << oneOverSqrt2 << std::endl;
#endif
    
    // test subtraction
    Vector3 less( .5, 1, 1.5);
    Vector3 diff = one23 - less;
#if VERBOSE_DEBUG
    cout << "diff  is " << diff.x << ", " << diff.y << ", " << diff.z << std::endl;
#endif
    DbgAssert( FCompare( diff.x, .5 ) );
    DbgAssert( FCompare( diff.y, 1 ) );
    DbgAssert( FCompare( diff.z, 1.5 ) );
    
    // test cross product
    Vector3 v1( 1, 0, 0 );
    Vector3 v2( 0, 1, 0 );
    Vector3 v3 = CrossProduct(v1, v2);
    DbgAssert( FCompare( v3.x, 0 ) );
    DbgAssert( FCompare( v3.y, 0 ) );
    DbgAssert( FCompare( v3.z, 1 ) );
#if VERBOSE_DEBUG
    cout << "cross product is " << v3.x << ", " << v3.y << ", " << v3.z << std::endl;
#endif
    // test from string
    string test_strings[] = {
        "0 1 2",
        "0 1 2 ",
        " 0 1 2 ",
        " 0 1 2 \n"
    };
    for (auto str : test_strings) {
#if VERBOSE_DEBUG
        cout << "Checking test string:" << str << "-\n";
#endif
        Vector3 vfs = Vector3::FromLineString(str);
        DbgAssert( FCompare( vfs.x, 0 ) );
        DbgAssert( FCompare( vfs.y, 1 ) );
        DbgAssert( FCompare( vfs.z, 2 ) );
    }
    
    // test << operator
    stringstream ss;
    ss << one23;
    DbgAssert(ss.str() == string("[1, 2, 3]"));
    
    // Test ToMatrix
    Vector3 fm(4,5,6);
    Matrix m;
    m.Identity(1);
    fm.ToMatrix(m);
    DbgAssert(m.Rows() == 4);
    DbgAssert(m.Cols() == 1);
    DbgAssert(m[0][0] == fm.x);
    DbgAssert(m[1][0] == fm.y);
    DbgAssert(m[2][0] == fm.z);
    DbgAssert(m[3][0] == 1.0);
    
    Vector3 fromM(m);
    DbgAssert(fromM == fm);
    
    Vector3 zv(0,0,0);
    DbgAssert(!zv.IsNan());
    
    zv.Normalize();
    DbgAssert(!zv.IsNan());
    
    if (DbgHasAssertFailed()) {
        return false;
    }
    return true;
}
