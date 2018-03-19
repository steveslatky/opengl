#include "Cube.h"

Cube::Cube() {
	buildCube();

	//get buffers for attributes and indices
	glGenBuffers(1, &VBO);

	//put the data on the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	//set up stuff for the body of the cube
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //associate the VBO with the active VAO

	assert((program = InitShader("vshaderCube_v150.glsl", "fshaderCube_v150.glsl"))!=-1);
	glUseProgram(program);  //make it the active one

	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);  //enable it
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    //get the locations of the shader variables
    assert((mmLoc = glGetUniformLocation(program, "model_matrix"))!=-1);
    assert((cmLoc = glGetUniformLocation(program, "cam_matrix")) != -1);
    assert((pmLoc = glGetUniformLocation(program, "proj_matrix")) != -1);
    assert((maLoc = glGetUniformLocation(program, "matAmbient")) != -1);
    assert((mdLoc = glGetUniformLocation(program, "matDiffuse")) != -1);
    assert((msLoc = glGetUniformLocation(program, "matSpecular")) != -1);
    assert((mAlphaLoc = glGetUniformLocation(program, "matAlpha")) != -1);
    
    assert((enabled1 = glGetUniformLocation(program, "enabled1")) != -1);
    assert((lpLoc1 = glGetUniformLocation(program, "lightPos1")) != -1);
    assert((laLoc1 = glGetUniformLocation(program, "lightAmbient1")) != -1);
    assert((ldLoc1 = glGetUniformLocation(program, "lightDiffuse1")) != -1);
    assert((lsLoc1 = glGetUniformLocation(program, "lightSpecular1")) != -1);
    
    assert((enabled2 = glGetUniformLocation(program, "enabled2")) != -1);
    assert((lpLoc2 = glGetUniformLocation(program, "lightPos2")) != -1);
    assert((laLoc2 = glGetUniformLocation(program, "lightAmbient2")) != -1);
    assert((ldLoc2 = glGetUniformLocation(program, "lightDiffuse2")) != -1);
    assert((lsLoc2 = glGetUniformLocation(program, "lightSpecular2")) != -1);
}

Cube::~Cube(){

}

//----------------------------------------------------------------------------
void Cube::makeQuad(int ind1, int ind2, int ind3, int ind4) {
	//triangle #1
	points[index] = vertices[ind1];
	index++;

	points[index] = vertices[ind2];
	index++;

	points[index] = vertices[ind3];
	index++;

	//Triangle #2
	points[index] = vertices[ind3];
	index++;

	points[index] = vertices[ind4];
	index++;

	points[index] = vertices[ind1];
	index++;
}

void Cube::buildCube() {
	index = 0;

	makeQuad(1, 0, 3, 2);  //front
	makeQuad(2, 3, 7, 6);  //right
	makeQuad(3, 0, 4, 7);  //bottom
	makeQuad(6, 5, 1, 2);  //top
	makeQuad(4, 5, 6, 7);  //back
	makeQuad(5, 4, 0, 1);  //left
}

//set the materials
void Cube::setMaterial(vec4 d, vec4 a, vec4 s, float sh) {
    matDiffuse = d;
    matAmbient = a;
    matSpecular = s;
    shininess = sh;
}

void Cube::draw(Camera cam, vector<Light> lights){

	//Draw the cube body
	glBindVertexArray(VAO);
	glUseProgram(program);  //also switch to using this shader program
    
    //set all the uniform variables
    glUniformMatrix4fv(mmLoc, 1, GL_TRUE, modelmatrix);
    glUniformMatrix4fv(cmLoc, 1, GL_TRUE, cam.getCameraMatrix());
    glUniformMatrix4fv(pmLoc, 1, GL_TRUE, cam.getProjectionMatrix());
    glUniform4fv(maLoc, 1, matAmbient);
    glUniform4fv(mdLoc, 1, matDiffuse);
    glUniform4fv(msLoc, 1, matSpecular);
    glUniform1f(mAlphaLoc, shininess);
    
    // spotlight
    glUniform1i(enabled1, lights[0].enabled);
    glUniform4fv(lpLoc1, 1, lights[0].getPosition());
    glUniform4fv(laLoc1, 1, lights[0].getAmbient());
    glUniform4fv(ldLoc1, 1, lights[0].getDiffuse());
    glUniform4fv(lsLoc1, 1, lights[0].getSpecular());
    
    // directional light
    glUniform1i(enabled2, lights[1].enabled);
    glUniform4fv(lpLoc2, 1, lights[1].getPosition());
    glUniform4fv(laLoc2, 1, lights[1].getAmbient());
    glUniform4fv(ldLoc2, 1, lights[1].getDiffuse());
    glUniform4fv(lsLoc2, 1, lights[1].getSpecular());
    
	glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);
}

bool Cube::testCollision(vec4 rayOrigin, vec4 rayDirection)
{
    // We do not need this for the cube
    return false;
}
