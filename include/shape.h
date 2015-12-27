#ifndef SHAPE
#define SHAPE

#ifndef RAY
    #include "ray.h"
#endif //RAY

#ifndef VEC3
    #include "vector3.h"
#endif //VEC3

#ifndef MAT
    #include "material.h"
#endif //MAT

#ifndef ITSC
    #include "intersection.h"
#endif //ITSC

class Shape {
    public:
        Material *material;
        
        Shape(Material *m); // if m == 0 then use default material
        virtual ~Shape() { delete material; }
       
        virtual bool intersect(Ray &ray, Intersection &itsc) = 0;
};


class Sphere : public Shape {
    Vector3 center;
    float radius;
    public:
        Sphere(const Vector3 center, float radius, Material *material = 0);
        
        bool intersect(Ray &ray, Intersection &itsc);
};


class Plane : public Shape {
    Vector3 position;
    Vector3 normal;
    public:
        Plane(Vector3 position, Vector3 normal, Material *material = 0);
        
        bool intersect(Ray &ray, Intersection &itsc);
    
};

#endif //SHAPE