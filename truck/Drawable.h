#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__
#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include <cassert>
#include <vector>

using namespace std;

class Drawable {
protected:
	GLuint program;
	GLuint VAO, VBO;
	mat4 modelmatrix;
    unsigned int theta;

public:
	virtual void draw(Camera, vector<Light>)=0;
    virtual bool testCollision(vec4 rayOrigin, vec4 rayDirection)=0;
    
	void setModelMatrix(mat4 mm) { modelmatrix = mm; }
    
    mat4 getModelMatrix() { return modelmatrix; }
    unsigned int getTheta() { theta++;
        if (theta > 360) theta = 0;
        return theta; }
	static unsigned char* ppmRead(char* filename, int* width, int* height);
    
	virtual ~Drawable()=0;
    vec3 center;
};

#endif
