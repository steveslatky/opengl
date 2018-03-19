#ifndef __CUBE_H__
#define __CUBE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Cube: public Drawable{


public:
	Cube();
	~Cube();
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
    vec2 vertexTextureLocations[6];
    
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
    
    
    vec4 vertices[8] = { vec4(-0.5,-0.5,0.5,1),vec4(-0.5,0.5,0.5,1),vec4(0.5,0.5,0.5,1),vec4(0.5,-0.5,0.5,1),
        vec4(-0.5,-0.5,-0.5,1),vec4(-0.5,0.5,-0.5,1),vec4(0.5,0.5,-0.5,1),vec4(0.5,-0.5,-0.5,1) };
//    vec4 vertices[8] = { vec4(-.75,-0.75,0.75,1),vec4(-0.75,0.75,0.75,1),vec4(0.75,0.75,0.75,1),vec4(0.75,-0.75,0.75,1),
//        vec4(-0.75,-0.75,-0.75,1),vec4(-0.75,0.75,-0.75,1),vec4(0.75,0.75,-0.75,1),vec4(0.75,-0.75,-0.75,1) };
//
    vec4 points[6];// * 2 * 3]; //6 faces, 2 triangles/face, 3 vertices per triangle
};
#endif
