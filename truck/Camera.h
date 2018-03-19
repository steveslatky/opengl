#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Angel.h"

class Camera {
public:
    Camera(vec4, vec4, vec4, vec4, int, int);
    mat4 getCameraMatrix() const { return cameraMatrix; }
    mat4 getProjectionMatrix() const { return projectionMatrix; }
    
    bool moveCam = true;
    
    void moveCamera(char dir);
    void flyCamera(char dir);
    vec4 eye, u, v, n;
    
private:
    unsigned int alpha;
    mat4 cameraMatrix, projectionMatrix;
    
    // perspective proj values
    GLfloat fov = 65, nearp = 1.0, farp = 100.0;
    
    // orthographic proj values
    GLfloat left = -1.0, bottom = -1.0, right = 1.0, top = 1.0, near = -10.0, far = 10.0;
    
    // angles for roll, pitch and yaw
    vec4 u_hat, v_hat, n_hat;
    
    // The amount that the camera should move
    float sensitivity = 0.3;
};

#endif
