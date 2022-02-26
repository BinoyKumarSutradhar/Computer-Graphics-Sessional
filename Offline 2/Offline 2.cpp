#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
#include <typeinfo>
#include <cmath>
#include<cmath>
#include <ctime>
#include "bitmap_image.hpp"
using namespace std;

//#define pi 3.141592654

#define pi 3.14159274101257324219

ofstream out("stage1.txt");
ofstream out_2("stage2.txt");
ofstream out_3("stage3.txt");
ofstream out_4("z_buffer.txt");


double smallest(double x, double y, double z){
    return std::min(std::min(x, y), z);
}

double largest(double x, double y, double z){
    return std::max(std::max(x, y), z);
}


class Point
{
	double x,y,z;

	public:
    Point()
    {
        x = 0;
	    y = 0;
	    z = 0;
    }
	Point(double argx, double argy, double argz)
	{
	    x = argx;
	    y = argy;
	    z = argz;
	}
    void setX(double argx)
    {
        x=argx;//Complete this function///
    }

    void setY(double argy)
    {
        y = argy;
    }

    void setZ(double argz)
    {
        z = argz;
    }

    double getX()
    {
        return x;
    }

    double getY()
    {
        return y;//Complete this function
    }

    double getZ()
    {
        return z;//Complete this function
    }
};

Point small_point(Point a, Point b)
{
    //return std::min(std::min(x, y), z);
    if(a.getX()<b.getX())
    {
        return a;
    }
    else
        return b;
}

Point large_point(Point a, Point b)
{
    if(a.getX()>b.getX())
    {
        return a;
    }
    else
        return b;
}

double dist(Point a, Point b)
{
    double x = pow(a.getX()-b.getX(), 2);
    double y = pow(a.getY()-b.getY(), 2);
    double z = pow(a.getZ()-b.getZ(), 2);
    double n = sqrt(x+y+z);
    return n;
}


class Triangle
{

    public:
    Point points[3];
    int color[3];

    //public:
    int limit;
    Triangle()
    {
        limit =0;
        color[0] = rand() %256;
        color[1] = rand() %256;
        color[2] = rand() %256;
    }
    Triangle(Point p1,Point p2,Point p3)
    {
        points[0]=p1;
        points[1]=p2;
        points[2]=p3;
        color[0] = rand() %256;
        color[1] = rand() %256;
        color[2] = rand() %256;
    }

    void setPoint(Point p)
    {
        if(limit==3)
        {
            return;
        }
        points[limit]=p;
        limit++;
    }

    void getColor()
    {
        for(int i=0;i<3;i++)
        {
            cout<<color[i]<<" ";
        }
        cout<<endl;
    }
    void getPoints()
    {
        for(int i=0;i<3;i++)
        {
            cout<<points[i].getX()<<" "<<points[i].getY()<<" "<<points[i].getZ()<<endl;
        }
    }

};



int print_triangle(int y,double ** A , double ** B)
{
    double num;
    double ** C;
    C = new double*[4];
    for(int i=0;i<4;i++)
    {
        C[i] = new double[1];
    }

    for (int i=0; i<4; i++)
    {
        for(int j=0; j<1; j++)
        {
            num = 0;
            for(int k=0; k<4; k++)
            {
                num += A[i][k]*B[k][j];
            }
            C[i][j]=num;
        }
    }

    while(C[3][0] != 1.0)
    {
        double h = C[3][0];
        for(int m=0;m<4;m++)
        {
            C[m][0] = C[m][0]/h;
        }
    }

    if(y==1)
    {
        for(int m=0;m<3;m++)
            out << fixed << setprecision(7) << C[m][0] <<" ";
        out<<endl;
    }
    else if(y==2)
    {
        for(int m=0;m<3;m++)
            out_2 << fixed << setprecision(7) << C[m][0] <<" ";
        out_2<<endl;
    }
    else if(y==3)
    {
        for(int m=0;m<3;m++)
            out_3 << fixed << setprecision(7) << C[m][0] <<" ";
        out_3<<endl;
    }


}



int main()
{
    srand(time(NULL));
    stack<double **> stack;
    vector<int> g1;
    double ** I ;
	I = new double*[4];
    for(int i=0;i<4;i++)
    {
        I[i] = new double[4];
    }
    for (int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(i==j)
            {
                I[i][j]= 1.0;
            }
            else
            {
                I[i][j]= 0.0;
            }
        }
    }

	ifstream infile;
	infile.open("scene.txt");

	string command;
	double eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ, fovY, aspectRatio, near, far;

    infile>>eyeX>>eyeY>>eyeZ;
	infile>>lookX>>lookY>>lookZ;
	infile>>upX>>upY>>upZ;
	infile>>fovY>>aspectRatio>>near>>far;

	int cnt;
	cnt=0;
	int push_cnt;
	push_cnt = 0;
	int t_cnt[1000];
	t_cnt[push_cnt]=0;


	stack.push(I);

	while(true)
    {
        double ** A;
        double ** B;// 4x1 matrix
        double ** C;
        double ** D;
        A = new double*[4];
        B = new double*[4];
        C = new double*[4];
        D = new double*[4];
        for(int i=0;i<4;i++)
        {
            A[i] = new double[4];
            B[i] = new double[1];
            C[i] = new double[4];
            D[i] = new double[4];
        }

        infile>>command;

        if(command == "triangle")
        {
            cnt++;
            A=stack.top();

            for(int m=0;m<3;m++)
            {
                infile>>B[0][0]>>B[1][0]>>B[2][0];
                B[3][0] = 1;
                print_triangle(1,A,B);
            }
            out<<endl;

            continue;
        }
        else if(command == "translate")
        {
            //break;
            A=stack.top();
            double tx,ty,tz;
            infile>>tx>>ty>>tz;
            for (int i=0; i<4; i++)
            {
                for(int j=0; j<4; j++)
                {
                    if(i==j)
                    {
                        D[i][j]= 1.0;
                    }
                    else
                    {
                        D[i][j]= 0.0;
                    }
                }
            }
            //D = I;
            D[0][3] = tx;
            D[1][3] = ty;
            D[2][3] = tz;

            double num;
            for (int i=0; i<4; i++)
            {
                for(int j=0; j<4; j++)
                {
                    num = 0;
                    for(int k=0; k<4; k++)
                    {
                        num += A[i][k]*D[k][j];
                    }
                    C[i][j]=num;
                }
            }
            stack.push(C);
            t_cnt[push_cnt]++;
            continue;
        }
        else if(command == "push")
        {
            int x= stack.size();
            g1.push_back(x);
            push_cnt++;
            t_cnt[push_cnt]=0;
            A=stack.top();

            continue;
        }
        else if(command == "scale")
        {
            A=stack.top();
            double sx,sy,sz;
            infile>>sx>>sy>>sz;
            for (int i=0; i<4; i++)
            {
                for(int j=0; j<4; j++)
                {
                    if(i==j)
                    {
                        D[i][j]= 1.0;
                    }
                    else
                    {
                        D[i][j]= 0.0;
                    }
                }
            }
            //D = I;
            D[0][0] = sx;
            D[1][1] = sy;
            D[2][2] = sz;

            double num;
            for (int i=0; i<4; i++)
            {
                for(int j=0; j<4; j++)
                {
                    num = 0;
                    for(int k=0; k<4; k++)
                    {
                        num += A[i][k]*D[k][j];
                    }
                    C[i][j]=num;
                }
            }
            stack.push(C);
            t_cnt[push_cnt]++;
            continue;

        }
        else if(command == "rotate")
        {
            A=stack.top();

            //}
            double ax, ay,az,a,angle,theta;
            infile>>angle>>ax>>ay>>az;


            a = sqrt((ax*ax)+(ay*ay)+(az*az)); // NORMALIZE
            ax = ax/a;
            ay = ay/a;
            az = az/a;
            //theta = (angle * pi)/180.0;
            theta = (angle * M_PI)/180;
            //cout<<"deg:"<<angle<<"\nrad:"<<theta<<endl<<endl;
            double cosine = cos(theta);
            double sine = sin(theta);
            if(angle ==90 || angle == -90)
            {
                cosine = 0.0;
            }
            //cout<<"cos:"<<cosine<<"\nsin:"<<sine<<endl<<endl;

            for (int i=0; i<4; i++)
            {
                for(int j=0; j<4; j++)
                {
//
                    D[i][j]= 0.0;
                }
            }

            D[3][3] = 1.0;

            D[0][0] = cosine + ((1-cosine)*ax*ax);
            D[1][0] = ((1-cosine)*ax*ay) + (sine*az);
            D[2][0] = ((1-cosine)*ax*az) - (sine*ay);

            D[0][1] = ((1-cosine)*ax*ay) - (sine*az);
            D[1][1] = cosine + ((1-cosine)*ay*ay);
            D[2][1] = ((1-cosine)*ay*az) + (sine*ax);

            D[0][2] = ((1-cosine)*ax*az) + (sine*ay);
            D[1][2] = ((1-cosine)*ay*az) - (sine*ax);
            D[2][2] = cosine + ((1-cosine)*az*az);

            //int num;
            double num;
            for (int i=0; i<4; i++)
            {
                for(int j=0; j<4; j++)
                {
                    num = 0;
                    for(int k=0; k<4; k++)
                    {
                        num += A[i][k]*D[k][j];
                    }
                    C[i][j]=num;
                }
            }
            stack.push(C);
            t_cnt[push_cnt]++;
            A = stack.top();

            continue;
            //break;
        }

        else if(command=="pop")
        {
            //int m = g1.at(g1.size()-1);
//            int m = g1.back();
//            g1.pop_back();
//            for(int i=stack.size();i>m;i--)
//            {
//                stack.pop();
//            }
//            while(stack.size()!= m)
//            {
//                stack.pop();
//            }
            //break;
            while(t_cnt[push_cnt]!=0)
            {
                stack.pop();
                t_cnt[push_cnt]--;
            }
            push_cnt--;
            A=stack.top();

            continue;
        }
        else if(command == "end")
        {
            break;
        }

    }// END OF LOOP

    out.close();


    //**********STAGE_2****************

    int line_cnt = cnt*3;
    ifstream infile2;
	infile2.open("stage1.txt");

	double ** V;
    double ** R;// 4x1 matrix
    double ** T;
    double ** W;
    V = new double*[4];
    R = new double*[4];
    T = new double*[4];
    W = new double*[4];
    for(int i=0;i<4;i++)
    {
        V[i] = new double[4];
        R[i] = new double[4];
        T[i] = new double[4];
        W[i] = new double[1];
    }

    double lx,ly,lz,rx,ry,rz,ux,uy,uz;
    double u,r,l;

    lx = lookX - eyeX;
    ly = lookY - eyeY;
    lz = lookZ - eyeZ;

    l = sqrt((lx*lx)+(ly*ly)+(lz*lz));
    lx = lx/l;
    ly = ly/l;
    lz = lz/l;

    rx = (ly*upZ) - (lz*upY);
    ry = (lz*upX) - (lx*upZ);
    rz = (lx*upY) - (ly*upX);

    r = sqrt((rx*rx)+(ry*ry)+(rz*rz));
    rx = rx/r;
    ry = ry/r;
    rz = rz/r;

    ux = (ry*lz) - (rz*ly);
    uy = (rz*lx) - (rx*lz);
    uz = (rx*ly) - (ry*lx);

    u = sqrt((ux*ux)+(uy*uy)+(uz*uz));
    ux = ux/u;
    uy = uy/u;
    uz = uz/u;

    for (int i=0; i<4; i++)// for V
    {
        for(int j=0; j<4; j++)
        {
            if(i==j)
            {
                V[i][j]= 1.0;
            }
            else
            {
                V[i][j]= 0.0;
            }
        }
    }

    for (int i=0; i<4; i++)// for R
    {
        for(int j=0; j<4; j++)
        {
            if(i==j)
            {
                R[i][j]= 1.0;
            }
            else
            {
                R[i][j]= 0.0;
            }
        }
    }

    for (int i=0; i<4; i++)// for T
    {
        for(int j=0; j<4; j++)
        {
            if(i==j)
            {
                T[i][j]= 1.0;
            }
            else
            {
                T[i][j]= 0.0;
            }
        }
    }

    R[0][0] = rx;
    R[0][1] = ry;
    R[0][2] = rz;

    R[1][0] = ux;
    R[1][1] = uy;
    R[1][2] = uz;

    R[2][0] = -lx;
    R[2][1] = -ly;
    R[2][2] = -lz;

    T[0][3] = -eyeX;
    T[1][3] = -eyeY;
    T[2][3] = -eyeZ;


    int num;
    for (int i=0; i<4; i++)  // V=RT
    {
        for(int j=0; j<4; j++)
        {
            num = 0;
            for(int k=0; k<4; k++)
            {
                num += R[i][k]*T[k][j];
            }
            V[i][j]=num;
        }
    }

    for(int m=1;m<=line_cnt;m++)
    {
        infile2>>W[0][0]>>W[1][0]>>W[2][0];
        W[3][0] = 1;
        print_triangle(2,V,W);
        if(m%3 == 0)
        {
           out_2<<endl;
        }
    }

    out_2.close();

    //*******STAGE 3****************

    ifstream infile3;
	infile3.open("stage2.txt");

	double t,r1,fovX;
	fovX = fovY * aspectRatio;
	fovX = ((fovX/2) * pi)/180.0;
	fovY = ((fovY/2) * pi)/180.0;
	t = near * tan(fovY);
	r1 = near * tan(fovX);

    double ** P;
    double ** Z;
    P = new double*[4];
    Z = new double*[4];
    for(int i=0;i<4;i++)
    {
        P[i] = new double[4];
        Z[i] = new double[1];
    }
    for (int i=0; i<4; i++)// for P
    {
        for(int j=0; j<4; j++)
        {
            if(i==j)
            {
                P[i][j]= 1.0;
            }
            else
            {
                P[i][j]= 0.0;
            }
        }
    }

    P[0][0] = near/r1;
    P[1][1] = near/t;
    P[2][2] = -(far+near)/(far-near);
    P[3][3] = 0;
    P[2][3] = -(2*far*near)/(far-near);
    P[3][2] = -1;

    for(int m=1;m<=line_cnt;m++)
    {
        infile3>>Z[0][0]>>Z[1][0]>>Z[2][0];
        Z[3][0] = 1;
        print_triangle(3,P,Z);
        if(m%3 == 0)
        {
           out_3<<endl;
        }
    }

    out_3.close();

    //*******STAGE 4 ****************

    ifstream infile4;
	infile4.open("config.txt");

	double screen_Width, screen_Height, left_lim_X, bott_lim_Y, front_lim, rear_lim;
	double right_lim_X, top_lim_Y, Top_Y, Left_X ;
	double dx,dy;

	infile4>>screen_Width>>screen_Height;
	infile4>>left_lim_X;
	infile4>>bott_lim_Y;
	infile4>>front_lim>>rear_lim;

	right_lim_X = -left_lim_X;
	top_lim_Y = -bott_lim_Y;
	dx = (right_lim_X - left_lim_X)/screen_Width;
	dy = (top_lim_Y - bott_lim_Y)/screen_Height;
	Top_Y = top_lim_Y - (dy/2);
	Left_X = left_lim_X + (dx/2);


	//cout<<screen_Width<<" "<<screen_Height<<" "<<left_lim_X<<" "<<bott_lim_Y<<" "<<front_lim<<" "<<rear_lim;
    ifstream infile5;
	infile5.open("stage3.txt");

	vector<Triangle> tv;

	for(int i=0;i<cnt;i++)
    {
        Triangle t;
        for(int j=0;j<3;j++)
        {
            double a,b,c;
            infile5>>a>>b>>c;
            Point p(a,b,c);
            t.setPoint(p);
        }
        tv.push_back(t);
    }

    // ************FOR DEBUGGING**************
//    for (int i = 0; i <tv.size(); i++)
//    {
//        cout<<"--------------------------------------"<<endl;
//        cout<<"("<<i+1<<")"<<endl;
//        cout<<"colors:"<<endl;
//        tv.at(i).getColor();
//        cout<<"\npoints:"<<endl;
//        tv.at(i).getPoints();
//    }
    double ** z_buffer;
    int ** R1;// 4x1 matrix
    int ** G1;
    int ** B1;
    z_buffer = new double*[500];
    R1 = new int*[500];
    G1 = new int*[500];
    B1 = new int*[500];
    for(int i=0;i<500;i++)
    {
        z_buffer[i] = new double[500];
        R1[i] = new int[500];
        G1[i] = new int[500];
        B1[i] = new int[500];
    }
    for (int i=0; i<500; i++)
    {
        for(int j=0; j<500; j++)
        {
            z_buffer[i][j] = rear_lim;
            R1[i][j] = 0;
            G1[i][j] = 0;
            B1[i][j] = 0;
        }
    }

    for (int i = 0; i <tv.size(); i++)
    {

        Triangle t = tv.at(i);
        double y_max,y_min;
        Point a,b,c;
        y_max = largest(t.points[0].getY(),t.points[1].getY(),t.points[2].getY());
        y_min = smallest(t.points[0].getY(),t.points[1].getY(),t.points[2].getY());
        a=t.points[0];
        b=t.points[1];
        c=t.points[2];

        int top_scan, bott_scan;
        top_scan=round((top_lim_Y - y_max)/dy);
        bott_scan=round((top_lim_Y - y_min)/dy);
        if(top_scan<0)
        {
            top_scan=0;
        }
        if(bott_scan>screen_Height-1)
        {
            bott_scan = screen_Height-1;
        }
        //cout<<top_scan<<" "<<bott_scan<<endl;

        for(int j=top_scan;j<=bott_scan;j++)
        {
            //cout<<j<<endl;
            double yy = Top_Y-(j*dy);
            Point dd,ee,ff;
            dd.setY(yy);
            ee.setY(yy);
            ff.setY(yy);

            dd.setX( (((yy-a.getY())/(b.getY()-a.getY()))*(b.getX()-a.getX())) + a.getX());
            ee.setX( (((yy-b.getY())/(c.getY()-b.getY()))*(c.getX()-b.getX())) + b.getX());
            ff.setX( (((yy-c.getY())/(a.getY()-c.getY()))*(a.getX()-c.getX())) + c.getX());

            dd.setZ( (((yy-a.getY())/(b.getY()-a.getY()))*(b.getZ()-a.getZ())) + a.getZ());
            ee.setZ( (((yy-b.getY())/(c.getY()-b.getY()))*(c.getZ()-b.getZ())) + b.getZ());
            ff.setZ( (((yy-c.getY())/(a.getY()-c.getY()))*(a.getZ()-c.getZ())) + c.getZ());

            vector<Point> sa;

            if( dist(a,dd)+ dist(dd,b)- dist(a,b) <0.001 )
            {
                sa.push_back(dd);
            }
            if( dist(b,ee)+ dist(ee,c)-dist(b,c) <0.001 )
            {
                sa.push_back(ee);
            }
            if( dist(c,ff)+dist(ff,a)-dist(c,a) <0.001 )
            {
                sa.push_back(ff);
            }

             int col_start,col_finish;
             if(sa.size() < 2)
             {
                 continue;
             }
             Point small = small_point(sa.at(0),sa.at(1));
             Point large = large_point(sa.at(0),sa.at(1));

             col_start = round((small.getX() - left_lim_X)/dx);
             col_start = max(0,col_start);

             col_finish = round( (large.getX() - left_lim_X)/dx );
             int width = (int)screen_Width;
             col_finish= min(width-1,col_finish);

             double dz = (large.getZ() - small.getZ())/(col_finish-col_start);

             for(int k=col_start;k<=col_finish;k++)
             {
                double buff = small.getZ()+(dz*(k-col_start));
                //cout<<buff<<endl;
                if(z_buffer[j][k] > buff )
                {
                    z_buffer[j][k] = buff;
                    R1[j][k] = t.color[0];
                    G1[j][k] = t.color[1];
                    B1[j][k] = t.color[2];
                }
             }
        }

    }

    for (int i=0; i<500; i++)
    {
        for(int j=0; j<500; j++)
        {
            if(z_buffer[i][j] != 2.000)
            {
                out_4 << fixed << setprecision(7) << z_buffer[i][j] <<"\t";
            }

        }
        out_4 <<endl;
    }

    out_4.close();

    bitmap_image image(screen_Width,screen_Height);

    for(int i=0;i<screen_Width;i++){
        for(int j=0;j<screen_Height;j++){
            image.set_pixel(i,j,R1[j][i],G1[j][i],B1[j][i]);
        }

    }

    image.save_image("out.bmp");;

    image.clear();

    //delete()

    for (int i = 0; i < 4; i++)
    {
        free(z_buffer[i]);
        free(R1[i]);
        free(G1[i]);
        free(B1[i]);
    }
    //free(matrix);

    free(z_buffer);
    free(R1);
    free(G1);
    free(B1);


	return 0;
}

