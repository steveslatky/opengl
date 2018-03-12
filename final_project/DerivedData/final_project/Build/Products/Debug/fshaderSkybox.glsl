#version 150

in vec3 texCoord;
uniform samplerCube cubeMap;

out vec4  fColor;

void main() 
{ 
    fColor = texture(cubeMap, texCoord);
	fColor.a = 1.0;
} 

