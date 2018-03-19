//
//  building_top.hpp
//  final_project
//
//  Created by Stephen Slatky on 3/19/18.
//  Copyright © 2018 mm ss. All rights reserved.
//

#ifndef building_top_hpp
#define building_top_hpp

#include <stdio.h>

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Building_Top: public Drawable{
    
    
public:
    Building_Top();
    ~Building_Top();
    void draw(Camera, vector<Light>);
    void testCollision(vec4 rayOrigin, vec4 rayDirection);
    void setMaterial(vec4, vec4, vec4, float);
    
private:
    void buildCube();
    //    void makeQuad(vec4, vec4, vec4, vec4);
    void makeQuad(int, int, int, int);
    GLuint vPosition, vTexture, nPosition;
    
    //uniform texture location
    GLuint tex_loc;
    GLuint texture;
    
    void makeIndexedQuad(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4);
    
    vec4 vertexLocations[6 * 2 * 3];
    vec3 vertexNormals[6 * 2 * 3];
    vec2 vertexTextureLocations[6 * 2 * 3];
    
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
    
    unsigned int index;
    
    
    //    vec4 vertices[8] = { vec4(-0.5,-0.5,0.5,1),vec4(-0.5,0.5,0.5,1),vec4(0.5,0.5,0.5,1),vec4(0.5,-0.5,0.5,1),
    //        vec4(-0.5,-0.5,-0.5,1),vec4(-0.5,0.5,-0.5,1),vec4(0.5,0.5,-0.5,1),vec4(0.5,-0.5,-0.5,1) };
    vec4 vertices[8] = { vec4(-4.75,7.75,4.75,1),vec4(-4.75,9.75,4.75,1),vec4(4.75,9.75,4.75,1),vec4(4.75,7.75,4.75,1),
        vec4(-4.75,7.75,-4.75,1),vec4(-4.75,9.75,-4.75,1),vec4(4.75,9.75,-4.75,1),vec4(4.75,7.75,-4.75,1) };
    
    vec4 points[6 * 2 * 3]; //6 faces, 2 triangles/face, 3 vertices per triangle
};

#endif /* building_top_hpp */