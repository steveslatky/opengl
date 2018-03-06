#include "Camera.h"

Camera::Camera(vec4 eye_in, vec4 u_in, vec4 v_in, vec4 n_in, int width, int height) {
    eye = eye_in;
    u = u_in;
    v = v_in;
    n = n_in;
    
    // New vertices for roll, pitch and yaw
    n_hat = vec4(0, 0, 1, 0);
    v_hat = vec4(0, 1, 0, 0);
    u_hat = vec4(1, 0, 0, 0);
    
    cameraMatrix = LookAt(eye, eye - n, v);
    projectionMatrix = Perspective(fov, GLfloat(width) / height, nearp, farp);
}

void Camera::moveCamera(char dir) {
    if (moveCam) {
        switch (dir) {
            case 'f': // forward
                eye = eye + sensitivity*(-n);
                break;
            case 'b': // backward
                eye = eye + sensitivity*(n);
                break;
            case 'r': // right
                eye = eye + sensitivity*(u);
                break;
            case 'l': // left
                eye = eye + sensitivity*(-u);
                break;
        }
    cameraMatrix = LookAt(eye, eye - n, v);
    }
}

void Camera::flyCamera(char dir) {
    if (moveCam) {
    // If we want to roll or yaw clockwise or pitch down
    if (dir == 'z' or dir == 'x' or dir == 'c') {
        alpha = 359;
    }
    else
        alpha = 1;
    float rangle = alpha*2.0*3.14 / 360;
    float c = cos(rangle);
    float s = sin(rangle);
    
    switch (dir) {
        // Clockwise
        case 'z': // roll
            u_hat = c*u - s*v;
            v_hat = s*u + c*v;
            break;
        case 'x': // pitch
            v_hat = c*v - s*n;
            n_hat = s*v + c*n;
            break;
        case 'c': // yaw
            n_hat = c*n - s*u;
            u_hat = s*n + c*u;
            break;
            
        // Counterclockwise
        case 'Z': // roll
            u_hat = c*u - s*v;
            v_hat = s*u + c*v;
            break;
        case 'X': // pitch
            v_hat = c*v - s*n;
            n_hat = s*v + c*n;
            break;
        case 'C': // yaw
            n_hat = c*n - s*u;
            u_hat = s*n + c*u;
            break;
    }
    u = u_hat;
    v = v_hat;
    n = n_hat;
    cameraMatrix = LookAt(eye, eye - n, v);
    }
}