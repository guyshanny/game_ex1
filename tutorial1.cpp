//  
//  cgp-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//  Tweaked by HUJI Computer Games Programming staff 2014

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr

using namespace glm;

#include "World.h"
#include "ShaderIO.h"

#include <iostream>

/** Internal Definitions */

#define	WINDOW_SIZE         (600) // initial size of the window               //
#define	WINDOW_POS_X        (100) // initial X position of the window         //
#define	WINDOW_POS_Y        (100) // initial Y position of the window         //

#define RC_OK                 (0) // Everything went ok                       //
#define RC_INVALID_ARGUMENTS  (1) // Invalid arguments given to the program   //
#define RC_INPUT_ERROR        (2) // Invalid input to the program             //

#define	ARGUMENTS_PROGRAM     (0) // program name position on argv            //
#define	ARGUMENTS_INPUTFILE   (1) // given input file position on argv        //
#define	ARGUMENTS_REQUIRED    (2) // number of required arguments             //

/** Key definitions */

// #define KEY_ANIMATE         ('a') // Key used to start animation              //
#define KEY_ESC				(27) // Key used to terminate the program - ESC  //
// #define KEY_QUIT            ('\e') // Key used to terminate the program        //
#define KEY_RESET           ('r') // Key used to reset the applied TX's	      //
#define KEY_RELOAD          ('l') // Key used to reload the shaders 	      //
// #define KEY_WIREFRAME       ('w') // Key used to toggle wireframe rendering   //
#define KEY_FORWARD			('w') // Key go forward
#define KEY_BACKWARD		('s') // Key go backward
#define KEY_TURN_LEFT		('a') // Key turn left
#define KEY_TURN_RIGHT		('d') // Key turn right

// Extras
#define KEY_MOVE_LEFT		('q') // Key to move left
#define KEY_MOVE_RIGHT		('e') // Key to move right
#define KEY_CROUCH			('c') // Key to crouch
#define KEY_JUMP			(' ') // Key jump
#define KEY_ZOOM			('z') // Key zoom
#define KEY_TELEPORT		('t') // Key teleport


/** display callback */
void display(void);

/** window reshape callback  */
void windowResize(int width, int height);

/** keyboard callback  */
void keyboard(unsigned char key, int x, int y);

/** keyboard up callback */
void keyboardUp(unsigned char key, int x, int y);

/** mouse click callback */
void mouse(int button, int state, int x, int y) ;

/** mouse dragging callback  */
void motion(int x, int y) ;

/** timer callback */
void timer(int value) ;

/** Global variables */

int     g_nFPS = 0, g_nFrames = 0;              // FPS and FPS Counter
int     g_dwLastFPS = 0;                        // Last FPS Check Time
bool    g_startAnimation = false;
bool    g_duringAnimation = false;

// A global variable for our model (a better practice would be to use a singletone that holds all model):
World _world;


/** main function */
int main(int argc, char* argv[])
{
    std::cout << "Starting ex0..." << std::endl;
	
    // Initialize GLUT
    glutInit(&argc, argv) ;
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE) ;
#else
    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
#endif
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
    glutCreateWindow("CGP Tutorial 1");
	
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    int glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
        exit(1);
    }
	
#ifdef __APPLE__
    GLint sync = 1;
    CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
#endif
		
    // Set callback functions:
    glutDisplayFunc(display) ;
    glutReshapeFunc(windowResize) ;
    glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(100, timer, 0);   // uint millis int value
	
    // Init anything that can be done once and for all:
    _world.init();

    // Set clear color to black:
    glClearColor(0.0, 0.0, 0.0, 0.0);
		
    // Start events/drawing loop
    glutMainLoop();
	
    return 0;
}

void display(void)
{
    // Clear the screen buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Let the model to draw itself...
    _world.draw();
	
    // Swap those buffers so someone will actually see the results... //
    glutSwapBuffers();
}

// This method is called when the window is resized
void windowResize(int w, int h)
{
    // Update model to fit the new resolution
    _world.resize(w, h);
    
    // set the new viewport //
    glViewport(0, 0, w, h);
    
    // Refresh the display //
    glutPostRedisplay();
}

/********************************************************************
 * Function  :	keyboard
 * Arguments :	key : the key that was pressed
 *              x   : x value of the current mouse location
 *              y   : y value of the current mouse location
 * Returns   : n/a
 * Throws    : n/a
 *
 * Purpose   : This function handles all the keyboard input from the user.
 *             It supports terminating the application when the KEY_QUIT is pressed.
 *
 \******************************************************************/
void keyboard(unsigned char key, int x, int y)
{
    unsigned int lower_key = tolower(key);

	int isShiftPressed = glutGetModifiers();

	if (GLUT_ACTIVE_SHIFT == isShiftPressed)
	{
		_world.shiftKeyPressed(true);
	}
	else
	{
		_world.shiftKeyPressed(false);
	}

    switch(lower_key)
    {
		case KEY_FORWARD:
			_world.moveForwardKeyPressed();
			break;
		case KEY_BACKWARD:
			_world.moveBackwardKeyPressed();
			break;
		case KEY_TURN_LEFT:
			_world.turnLeftKeyPressed();
			break;
		case KEY_TURN_RIGHT:
			_world.turnRightKeyPressed();
			break;
		case KEY_MOVE_LEFT:
			_world.strafeLeftKeyPressed();
 			break;
		case KEY_MOVE_RIGHT:
			_world.strafeRightKeyPressed();
 			break;
		case KEY_CROUCH:
			_world.crouchKeyPressed(true);
			break;
		case KEY_JUMP:
			_world.jumpKeyPressed(true);
			break;
		case KEY_ZOOM:
			_world.zoomKeyPressed(true);
			break;
 		case KEY_RESET:
			_world.resetKeyPressed();
 			break;
		case KEY_TELEPORT:
			_world.teleportKeyPressed();
			break;
//		case KEY_RELOAD:
//			// Reload the shading programs of the object
//			// For use in a future exercise
//			break;
// 		case KEY_WIREFRAME:
// 			_world.toggle_polygon_mode();
// 			break;
// 		case KEY_ANIMATE:
// 			if (!g_duringAnimation) {
// 				g_startAnimation = true;
// 			}
//			break;
// 		case KEY_QUIT:
		case KEY_ESC:
			// Terminate the program:
			exit(RC_OK);
			break;
		default:
			std::cerr << "Key " << lower_key << " undefined\n";
			break;
    }
	glutPostRedisplay();
    return;
}

void keyboardUp(unsigned char key, int x, int y)
{
	unsigned int lower_key = tolower(key);

	switch (lower_key)
	{
		case KEY_CROUCH:
			_world.crouchKeyPressed(false);
			break;
		case KEY_JUMP:
			_world.jumpKeyPressed(false);
			break;
		case KEY_ZOOM:
			_world.zoomKeyPressed(false);
			break;
		default:
			//std::cerr << "Key " << lower_key << " undefined\n";
			break;
	}
	glutPostRedisplay();
	return;
}

/********************************************************************
 * Function  :   mouse
 * Arguments :   button  : the button that was engaged in some action
 *               state   : the new state of that button
 *               x       : x value of the current mouse location
 *               y       : y value of the current mouse location
 * Returns   :   n/a
 * Throws    :   n/a
 *
 * Purpose   :   This function handles mouse actions.
 *
 \******************************************************************/
void mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
    {
		
    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
        
    }
    
    return;
}


/********************************************************************
 * Function  :   motion
 * Arguments :   x   : x value of the current mouse location
 *               y   : y value of the current mouse location
 * Returns   :   n/a
 * Throws    :   n/a
 *
 * Purpose   :   This function handles mouse dragging events.
 *
 \******************************************************************/
void motion(int x, int y)
{
    return;
}

static const float animationDurationInFrames = 300;

void timer(int value) {
    /* Set the timer to be called again in X milli - seconds. */
    if (g_startAnimation)
    {
        value = 0;
        g_duringAnimation = true;
        g_startAnimation = false;
    }
    
    glutTimerFunc(25, timer, ++value);   // uint millis int value

    if (g_duringAnimation) {
        float t = (float)value / (float)animationDurationInFrames;
        if (t > 1) {
            g_duringAnimation = false;
            return;
        }
        glutPostRedisplay();
    }
}
