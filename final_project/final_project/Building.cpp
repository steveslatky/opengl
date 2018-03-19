#include "Building.h"

Building::Building() {
	buildCube();

	//get buffers for attributes and indices
	glGenBuffers(1, &VBO);

	//put the data on the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexLocations) + sizeof(vertexNormals) + sizeof(vertexTextureLocations),NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexLocations), vertexLocations);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexLocations), sizeof(vertexNormals), vertexNormals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexLocations) + sizeof(vertexNormals), sizeof(vertexTextureLocations), vertexTextureLocations);

	//set up stuff for the body of the cube
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //associate the VBO with the active VAO

	assert((program = InitShader("vshaderCube_v150.glsl", "fshaderCube_v150.glsl"))!=-1);
	glUseProgram(program);  //make it the active one

	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);  //enable it
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    nPosition = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(nPosition);
    glVertexAttribPointer(nPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexLocations)));
    
    vTexture = glGetAttribLocation(program, "vTexture");
    glEnableVertexAttribArray(vTexture);
    glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexLocations) + sizeof(vertexNormals)));
    
    //get the texture data for the quad
    glGenTextures(1, &texture);
    int imgsize = 512;
//    int height = 837;
//    int width = 750;
    //837x750
    GLubyte *data = ppmRead("../../../../../../images/building/ppm/build.PPM", &imgsize, &imgsize);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  //move the data onto the GPU
    delete[] data;  //dont' need this data now that its on the GPU

    //set the texturing parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    assert((tex_loc = glGetUniformLocation(program, "textureID")) != -1);
    
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
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

Building::~Building(){

}

//----------------------------------------------------------------------------
void Building::makeQuad(int ind1, int ind2, int ind3, int ind4) {
	
    //triangle #1
	points[index] = vertices[ind1];
    vertexTextureLocations[index] = vec2(0,0);
	index++;

	points[index] = vertices[ind2];
    vertexTextureLocations[index] = vec2(1,0);
	index++;

	points[index] = vertices[ind3];
    vertexTextureLocations[index] = vec2(1,1);
	index++;

	//Triangle #2
	points[index] = vertices[ind3];
    vertexTextureLocations[index] = vec2(1,1);
	index++;

	points[index] = vertices[ind4];
    vertexTextureLocations[index] = vec2(0,1);
	index++;

	points[index] = vertices[ind1];
    vertexTextureLocations[index] = vec2(0,0);
	index++;
}

void Building::makeIndexedQuad(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4) {
    
    //Triangle 1
    vec3 N = normalize(cross(vertices[ind2] - vertices[ind1], vertices[ind3] - vertices[ind1]));
    vertexLocations[index] = vertices[ind1];
    vertexNormals[index] = N;
    vertexTextureLocations[index] = vec2(0,0);
    index++;
    
    vertexLocations[index] = vertices[ind2];
    vertexNormals[index] = N;
    vertexTextureLocations[index] = vec2(1,0);
    index++;
    
    vertexLocations[index] = vertices[ind3];
    vertexNormals[index] = N;
    vertexTextureLocations[index] = vec2(1,1);
    index++;
    
    //Triangle 2
    N = normalize(cross(vertices[ind3] - vertices[ind1], vertices[ind4] - vertices[ind1]));
    vertexLocations[index] = vertices[ind3];
    vertexNormals[index] = N;
    vertexTextureLocations[index] = vec2(1,1);
    index++;
    
    vertexLocations[index] = vertices[ind4];
    vertexNormals[index] = N;
    vertexTextureLocations[index] = vec2(0,1);
    index++;
    
    vertexLocations[index] = vertices[ind1];
    vertexNormals[index] = N;
    vertexTextureLocations[index] = vec2(0,0);
    index++;
}

void Building::buildCube() {
	index = 0;

//    makeQuad(1, 0, 3, 2);  //front
//    makeQuad(2, 3, 7, 6);  //right
//    makeQuad(3, 0, 4, 7);  //bottom
//    makeQuad(6, 5, 1, 2);  //top
//    makeQuad(4, 5, 6, 7);  //back
//    makeQuad(5, 4, 0, 1);  //left
    
    makeIndexedQuad(1, 0, 3, 2);  //front
    makeIndexedQuad(2, 3, 7, 6);  //right
    makeIndexedQuad(3, 0, 4, 7);  //bottom
    makeIndexedQuad(6, 5, 1, 2);  //top
    makeIndexedQuad(4, 5, 6, 7);  //back
    makeIndexedQuad(5, 4, 0, 1);  //left
}

//set the materials
void Building::setMaterial(vec4 d, vec4 a, vec4 s, float sh) {
    matDiffuse = d;
    matAmbient = a;
    matSpecular = s;
    shininess = sh;
}

void Building::draw(Camera cam, vector<Light> lights){

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
    glUniform1i(tex_loc, 0);
    
    
    //enable the necessary texturing
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
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

void Building::testCollision(vec4 rayOrigin, vec4 rayDirection)
{
    // We do not need this for the cube
}
