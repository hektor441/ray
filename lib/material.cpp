#include "../include/material.h"
#include <math.h>

SimpleMaterial::SimpleMaterial(Vector3 t_color)
{
    color = t_color;
}

SimpleMaterial::SimpleMaterial(int r, int g, int b)
{
    color = Vector3(r,g,b);
}


Vector3 SimpleMaterial::getColor(Intersection &itsc)
{
    return color;
}


CheckerMaterial::CheckerMaterial(Vector3 _color1, Vector3 _color2)
{
    color1 = _color1;
    color2 = _color2;
    
}

CheckerMaterial::CheckerMaterial()
{
    color1 = Vector3(200,200,200);
    color2 = Vector3(50,50,50);    
}



Vector3 CheckerMaterial::getColor(Intersection &itsc)
{
    int x = (int)floor(itsc.point.x*0.5);
    int y = (int)floor(itsc.point.z*0.5);
    if(((x+y)&1)==0)
      return color1;
    else
      return color2;
}
