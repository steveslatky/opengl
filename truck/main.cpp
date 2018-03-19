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
void createFalafel(char corner, vec3 ball);

//Objects
Sphere *sphere1, *sphere2, *sphere3, *sphere4, *falafel;
Cube* cube;
Ground* ground;
Camera *cam, *cam1, *cam2;
vector<Light> lights;
vector<Drawable*>drawables;

GLuint windowID=0;
// Window size
int w = 1000, h = 700;
int theta, interval = 100; // time interval is in milliseconds
int move_truck = 0; // value that determines whether or not the truck should move
vec3 ball = vec3(1.45,1,3); // coordinates to add falafel balls
vec3 center, origin;
vec4 eye, u, v, n;
// The flag indicates whether or not the truck is moving
bool flag = true, collision;
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

	windowID = glutCreateWindow("Malhotra_Slatky_Final_Project");

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
    glutMouseFunc(mymouse);
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

    // Camera 2
    eye = vec4(0, 2, -5, 1);
    u = vec4(1, 0, 0, 0);
    v = vec4(0, 1, 0, 0);
    n = vec4(0, 0, -1, 0);
    cam2 = new Camera(eye,u,v,n,w,h);
    
    // Camera 1
    eye = vec4(0, 2, 5, 1);
    u = vec4(1, 0, 0, 0);
    v = vec4(0, 1, 0, 0);
    n = vec4(0, 0, 1, 0);
    cam1 = new Camera(eye,u,v,n,w,h);
    
    cam = cam1;
    
    ground = new Ground();
    ground->setMaterial(vec4(0, 1, 0, 1), vec4(0, 1, 0, 1), vec4(0, 1, 0, 1), 100);
    drawables.push_back(ground);
    
    cube = new Cube();
    cube->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
    cube->setModelMatrix(Translate(-0.8,1.5,3.5)*Scale(0.5, 0.3, 0.3));
    drawables.push_back(cube);
    
    // left back
    sphere1 = new Sphere();
    sphere1->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
    sphere1->setModelMatrix(Translate(-1.45,1,2.5)*Scale(0.05, 0.05, 0.05));
    drawables.push_back(sphere1);
    
    // right back
    sphere2 = new Sphere();
    sphere2->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
    sphere2->setModelMatrix(Translate(-0.9,1,2.5)*Scale(0.05, 0.05, 0.05));
    drawables.push_back(sphere2);
    
    // left front
    sphere3 = new Sphere();
    sphere3->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
    sphere3->setModelMatrix(Translate(-1.45,1,3)*Scale(0.05, 0.05, 0.05));
    drawables.push_back(sphere3);
    
    // right front
    sphere4 = new Sphere();
    sphere4->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
    sphere4->setModelMatrix(Translate(-0.9,1,3)*Scale(0.05, 0.05, 0.05));
    drawables.push_back(sphere4);
    
    // spotlight - position, ambient, specular, diffuse
    lights.push_back(Light(vec4(0, 2, 0, 1), vec4(1, 1, 1, 1), vec4(0, 0, 0.2f, 1), vec4(1, 1, 1, 1)));
    lights[0].enabled = 0; // disable the spotlight at the beginning
    
    // directional light
    lights.push_back(Light(vec4(0, 100, 0, 1), vec4(1, 1, 1, 1), vec4(0, 0, 0.2f, 1), vec4(1, 1, 1, 1)));
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
    // When left mouse button is pressed down
    if (button == GLUT_LEFT_BUTTON and state == GLUT_DOWN) {
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
        for (unsigned int i = 0; i < drawables.size(); i++) {
            collision = drawables[i]->testCollision(cam->eye, rworld);
            // Remove a falafel ball if I click on it
            if (collision)
                drawables.erase(drawables.begin()+i);
        }
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
            // Toggle between cameras when the spacebar is pressed
            if (flag == false) {
                flag = true;
                cam = cam1;
                cam->moveCam = true;
            }
            else {
                flag = false;
                cam = cam2;
                cam->moveCam = false;
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

void createFalafel (char corner, vec3 ball) {
    switch (corner) {
        case 'L':
            cout << "create falafel on left corner" << endl;
            falafel = new Sphere();
            falafel->setMaterial(vec4(1, 0, 0, 1), vec4(1, 0, 0, 1), vec4(1, 0, 0, 1), 100);
            falafel->setModelMatrix(Translate(-ball.x,ball.y,ball.z)*Scale(0.05, 0.05, 0.05));
            drawables.push_back(falafel);
            break;
        case 'R':
            cout << "create falafel on right corner" << endl;
            falafel = new Sphere();
            falafel->setMaterial(vec4(1, 0, 0, 1), vec4(1, 0, 0, 1), vec4(1, 0, 0, 1), 100);
            falafel->setModelMatrix(Translate(ball.x,ball.y,ball.z)*Scale(0.05, 0.05, 0.05));
            drawables.push_back(falafel);
            break;
    }
    glutPostRedisplay();
}

// Timer callback
void timerCallback(int value)
{
    // TODO :
    // New object shape - other than what was implemented in class
    // Texture mapped objects - don't really need skybox

    mat4 cube_mm = cube->getModelMatrix();
    mat4 sp1_mm = sphere1->getModelMatrix();
    mat4 sp2_mm = sphere2->getModelMatrix();
    mat4 sp3_mm = sphere3->getModelMatrix();
    mat4 sp4_mm = sphere4->getModelMatrix();
    
    // when we have reached the right corner, start moving left
    if (move_truck == 190) move_truck = 0;
    
    // until we have reached the right corner, move objects right
    if (move_truck >= 0 and move_truck < 95) {
        move_truck += 1;
        cube->setModelMatrix(cube_mm*Translate(0.034, 0, 0));
        sphere1->setModelMatrix(sp1_mm*Translate(0.5, 0, 0));
        sphere2->setModelMatrix(sp2_mm*Translate(0.5, 0, 0));
        sphere3->setModelMatrix(sp3_mm*Translate(0.5, 0, 0));
        sphere4->setModelMatrix(sp4_mm*Translate(0.5, 0, 0));
    }
    
    // until we have reached the left corner, move objects left
    else if (move_truck >= 95 and move_truck < 190) {
        move_truck += 1;
        cube->setModelMatrix(cube_mm*Translate(-0.034, 0, 0));
        sphere1->setModelMatrix(sp1_mm*Translate(-0.5, 0, 0));
        sphere2->setModelMatrix(sp2_mm*Translate(-0.5, 0, 0));
        sphere3->setModelMatrix(sp3_mm*Translate(-0.5, 0, 0));
        sphere4->setModelMatrix(sp4_mm*Translate(-0.5, 0, 0));
    }
    
    // Dynamically create falafel balls
    if (move_truck == 189) {
        ball.x = ball.x + 0.05;
        createFalafel('L', ball);
    }
    else if (move_truck == 95) {
        ball.x = ball.x + 0.05;
        createFalafel('R', ball);
    }
    
    glutTimerFunc(interval, timerCallback, value+1);
    glutPostRedisplay();
}

//        vec4 pos = lights[1].getPosition();
//        int x = 0, y = 0;
//    
//        // Quadrant 1 - dec x, inc y
//        if (pos.x <= 100 and pos.x > 0 and pos.y >= 0 and pos.y < 100) {
//            x = (int)(pos.x - 10) % 360;
//            y = (int)(pos.y + 10) % 360;
//        }
//        // Quadrant 2 - dec x, dec y
//        else if (pos.x <= 0 and pos.x > -100 and pos.y > 0 and pos.y <= 100) {
//            x = (int)(pos.x - 10) % 360;
//            y = (int)(pos.y - 10) % 360;
//        }
//        // Quadrant 3 - inc x, dec y
//        else if (pos.x < 0 and pos.x >= -100 and pos.y <= 0 and pos.y > -100) {
//            x = (int)(pos.x + 10) % 360;
//            y = (int)(pos.y - 10) % 360;
//        }
//        // Quadrant 4 - inc x, inc y
//        else if (pos.x < 100 and pos.x >= 0 and pos.y < 0 and pos.y >= -100) {
//            x = (int)(pos.x + 10) % 360;
//            y = (int)(pos.y + 10) % 360;
//        }

//        lights[1].setPosition(vec4(x, y, pos.z, 1));
//    
//        // if we go below xz plane, ie. if y value is negative, then turn the directional light off
//        if (y < 0) {
//            lights[1].enabled = 0;
//        }
//        else {
//            lights[1].enabled = 1;
//        }