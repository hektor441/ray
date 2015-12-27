#include <cmath>
#include "../include/shape.h"
using namespace std;


Shape::Shape(Material *m)
{
    if(m==0) m = new SimpleMaterial(Vector3(200,200,200));
    material = m;
}

Sphere::Sphere(const Vector3 t_center, float t_radius, Material *material)
: Shape(material)
{
    center = t_center;
    radius = t_radius;
}

bool Sphere::intersect(Ray &ray, Intersection &itsc)
{   // Ray    = vt + origin
    // Sphere = {v : |v - center| = radius}
    // Itsc   = {v : |vt + origin - center| = radius}
    
    Vector3 v = ray.ray;
    Vector3 d = ray.origin - center;
    
    // |vt + d| = r  <=>  |vt + d|^2 = r^2  <=>  (vt+d).(vt+d) = r^2
    // (vt+d).(vt+d) = (vt+d).vt + (vt+d).d = (vt.vt) + 2(vt.d) + (d.d)
    // Itsc = {v : t^2(v.v) + 2t(v.d) + d.d - r^2 = 0}
    
    // Finding out whether the ray intersects this sphere means
    // finding the solutions (if any) to that equation in t 
    
    float a = v*v;
    float b = v*d; // actually 2b
    float c = d*d - radius*radius;
    
    float discr = b*b - a*c;
    
    if(discr >= 0)
    {   
        float t1 = -b/a - sqrt(discr)/a;
        float t2 = -b/a + sqrt(discr)/a;
        float t = t1;
        if (t<0) t = t2;
        
        if (t < itsc.tmin && t > 0)
        {
            itsc.tmin   = t;
            itsc.point = ray.pointAt(t);
            itsc.normal = (itsc.point - center).normalize();
            itsc.material = material;
            return true;
        }    
    }
    
    return false;
}

Plane::Plane(Vector3 t_position, Vector3 t_normal, Material *material)
: Shape(material)
{
    position = t_position;
    normal   = t_normal;
}

bool Plane::intersect(Ray &ray, Intersection &itsc)
{
    // Ray   = vt + origin
    // Plane = { v : (v - position).normal = 0} 
    // Intersection = {v : vt + origin - position).normal = 0}
    
    float num = (position - ray.origin) * normal;
    float den = ray.ray * normal;

    // vt + origin - position = vt + d
    // (vt + d).normal = vt.normal + d.normal = 0
    // t = -d.normal / v.normal
    
    if(den != 0)
    {
        float t = num/den;
        if(t>0.0 && t<itsc.tmin)
        {
            itsc.tmin   = t;
            itsc.normal = normal.normalize();
            itsc.point = ray.pointAt(t);
            itsc.material = material;
            return true;
        }
    }

    return false;
}
