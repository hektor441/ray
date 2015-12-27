#ifndef ITSC
#define ITSC

#ifndef VEC3
    #include "vector3.h"
#endif // VEC3

class Material;

class Intersection {
    public:
        float tmin;
        Vector3 point;
        Vector3 normal;
        Material *material;
        
        // constructor
        Intersection();
};

#endif //ITSC
