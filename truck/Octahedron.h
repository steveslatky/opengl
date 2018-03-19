#ifndef __OCTAHEDRON_H__
#define __OCTAHEDRON_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Octahedron: public Drawable{


public:
	Octahedron(vec3);
	~Octahedron();
	void draw(Camera, vector<Light>);

private:
	void buildOctahedron();
	void makeTriangle(GLuint, GLuint, GLuint);
	GLuint vPosition, vColor;
    // Matrix locations for the polyhedron
	GLuint mmLoc, cmLoc, pmLoc;
    // Matrix locations for the frame
    GLuint mm2, cm2, pm2;

	unsigned int index;
    
	vec4 potentialColors[6] = { vec4(0,1,1,1),
                                vec4(1,1,0,1),
                                vec4(0,1,0,1),
                                vec4(1,0,0,1),
                                vec4(0,0,1,1),
                                vec4(0.5f,0.2f,0.4f,1) };

    GLubyte indices[8 * 1 * 3];
    GLubyte frameIndices[24] = {0,1, 0,2, 0,3, 0,4, 4,1, 1,2, 2,3, 3,4, 4,5, 5,2, 5,3, 5,1};
};
#endif
