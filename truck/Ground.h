#ifndef __Ground_h__
#define __Ground_h__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>
#include <cmath>

using namespace std;

class Ground : public Drawable{	
	
public:
	Ground();
	void draw(Camera, vector<Light>);
    bool testCollision(vec4 rayOrigin, vec4 rayDirection);
    void setMaterial(vec4, vec4, vec4, float);
	~Ground();
	
private:
	vec4 defColor = vec4(0.0, 1.0, 0.0, 1.0);
    GLuint vPosition;

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
    
    //(6 vertices to create 2 triangles)*(4 vertices in a square)^(numDivisions+1)
    static const unsigned int numDivisions = 8;
    static const unsigned int numVertices = 1572864;
    void build();
    
    //data
    vec4 vertexLocations[numVertices];
    vec4 vertexColors[numVertices];
    //helper stuff
    unsigned int index;
    void square(int);
    void divideSquare(vec4, vec4, vec4, vec4, int);
    void triangle(vec4, vec4, vec4);
};

#endif /* Ground_h */
