#ifndef RAY
#define RAY


#ifndef VEC3
    #include "vector3.h"
#endif //VEC3


class Ray {
    public:
        Vector3 origin;
        Vector3 ray;
        
        //constructor
        Ray(Vector3 t_origin, Vector3 t_ray);
        
        Vector3 pointAt(float t) { return origin + ray.scale(t); }
};


#endif //RAY