
/* Proj1 
 * Rebecca Bever
 */

#include <stdio.h>
#include "glut.h"
#include <math.h>
#include <string.h>

void printFont(float x, float y, float r, float g, float b, char *string);
void createCircle(float k, float r, float h);
void createPeople();
void createBoy(float x1, float x2, float y1, float y2);
void createGirl(float x1, float x2, float x3);
void displayCar();
void displayCity();
void displayControl();
void idle();
void key(unsigned char key, int x, int y);
void specialKey(char key, int x, int y);
void createMenu();
void menu(int value);
void createQuad(float x1, float y1, float x2, float y2);
void createQuad2(float x1, float y1, float x2, float x3, float y2, float x4);
void createTri(float x1, float y1, float x2, float x3, float y2);
void drawOneLine(float x1, float y1, float x2, float y2);
void createFinish();
void batman();

typedef struct
{	float x;
	float y;
}CIRCLE;

//globals
CIRCLE circle;
static GLsizei g_w, g_h;
double ratio = 0.0, inv_ratio = 0.0;
float animate = 0.0, rotationMan = 0.0, speed = 0.5, PI = 3.14,
	rotationCar = 0.0, rotationDog = 0.0, xloc = 0.0, yloc = 0.0, currentx = 0.0, currenty = 0.0;
int startgame = 0, subRender, subDisplay, mainMenu, speedDisplay, displayList = 0, 
	carMin = 0, carSec = 0, carMSec = 0, reset = 0, won = 0, msec = 0, sec = 0, min = 0;
int outlinePoly = GL_POLYGON; //used for outline/filled option
int outlineTri = GL_POLYGON; //used for outline/filled option
int windowWidth = 10.0;
int windowHeight = 10.0;
GLuint list; //display list
char text[150];	

/* The rountine display() gets called each time the window is redrawn*/
void display()
{
	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT);

	/* city viewport */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, g_h/6, g_w, (g_h * 5)/6);

	if (g_w < g_h)
        glOrtho(-10.0,10.0,-10.0*inv_ratio,10.0*inv_ratio,-1.0, 1.0);
    else
        glOrtho(-10.0*ratio,10.0*ratio,-10.0,10.0,-1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//if display list is OFF
	if(displayList == 0)
	{
		displayCity();
	}
	else //if ON
	{
		glCallList(list);
	}
	
	createPeople();
	createFinish();
	
	glPushMatrix();
		glTranslatef(0.0,-9.2,0.0);
		glTranslatef(xloc,yloc,0.0);
		currentx = xloc;
		currenty = yloc;
		glRotatef(rotationCar,0.0,0.0,1.0);
		displayCar();
	glPopMatrix();

	//batman();

	/* control panel viewport */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0.0, 0.0, g_w, g_h/6);

	if (g_w < g_h)
		glOrtho(-1.0,1.0,-1.0*inv_ratio,1.0*inv_ratio, -1.0, 1.0);
    else
        glOrtho(-1.0*ratio,1.0*ratio,-1.0,1.0,-1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	displayControl();

	/* ready to draw now! forces buffered OGL commands to execute */
	glFlush();
	glutSwapBuffers();
}

void displayCity()
{
	/* middle road */
	glColor3f(1.0, 1.0, 0.0);
	createQuad(-1.0, -10.0, 1.0, 4.0);
	/* top road */
	createQuad(-10.0, 2.0, 10.0, 4.0);
	/* bottom road */
	createQuad(-10.0, -5.0, 10.0, -3.0);

	//line stipple on street
	glEnable(GL_LINE_STIPPLE);
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(2.0);
	glLineStipple(5, 0x1C47);
	drawOneLine(0, -10.0, 0, 4.0);
	drawOneLine(-10.0, 3, 10.0, 3.0);
	drawOneLine(-10.0, -4, 10.0, -4.0);
	glDisable(GL_LINE_STIPPLE);

	/* side walk */
	glColor3f(0.658824, 0.658824, 0.658824);
	createQuad(-10.0, 4.0, 10.0, 5.0);

	/* top building */
	glColor3f(0.55, 0.09, 0.09); //scarlet
	createQuad(-5.0, 5.0, 5.0, 9.0);
	/* windows */
	glColor3f(1.0, 1.0, 1.0); //white
	createQuad(-4.5, 5.5, -3.5, 8.5);
	createQuad(-2.5, 5.5, -1.5, 8.5);
	createQuad(-0.5, 5.5, 0.5, 8.5);
	createQuad(2.5, 5.5, 1.5, 8.5);
	createQuad(4.5, 5.5, 3.5, 8.5);

	/* top left building */
	glColor3f(0.196078 , 0.6, 0.8); //skyblue
	createQuad(-8.0, -2.5, -3.0, -0.5);
	//roof
	glColor3f(0.137255, 0.419608, 0.556863); //steelblue
	createTri(-9.0, -0.5, -2.0, -5.5, 1.5);
	//window
	glColor3f(1.0, 1.0, 1.0); 
	createQuad(-5.0, -2.0, -4.0, -1.0);
	createQuad(-7.0, -2.0, -6.0, -1.0);

	/* top right building */
	glColor3f(0.623529, 0.623529, 0.372549); //khaki
	createQuad(3.0, -2.5, 8.0, -0.5);
	//roof
	glColor3f(1.0, 0.498039, 0.0); //coral
	createQuad2(2.0, -0.5, 9.0 , 8.0, 1.0, 3.0);
	//door
	glColor3f(0.55, 0.09, 0.09); 
	createQuad(3.5, -2.5, 4.5, -1.0);

	/* bottom left building */
	glColor3f(0.917647, 0.678431, 0.917647); //plum
	createQuad(-8.0, -9.5, -3.0, -7.0);
	//roof
	glColor3f(0.309804, 0.184314, 0.309804); //purple
	createQuad2(-9.0, -7.5, -2.0 , -3.0, -6.0, -8.0);
	//door
	glColor3f(0.6, 0.2, 0.2); 
	createQuad(-7.5, -9.5, -6.5, -8.0);

	/* bottom right building */
	glColor3f(0.576471, 0.858824, 0.439216); //greenyellow
	createQuad(5.0, -9.5, 9.0, -7.0);
	//roof
	glColor3f(0.137255, 0.556863, 0.137255); //forestgreen
	createTri(4.5,-7.0,9.5,7.0,-5.5);
	//door
	glColor3f(0.81, 0.71, 0.1); 
	createQuad(7.0, -9.5, 8.0, -8.0);

	/* draw a filled polygon - mailbox */
	glColor3f(0.196078, 0.196078, 0.8); //blue
	createQuad(3.0, -8.6, 3.1, -7.8);
	createQuad(2.8, -7.8, 3.3, -7.2);
}

void createFinish()
{
	glColor3f(0.0,0.0,0.0);
	createQuad(-10.0, 2.0, -9.0, 4.0);
	glColor3f(1.0,1.0,1.0);
	createQuad(-10.0, 3.5, -9.5, 4.0);
	createQuad(-9.5, 3.0, -9.0, 3.5);
	createQuad(-10.0, 2.5, -9.5, 3.0);
	createQuad(-9.5, 2.0, -9.0, 2.5);
}

void displayCar()
{
	glColor3f(1.0, 0.0, 0.0);
	createQuad(-0.4, -0.6, 0.4, 0.6);
	glColor3f(1.0,1.0,1.0);
	glBegin(outlinePoly);
		glVertex2f(-0.2,0.5); 
		glVertex2f(0.2,0.5); 
		glVertex2f(0.3,0.3); 
		glVertex2f(-0.3,0.3); 
	glEnd();

	glColor3f(0.0,0.0,0.0);
	createQuad(-0.6,0.5,-0.4,0.2);
	createQuad(0.6,0.5,0.4,0.2);
	createQuad(-0.6,-0.5,-0.4,-0.2);
	createQuad(0.6,-0.5,0.4,-0.2);
}

void createQuad(float x1,float y1, float x2,float y2) 
{	
	glBegin(outlinePoly);
		glVertex2f((x1),(y1)); 
		glVertex2f((x2),(y1)); 
		glVertex2f((x2),(y2)); 
		glVertex2f((x1),(y2)); 
	glEnd();
}

void createQuad2(float x1, float y1,float x2, float x3, float y2, float x4)
{
	glBegin(outlinePoly);
	glVertex2f((x1),(y1)); 
	glVertex2f((x2),(y1)); 
	glVertex2f((x3),(y2)); 
	glVertex2f((x4),(y2)); 
	glEnd();
}

void createTri(float x1,float y1,float x2,float x3,float y2) 
{
	glBegin(outlineTri);
	glVertex2f((x1),(y1)); 
	glVertex2f((x2),(y1)); 
	glVertex2f((x3),(y2)); 
	glEnd();
}

void createCircle (float k, float r, float h)
{
	int i;
	glBegin(outlinePoly);
		for (i = 0; i < 180; i++)
		{
			circle.x = r * cos(i + 0.1) - h;
			circle.y = r * sin(i + 0.1) + k;
			glVertex3f(circle.x + k, circle.y - h,0);
		}
    glEnd();
}

void drawOneLine(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	glVertex2f((x1),(y1)); 
	glVertex2f((x2),(y2)); 
	glEnd();
}

void createPeople()
{ 
	glColor3f(0.47,.8,.8);
	glPushMatrix();
	glTranslatef(animate,0.0,0.0);
	createBoy(-30.0,-29.5, 4.5, 5.5);
	glPopMatrix();

	glColor3f(.6,.8,.19);
	glPushMatrix();
	glTranslatef(animate,0.0,0.0);
	createGirl(-28.0,-27.5,-27.75);
	glPopMatrix();

	glColor3f(.77, .44, .44);
	glPushMatrix();
	glTranslatef(animate,0.0,0.0);
	createBoy(-26.0,-25.5, 4.5,5.5);
	glPopMatrix();

	glColor3f(.77, .75, .66);
	glPushMatrix();
	glTranslatef(animate,0.0,0.0);
	createGirl(-24.0,-23.5,-23.75);
	glPopMatrix();

	glColor3f(.55, .55, .219);
	glPushMatrix();
	glTranslatef(animate,0.0,0.0);
	createBoy(-22.0,-21.5,4.5,5.5);
	glPopMatrix();

	glColor3f(.59, .77, .59);
	glPushMatrix();
	glTranslatef(animate,0.0,0.0);
	createGirl(-20.0,-19.5,-19.75);
	glPopMatrix();

	glColor3f(.219, .55, .55);
	glPushMatrix();
	glTranslatef(animate,0.0,0.0);
	createBoy(-18.0,-17.5,4.5,5.5);
	glPopMatrix();

	glColor3f(.49, .619, .75);
	glPushMatrix();
	glTranslatef(animate,0.0,0.0);
	createGirl(-16.0,-15.5,-15.75);
	glPopMatrix();

	glColor3f(.44, .44, .77);
	glPushMatrix();
	glTranslatef(animate,0.0,0.0);
	createBoy(-14.0,-13.5,4.5,5.5);
	glPopMatrix();

	glColor3f(.55, .219, .55);
	glPushMatrix();
	glTranslatef(animate,0.0,0.0);
	createGirl(-12.0,-11.5,-11.75);
	glPopMatrix();

	glColor3f(.29,0.0,.5);
	glPushMatrix();
	glTranslatef(animate,0.0,0.0);
	createBoy(-10.0,-9.5,4.5,5.5);
	glPopMatrix();

	//mailman & dog rotation matrix
	glPushMatrix();
	glTranslatef(3.2,-7.5,0.0);

	//mailman
	glRotatef(rotationMan,0.0,0.0,1.0);
	glTranslatef(2.0,0.0,0.0);//radius around middle point
	glTranslatef(-3.2,7.5,0.0);
	glColor3f(1.0, 0.0, 0.0);
	createBoy(2.5,2.0,-8.0,-7.5);
	
	//dog
	glRotatef(rotationDog,0.0,0.0,1.0);
	glTranslatef(1.2,0.0,0.0);
	glColor3f(.63,.32,.17);
	batman();

	glPopMatrix();
}

void batman()
{
	glColor3f(0,0,0);
	glBegin(outlinePoly);
		//left
		glVertex2f(0,2.1/10);
		glVertex2f(-.5/10,2.1/10);
		glVertex2f(-.7/10,2.8/10);
		glVertex2f(-1.0/10,1.0/10);
		glVertex2f(-1.7/10,0.7/10);
		glVertex2f(-2.2/10,0.9/10);
		glVertex2f(-2.8/10,1.4/10);
		glVertex2f(-2.9/10,2.1/10);
		glVertex2f(-3.0/10,2.7/10);
		glVertex2f(-5.6/10,1.7/10);
		glVertex2f(-6.3/10,1.0/10);
		glVertex2f(-6.9/10,0.7/10);
		glVertex2f(-7.0/10,0.0/10);
		glVertex2f(-6.9/10,-0.7/10);
		glVertex2f(-6.3/10,-1.0/10);
		glVertex2f(-5.6/10,-1.7/10);
		glVertex2f(-4.0/10,-2.7/10);
		glVertex2f(-3.9/10,-2.1/10);
		glVertex2f(-3.1/10,-1.4/10);
		glVertex2f(-2.4/10,-1.3/10);
		glVertex2f(-2.1/10,-1.7/10);
		glVertex2f(-2.0/10,-2.3/10);
		glVertex2f(-1.7/10,-1.7/10);
		glVertex2f(-1.4/10,-1.5/10);
		glVertex2f(-0.8/10,-1.6/10);
		glVertex2f(-0.3/10,-1.8/10);
		glVertex2f(-0.1/10,-2.4/10);
		glVertex2f(0.0/10,-2.8/10);
		//right
		glVertex2f(0,2.1/10);
		glVertex2f(.5/10,2.1/10);
		glVertex2f(.7/10,2.8/10);
		glVertex2f(1.0/10,1.0/10);
		glVertex2f(1.7/10,0.7/10);
		glVertex2f(2.2/10,0.9/10);
		glVertex2f(2.8/10,1.4/10);
		glVertex2f(2.9/10,2.1/10);
		glVertex2f(3.0/10,2.7/10);
		glVertex2f(5.6/10,1.7/10);
		glVertex2f(6.3/10,1.0/10);
		glVertex2f(6.9/10,0.7/10);
		glVertex2f(7.0/10,0.0/10);
		glVertex2f(6.9/10,-0.7/10);
		glVertex2f(6.3/10,-1.0/10);
		glVertex2f(5.6/10,-1.7/10);
		glVertex2f(4.0/10,-2.7/10);
		glVertex2f(3.9/10,-2.1/10);
		glVertex2f(3.1/10,-1.4/10);
		glVertex2f(2.4/10,-1.3/10);
		glVertex2f(2.1/10,-1.7/10);
		glVertex2f(2.0/10,-2.3/10);
		glVertex2f(1.7/10,-1.7/10);
		glVertex2f(1.4/10,-1.5/10);
		glVertex2f(0.8/10,-1.6/10);
		glVertex2f(0.3/10,-1.8/10);
		glVertex2f(0.1/10,-2.4/10);
		glVertex2f(0.0/10,-2.8/10);

	glEnd();

	glColor3f(1,1,0);
	glBegin(GL_LINE_LOOP);
		//left
		glVertex2f(0,2.1/10);
		glVertex2f(-.5/10,2.1/10);
		glVertex2f(-.7/10,2.8/10);
		glVertex2f(-1.0/10,1.0/10);
		glVertex2f(-1.7/10,0.7/10);
		glVertex2f(-2.2/10,0.9/10);
		glVertex2f(-2.8/10,1.4/10);
		glVertex2f(-2.9/10,2.1/10);
		glVertex2f(-3.0/10,2.7/10);
		glVertex2f(-5.6/10,1.7/10);
		glVertex2f(-6.3/10,1.0/10);
		glVertex2f(-6.9/10,0.7/10);
		glVertex2f(-7.0/10,0.0/10);
		glVertex2f(-6.9/10,-0.7/10);
		glVertex2f(-6.3/10,-1.0/10);
		glVertex2f(-5.6/10,-1.7/10);
		glVertex2f(-4.0/10,-2.7/10);
		glVertex2f(-3.9/10,-2.1/10);
		glVertex2f(-3.1/10,-1.4/10);
		glVertex2f(-2.4/10,-1.3/10);
		glVertex2f(-2.1/10,-1.7/10);
		glVertex2f(-2.0/10,-2.3/10);
		glVertex2f(-1.7/10,-1.7/10);
		glVertex2f(-1.4/10,-1.5/10);
		glVertex2f(-0.8/10,-1.6/10);
		glVertex2f(-0.3/10,-1.8/10);
		glVertex2f(-0.1/10,-2.4/10);
		glVertex2f(0.0/10,-2.8/10);
		//right
		glVertex2f(0,2.1/10);
		glVertex2f(.5/10,2.1/10);
		glVertex2f(.7/10,2.8/10);
		glVertex2f(1.0/10,1.0/10);
		glVertex2f(1.7/10,0.7/10);
		glVertex2f(2.2/10,0.9/10);
		glVertex2f(2.8/10,1.4/10);
		glVertex2f(2.9/10,2.1/10);
		glVertex2f(3.0/10,2.7/10);
		glVertex2f(5.6/10,1.7/10);
		glVertex2f(6.3/10,1.0/10);
		glVertex2f(6.9/10,0.7/10);
		glVertex2f(7.0/10,0.0/10);
		glVertex2f(6.9/10,-0.7/10);
		glVertex2f(6.3/10,-1.0/10);
		glVertex2f(5.6/10,-1.7/10);
		glVertex2f(4.0/10,-2.7/10);
		glVertex2f(3.9/10,-2.1/10);
		glVertex2f(3.1/10,-1.4/10);
		glVertex2f(2.4/10,-1.3/10);
		glVertex2f(2.1/10,-1.7/10);
		glVertex2f(2.0/10,-2.3/10);
		glVertex2f(1.7/10,-1.7/10);
		glVertex2f(1.4/10,-1.5/10);
		glVertex2f(0.8/10,-1.6/10);
		glVertex2f(0.3/10,-1.8/10);
		glVertex2f(0.1/10,-2.4/10);
		glVertex2f(0.0/10,-2.8/10);

	glEnd();
}

void createBoy(float x1, float x2, float y1, float y2)
{
	createQuad(x1,y1,x2,y2);
	glColor3f(1.0,0.8,0.7);
	glTranslatef((x1+x2)/2,y2,1.0);
	createCircle(0.0,0.3,0.0);
}

void createGirl(float x1, float x2, float x3)
{
	//createTri(x1,4.5,x2,x3,5.5);
	glBegin(outlineTri);
		glVertex2f((x1),(4.5)); 
		glVertex2f((x2),(4.5)); 
		glVertex2f((x3),(5.5)); 
	glEnd();

	glColor3f(1.0,0.8,0.7);
	glTranslatef(x3,5.5,1.0);
	createCircle(0.0,0.3,0.0);
}

void displayControl()
{
	/* color shade */
	glShadeModel(GL_SMOOTH);
	glBegin(GL_POLYGON);
		glColor3f(0.658824, 0.658824, 0.658824);
		glVertex2f(-1.0, -1.0);
		glColor3f(0.658824, 0.658824, 0.658824);
		glVertex2f(1.0, -1.0);
		glColor3f(1.0, 1.0, 1.0);
		glVertex2f(1.0, 1.0);
		glColor3f(1.0, 1.0, 1.0);
		glVertex2f(-1.0, 1.0);
	glEnd();

	if(g_w >= 500)//time removed when width is too small
	{
		glColor3f(0.0, 0.0, 0.0);
		sprintf(text, "Time:   %d:%d:%d ",carMin,carSec,carMSec);
		printFont(-0.6,-0.1,0.0,0.0,0.0,text);
		printFont(-0.2,-0.1,0.0,0.0,0.0,"min");

		if(won == 1)
		{
			startgame = 0;
			glColor3f(0.0, 0.0, 0.0);
			printFont(0.2,-0.1,0.0,0.0,0.0,"Finished!");
			
		}
	}
}

void printFont(float x, float y, float r, float g, float b, char *string)
{
	int i;
	glColor3f(r, g, b);
	glRasterPos2f(x, y);

	for (i = 0; i <= strlen(string); i++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}	
}

void idle()
{
    animate += .01;//Increment time variable
	rotationMan += 0.1; // Increment rotation value  
	rotationDog += 0.5;
	
	if(startgame == 1)
		carMSec += 1.0;
	
	if(xloc < -8.5 && yloc > 11.0)
	{	
		won = 1;
	}

    if(animate > 40)
        animate = 0;//reset the time variable
  
	if (rotationMan > 360.0) // rotated beyond 360 degrees (a full rotation)  
		rotationMan -= 360.0; // Subtract 360 degrees off of our rotation 

	if (rotationDog > 360.0)  
		rotationDog -= 360.0;

	if(rotationCar == -360)
		rotationCar = 0;//reset

	if(rotationCar == 360)
		rotationCar = 0;//reset

	if (carMSec > 60)
	{
		carMSec = 0;
		carSec++;
	}
	if (carSec > 60)
	{
		carSec = 0;
		carMin++;
	}

	if(reset == 1)
	{
		carMSec = 0;
		carSec = 0;
		carMin = 0;
	}

	if(xloc < -10.5)
	{
		xloc = 10.5;
	}
	if(xloc > 10.5)
	{
		xloc = -10.5;
	}
	if(yloc < -1.0)
	{
		yloc = 18.5;
	}
	if(yloc > 18.5)
	{
		yloc = -1.0;
	}

	glutPostRedisplay();
}

void key(unsigned char key, int x, int y)
{
	switch (key) 
    {       
        case 'Q': 
            exit(1); 
            break; 
        case 'q': 
            exit(1); 
            break;   
    } 

    glutPostRedisplay(); 
}

void specialKey(char key, int x, int y) 
{  
	switch(key) 
	{
		case GLUT_KEY_LEFT:
			rotationCar += 90.0;
			if(won == 1)
			{
				startgame = 0;
			}
			else
			{
				startgame = 1;
			}

			reset = 0;
			break;
		case GLUT_KEY_RIGHT:
			rotationCar -= 90.0;
			if(won == 1)
			{
				startgame = 0;
			}
			else
			{
				startgame = 1;
			}

			reset = 0;
			break;
		case GLUT_KEY_UP:
			if(rotationCar == 90 || rotationCar == -270)
			{
				xloc = currentx - speed;
			}
			else if(rotationCar == 270 || rotationCar == -90)
			{
				xloc = currentx + speed;
			}
			else if(rotationCar == 180 || rotationCar == -180)
			{
				yloc = currenty - speed;
			}
			else
			{
				yloc += speed;
			}

			if(won == 1)
			{
				startgame = 0;
			}
			else
			{
				startgame = 1;
			}
			reset = 0;
			break;
		case GLUT_KEY_DOWN:
			if(rotationCar == -90 || rotationCar == 270)
			{
				xloc = currentx - speed;
			}
			else if(rotationCar == 90 || rotationCar == -270)
			{
				xloc = currentx + speed;
			}
			else if(rotationCar == 180 || rotationCar == -180)
			{
				yloc = currenty + speed;
			}
			else
			{
				yloc -= speed;
			}


			if(won == 1)
			{
				startgame = 0;
			}
			else
			{
				startgame = 1;
			}
			reset = 0;
			break;
	}

	glutPostRedisplay(); 
}

void createMenu()
{
	// Create a submenu, this has to be done first.
	subRender = glutCreateMenu(menu);
	
	// Add sub menu entry
	glutAddMenuEntry("Filled", 1);
	glutAddMenuEntry("Outline", 2);
	
	subDisplay = glutCreateMenu(menu);
	
	glutAddMenuEntry("ON", 3);
	glutAddMenuEntry("OFF", 4);

	speedDisplay = glutCreateMenu(menu);
	
	glutAddMenuEntry("slow", 5);
	glutAddMenuEntry("medium", 6);
	glutAddMenuEntry("fast", 7);
					 
	// Create the menu, this menu becomes the current menu
	mainMenu = glutCreateMenu(menu);
	
	// Create an entry
	glutAddSubMenu("Render Polygon", subRender);
	glutAddSubMenu("Display List", subDisplay);
	glutAddSubMenu("Change Speed", speedDisplay);
	// Create an entry
	glutAddMenuEntry("Restart", 8);
	glutAddMenuEntry("Quit", 9);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(int value)
{
	if(value == 1)//filled
	{
		outlinePoly = GL_POLYGON;
		outlineTri = GL_TRIANGLES;
	}
	else if(value == 2)//outline
	{
		outlinePoly = GL_LINE_LOOP;
		outlineTri = GL_LINE_LOOP;
	}
	else if(value == 3)//on displaylist
	{
		displayList = 1;
	}
	else if(value == 4)//off displaylist
	{
		displayList = 0;
	}
	else if(value == 5)//slow
	{
		speed = 0.05;
	}
	else if(value == 6)//medium
	{
		speed = 0.5;
	}
	else if(value == 7)//fast
	{
		speed = 1.0;
	}
	else if(value == 8)
	{
		xloc = 0.0;
		yloc = 0.0;
		reset = 1;
		rotationCar = 0.0;
		won = 0;
	}
	else if(value == 9)
	{
		exit(1); 
	}

	glutPostRedisplay();
}

/* Initialize states -- called before */
static void init()
{
	//display list to display the city
	list = glGenLists (1);
	glNewList(list, GL_COMPILE);
		displayCity();
	glEndList();

	/* set background clear color to black */
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void reshape(int w, int h)
{
	g_w = w;
	g_h = h;
	
	ratio = (double)g_w / (double)g_h;
	inv_ratio = (double)g_h / (double)g_w;
	glViewport(0,0,w,h);
}

/* The main program */
int main(int argc, char** argv)
{
	/* create a window, position it, and name it */
	glutInit(&argc, argv);
	//Use double buffering to avoid flicker
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Project 1 - Car Game");

	createMenu();
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(key);

	/* entering the event loop */
	glutMainLoop();

	/* code here will not be executed */
}
