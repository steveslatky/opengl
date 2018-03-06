#version 150

in vec3 fN;
in vec3 fE;

in vec3 fL1;
in vec3 fL2;

uniform int enabled1;
uniform vec4 lightAmbient1, lightDiffuse1, lightSpecular1;

uniform int enabled2;
uniform vec4 lightAmbient2, lightDiffuse2, lightSpecular2;

uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;

out vec4 fColor;

void main()
{
    // spotlight
    vec4 color1 = vec4(0,0,0,0);
    if (enabled1 == 1) {
        vec3 N1 = normalize(fN);
        vec3 E1 = normalize(fE);
        vec3 L1 = normalize(fL1);
    
        vec3 H1 = normalize(L1+E1);
        vec4 ambient1 = lightAmbient1*matAmbient;
    
        float Kd1 = max(dot(L1,N1),0.0);
        vec4 diffuse1 = Kd1*lightDiffuse1*matDiffuse;
    
        vec4 specular1;
        if(dot(L1,N1) < 0.0)
            specular1 = vec4(1,0,0,1);
        else {
            float Ks1 = pow(max(dot(N1,H1),0.0),matAlpha);
            specular1 = Ks1*lightSpecular1*matSpecular;
        }
        
        // ray from light source to point
        vec3 ls = -fL1;
        // direction of light
        vec3 l = normalize(vec3(0.0,0.0,-1.0));
        
        float sexp = 20; // drop-off rate
        float cspot = pow(max(dot(l,ls),0.0),sexp);
        color1 = cspot * (ambient1 + diffuse1 + specular1);
    }
    
    // directional light
    vec4 color2 = vec4(0,0,0,0);
    if (enabled2 == 1) {
        vec3 N2 = normalize(fN);
        vec3 E2 = normalize(fE);
        vec3 L2 = normalize(fL2);
        
        vec3 H2 = normalize(L2+E2);
        vec4 ambient2 = lightAmbient2*matAmbient;
        
        float Kd2 = max(dot(L2,N2),0.0);
        vec4 diffuse2 = Kd2*lightDiffuse2*matDiffuse;
        
        vec4 specular2;
        if(dot(L2,N2) < 0.0)
            specular2 = vec4(1,0,0,1);
        else {
            float Ks2 = pow(max(dot(N2,H2),0.0),matAlpha);
            specular2 = Ks2*lightSpecular2*matSpecular;
        }
        
        color2 = ambient2 + diffuse2 + specular2;
    }

    fColor = color1 + color2;
    fColor.a = 1.0;
}