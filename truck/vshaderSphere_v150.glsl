#version 150

in vec4 vPosition;	//vertex attribute for vertex position

out vec4 color;  //to send to the fragment shader, interpolated along the way

uniform mat4 model_matrix;  //the model matrix to transpose vertices from object coord to world coord
uniform mat4 proj_matrix;
uniform mat4 cam_matrix;

uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;

// spotlight
uniform int enabled1; // 1 = is enabled
uniform vec4 lightPos1;
uniform vec4 lightAmbient1, lightDiffuse1, lightSpecular1;

// directional light
uniform int enabled2;
uniform vec4 lightPos2;
uniform vec4 lightAmbient2, lightDiffuse2, lightSpecular2;

void main() 
{
    vec3 vNormal = normalize(vPosition.xyz);
    vec3 posInCam = (cam_matrix*model_matrix*vPosition).xyz;
    
    // spotlight
    vec4 color1 = vec4(0,0,0,0);
    
    if (enabled1 == 1) {
        vec3 lightInCam1 = (cam_matrix*lightPos1).xyz;
        
        // ray from light source to point
        vec3 ls = normalize(posInCam - lightInCam1);
        // direction of light
        vec3 l = normalize(vec3(0.0,0.0,-1.0));
        
        vec3 L1 = normalize(lightInCam1 - posInCam);
        vec3 V1 = normalize(-posInCam);
        vec3 H1 = normalize(L1+V1);
        vec3 N1 = normalize((cam_matrix*model_matrix*vec4(vNormal,0)).xyz);
        
        vec4 ambient1 = lightAmbient1*matAmbient;
        
        float Kd1 = max(dot(L1,N1),0.0);
        vec4 diffuse1 = Kd1*lightDiffuse1*matDiffuse;
        
        vec4 specular1;
        if(dot(L1,N1)<0.0)
            specular1 = vec4(1,0,0,1);
        else{
            float Ks1 = pow(max(dot(N1,H1),0.0),matAlpha);
            specular1 = Ks1*lightSpecular1*matSpecular;
        }
        
        float sexp = 20; // drop-off rate
        float cspot = pow(max(dot(l,ls),0.0),sexp);
        color1 = cspot * (ambient1 + diffuse1 + specular1);
    }
    
    // directional light
    vec4 color2 = vec4(0,0,0,0);
    
    if (enabled2 == 1) {
        vec3 lightInCam2 = (cam_matrix*lightPos2).xyz;
	
        vec3 L2 = normalize(lightInCam2 - posInCam);
        vec3 V2 = normalize(-posInCam);
        vec3 H2 = normalize(L2+V2);
        vec3 N2 = normalize((cam_matrix*model_matrix*vec4(vNormal,0)).xyz);
	
        vec4 ambient2 = lightAmbient2*matAmbient;
	
        float Kd2 = max(dot(L2,N2),0.0);
        vec4 diffuse2 = Kd2*lightDiffuse2*matDiffuse;
	
        vec4 specular2;
        if(dot(L2,N2)<0.0)
		specular2 = vec4(1,0,0,1);
        else{
            float Ks2 = pow(max(dot(N2,H2),0.0),matAlpha);
            specular2 = Ks2*lightSpecular2*matSpecular;
        }
    
        color2 = ambient2 + diffuse2 + specular2;
    }
    
    color = color1 + color2;
	color.a = 1.0;
	
  	gl_Position = proj_matrix*cam_matrix*model_matrix*vPosition;  //assign the vertex position to the vPosition attribute multiplied by the model matrix
}