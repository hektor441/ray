#ifndef SCENE
#define SCENE

#ifndef SHAPE
    #include "shape.h"
#endif //SHAPE

#ifndef LiGHT
    #include "light.h"
#endif //LIGHT

#include <vector>

class Scene {
    public:
        Scene();
        std::vector<Shape *> shapes;
        std::vector<Light *> lights;
        
        void add_shape(Shape *shape) { shapes.push_back(shape); }
        void add_light(Light *light) { lights.push_back(light); }
};

#endif //SCENE