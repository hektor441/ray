#ifndef VEC3
#define VEC3

class Vector3 {
    public:
        float x, y, z;
        // Constructors
        Vector3(float, float, float);
        Vector3(float);
        Vector3();
        
            // Binary Operators
        // Arithmetic
        Vector3 operator+(Vector3);
        Vector3 operator-(Vector3);
        float operator*(Vector3); // dot product
        Vector3 cross(Vector3);
        Vector3 scale(float);
        // Logic
        Vector3 operator== (Vector3);
        
            // Unary Operators
        // Arithmetic
        Vector3 operator-();
        
            // Nullary Operators
        // Arithmetic
        float norm();
        Vector3 normalize() {return scale(1.0/norm());}
        Vector3 clamp() { return Vector3(
                            x > 255? 255 : x < 0 ? 0 : x,
                            y > 255? 255 : y < 0 ? 0 : y,
                            z > 255? 255 : z < 0 ? 0 : z); }
};

#endif //VEC3