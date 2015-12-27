#include <iostream>
#include <sstream>
#include <cmath>
#include "../include/ray.h"
#include "../include/light.h"
#include "../include/scene.h"
#include "../include/shape.h"
#include "../include/lodepng.h"
#include "../include/vector3.h"
#include "../include/material.h"
using namespace std; // Sue me



class Image {
  int width, height;
  vector<unsigned char> buffer;
public:
  Image(int w, int h) {
    width = w;
    height = h;
    buffer.resize(w*h*4);
    clear();
  }
  
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  
  void clear() {
    int n = width*height*4;
    for(int i=0;i<n;i++) buffer[i] = 0;
    for(int i=3;i<n;i+=4) buffer[i] = 255;    
  }
  
  
  void add(int x, int y, Vector3 color) {
    int k = (y*width+x)*4;
    buffer[k++] += color.x;
    buffer[k++] += color.y;
    buffer[k++] += color.z;
  }
  
  void encodePNG(const char* filename) {
    vector<unsigned char> png;
    unsigned error = lodepng::encode(png, buffer, width, height);    
    if(error)
      cout << "encoder error " << error << ": "<< lodepng_error_text(error) << endl;
    else
      lodepng::save_file(png, filename);    
  }
};


class Camera {
  Vector3 origin;
  Vector3 direction;
  Vector3 right;
  float focus;
  
public:
  Camera() {
    
  }
};


void closestIntersection(Intersection &itsc, Ray &ray, Scene *scene)
{
  for(int i=0; i < scene->shapes.size(); i++) {
    scene->shapes[i]->intersect(ray, itsc);
  }  
}



Vector3 trace(Vector3 &camera, Ray &ray, Scene *scene)
{
  Intersection itsc;
  // Intersect the ray with every object in the scene,
  // recovering the position (tmin) of the closest one
  // its color and the normal vector to its surface
  
  closestIntersection(itsc, ray, scene);
  
  Vector3 pixelColor(0);

  if(itsc.material != 0)
  {
    // Shading
    
    // ray vector at the closest object
    Vector3 vt = ray.ray.scale(itsc.tmin) + ray.origin;
    
    Vector3 baseColor = itsc.material->getColor(itsc);
    Vector3 c = baseColor.scale(0.5);
    Vector3 N = itsc.normal.normalize(); // TODO: no need to normalize()
    Vector3 V = (camera - vt).normalize(); // versor point -> eye
    
    for(int i=0; i < scene->lights.size(); i++) {
      
      Light *light = scene->lights[i];
      
      Vector3 L = (light->pos - vt).normalize(); // light direction
     
      float NL = N*L;
      if(NL<=0) continue;
      
      Ray lightRay(itsc.point, light->pos - itsc.point); // ray : point -> light
      
      lightRay.origin = lightRay.pointAt(0.001);
      
      Intersection shadowItsc;
      closestIntersection(shadowItsc, lightRay, scene);
      if(shadowItsc.tmin>0 && shadowItsc.tmin<1.0)
      {
        continue;
      }
      
      
      Vector3 R = N.scale(2*NL) - L; // reflected light direction
      
      R = R.normalize(); // is this needed? I don't think so
      
      float rv = R*V; if(rv<0.0) rv=0.0;
      c = c + Vector3(255).scale(pow(rv,8.5));
      // if(rv==0) c = c + Vector3(0,0,255);
      
      c = c +  baseColor.scale(NL*0.4) ;          
    }
    pixelColor = c.clamp();
  }
  return pixelColor;  
}

void trace(Vector3 camera, Scene *scene, Image &image, const int aa)
{
 
  Vector3 color = Vector3(0);
  Vector3 normal = Vector3(0);
  
  int w = image.getWidth()*aa;
  int h = image.getHeight()*aa;
  
  for(int iy=0; iy<h; iy++) {
    for(int ix=0; ix<w; ix++) {
      
      
      float x = ix - w/2;
      float y = h/2 - iy;
            
      Ray ray = Ray(camera, Vector3(x, y, 600*aa));
      
      Vector3 pixelColor = trace(camera, ray, scene);
  
      image.add(ix/aa, iy/aa, pixelColor.scale(1./(aa*aa)));
    }
  }
}


int main(int argc, char *argv[]) 
{
  int width  = 1000;
  int height = 1000;

  Scene *scene = new Scene();
  Image image(width, height);  

  scene->add_shape(new Sphere(Vector3(0, 0, 0), 5, new SimpleMaterial(200,100,20)));
  scene->add_shape(new Sphere(Vector3(2, 5, -3), 2, new SimpleMaterial(200,20,180)));    
  scene->add_shape(new Plane(Vector3(0, -3, 0), Vector3(0, 1, 0), new CheckerMaterial()));
    
  scene->add_light(new Light(Vector3(0, 8, -10), 0.5));

  
  for(int k=0; k<1; k++) {
    
    Vector3 camera = Vector3(0, 0, -15);
          
    
    trace(camera, scene, image, 6);
    
    // Make multiple files
    char buf[50];
    sprintf(buf, "Render%i.png", k);
    string name(buf);
    const char* filename = name.c_str();
    
    image.encodePNG(filename);
    image.clear();
  }
  return 0;
}