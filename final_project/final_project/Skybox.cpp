//
//  skybox.cpp
//  final_project
//
//  Created by Stephen Slatky on 3/6/18.
//  Copyright © 2018 mm ss. All rights reserved.
//

#include "Skybox.hpp"
#include "lodepng.h"

Skybox::Skybox() {
    //load the shader and get the location of it's variables
    build();
}

//on destroy, delete the buffer (cleanup)
Skybox::~Skybox(){
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Skybox::buildCube() {
    vertexLocations[0] = vec4(-1, -1, 1, 1);
    vertexLocations[1] = vec4(-1, 1, 1, 1);
    vertexLocations[2] = vec4(1, 1, 1, 1);
    vertexLocations[3] = vec4(1, -1, 1, 1);
    
    vertexLocations[4] = vec4(-1, -1, -1, 1);
    vertexLocations[5] = vec4(-1, 1, -1, 1);
    vertexLocations[6] = vec4(1, 1, -1, 1);
    vertexLocations[7] = vec4(1, -1, -1, 1);
    
    indexs[0] = 0;
    indexs[1] = 2;
    indexs[2] = 1;
    indexs[3] = 0;
    indexs[4] = 3;
    indexs[5] = 2;
    
    indexs[6] = 3;
    indexs[7] = 6;
    indexs[8] = 2;
    indexs[9] = 3;
    indexs[10] = 7;
    indexs[11] = 6;
    
    indexs[12] = 7;
    indexs[13] = 5;
    indexs[14] = 6;
    indexs[15] = 7;
    indexs[16] = 4;
    indexs[17] = 5;
    
    indexs[18] = 4;
    indexs[19] = 1;
    indexs[20] = 5;
    indexs[21] = 4;
    indexs[22] = 0;
    indexs[23] = 1;
    
    indexs[24] = 1;
    indexs[25] = 6;
    indexs[26] = 5;
    indexs[27] = 1;
    indexs[28] = 2;
    indexs[29] = 6;
    
    indexs[30] = 0;
    indexs[31] = 7;
    indexs[32] = 4;
    indexs[33] = 0;
    indexs[34] = 3;
    indexs[35] = 7;
}



void Skybox::build() {
    
    buildCube();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    program = InitShader("vshaderSkybox.glsl", "fshaderSkybox.glsl");
    glUseProgram(program);
    
    glBindVertexArray(VAO);
    
    glGenTextures(1, &texId);
    glActiveTexture(GL_TEXTURE0);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    vector<unsigned char> image;
    unsigned width, height;
    
    
    lodepng::decode(image, width, height, "/Users/stephenslatky/dev/classes/cs432/final_project/opengl/images/skybox/skybox_top.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA ,GL_UNSIGNED_BYTE, image.data());
    image.clear();
    lodepng::decode(image, width, height, "/Users/stephenslatky/dev/classes/cs432/final_project/opengl/images/skybox/skybox_bottom.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    image.clear();
    lodepng::decode(image, width, height, "/Users/stephenslatky/dev/classes/cs432/final_project/opengl/images/skybox/skybox_right.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    image.clear();
    lodepng::decode(image, width, height, "/Users/stephenslatky/dev/classes/cs432/final_project/opengl/images/skybox/skybox_left.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    image.clear();
    lodepng::decode(image, width, height, "/Users/stephenslatky/dev/classes/cs432/final_project/opengl/images/skybox/skybox_front.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    image.clear();
    lodepng::decode(image, width, height, "/Users/stephenslatky/dev/classes/cs432/final_project/opengl/images/skybox/skybox_back.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    image.clear();
    
    
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexLocations), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexLocations), vertexLocations);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indexs), indexs);
    
    GLuint vPosition1 = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition1);
    
    glVertexAttribPointer(vPosition1, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    
    
    
}



void Skybox::draw(Camera camera, vector<Light>){
    glBindVertexArray(VAO);
    glUseProgram(program);
    
    GLuint model_loc = glGetUniformLocation(program, "model_matrix");
    glUniformMatrix4fv(model_loc, 1, GL_TRUE, mat4(1.0));
    
    
    GLuint proj_loc = glGetUniformLocation(program, "proj_matrix");
    glUniformMatrix4fv(proj_loc, 1, GL_TRUE, camera.getProjectionMatrix());
    
    GLuint view_loc = glGetUniformLocation(program, "view_matrix");
    mat4 viewMatrix = LookAt(vec4(0, 0, 0, 1), vec4(0, 0, 0, 1) + camera.geteye(), camera.getv());
    glUniformMatrix4fv(view_loc, 1, GL_TRUE, viewMatrix);
    
    glEnable(GL_TEXTURE_CUBE_MAP);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
    
    GLuint mapPos = glGetUniformLocation(program, "cubeMap");
    glUniform1i(mapPos, 0);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    
    
}

void Skybox::testCollision(vec4 rayOrigin, vec4 rayDirection){
    // Not needed
}



