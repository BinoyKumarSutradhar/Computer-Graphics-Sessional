#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double rt;
double z_angle;// for q and w
double y_angle;// for e and r
double x_angle;
double pipe_y_angle;// for a and s
double pipe_rotate;
double t; // square er side length
double rad;// bubble er radius
int f1x,f1y;// bubble er jonno flag
int f2x,f2y;
int f3x,f3y;
int f4x,f4y;
int f5x,f5y;

int cf1,cf2,cf3,cf4,cf5;// flag---if the bubble has entered the circle
double sc;
double t_sc;

int play;


struct point
{
	double x,y,z;
};

point pos;
point u,r,l;
point l1,l2;
point u1,u2;
point r1,r2;
point pos1,pos2,pos3,pos4,pos5;
point v1,v2,v3,v4,v5;

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 1000,0,0);
			glVertex3f(-1000,0,0);

			glVertex3f(0,-1000,0);
			glVertex3f(0, 1000,0);

			glVertex3f(0,0, 1000);
			glVertex3f(0,0,-1000);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    if(radius<70)
    {
        glColor3f(1,0.5,0);
    }
    else
    {
       glColor3f(1,0,0);
    }

    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        //(0,0,0)--> BLACK
        //(1,1,1)--> WHITE
        //glColor3f(0,0,0);
		for(j=0;j<slices;j++)
		{
			if(j%2 ==0)
            {
               glColor3f(0,0,0);
            }
            else
            {
                glColor3f(1,1,1);
            }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
//                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
//				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
//				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
//				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawSphere1(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        //(0,0,0)--> BLACK
        //(1,1,1)--> WHITE
        //glColor3f(0,0,0);
		for(j=0;j<slices;j++)
		{
			if(j%2 ==0)
            {
               glColor3f(0,0,0);
            }
            else
            {
                glColor3f(1,1,1);
            }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
//                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
//				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
//				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
//				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawSphere2(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			if(j%2 ==0)
            {
               glColor3f(1,1,1);
            }
            else
            {
                glColor3f(0,0,0);
            }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

			}glEnd();
		}
	}
}

void drawPipe(double length,double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		//h=radius*sin(((double)i/(double)stacks)*(pi/2));
		//r=radius*cos(((double)i/(double)stacks)*(pi/2));
		h=length*sin(((double)i/(double)stacks)*(pi/2));
		r=radius;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			if(j%2 ==0)
            {
               glColor3f(1,1,1);
            }
            else
            {
                glColor3f(0,0,0);
            }
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

			}glEnd();
		}
	}
}

void drawPipeHead(double length,double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks/3;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		//h= length;
		r= radius + (i*i);
		//r= 2*radius - r;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks/3;i++)
	{
		for(j=0;j<slices;j++)
		{
			if(j%2 ==0)
            {
               glColor3f(1,1,1);
            }
            else
            {
                glColor3f(0,0,0);
            }
			glBegin(GL_QUADS);{
			    //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);

			}glEnd();
		}
	}
}



void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
		    r1.x = r.x;
		    r1.y = r.y;
		    r1.z = r.z;
			r.x = (r.x * cos(rt)) + (l.x * sin(rt));
			r.y = (r.y * cos(rt)) + (l.y * sin(rt));
			r.z = (r.z * cos(rt)) + (l.z * sin(rt));

			l.x = (l.x * cos(rt)) - (r.x * sin(rt));
			l.y = (l.y * cos(rt)) - (r.y * sin(rt));
			l.z = (l.z * cos(rt)) - (r.z * sin(rt));
			break;
        case '2':
            r2.x = r.x;
		    r2.y = r.y;
		    r2.z = r.z;
			r.x = (r.x * cos(rt)) - (l.x * sin(rt));
			r.y = (r.y * cos(rt)) - (l.y * sin(rt));
			r.z = (r.z * cos(rt)) - (l.z * sin(rt));

			l.x = (l.x * cos(rt)) + (r.x * sin(rt));
			l.y = (l.y * cos(rt)) + (r.y * sin(rt));
			l.z = (l.z * cos(rt)) + (r.z * sin(rt));
			break;
        case '3':
            l1.x = l.x;
            l1.y = l.y;
            l1.z = l.z;
			l.x = (l.x * cos(rt)) + (u.x * sin(rt));
			l.y = (l.y * cos(rt)) + (u.y * sin(rt));
			l.z = (l.z * cos(rt)) + (u.z * sin(rt));

			u.x = (u.x * cos(rt)) - (l1.x * sin(rt));
			u.y = (u.y * cos(rt)) - (l1.y * sin(rt));
			u.z = (u.z * cos(rt)) - (l1.z * sin(rt));
			break;
        case '4':
            l2.x = l.x;
            l2.y = l.y;
            l2.z = l.z;
			l.x = (l.x * cos(rt)) - (u.x * sin(rt));
			l.y = (l.y * cos(rt)) - (u.y * sin(rt));
			l.z = (l.z * cos(rt)) - (u.z * sin(rt));

			u.x = (u.x * cos(rt)) + (l2.x * sin(rt));
			u.y = (u.y * cos(rt)) + (l2.y * sin(rt));
			u.z = (u.z * cos(rt)) + (l2.z * sin(rt));
			break;
        case '5':
            u1.x = u.x;
            u1.y = u.y;
            u1.z = u.z;
			u.x = (u.x * cos(rt)) + (r.x * sin(rt));
			u.y = (u.y * cos(rt)) + (r.y * sin(rt));
			u.z = (u.z * cos(rt)) + (r.z * sin(rt));

			r.x = (r.x * cos(rt)) - (u.x * sin(rt));
			r.y = (r.y * cos(rt)) - (u.y * sin(rt));
			r.z = (r.z * cos(rt)) - (u.z * sin(rt));
			break;
        case '6':
            u2.x = u.x;
            u2.y = u.y;
            u2.z = u.z;
			u.x = (u.x * cos(rt)) - (r.x * sin(rt));
			u.y = (u.y * cos(rt)) - (r.y * sin(rt));
			u.z = (u.z * cos(rt)) - (r.z * sin(rt));

			r.x = (r.x * cos(rt)) + (u.x * sin(rt));
			r.y = (r.y * cos(rt)) + (u.y * sin(rt));
			r.z = (r.z * cos(rt)) + (u.z * sin(rt));
			break;
        case 'q':
            if(z_angle<60)
            {
                z_angle += 15;
            }
            else
            {
                z_angle = 60;
            }
            break;
        case 'w':
            if(z_angle>-60)
            {
                z_angle -= 15;
            }
            else
            {
                z_angle = -60;
            }
            break;
        case 'e':
            if(y_angle<60)
            {
                y_angle += 15;
            }
            else
            {
                y_angle = 60;
            }
            break;
        case 'r':
            if(y_angle>-60)
            {
                y_angle -= 15;
            }
            else
            {
                y_angle = -60;
            }
            break;
        case 'a':
            if(pipe_y_angle<60)
            {
                pipe_y_angle += 15;
            }
            else
            {
                pipe_y_angle = 60;
            }
            break;
        case 's':
            if(pipe_y_angle>-60)
            {
                pipe_y_angle -= 15;
            }
            else
            {
                pipe_y_angle = -60;
            }
            break;
        case 'd':
            if(pipe_rotate<60)
            {
                pipe_rotate += 5;
            }
            else
            {
                pipe_rotate = 60;
            }
            break;
        case 'f':
            if(pipe_rotate>-60)
            {
                pipe_rotate -= 5;
            }
            else
            {
                pipe_rotate = -60;
            }
            break;
        case 'p':
            if(play%2 == 0)
            {
                t_sc = sc;
                sc = 0;
            }
            else
            {
                sc = t_sc;
            }
            play++;
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
//			cameraHeight -= 3.0;
			sc -= 0.02;
            if(sc <= 0)
            {
                sc = 0.01;
            }
			break;
		case GLUT_KEY_UP:		// up arrow key
//			cameraHeight += 3.0;
            sc += 0.02;
			break;

		case GLUT_KEY_RIGHT:
			//cameraAngle += 0.03;
			pos.x += 4*r.x;
			pos.y += 4*r.y;
			pos.z += 4*r.z;
			break;
		case GLUT_KEY_LEFT:
			//cameraAngle -= 0.03;
			pos.x -= 4*r.x;
			pos.y -= 4*r.y;
			pos.z -= 4*r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x += 4*u.x;
			pos.y += 4*u.y;
			pos.z += 4*u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
		    pos.x -= 4*u.x;
			pos.y -= 4*u.y;
			pos.z -= 4*u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}


double randfrom(double min, double max)
{
    //srand(time(NULL));
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);

	//gluLookAt(pos.x, pos.y, pos.z, pos.x+l.x, pos.y+l.y, pos.z+l.z, u.x, u.y, u.z);
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	//drawAxes();
    glColor3f(0, 1, 0);
		glBegin(GL_LINES);{
			glVertex3f( t,t,2);
			glVertex3f(-t,t,2);

			glVertex3f(-t,t,2);
			glVertex3f(-t,-t,2);

			glVertex3f(-t,-t,2);
			glVertex3f(t,-t,2);

			glVertex3f(t,-t,2);
			glVertex3f(t,t,2);
		}glEnd();
    drawCircle(70,80);

    //*********1st
    glPushMatrix();
    {
        glTranslatef(pos1.x,pos1.y,pos1.z);
        drawCircle(rad,80);
    }
    glPopMatrix();

    //**********2nd
    glPushMatrix();
    {
        glTranslatef(pos2.x,pos2.y,pos2.z);
        drawCircle(rad,80);
    }
    glPopMatrix();

    //*******3rd
    glPushMatrix();
    {
        glTranslatef(pos3.x,pos3.y,pos3.z);
        drawCircle(rad,80);
    }
    glPopMatrix();

    //****4th
    glPushMatrix();
    {
        glTranslatef(pos4.x,pos4.y,pos4.z);
        drawCircle(rad,80);
    }
    glPopMatrix();

    //****5th
    glPushMatrix();
    {
        glTranslatef(pos5.x,pos5.y,pos5.z);
        drawCircle(rad,80);
    }
    glPopMatrix();


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}




void animate(){
	angle+=0.05;

    srand(time(NULL));

	//******** 1st BUBBLE*********
    v1.x = randfrom(1,8);
	v1.y = randfrom(1,8);

	double d1 = sqrt(pow(pos1.x,2)+pow(pos1.y,2)) + rad;
	if(d1 <= 70)
    {
        cf1 = 1;
    }

	if(pos1.x+rad >= t)
    {
        f1x = 0;
    }
    else if(pos1.x-rad <= -t)
    {
        f1x = 1;
    }
    else if( cf1 && (d1>=70) )
    {
        f1x = 1-f1x;
    }

    if(f1x)
    {
        pos1.x += sc*v1.x;
    }
    else
    {
        pos1.x -= sc*v1.x;
    }

	if(pos1.y+rad >= t)
    {
        f1y = 0;
    }
    else if(pos1.y-rad <= -t)
    {
        f1y = 1;
    }
    else if( cf1 && (d1>=70) )
    {
        f1y = 1-f1y;
    }

    if(f1y)
    {
       pos1.y += sc*v1.y;
    }
    else
    {
        pos1.y -= sc*v1.y;
    }
    //******** 1st BUBBLE*********

    //******** 2nd BUBBLE*********
    v2.x = randfrom(1,8);
	v2.y = randfrom(1,8);

	double d2 = sqrt(pow(pos2.x,2)+pow(pos2.y,2)) + rad;
	if(d2 <= 70)
    {
        cf2 = 1;
    }

	if(pos2.x+rad >= t)
    {
        f2x = 0;
    }
    else if(pos2.x-rad <= -t)
    {
        f2x = 1;
    }
    else if( cf2 && (d2>=70) )
    {
        f2x = 1-f2x;
    }

    if(f2x)
    {
        pos2.x += sc*v2.x;
    }
    else
    {
        pos2.x -= sc*v2.x;
    }

	if(pos2.y+rad >= t)
    {
        f2y = 0;
    }
    else if(pos2.y-rad <= -t)
    {
        f2y = 1;
    }
    else if( cf2 && (d2>=70) )
    {
        f2y = 1-f2y;
    }

    if(f2y)
    {
       pos2.y += sc*v2.y;
    }
    else
    {
        pos2.y -= sc*v2.y;
    }
    //******** 2nd BUBBLE*********

    //******** 3rd BUBBLE*********
    v3.x = randfrom(1,8);
	v3.y = randfrom(1,8);

	double d3 = sqrt(pow(pos3.x,2)+pow(pos3.y,2)) + rad;
	if(d3 <= 70)
    {
        cf3 = 1;
    }

	if(pos3.x+rad >= t)
    {
        f3x = 0;
    }
    else if(pos3.x-rad <= -t)
    {
        f3x = 1;
    }
    else if( cf3 && (d3>=70) )
    {
        f3x = 1-f3x;
    }

    if(f3x)
    {
        pos3.x += sc*v3.x;
    }
    else
    {
        pos3.x -= sc*v3.x;
    }

	if(pos3.y+rad >= t)
    {
        f3y = 0;
    }
    else if(pos3.y-rad <= -t)
    {
        f3y = 1;
    }
    else if( cf3 && (d3>=70) )
    {
        f3y = 1-f3y;
    }

    if(f3y)
    {
       pos3.y += sc*v3.y;
    }
    else
    {
        pos3.y -= sc*v3.y;
    }
    //******** 3rd BUBBLE*********

    //******** 4th BUBBLE*********
    v4.x = randfrom(1,8);
	v4.y = randfrom(1,8);

	double d4 = sqrt(pow(pos4.x,2)+pow(pos4.y,2)) + rad;
	if(d4 <= 70)
    {
        cf4 = 1;
    }

	if(pos4.x+rad >= t)
    {
        f4x = 0;
    }
    else if(pos4.x-rad <= -t)
    {
        f4x = 1;
    }
    else if( cf4 && (d4>=70) )
    {
        f4x = 1-f4x;
    }

    if(f4x)
    {
        pos4.x += sc*v4.x;
    }
    else
    {
        pos4.x -= sc*v4.x;
    }

	if(pos4.y+rad >= t)
    {
        f4y = 0;
    }
    else if(pos4.y-rad <= -t)
    {
        f4y = 1;
    }
    else if( cf4 && (d4>=70) )
    {
        f4y = 1-f4y;
    }

    if(f4y)
    {
       pos4.y += sc*v4.y;
    }
    else
    {
        pos4.y -= sc*v4.y;
    }
    //******** 4th BUBBLE*********

    //******** 5th BUBBLE*********
    v5.x = randfrom(1,8);
	v5.y = randfrom(1,8);

	double d5 = sqrt(pow(pos5.x,2)+pow(pos5.y,2)) + rad;
	if(d5 <= 70)
    {
        cf5 = 1;
    }

	if(pos5.x+rad >= t)
    {
        f5x = 0;
    }
    else if(pos5.x-rad <= -t)
    {
        f5x = 1;
    }
    else if( cf5 && (d5>=70) )
    {
        f5x = 1-f5x;
    }

    if(f5x)
    {
        pos5.x += sc*v5.x;
    }
    else
    {
        pos5.x -= sc*v5.x;
    }

	if(pos5.y+rad >= t)
    {
        f5y = 0;
    }
    else if(pos5.y-rad <= -t)
    {
        f5y = 1;
    }
    else if( cf5 && (d5>=70) )
    {
        f5y = 1-f5y;
    }

    if(f5y)
    {
       pos5.y += sc*v5.y;
    }
    else
    {
        pos5.y -= sc*v5.y;
    }
    //******** 5th BUBBLE*********


	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	rt = 0.175;// for 30 degree
	z_angle = 0;// for q and w
	y_angle = 0;
	x_angle = 0;
	pipe_y_angle = 0;
	pipe_rotate = 0;

	t = 100;
	rad = 12;

	play = 0;

	cf1 = 0;
	cf2 = 0;
	cf3 = 0;
	cf4 = 0;
	cf5 = 0;

	sc = 0.01;
	t_sc = 0;

	//******1st BUBBLE********
	pos1.x = -t+12;
	pos1.y = -t+12;
	pos1.z = 2;

	v1.x = 0;
	v1.y = 0;
	v1.z = 0;

	f1x = 1;
	f1y = 1;
	//******1st BUBBLE********

	//******2nd BUBBLE********
	pos2.x = -t+12;
	pos2.y = -t+12;
	pos2.z = 2;

	v2.x = 0;
	v2.y = 0;
	v2.z = 0;

	f2x = 1;
	f2y = 1;
	//******2nd BUBBLE********

	//******3rd BUBBLE********
	pos3.x = -t+12;
	pos3.y = -t+12;
	pos3.z = 2;

	v3.x = 0;
	v3.y = 0;
	v3.z = 0;

	f3x = 1;
	f3y = 1;
	//******3rd BUBBLE********

	//******4th BUBBLE********
	pos4.x = -t+12;
	pos4.y = -t+12;
	pos4.z = 2;

	v4.x = 0;
	v4.y = 0;
	v4.z = 0;

	f4x = 1;
	f4y = 1;
	//******4th BUBBLE********

	//******5th BUBBLE********
	pos5.x = -t+12;
	pos5.y = -t+12;
	pos5.z = 2;

	v5.x = 0;
	v5.y = 0;
	v5.z = 0;

	f5x = 1;
	f5y = 1;
	//******5th BUBBLE********


	pos.x = 0;
	pos.y = 0;
	pos.z = 100;

	u.x = 0;
	u.y = 0;
	u.z = 1;

	r.x = -1.0/sqrt(2);
	r.y = 1.0/sqrt(2);
	r.z = 0;

	l.x = -1.0/sqrt(2);
	l.y = -1.0/sqrt(2);
	l.z = 0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
