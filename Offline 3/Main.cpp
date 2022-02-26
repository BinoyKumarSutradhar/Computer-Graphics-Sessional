
#include<bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<fstream>

#include "header.h"
#include "bitmap_image.hpp"

#include <windows.h>
#include <glut.h>
using namespace std;

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


struct point pos;
struct point u,r,l;
struct point l1,l2;
struct point u1,u2;
struct point r1,r2;

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
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


vector<Object *> obj;

void capture()
{
    int windowWidth,windowHeight;
    windowWidth = 500;
    windowHeight = 500;
    double viewAngle;
    viewAngle = (90*pi)/180;
    double planeDistance;
    planeDistance = (windowHeight/2.0) /tan(viewAngle/2.0);
    //cout<<planeDistance<<endl;
    Point3D topleft;
    topleft.x = pos.x + (l.x * planeDistance) - (r.x * windowWidth/2) + (u.x * windowHeight/2);
    topleft.y = pos.y + (l.y * planeDistance) - (r.y * windowWidth/2) + (u.y * windowHeight/2);
    topleft.z = pos.z + (l.z * planeDistance) - (r.z * windowWidth/2) + (u.z * windowHeight/2);

    double imageWidth, imageHeight;
    imageWidth = 768;
    imageHeight = 768;
    bitmap_image image(imageWidth, imageHeight);

    double du,dv;
    du = windowWidth/imageWidth;
    dv = windowHeight/imageHeight;
    topleft.x = topleft.x + r.x *(0.5*du) - u.x *(0.5*dv);
    topleft.y = topleft.y + r.y *(0.5*du) - u.y *(0.5*dv);
    topleft.z = topleft.z + r.z *(0.5*du) - u.z *(0.5*dv);

    int nearest;
    double t, tMin;
    for(int i=0; i<imageWidth; i++)
    {
        for(int j=0; j<imageHeight; j++)
        {
            Point3D curPixel;
            curPixel.x = topleft.x - u.x*i*dv + r.x*j*du;
            curPixel.y = topleft.y - u.y*i*dv + r.y*j*du;
            curPixel.z = topleft.z - u.z*i*dv + r.z*j*du;

            Point3D temp;
            temp.x = curPixel.x - pos.x;
            temp.y = curPixel.y - pos.y;
            temp.z = curPixel.z - pos.z;
            double a = sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
            temp.x = temp.x/a;
            temp.y = temp.y/a;
            temp.z = temp.z/a;
            Point3D dir = temp;

            Point3D strt(pos.x,pos.y,pos.z);

            nearest = -1;
            tMin = 99999;

            Ray * ray = new Ray(strt,dir);
            double* dummyColor;
            dummyColor = new double[3];
            int cnt=0;

            for(int k = 0; k < obj.size(); k++)
            {
                t = obj[k]->intersect(ray,dummyColor,0);
                if(t>0 && t<tMin)
                {
                    tMin =t;
                    nearest =k;

                }

                if(t<0)
                {
                    continue;
                }
            }

            if(nearest != -1)
            {
                double* cl = obj[nearest]->getColor();
                image.set_pixel(j,i,cl[0]*255, cl[1]*255, cl[2]*255);
                //cout<<j<<"  "<<i<<endl;
            }

        }
    }

    image.save_image("output.bmp");
    cout<<"*****Done!!!!****"<<endl;

}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '0':
            capture();
            break;
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

    for(int k = 0; k < obj.size(); k++)
    {
        obj[k]->draw();
    }

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


void load()
{
    ifstream infile;
	infile.open("scene.txt");
	Object *s;

	double recursion,pixel,obj_cnt;
	string command;
	double c_r, c_g, c_b, am_coff, dif_coff, spec_coff, rec_coff, shine;

    infile>>recursion>>pixel>>obj_cnt;
	//infile>>command;
	while (infile>>command)
    {
        if(command == "sphere")
        {
            double centerX, centerY, centerZ, radius;
            infile>>centerX>>centerY>>centerZ;
            Point3D p(centerX,centerY,centerZ);

            infile>>radius;

            s = new Sphere(p, radius);

            infile>>c_r>>c_g>>c_b;
            s->setColor(c_r,c_g,c_b);

            infile>>am_coff>>dif_coff>>spec_coff>>rec_coff;
            s->setCoEfficients(am_coff,dif_coff,spec_coff,rec_coff);

            infile>>shine;
            s->setShine(shine);
            //s->draw();
            obj.push_back(s);
        }

        else if(command == "triangle")
        {
            double ax,ay,az,bx,by,bz,cx,cy,cz;
            infile>>ax>>ay>>az;
            infile>>bx>>by>>bz;
            infile>>cx>>cy>>cz;
            Point3D p1(ax,ay,az);
            Point3D p2(bx,by,bz);
            Point3D p3(cx,cy,cz);

            s = new Triangle(p1,p2,p3);

            infile>>c_r>>c_g>>c_b;
            s->setColor(c_r,c_g,c_b);

            infile>>am_coff>>dif_coff>>spec_coff>>rec_coff;
            s->setCoEfficients(am_coff,dif_coff,spec_coff,rec_coff);

            infile>>shine;
            s->setShine(shine);
            //s->draw();
            obj.push_back(s);
        }
        else if(command == "general")
        {
            double a,b,c,d,e,f,g,h,i,j;
            infile>>a>>b>>c>>d>>e>>f>>g>>h>>i>>j;
            s = new General(a,b,c,d,e,f,g,h,i,j);

            double px,py,pz,l,w,ht;
            infile>>px>>py>>pz>>l>>w>>ht;
            Point3D p(px,py,pz);
            s->setPoint3D(p);
            s->setLength(l);
            s->setWidth(w);
            s->setHeight(ht);

            infile>>c_r>>c_g>>c_b;
            s->setColor(c_r,c_g,c_b);

            infile>>am_coff>>dif_coff>>spec_coff>>rec_coff;
            s->setCoEfficients(am_coff,dif_coff,spec_coff,rec_coff);

            infile>>shine;
            s->setShine(shine);
            //s->draw();
            obj.push_back(s);
        }

    }

    Object *temp = new Floor(1000,20);
    obj.push_back(temp);
}


int main(int argc, char **argv){

    load();
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
