#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "lodepng.h"


class Skybox: public Drawable{  //derived from drawable class
    
public:
    //constructors
    Skybox();
    void draw(Camera camera,vector<Light>);
    void testCollision(vec4 rayOrigin, vec4 rayDirection);
    void setMaterial(vec4, vec4, vec4, float);
    
    //destructor
    ~Skybox();
    
    unsigned int getTheta() { theta++;
        if (theta > 360) theta = 0;
        return theta; }
    
    mat4 modelmatrix;
    
    void buildCube();
    
private:
    
    
    void build();
    GLuint vpos, cpos, mmpos, vmpos, pmpos, texId;
    static const unsigned int numVertices = 8;
    vec4 vertexLocations[numVertices];
    int indexs[12 * 3];
    

    
};
#endif


