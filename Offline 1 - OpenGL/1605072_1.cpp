#include<stdio.h>
#include<stdlib.h>
#include<math.h>

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



struct point
{
	double x,y,z;
};

point pos;
point u,r,l;
point l1,l2;
point u1,u2;
point r1,r2;

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
    glColor3f(0.7,0.7,0.7);
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


		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
//			cameraHeight -= 3.0;
			pos.x -= 4*l.x;
			pos.y -= 4*l.y;
			pos.z -= 4*l.z;
			break;
		case GLUT_KEY_UP:		// up arrow key
//			cameraHeight += 3.0;
            pos.x += 4*l.x;
			pos.y += 4*l.y;
			pos.z += 4*l.z;
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

	gluLookAt(pos.x, pos.y, pos.z, pos.x+l.x, pos.y+l.y, pos.z+l.z, u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();


    //******* Z-axis Rotation*****
    glRotatef(z_angle,0,0,1);

    glRotatef(90,1,0,0);
    drawSphere1(30,96,80);

    glRotatef(90,1,0,0);
    glRotatef(90,1,0,0);
    //******* Y-axis Rotation*****
    glRotatef(y_angle,1,0,0);
    drawSphere2(30,96,80);


//	// pipe er starting half sphere
	glTranslatef(0,0,42);
	glRotatef(90,1,0,0);
    glRotatef(90,1,0,0);
//	//********** for pipe rotation along Y-axis****
	glRotatef(pipe_y_angle,1,0,0);
	glRotatef(pipe_rotate,0,0,1);
	drawSphere2(12,96,80);
//
	glTranslatef(0,0,-80);
    drawPipe(80,12,96,80);
//
	glTranslatef(0,0,0);
	drawPipeHead(5,12,96,10);
	// pipe shesh


    // plane
    glTranslatef(0,0,-150);
    glRotatef(90,0,0,1);
    glColor3f(1,0,1);
    drawSquare(100);


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}



void animate(){
	angle+=0.05;
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


	pos.x = 100;
	pos.y = 100;
	pos.z = 0;

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
