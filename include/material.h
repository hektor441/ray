#ifndef MAT
#define MAT

#ifndef VEC3
    #include "vector3.h"
#endif //VEC3

#ifndef ITSC
    #include "intersection.h"
#endif //ITSC

class Material
{
public:
    virtual ~Material() {}
    virtual Vector3 getColor(Intersection &itsc) = 0;
};


class SimpleMaterial : public Material
{
    Vector3 color;
    public:
        SimpleMaterial(Vector3 color);
        SimpleMaterial(int r, int g, int b);
        Vector3 getColor(Intersection &itsc);

};

class CheckerMaterial : public Material
{
    Vector3 color1, color2;
    public:
        CheckerMaterial(Vector3 color1, Vector3 color2);
        CheckerMaterial();
        Vector3 getColor(Intersection &itsc);
    
};



#endif //MAT