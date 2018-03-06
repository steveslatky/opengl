#version 150

in vec4 vPosition;	//vertex attribute for vertex position

out vec3 fN;
out vec3 fE;
out vec3 fL1;
out vec3 fL2;

uniform mat4 model_matrix;  //the model matrix to transpose vertices from object coord to world coord
uniform mat4 proj_matrix;
uniform mat4 cam_matrix;

uniform vec4 lightPos1;
uniform vec4 lightPos2;

void main()
{
    vec3 vNormal = normalize(vPosition.xyz);
    
    // the vertex in camera coordinates
    vec3 pos = (cam_matrix*model_matrix*vPosition).xyz;
    
    // lights in camera coordinates
    vec3 lightInCam1 = (cam_matrix*lightPos1).xyz;
    vec3 lightInCam2 = (cam_matrix*lightPos2).xyz;
    
    fN = normalize((cam_matrix*model_matrix*vec4(vNormal,0)).xyz);
    
    // ray from vertex towards camera
    fE = normalize(vec3(0,0,0)-pos);
    
    // ray from vertex towards light
    fL1 = normalize(lightInCam1.xyz-pos);
    fL2 = normalize(lightInCam2.xyz-pos);
    
    gl_Position = proj_matrix*cam_matrix*model_matrix*vPosition;  //assign the vertex position to the vPosition attribute multiplied by the model matrix
}