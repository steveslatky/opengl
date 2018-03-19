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
    
//    Skybox::bool load_cube_map_side(GLuint texture, GLenum side_target, const char* file_name);
private:
    
    
    void build();
    GLuint vpos, cpos, mmpos, vmpos, pmpos, texId;
    static const unsigned int numVertices = 8;
    vec4 vertexLocations[numVertices];
    int indexs[12 * 3];
    
    //uniform transformation matrix variable locations
    GLuint mmLoc, cmLoc, pmLoc;
    //uniform material variables
    GLuint maLoc, msLoc, mdLoc, mAlphaLoc;
    
    //uniform light variables
    GLuint enabled1, laLoc1, lsLoc1, ldLoc1, lpLoc1;
    GLuint enabled2, laLoc2, lsLoc2, ldLoc2, lpLoc2;
    
    //materials
    vec4 matDiffuse, matSpecular, matAmbient;
    float shininess;
    
    //shader variable locations
    GLuint diffuse_loc, spec_loc, ambient_loc, alpha_loc;
    

    
};
#endif


