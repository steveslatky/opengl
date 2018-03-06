//
// A GLSL "Hello World"
// Display a blue square
//

#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Camera.h"  //for camera objects
#include "Light.h"	//for lights
#include "Sphere.h"
#include "Ground.h"
#include "Cube.h"

//Forward declarations
void init(void);
void display(void);
void keyboard(unsigned char, int, int);
void specialKeys(int, int, int);
void resize(int width, int height);
void close(void);
void mymouse(GLint button, GLint state, GLint x, GLint y);
void timerCallback(int value);

//Objects
Sphere* sphere;
Cube* cube;
Ground* ground;
Camera *cam, *cam1;
vector<Light> lights;
vector<Drawable*>drawables;

GLuint windowID=0;
// Window size
int w = 512, h = 512;
int theta, interval = 100; // time interval is in milliseconds
vec3 center, origin;
vec4 eye, u, v, n;
// The flag indicates whether or not the spotlight is enabled
bool flag = false;
//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	//initialize GLUT
	glutInit(&argc, argv);
#ifdef __APPLE__
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
	glutInitWindowSize(w, h);

	windowID = glutCreateWindow("Malhotra_A5");

	//print out info about our system
	fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//initialize glew on non-apple systems
#ifndef __APPLE__
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

	init(); //initalize objects

	//set up the callback functions
	glutDisplayFunc(display);
	glutWMCloseFunc(close);
	glutKeyboardFunc(keyboard);  //What to do if a keyboard event is detected
    glutSpecialFunc(specialKeys);
    glutTimerFunc(interval,timerCallback,0);

	//start the main event listening loop
	glutMainLoop();
	return 0;
}

// Initialization
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
    
    eye = vec4(0, 2, 0, 1);
    u = vec4(1, 0, 0, 0);
    v = vec4(0, 1, 0, 0);
    n = vec4(0, 0, 1, 0);
    cam = new Camera(eye,u,v,n,w,h);
    
    ground = new Ground();
    ground->setMaterial(vec4(0, 1, 0, 1), vec4(0, 1, 0, 1), vec4(0, 1, 0, 1), 100);
    drawables.push_back(ground);
    
    cube = new Cube();
    cube->setModelMatrix(Translate(2,1,-6));
    cube->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
    drawables.push_back(cube);
    
    sphere = new Sphere();
    sphere->setModelMatrix(Translate(0,1,-5));
    sphere->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
    drawables.push_back(sphere);
    
    // spotlight - position, ambient, specular, diffuse
    lights.push_back(Light(vec4(0, 2, 0, 1), vec4(1, 1, 1, 1), vec4(0, 0, 0.2f, 1), vec4(1, 1, 1, 1)));
    lights[0].enabled = 0; // disable the spotlight at the beginning
    
    // directional light
    lights.push_back(Light(vec4(100, 0, 0, 1), vec4(1, 1, 1, 1), vec4(0, 0, 0.2f, 1), vec4(1, 1, 1, 1)));
}

//----------------------------------------------------------------------------
//Display callback
void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for (unsigned int i = 0; i < drawables.size(); i++)
		drawables[i]->draw(*cam, lights);
	glutSwapBuffers();
}

//----------------------------------------------------------------------------
//Window resize callback
void resize(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);  //make the viewport the entire window
}

// Mouse callback
void mymouse(GLint button, GLint state, GLint x, GLint y) {
    float x_nearp, y_nearp;
    vec4 pfront, pcam, pworld, rworld;
    // When left mouse button is pressed down, and we're in camera 1 mode
    if (button == GLUT_LEFT_BUTTON and state == GLUT_DOWN and flag == true) {

        // Find the pixel in screen coords that we clicked on
        // Convert this to be the location on the front of the view volume
        x_nearp = (2 * (float)x/w) - 1;
        y_nearp = 1 - (2 * (float)y/h);
        pfront = vec4(x_nearp, y_nearp, -1, 1);
        
        // Adjust for projection
        pcam = inverse(cam->getProjectionMatrix()) * pfront;
        pcam = pcam/pcam[3];
        
        // Get world coordinates
        pworld = inverse(cam->getCameraMatrix()) * pcam;
        pworld = pworld/pworld[3];
        
        // Trace a ray through the camera's COP in the direction of the point picked
        rworld = pworld - cam->eye;
        
        // Test for collision
        for (unsigned int i = 0; i < drawables.size(); i++)
            drawables[i]->testCollision(cam->eye, rworld);
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y)
{
    switch (key) {
        // Slide camera forward
        case GLUT_KEY_UP:
            cam->moveCamera('f');
            glutPostRedisplay();
            break;
        // Slide camera backward
        case GLUT_KEY_DOWN:
            cam->moveCamera('b');
            glutPostRedisplay();
            break;
        // Slide camera right
        case GLUT_KEY_RIGHT:
            cam->moveCamera('r');
            glutPostRedisplay();
            break;
    // Slide camera left
        case GLUT_KEY_LEFT:
            cam->moveCamera('l');
            glutPostRedisplay();
            break;
    }
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
    // Roll clockwise in the xy-coordinate plane
    case 'z':
        cam->flyCamera('z');
        glutPostRedisplay();
        break;
    // Pitch down
    case 'x':
        cam->flyCamera('x');
        glutPostRedisplay();
        break;
    // Yaw clockwise in the zx-coordinate plane
    case 'c':
        cam->flyCamera('c');
        glutPostRedisplay();
        break;
            
    // Roll counterclockwise in the xy-coordinate plane
    case 'Z':
        cam->flyCamera('Z');
        glutPostRedisplay();
        break;
    // Pitch up
    case 'X':
        cam->flyCamera('X');
        glutPostRedisplay();
        break;
    // Yaw counterclockwsise in the zx-coordinate plane
    case 'C':
        cam->flyCamera('C');
        glutPostRedisplay();
        break;
            
    case 32:
        // Toggle flashlight on or off - set enabled
            if (flag == false) {
                flag = true;
                lights[0].enabled = 1; // enable spotlight
                lights[1].enabled = 0; // disable directional light
            }
            else {
                flag = false;
                lights[0].enabled = 0; // disable spotlight
                lights[1].enabled = 1; // enable directional light
                glutTimerFunc(interval, timerCallback, 1);
            }
        glutPostRedisplay();
        break;
	case 'q': case 'Q':
		close();
		break;
	}
}

void close(){
	for (unsigned int i = 0; i < drawables.size(); i++)
		delete(drawables[i]);

	if(windowID>0)
			glutDestroyWindow(windowID);

    exit(EXIT_SUCCESS);
}

// Timer callback
void timerCallback(int value)
{
    // if directional light is enabled
    if (flag == false) {
        vec4 pos = lights[1].getPosition();
        int x = 0, y = 0;
    
        // Quadrant 1 - dec x, inc y
        if (pos.x <= 100 and pos.x > 0 and pos.y >= 0 and pos.y < 100) {
            x = (int)(pos.x - 10) % 360;
            y = (int)(pos.y + 10) % 360;
        }
        // Quadrant 2 - dec x, dec y
        else if (pos.x <= 0 and pos.x > -100 and pos.y > 0 and pos.y <= 100) {
            x = (int)(pos.x - 10) % 360;
            y = (int)(pos.y - 10) % 360;
        }
        // Quadrant 3 - inc x, dec y
        else if (pos.x < 0 and pos.x >= -100 and pos.y <= 0 and pos.y > -100) {
            x = (int)(pos.x + 10) % 360;
            y = (int)(pos.y - 10) % 360;
        }
        // Quadrant 4 - inc x, inc y
        else if (pos.x < 100 and pos.x >= 0 and pos.y < 0 and pos.y >= -100) {
            x = (int)(pos.x + 10) % 360;
            y = (int)(pos.y + 10) % 360;
        }

        lights[1].setPosition(vec4(x, y, pos.z, 1));
    
        // if we go below xz plane, ie. if y value is negative, then turn the directional light off
        if (y < 0) {
            lights[1].enabled = 0;
        }
        else {
            lights[1].enabled = 1;
        }
    
        glutTimerFunc(interval, timerCallback, value+1);
        glutPostRedisplay();
    }
}