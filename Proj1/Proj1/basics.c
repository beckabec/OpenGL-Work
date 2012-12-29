
/* basics 
 * 
 * A basic, first OpenGL program.
 * 
 * This program illustrates how you open an window, specify its position and size,
 * and draw something in the window.
 *
 * Also illustrated: the state-machine operation of OGL. Notice that if you resize
 * the window, the box becomes blue -- the last color specified! This isn't the right
 * idea! Where should you put the color of the box?
 *
 * When you resize the window so it is not square, the box changes shape. This might
 * not be desired -- we will look into fixing this later. (Keep in mind, this has
 * something to do with the aspect ratio.)
 *
 * Try changing the size of gluOrtho2D -- see in code below -- and you'll see how the
 * relative size of the box changes within the window.
 *
 *
 * Dianne Hansford, August 2004
 * Modified by Ross Maciejewski August 2011
 */


#include <stdio.h>

/* Include the GLUT library. This file (glut.h) contains gl.h and glu.h */
/* See how this is "glut.h" as opposed to <glut.h> - this is due to the fact that you need
   glut in your local directory. */
#include "glut.h"


/* 
 * The rountine display() gets called each time the window is redrawn.
 */
void display()
{
	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT);

	/* draw a filled polygon */
	glBegin(GL_POLYGON);
		glVertex2f(-0.5, -0.5);
		glVertex2f( 0.5, -0.5);
		glVertex2f( 0.5,  0.5);
		glVertex2f(-0.5,  0.5);
	glEnd();

	/* draw a fancy line -- red to blue variation in color */
	/* variation is created with linear interpolation */
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(-0.75, -0.75);
		glColor3f(0.0, 0.0, 1.0);
		glVertex2f(0.75, -0.75);
	glEnd();

	//Make sure when window is resized, the box remains color it is inititally set to
	glColor3f(0.0, 1.0, 1.0);

	/* ready to draw now! forces buffered OGL commands to execute */
	glFlush();
}

/* Initialize states -- called before */

void init()
{
	/* set background clear color to black */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	/* set current color to aqua */
	glColor3f(0.0, 1.0, 1.0);

	/* identify the projection matrix that we would like to alter */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* the window will correspond to these world coorinates */
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

	/* we can play ... see what happens if we swap out this one with that above */
	//gluOrtho2D(-10.0, 10.0, -10.0, 10.0);

	/* identify the modeling and viewing matrix that can be modified from here on */
	/* we leave the routine in this mode in case we want to move the object around */
	/* or specify the camera */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* The main program */

int main(int argc, char** argv)
{

	/* create a window, position it, and name it */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Project 1 - basics");

	/* create a callback routine for (re-)display */
	glutDisplayFunc(display);

	/* init some states */
	init();

	/* entering the event loop */
	glutMainLoop();

	/* code here will not be executed */
}
