#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include "float.h"
#include <string>
#include <vector>

using namespace std;

class Sphere:public Drawable {
    
public:
    Sphere();
    void draw(Camera,vector<Light>);
    void testCollision(vec4 rayOrigin, vec4 rayDirection);
    void setMaterial(vec4, vec4, vec4, float);
    ~Sphere();
    
private:
    //(4 triangular faces per tetrahedron)^(numDivisions+1)*3 vertices per triangle
    static const unsigned int numVertices = 3072;
    void build();
    
    //data
    vec4 vertexLocations[numVertices];
    
    //shader variable locations
    GLuint vpos;
    GLuint mmpos, vmpos, pmpos;
    GLuint diffuse_loc, spec_loc, ambient_loc, alpha_loc;
    
    GLuint enabled1, light_loc1, ambient_loc1, diffuse_loc1, specular_loc1;
    GLuint enabled2, light_loc2, ambient_loc2, diffuse_loc2, specular_loc2;
    
    //material properties
    vec4 diffuse, specular, ambient;
    float shine;
    
    void assignGouradVertices();
    
    //helper stuff
    unsigned int index;
    float sqrt2, sqrt6;
    void tetrahedron(int);
    void divideTriangle(vec4, vec4, vec4, int);
    void triangle(vec4, vec4, vec4);
    vec4 unit(vec4);
    
    // Collision detection
    int myi;
    float t, smallest_t;
    vec4 vertexWorld[numVertices], N, E, F, G, P;
    vec4 cross12, cross23, cross31;
    float dot12, dot23, dot31;
};

#endif
