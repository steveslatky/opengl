#include "Sphere.h"

Sphere::Sphere() {
    index = 0;
	build();

	glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexLocations), vertexLocations, GL_STATIC_DRAW);

	assert((program = InitShader("vshaderSphere_v150.glsl", "fshaderSphere_v150.glsl"))!=-1);
	glUseProgram(program);

	assert((vpos = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vpos); 
	glVertexAttribPointer(vpos, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//get the locations of the model , camera and projection matrices
	assert((mmpos = glGetUniformLocation(program, "model_matrix")) != -1);
    assert((vmpos = glGetUniformLocation(program, "cam_matrix")) != -1);
    assert((pmpos = glGetUniformLocation(program, "proj_matrix")) != -1);
    
    assert((diffuse_loc = glGetUniformLocation(program, "matDiffuse")) != -1);
    assert((spec_loc = glGetUniformLocation(program, "matSpecular")) != -1);
    assert((ambient_loc = glGetUniformLocation(program, "matAmbient")) != -1);
    assert((alpha_loc = glGetUniformLocation(program, "matAlpha")) != -1);
    
    assert((enabled1 = glGetUniformLocation(program, "enabled1")) != -1);
    assert((light_loc1 = glGetUniformLocation(program, "lightPos1")) != -1);
    assert((ambient_loc1 = glGetUniformLocation(program, "lightAmbient1")) != -1);
    assert((diffuse_loc1 = glGetUniformLocation(program, "lightDiffuse1")) != -1);
    assert((specular_loc1 = glGetUniformLocation(program, "lightSpecular1")) != -1);
    
    assert((enabled2 = glGetUniformLocation(program, "enabled2")) != -1);
    assert((light_loc2 = glGetUniformLocation(program, "lightPos2")) != -1);
    assert((ambient_loc2 = glGetUniformLocation(program, "lightAmbient2")) != -1);
    assert((diffuse_loc2 = glGetUniformLocation(program, "lightDiffuse2")) != -1);
    assert((specular_loc2 = glGetUniformLocation(program, "lightSpecular2")) != -1);
}

Sphere::~Sphere(){
}

//----------------------------------------------------------------------------
void Sphere::build() {
    sqrt2 = (float)sqrt(2.0);
    sqrt6 = (float)sqrt(6.0);
    tetrahedron(4);
}

void Sphere::tetrahedron(int count) {
    vec4 v[4] = {
        vec4(0,0,1,1),
        vec4(0,2 * sqrt2 / 3, -1.0f / 3.0f,1),
        vec4(-sqrt6 / 3.0f, -sqrt2 / 3.0f, -1.0f / 3.0f, 1.0f),
        vec4(sqrt6 / 3.0f, -sqrt2 / 3.0f, -1.0f / 3.0f, 1.0f)
    };
    
    // subdivide each of the 4 faces
    divideTriangle( v[0], v[1], v[2], count );
    divideTriangle( v[3], v[2], v[1], count );
    divideTriangle( v[0], v[3], v[1], count );
    divideTriangle( v[0], v[2], v[3], count );
}

void Sphere::divideTriangle(vec4 a, vec4 b, vec4 c, int count) {
    if (count>0) {
        vec4 v1 = unit(a+b);
        v1.w = 1.0;
        vec4 v2 = unit(a+c);
        v2.w = 1.0;
        vec4 v3 = unit(b+c);
        v3.w = 1.0;
        
        divideTriangle(a, v1, v2, count - 1);
        divideTriangle(c, v2, v3, count - 1);
        divideTriangle(b, v3, v1, count - 1);
        divideTriangle(v1, v3, v2, count - 1);
    }
    else
        triangle(a,b,c);
}

void Sphere::triangle(vec4 a, vec4 b, vec4 c) {
    vertexLocations[index] = a;
    index ++;
    
    vertexLocations[index] = b;
    index ++;
    
    vertexLocations[index] = c;
    index ++;
}

vec4 Sphere::unit(vec4 p) {
    float len = p.x*p.x + p.y*p.y + p.z*p.z;
    vec4 t;
    if (len > DivideByZeroTolerance) {
        t = p / sqrt(len);
        t.w = 1.0;
    }
    return t;
}

void Sphere::setMaterial(vec4 a, vec4 d, vec4 s, float sh) {
    diffuse = d;
    specular = s;
    ambient = a;
    shine = sh;
}

void Sphere::draw(Camera cam, vector<Light> lights){
    glBindVertexArray(VAO);
    glUseProgram(program);
    glUniformMatrix4fv(mmpos, 1, GL_TRUE,modelmatrix);
    glUniformMatrix4fv(vmpos, 1, GL_TRUE, cam.getCameraMatrix());
    glUniformMatrix4fv(pmpos, 1, GL_TRUE, cam.getProjectionMatrix());
    
    glUniform4fv(diffuse_loc, 1, diffuse);
    glUniform4fv(spec_loc, 1, specular);
    glUniform4fv(ambient_loc, 1, ambient);
    glUniform1f(alpha_loc, shine);
    
    // spotlight
    glUniform1i(enabled1, lights[0].enabled);
    glUniform4fv(light_loc1, 1, lights[0].getPosition());
    glUniform4fv(ambient_loc1, 1, lights[0].getAmbient());
    glUniform4fv(diffuse_loc1, 1, lights[0].getDiffuse());
    glUniform4fv(specular_loc1, 1, lights[0].getSpecular());
    
    // directional light
    glUniform1i(enabled2, lights[1].enabled);
    glUniform4fv(light_loc2, 1, lights[1].getPosition());
    glUniform4fv(ambient_loc2, 1, lights[1].getAmbient());
    glUniform4fv(diffuse_loc2, 1, lights[1].getDiffuse());
    glUniform4fv(specular_loc2, 1, lights[1].getSpecular());
    
    glDrawArrays(GL_TRIANGLES, 0,numVertices);
}

bool Sphere::testCollision(vec4 rayOrigin, vec4 rayDirection)
{
    smallest_t = FLT_MAX;
    myi = -1;
    // Find the first object this "infinte" ray intersects with
    // For every triangle in every object, put the vertices v of the triangle into world coords
    for (unsigned int i = 0; i < numVertices; i += 3) {
        vertexWorld[i] = modelmatrix * vertexLocations[i];
        vertexWorld[i+1] = modelmatrix * vertexLocations[i+1];
        vertexWorld[i+2] = modelmatrix * vertexLocations[i+2];
        
        F = vertexWorld[i];
        E = vertexWorld[i+1];
        G = vertexWorld[i+2];
        N = cross((F - E), (G - E));
        N[3] = 0; // Set the 4th coord to 0 because N is a vector
        
        // Test if the ray intersects that triangle and compute the intersection time/location
        t = -(dot(N,rayOrigin) - dot(E,N)) / dot(N,rayDirection);
            
        // Only proceed if we know that the ray and plane will intersect
        if ((t >= 0) and (dot(N,rayDirection) != 0)) {
            
            // point = A + tD
            P = rayOrigin + t * rayDirection;
            
            // Use the inside out test to determine whether the point is inside the triangle
            cross12 = cross((F - E),(P - E));
            cross12[3] = 0;
            dot12 = dot(N,cross12);
            
            cross23 = cross((E - G),(P - G));
            cross23[3] = 0;
            dot23 = dot(N,cross23);
            
            cross31 = cross((G - F),(P - F));
            cross31[3] = 0;
            dot31 = dot(N,cross31);
            
            if (dot12 > 0 and dot23 > 0 and dot31 > 0)
                // Choose the closest intersection point (smallest time) > than the near plane
                // Keep track of vertices of triangle
                if (t < smallest_t) {
                    smallest_t = t;
                    myi = i;
                }
        }
    } // end for
    
    if (myi != -1) {
        return true;
    }
    return false;
        //glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexLocations)+(myi+2)*sizeof(vec4), sizeof(vec4), vec4(0.0,0.0,0.0,1.0));
}
