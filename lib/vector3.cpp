#include <cmath>
#include "../include/vector3.h"
using namespace std;

// --- Constructors ---

Vector3::Vector3(float tx, float ty, float tz)
{
    x = tx; y = ty; z = tz;
}
Vector3::Vector3(float t)
{
    x = t; y = t; z = t;    
}
Vector3::Vector3()
{
    x = 0; y = 0; z = 0;
}

// --- Nullary Operators ---
// Arithmetic
float Vector3::norm()
{// Length of a vector
    return sqrt(x*x + y*y + z*z);
}

// --- Unary Operators ---
// Arithmetic
Vector3 Vector3::operator-()
{// Addictive inverse of a
    return Vector3(-x, -y, -z);
}

// --- Binary Operators ---
// Arithmetic
Vector3 Vector3::operator+(Vector3 a)
{// Addition 
    return Vector3(x + a.x, y + a.y, z + a.z);
}

Vector3 Vector3::operator-(Vector3 a)
{// Subtraction
    return Vector3(x - a.x, y - a.y, z - a.z);
}

float Vector3::operator*(Vector3 a)
{// Dot Product
    return x*a.x + y*a.y + z*a.z;
}

Vector3 Vector3::cross(Vector3 a)
{// Cross Product
    float cx = (y*a.z - z*a.y);
    float cy = (z*a.x - x*a.z);
    float cz = (x*a.y - y*a.x);
    return Vector3(cx, cy, cz);
}

Vector3 Vector3::scale(float s)
{// Scalar product
    return Vector3(s*x, s*y, s*z);
}

// Logic
Vector3 Vector3::operator==(Vector3 a)
{// Equality, a_i == b_i
    return (x == a.x) && (y == a.y) && (z == a.z); 
}