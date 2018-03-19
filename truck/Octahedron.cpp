#include "Octahedron.h"

Octahedron::Octahedron(vec3 origin) {
    
    // Each time I create an object of this type, I would like these to be the initial values
    // Angle of rotation
    theta = 0;
    // Center of object
    center = origin;
    
    // there are a total of 6 unique vertices in this octahedron
    // we will use indexes to actually generate the octahedron using these vertices
    vec4 vertices[6] = {vec4(center.x,    center.y+0.5,  center.z,     1),  // 0
                        vec4(center.x-0.5, center.y,   center.z-0.5,   1), // 1
                        vec4(center.x+0.5,  center.y,   center.z-0.5,   1),  // 2
                        vec4(center.x+0.5,  center.y,   center.z+0.5,    1),  // 3
                        vec4(center.x-0.5,  center.y,  center.z+0.5,   1),   // 4
                        vec4(center.x,   center.y-0.5,  center.z,     1)};// 5
    
	buildOctahedron();

	//get buffers for attributes and indices
	glGenBuffers(1, &VBO);
    glGenBuffers(1, &VIO); //vertex indices
    
    //set up stuff for the body of the cube
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); //make this VAO active
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  //associate the VBO with the active VAO
    
    //put the data on the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(potentialColors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(potentialColors), potentialColors);
    
    //associate this element array buffer with the VAO
    // and put the index on the element array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)+sizeof(frameIndices), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), sizeof(frameIndices), frameIndices);

	assert((program = InitShader("vshader00_v150.glsl", "fshader00_v150.glsl"))!=-1);
	glUseProgram(program);  //make it the active one

	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);  //enable it
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//get the location of the uniform color in the shader
	assert((vColor = glGetAttribLocation(program, "vColor"))!=-1);
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	//get the location of the model matrix
	assert((mmLoc = glGetUniformLocation(program, "model_matrix")) != -1);
    
    //get the location of the camera and projection matrices
    assert((cmLoc = glGetUniformLocation(program, "cam_matrix")) != -1);
    assert((pmLoc = glGetUniformLocation(program, "proj_matrix")) != -1);
    
    //for the frame..
    glGenVertexArrays(1, &frameVAO);
    glBindVertexArray(frameVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);
    assert((program2 = InitShader("vshader01_v150.glsl", "fshader01_v150.glsl")) != -1);
    glUseProgram(program2); //make it the active one
    assert((vPosition = glGetAttribLocation(program2, "vPosition")) != -1);
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    assert((vColor = glGetUniformLocation(program2, "color")) != -1);
    assert((mm2 = glGetUniformLocation(program2, "model_matrix")) != -1);
    assert((cm2 = glGetUniformLocation(program2, "cam_matrix")) != -1);
    assert((pm2 = glGetUniformLocation(program2, "proj_matrix")) != -1);
}

Octahedron::~Octahedron(){
    glDeleteBuffers(1, &VIO);  //delete the buffers (free up space on GPU)
}

//----------------------------------------------------------------------------
void Octahedron::makeTriangle(GLuint ind1, GLuint ind2, GLuint ind3) {
    indices[index] = ind1;
    index++;
    indices[index] = ind2;
    index++;
    indices[index] = ind3;
    index++;
}

void Octahedron::buildOctahedron() {
	index = 0;

    // top
	makeTriangle(1, 0, 2);  //front
	makeTriangle(2, 0, 3);  //right
    makeTriangle(3, 0, 4);  //back
    makeTriangle(4, 0, 1);  //left
    
    // bottom
    makeTriangle(2, 5, 1);  //front
    makeTriangle(1, 5, 4);  //right
    makeTriangle(4, 5, 3);  //back
    makeTriangle(3, 5, 2);  //left
}

void Octahedron::draw(Camera cam, vector<Light> lights){

	//Draw the octahedron body
	glBindVertexArray(VAO);
	glUseProgram(program);  //also switch to using this shader program
	glUniformMatrix4fv(mmLoc, 1, GL_TRUE,modelmatrix);
	glUniformMatrix4fv(cmLoc, 1, GL_TRUE, cam.getCameraMatrix());
    glUniformMatrix4fv(pmLoc, 1, GL_TRUE, cam.getProjectionMatrix());
    glDrawElements(GL_TRIANGLES,8 * 1 * 3, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
    
    //Draw the cube frame
    glBindVertexArray(frameVAO);
    glUseProgram(program2); //also switch to using this shader program
    glUniformMatrix4fv(mm2, 1, GL_TRUE, modelmatrix);
    glUniformMatrix4fv(cm2, 1, GL_TRUE, cam.getCameraMatrix());
    glUniformMatrix4fv(pm2, 1, GL_TRUE, cam.getProjectionMatrix());
    glUniform4fv(vColor, 1, vec4(0, 0, 0, 1));
    glLineWidth(5.0);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, BUFFER_OFFSET(sizeof(indices)));
}
