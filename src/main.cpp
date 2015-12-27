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
public:
  Vector3 origin;
  Vector3 direction;
  Vector3 right;
  Vector3 up;
  float focus;
  
  Camera() {
    origin = Vector3(0);
    direction = Vector3(0, 0, 1);
    right = Vector3(1, 0, 0);
    computeUp();
    focus = 1.2;
  }
  
  void computeUp() {
    up = direction.cross(right).normalize();
  }
  
  Ray getRay(int x, int y, int width, int height) {
    float f = 2/(float) height;
    float u = (x - width/2.) * f;
    float v = (height/2. - y) * f;
    Ray ray(origin, direction.scale(focus) + right.scale(u) + up.scale(v));
    return ray;
  }
  
  void lookAt(Vector3 target) {
    direction = (target - origin).normalize();
    right = Vector3(0, 1, 0).cross(direction).normalize();
    computeUp();
  }
  void lookAt(float x, float y, float z) {
    lookAt(Vector3(x, y, z));
  }
};


void closestIntersection(Intersection &itsc, Ray &ray, Scene *scene)
{
  for(int i=0; i < scene->shapes.size(); i++) {
    scene->shapes[i]->intersect(ray, itsc);
  }  
}



Vector3 trace(Ray &ray, Scene *scene, int level)
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
    Vector3 V = -ray.ray.normalize(); // versor point -> eye
    
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
    
    if(level<5)
    {
      Ray refl(vt, N.scale(2*(V*N)) - V);
    refl.origin = refl.pointAt(0.002);

      c = c + trace(refl, scene, level + 1).scale(0.4);
    }
    
    pixelColor = c.clamp();
  }
  return pixelColor;  
}

void trace(Camera &camera, Scene *scene, Image &image)
{
 
  Vector3 color = Vector3(0);
  Vector3 normal = Vector3(0);
  
  int w = image.getWidth()*2;
  int h = image.getHeight()*2;
  
  for(int iy=0; iy<h; iy++) {
    for(int ix=0; ix<w; ix++) {
     
      Ray ray = camera.getRay(ix, iy, w, h);
      
      Vector3 pixelColor = trace(ray, scene, 0);
  
      image.add(ix/2, iy/2, pixelColor.scale(0.25));
    }
  }
}


int main(int argc, char *argv[]) 
{
  int width  = 300;
  int height = 300;

  Scene *scene = new Scene();
  Image image(width, height);  

  scene->add_shape(new Sphere(Vector3(0, 0, 0), 5, new SimpleMaterial(200,100,20)));
  scene->add_shape(new Sphere(Vector3(2, 5, -3), 2, new SimpleMaterial(200,20,180)));    
  scene->add_shape(new Plane(Vector3(0, -3, 0), Vector3(0, 1, 0), new CheckerMaterial()));
    
  scene->add_light(new Light(Vector3(0, 8, -10), 0.5));

  Camera camera;
  camera.origin = Vector3(0, 10, -10);
  camera.lookAt(0, 0, 0);
  
  int frameCount = 50;
  
  for(int k=0; k<frameCount; k++) {
    
    float phi = 2*M_PI*k/frameCount;
    float r = 10;
    camera.origin = Vector3(r*cos(phi),10,r*sin(phi));
    camera.lookAt(0, 0, 0);
    
    

    
    trace(camera, scene, image);
    
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
