//#ifndef HEADER_H
//#define HEADER_H

#include<bits/stdc++.h>
#include<stdio.h>
#include<iostream>
#include <windows.h>
#include <glut.h>
#include "bitmap_image.hpp"
using namespace std;


#define pi (2*acos(0.0))


struct point
{
	double x,y,z;
};

class Point3D
{
public:
    double x;
    double y;
    double z;
    Point3D()
    {
        x=0;
        y=0;
        z=0;
    }
    Point3D(double a,double b, double c)
    {
        x=a;
        y=b;
        z=c;
    }
    Point3D cross_prod(Point3D p)
    {
        Point3D temp;
        temp.x = (p.z * y) - (p.y * z);
        temp.y = (p.x * z) - (p.z * x);
        temp.z = (p.y * x) - (p.x * y);
        p = temp;
        return p;
    }
    double dot_prod(Point3D p)
    {
        double r;
        r = (p.x *x) + (p.y *y) + (p.z *z);
        return r;
    }

};

class Ray
{
public:
    Point3D start;
    Point3D dir;

    Ray(Point3D p1,Point3D p2)
    {
        start.x = p1.x;
        start.y = p1.y;
        start.z = p1.z;

        dir.x = p2.x;
        dir.y = p2.y;
        dir.z = p2.z;
    }
};

class Object
{
    Point3D reference_point; // should have x, y, z
    double height, width, length;
    double color[3];
    double coEfficients[4]; // reflection coefficients
    int shine; // exponent term of specular component
public:
    Object(){}
    virtual void draw(){}
    virtual double intersect(Ray *r, double *color, int level)
    {
        return -1.0;
    }
    void setPoint3D(Point3D p)
    {
        reference_point.x = p.x;
        reference_point.y = p.y;
        reference_point.z = p.z;
    }
    Point3D getPoint3D()
    {
        return reference_point;
    }
    void setHeight(double h)
    {
        height = h;
    }
    double getHeight()
    {
        return height;
    }
    void setWidth(double w)
    {
        width = w;
    }
    double getWidth()
    {
        return width;
    }
    void setLength(double l)
    {
        length = l;
    }
    double getLength()
    {
        return length;
    }
    void setColor(double a, double b, double c)
    {
        color[0]=a;
        color[1]=b;
        color[2]=c;
    }
    double* getColor()
    {
        return color;
    }
    void setShine(int s)
    {
        shine = s;
    }
    void setCoEfficients(double a, double b, double c, double d)
    {
        coEfficients[0]=a;
        coEfficients[1]=b;
        coEfficients[2]=c;
        coEfficients[3]=d;
    }

    void getAll()
    {
        cout<<"color: "<<color[0]<<" "<<color[1]<<" "<<color[2]<<endl;
        cout<<"Co-efficients: "<<coEfficients[0]<<" "<<coEfficients[1]<<" "<<coEfficients[2]<<" "<<coEfficients[3]<<endl;
        cout<<"Shine: "<<shine<<endl;
    }
};

class Sphere : public Object
{
public:
    Sphere(Point3D p, double r)
    {
        setPoint3D(p);
        setLength(r);// length = radius
    }
    void draw()
    {
        int slices = 96;
        int stacks = 80;
        Point3D p = getPoint3D();
        double radius = getLength();

        glPushMatrix();
        {
            glTranslatef(p.x,p.y,p.z);
            struct point points[100][100];
            int i,j;
            double h,r;

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

            for(i=0;i<stacks;i++)
            {
                for(j=0;j<slices;j++)
                {
                    double* clr = getColor();
                    glColor3f(clr[0],clr[1],clr[2]);
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
        }glPopMatrix();


    }
    double intersect(Ray *r, double *color, int level)
    {
        double a,b,c,d,t;
        double t1,t2;
        double radius = getLength();
        Point3D r0,rd;
        Point3D center = getPoint3D();
        //r0 = r->start;
        Point3D temp;
        temp.x = r->start.x - center.x;
        temp.y = r->start.y - center.y;
        temp.z = r->start.z - center.z;
        r0 = temp;
        rd = r->dir;

        a = 1;
        b = r0.dot_prod(rd) * 2;
        c = r0.dot_prod(r0) - (radius * radius);
        d = (b*b)-(4*a*c);

        if(d<0)
        {
            return -1;
        }
        else
        {
            d = sqrt( (b*b)-(4*a*c) );
            t1 = (-b+d)/(2*a);
            t2 = (-b-d)/(2*a);

            double y = std::min(t1,t2);

            if(y>0)
            {
                return y;
            }
            else
            {
                return std::max(t1,t2);
            }

        }

    }


};

class Triangle : public Object
{
    Point3D a,b,c;
public:
    Triangle(Point3D p1,Point3D p2, Point3D p3)
    {
        a.x = p1.x;
        a.y = p1.y;
        a.z = p1.z;

        b.x = p2.x;
        b.y = p2.y;
        b.z = p2.z;

        c.x = p3.x;
        c.y = p3.y;
        c.z = p3.z;
    }

    void draw()
    {
        double* clr = getColor();
        glColor3f(clr[0],clr[1],clr[2]);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(a.x, a.y, a.z);
			glVertex3f(b.x, b.y, b.z);
			glVertex3f(c.x, c.y, c.z);
        }
        glEnd();
    }
    double intersect(Ray *r, double *color, int level)
    {
        double EPSILON = 0.0000001;
        Point3D edge1, edge2, h, s, q;
        double ap,f,u,v;
        edge1.x = b.x - a.x;
        edge1.y = b.y - a.y;
        edge1.z = b.z - a.z;

        edge2.x = c.x - a.x;
        edge2.y = c.y - a.y;
        edge2.z = c.z - a.z;

        h = r->dir.cross_prod(edge2);
        ap = edge1.dot_prod(h);
        if (ap > -EPSILON && ap < EPSILON)
        {
            return -1;
        }
        f = 1.0/ap;
        //s = rayOrigin - vertex0;
        s.x = r->start.x - a.x;
        s.y = r->start.y - a.y;
        s.z = r->start.z - a.z;
        u = f * (s.dot_prod(h));
        if (u < 0.0 || u > 1.0)
        {
            return -1;
        }
        q = s.cross_prod(edge1);
        v = f * (r->dir.dot_prod(q));
        if (v < 0.0 || (u + v) > 1.0)
        {
            return -1;
        }

        double t = f * (edge2.dot_prod(q));
        if (t > EPSILON)
        {
            return t;
        }
        else
        {
            return -1;
        }

    }
};

class Floor : public Object
{
public:
    double floor_wid;
    double tile_wid;
    Floor(double floorWidth, double tileWidth)
    {
        Point3D p(-floorWidth/2,-floorWidth/2,0);
        setPoint3D(p);
        setLength(tileWidth);
        floor_wid = floorWidth;
    }


    void draw()
    {
        Point3D p = getPoint3D();
        double a = getLength();
        double y_limit = -p.y;
        double x_limit = -p.x;
        int i,j;
        i=0;
        j=0;
        for(double refY=p.y; refY<y_limit; refY = refY+a)
        {
            i++;
            for(double refX=p.x; refX<x_limit; refX = refX+a)
            {
                j++;
                if((i+j)%2 == 1)
                {
                    glColor3f(0,0,0);
                }
                else
                {
                    glColor3f(1,1,1);
                }

                glBegin(GL_QUADS);
                glVertex3f(refX+a, refY, 0);
                glVertex3f(refX, refY, 0);
                glVertex3f(refX, refY+a, 0);
                glVertex3f(refX+a, refY+a, 0);
                glEnd();
            }
        }

    }
    double intersect(Ray *r, double *color, int level)
    {
        Point3D n(0.0,0.0,1.0);
        double numerator, denominator,t;
        numerator = r->start.dot_prod(n);
        denominator = r->dir.dot_prod(n);
        t = (-numerator)/denominator;

        Point3D in_sect;
        in_sect.x = r->start.x + (r->dir.x * t);
        in_sect.y = r->start.y + (r->dir.y * t);
        in_sect.z = r->start.z + (r->dir.z * t);
        Point3D p = getPoint3D();
        double refX = p.x;
        double refY = p.y;
        double a = getLength();
        double limit = floor_wid/2;

        if((in_sect.x<limit && in_sect.x>-limit) && (in_sect.y<limit && in_sect.y>-limit))
        {
            double dis_x, dis_y;
            dis_x = (in_sect.x - refX)/a;
            dis_y = (in_sect.y - refY)/a;

            if(((int)dis_x + (int)dis_y)% 2 == 1)
            {
                setColor(0,0,0);
            }
            else
            {
                setColor(1,1,1);
            }

            return t;
        }

        return -1;
    }

};

class General : public Object
{
public:
    double A,B,C,D,E,F,G,H,I,J;
    General(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j)
    {
        A = a;
        B = b;
        C = c;
        D = d;
        E = e;
        F = f;
        G = g;
        H = h;
        I = i;
        J = j;
    }
    double intersect(Ray *r, double *color, int level)
    {
        double aq,bq,cq;
        aq = (A*pow(r->dir.x, 2))+(B*pow(r->dir.y, 2))+(C*pow(r->dir.z, 2))+(D*r->dir.x*r->dir.y)+(E*r->dir.x*r->dir.z)+(F*r->dir.y*r->dir.z);
        bq = (2*A*r->start.x*r->dir.x)+(2*B*r->start.y*r->dir.y)+(2*C*r->start.z*r->dir.z)+(D*(r->start.x*r->dir.y + r->start.y*r->dir.x))+(E*(r->start.x*r->dir.z + r->start.z*r->dir.x))+(F*(r->start.y*r->dir.z + r->start.z*r->dir.y))+(G*r->dir.x)+(H*r->dir.y)+(I*r->dir.z);
        cq = (A*pow(r->start.x, 2))+(B*pow(r->start.y, 2))+(C*pow(r->start.z, 2))+(D*r->start.x*r->start.y)+(E*r->start.x*r->start.z)+(F*r->start.y*r->start.z)+(G*r->start.x)+(H*r->start.y)+(I*r->start.z)+J;

        double t1,t2;
        t1 =( -bq -sqrt( pow(bq,2) - 4*aq*cq ) )/(2*aq);
        t2 =( -bq +sqrt( pow(bq,2) - 4*aq*cq ) )/(2*aq);

        //double y = std::min(t1,t2);
        double mini,maxim;

        if(t1<t2)
        {
            mini = t1;
            maxim = t2;
        }
        else
        {
            mini = t2;
            maxim = t1;
        }
        double y = check_more(r,mini);
        if(y==-1)
        {
            y = check_more(r,maxim);
            if(y==-1)
            {
                return -1;
            }
            return y;
        }
        return y;
    }
    double check_more(Ray *r, double t)
    {
        Point3D in_sect;
        in_sect.x = r->start.x + (r->dir.x * t);
        in_sect.y = r->start.y + (r->dir.y * t);
        in_sect.z = r->start.z + (r->dir.z * t);

        double l,w,ht;
        l = getLength();
        w = getWidth();
        ht = getHeight();

        if( ht!=0 && (in_sect.z<0 || in_sect.z>ht) )
        {
            return -1;
        }
        if( l!=0 && (in_sect.x<(-l/2) && in_sect.x>(l/2)) )
        {
            return -1;
        }
        if( w!=0 && (in_sect.y<(-w/2) && in_sect.y>(w/2)) )
        {
            return -1;
        }

        return t;
    }

};



//#endif // HEADER_H
