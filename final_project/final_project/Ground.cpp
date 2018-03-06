#include "Ground.h"

Ground::Ground() {

    build();
	
	//get buffers for attributes and indices
	glGenBuffers(1, &VBO);
	
	//put the data on the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexLocations), vertexLocations, GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //associate the VBO with the active VAO
	
	assert((program = InitShader("vshaderGround_v150.glsl", "fshaderGround_v150.glsl"))!=-1);
	glUseProgram(program);
	
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);
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

Ground::~Ground() {
}

void Ground::build() {
    index = 0; 
    square(numDivisions);
}

void Ground::square(int count) {
    vec4 a = vec4(10,0,-10,1);
    vec4 b = vec4(-10,0,-10,1);
    vec4 c = vec4(-10,0,10,1);
    vec4 d = vec4(10,0,10,1);
    
    vec4 mid = vec4((a.x+c.x)/2,0,(a.z+c.z)/2,1);
    vec4 ab = vec4((a.x+b.x)/2,0,(a.z+b.z)/2,1);
    vec4 bc = vec4((b.x+c.x)/2,0,(b.z+c.z)/2,1);
    vec4 cd = vec4((c.x+d.x)/2,0,(c.z+d.z)/2,1);
    vec4 da = vec4((d.x+a.x)/2,0,(d.z+a.z)/2,1);
    
    // subdivide each of the 4 squares
    divideSquare(ab, b, bc, mid, count);
    divideSquare(mid, bc, c, cd, count);
    divideSquare(da, mid, cd, d, count);
    divideSquare(a, ab, mid, da, count);
}

void Ground::divideSquare(vec4 a, vec4 b, vec4 c, vec4 d, int count) {
    if (count > 0) {
        vec4 mid = vec4((a.x+c.x)/2,0,(a.z+c.z)/2,1);
        vec4 ab = vec4((a.x+b.x)/2,0,(a.z+b.z)/2,1);
        vec4 bc = vec4((b.x+c.x)/2,0,(b.z+c.z)/2,1);
        vec4 cd = vec4((c.x+d.x)/2,0,(c.z+d.z)/2,1);
        vec4 da = vec4((d.x+a.x)/2,0,(d.z+a.z)/2,1);
        
        // subdivide each of the 4 squares
        divideSquare(ab, b, bc, mid, count-1);
        divideSquare(mid, bc, c, cd, count-1);
        divideSquare(da, mid, cd, d, count-1);
        divideSquare(a, ab, mid, da, count-1);
    }
    else {
        triangle(a,b,c);
        triangle(c,d,a);
    }
}

void Ground::triangle(vec4 a, vec4 b, vec4 c) {
    vec4 color(1.0*rand() / RAND_MAX, 1.0*rand() / RAND_MAX, 1.0*rand() / RAND_MAX, 1.0);
    
    vertexLocations[index] = a;
    vertexColors[index] = color;
    index ++;
    
    vertexLocations[index] = b;
    vertexColors[index] = color;
    index ++;
    
    vertexLocations[index] = c;
    vertexColors[index] = color;
    index ++;
}

//set the materials
void Ground::setMaterial(vec4 d, vec4 a, vec4 s, float sh) {
    matDiffuse = d;
    matAmbient = a;
    matSpecular = s;
    shininess = sh;
}

void Ground::draw(Camera cam, vector<Light> lights){
	glBindVertexArray(VAO);
	glUseProgram(program);
	
    glUniformMatrix4fv(mmLoc, 1, GL_TRUE,modelmatrix);
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
    
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

void Ground::testCollision(vec4 rayOrigin, vec4 rayDirection)
{
    // We do not need this for the ground
}