/************************************************************************/
/*    Tracking eBugs' locations and ID based on deBruijn Sequences      */
/*                Originally Written by Tony Grubman                    */
/*          Modified by Erwin Mochtar Wijaya on 05/02/2014              */
/************************************************************************/

#ifndef EBUGTRACKING_H
#define EBUGTRACKING_H

#include<iostream>
#include<fstream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include<array>
#include<queue>
#include<bitset>

#include<Eigen/Dense>
#include<cmath>

#include <QtCore>

#define WIDTH 1280
#define HEIGHT 960
#define FOCAL_WIDTH (1*WIDTH)
#define MAX_BLOBS 256

#define SCALE 0.5f

using namespace cv;
using namespace std;

cv::Mat image;

typedef u_int8_t uint8;

struct
{
    float x,y,size;
    uint8_t colour;
} points[MAX_BLOBS];

struct myEllipse
{
    float x,y,rx,ry,t;
};

struct eBug
{
    int ID;
    QPointF centre,direction,radius;
};


const cv::Scalar colours[]={cv::Scalar(0,0,255),cv::Scalar(0,128,0),cv::Scalar(255,0,0),cv::Scalar(0)};

Eigen::Vector3f eig(Eigen::Matrix3f &M)
{
    using namespace Eigen;

    float q=M.trace()/3;
    Matrix3f A=M-q*Matrix3f::Identity();
    float det=A.determinant();

    Matrix3f B=A*A;
    float p2=B.trace()/6;
    float p=sqrt(p2);
    det/=(p*p2);

    #if 0
    float z=sqrt(det+2);
    z=z/2+1;
    if(det>-1) z=(z/2+.99985)*(1.0145-.00725*det);
    else z=z/1.75+0.921620629618769-0.0393396851906154*det;
    #else
    float z=2*cos(acos(det/2)/3);
    #endif

    float e0=q+p*z;
    z/=2;
    float e1=q+p*(sqrt(3-3*z*z)-z);

    Matrix3f E0=M-e0*Matrix3f::Identity();
    Matrix3f E1=M-e1*Matrix3f::Identity();

    return (E0*E1).col(0);
}

myEllipse fitEllipse(std::vector<uint8> &component)
{
    using namespace Eigen;

    float xoffset,yoffset;
    Vector3f vx,vy;
    {
        uint8 n;
        Matrix<float,32,1> x,y,size;
        float xrange,yrange;
        {
            n=component.size();
            if(n>32) n=32;
            for(uint8 i=0;i<n;i++)
            {
                x[i]=points[component[i]].x;
                y[i]=points[component[i]].y;
                size[i]=points[component[i]].size;
            }
            for(uint8 i=n;i<32;i++) x[i]=y[i]=size[i]=0;

            float xmin=x.segment(0,n).minCoeff();
            float ymin=y.segment(0,n).minCoeff();
            float xmax=x.segment(0,n).maxCoeff();
            float ymax=y.segment(0,n).maxCoeff();

            xoffset=(xmax+xmin)/2;
            yoffset=(ymax+ymin)/2;
            xrange=(xmax-xmin)/2;
            yrange=(ymax-ymin)/2;

            for(uint8 i=0;i<n;i++)
            {
                x[i]-=xoffset;
                x[i]/=xrange;
                y[i]-=yoffset;
                y[i]/=yrange;
            }
        }

        Matrix3f A,B,C,D;

        {
            Matrix<float,32,1> xx=x.array()*x.array(),xy=x.array()*y.array(),yy=y.array()*y.array();
            Matrix<float,32,1> xxs=xx.array()*size.array(),xys=xy.array()*size.array(),yys=yy.array()*size.array();
            A<<xx.dot(xxs),xx.dot(xys),xx.dot(yys),
               xx.dot(xys),xy.dot(xys),xy.dot(yys),
               xx.dot(yys),xy.dot(yys),yy.dot(yys);
            C<<  xxs.sum(),  xys.sum(),x.dot(size),
                 xys.sum(),  yys.sum(),y.dot(size),
               x.dot(size),y.dot(size), size.sum();
            B<<x.dot(xxs),x.dot(xys),x.dot(yys),
               x.dot(xys),x.dot(yys),y.dot(yys),
                   C(0,0),    C(0,1),    C(1,1);
            D<<   0,   0,-0.5,
                  0,   1,   0,
               -0.5,   0,   0;
        }

        C=-C.inverse()*B;
        D*=(A+B.transpose()*C);

        vx=eig(D);
        vy=C*vx;

        vx[0]*=yrange*yrange;
        vx[1]*=xrange*yrange;
        vx[2]*=xrange*xrange;
        vy[0]*=xrange*yrange*yrange;
        vy[1]*=xrange*xrange*yrange;
        vy[2]*=xrange*xrange*yrange*yrange;
    }

    float t=0.5*atan2(vx[1],vx[0]-vx[2]);
    float cost=cos(t);
    float sint=sin(t);

    float cos_squared=cost*cost,cos_sin=cost*sint,sin_squared=sint*sint;
    float Au= vy[0]*cost+vy[1]*sint;
    float Av=-vy[0]*sint+vy[1]*cost;
    float Auu=vx[0]*cos_squared+vx[1]*cos_sin+vx[2]*sin_squared;
    float Avv=vx[0]*sin_squared-vx[1]*cos_sin+vx[2]*cos_squared;

    float tuCentre=-Au/(2*Auu);
    float tvCentre=-Av/(2*Avv);
    float wCentre=vy[2]-Auu*tuCentre*tuCentre-Avv*tvCentre*tvCentre;
    float uCentre=tuCentre*cost-tvCentre*sint;
    float vCentre=tuCentre*sint+tvCentre*cost;

    float Ru=sqrt(fabs(wCentre/Auu));
    float Rv=sqrt(fabs(wCentre/Avv));

    uCentre+=xoffset;
    vCentre+=yoffset;

    if(t<0) t+=4*atan(1);

    return {uCentre,vCentre,Ru,Rv,t};
}

const float pi=4*atan(1);

inline float ellipse_to_circle(float phi,float rz) //projects ellipse angle onto 3D circle
{
    float K=rz*cos(phi);
    return 2*atan2(sqrt(1-K*K)-cos(phi),K+sin(phi));
}

inline float circle_to_ellipse(float theta,float rz) //projects circle angle onto ellipse in image
{
    return atan2(rz+sin(theta),cos(theta));
}

inline float min_rounding(std::vector<float> &angles) //minimise rounding when selecting 16 evenly placed points on circle
{
    float angle=0,min=INFINITY;
    for(auto j:angles)
    {
        float r=0;
        for(auto k:angles)
        {
            float x=(k-j)*8/pi+16;
            x-=round(x);
            r+=x*x;
        }
        if(r<min)
        {
            min=r;
            angle=j;
        }
    }
    return angle;
}

/* 
BBGRRBRBGBBGBBRG
RGRRRGRBGBRGBGRG
BRGGGGRRRBBBBBGR
RBRGRBBRBBBRGRGB
GGBGBRBBGRGBBBGG
RGBRGGBBBRRGBGGR
GRRBBRGGRBBBGBRR
BGGRBRBBRRRBGBGR
GBRRBBGGBRBGRRGG
RGGRGBGBBBBRBGGG
RGBRRRRRGBBRBRRB
RRGRGGGRBGRGGBRG
RGRBRRRGGBGGBBGG
GGGBBRRBGRBBGBGB
RBGGBGRRRRBRRGGR

0 = R, 1 = G, 2 = B
*/

using std::array;
const array<array<uint8,16>,15> seqs{{
{{2,2,1,0,0,2,0,2,1,2,2,1,2,2,0,1}},
{{0,1,0,0,0,1,0,2,1,2,0,1,2,1,0,1}},
{{2,0,1,1,1,1,0,0,0,2,2,2,2,2,1,0}},
{{0,2,0,1,0,2,2,0,2,2,2,0,1,0,1,2}},
{{1,1,2,1,2,0,2,2,1,0,1,2,2,2,1,1}},
{{0,1,2,0,1,1,2,2,2,0,0,1,2,1,1,0}},
{{1,0,0,2,2,0,1,1,0,2,2,2,1,2,0,0}},
{{2,1,1,0,2,0,2,2,0,0,0,2,1,2,1,0}},
{{1,2,0,0,2,2,1,1,2,0,2,1,0,0,1,1}},
{{0,1,1,0,1,2,1,2,2,2,2,0,2,1,1,1}},
{{0,1,2,0,0,0,0,0,1,2,2,0,2,0,0,2}},
{{0,0,1,0,1,1,1,0,2,1,0,1,1,2,0,1}},
{{0,1,0,2,0,0,0,1,1,2,1,1,2,2,1,1}},
{{1,1,1,2,2,0,0,2,1,0,2,2,1,2,1,2}},
{{0,2,1,1,2,1,0,0,0,0,2,0,0,1,1,0}},
}};

void identify(std::vector<uint8> leds,std::vector<eBug> &eBugsInfo,int &count)
{
    myEllipse e=fitEllipse(leds); //fit ellipse to all leds in component

    float s=sin(e.t);
    float c=cos(e.t);
    vector<uint8> good;
    for(auto i:leds)
    {
        float x0=points[i].x-e.x;
        float y0=points[i].y-e.y;
        float x=( c*x0+s*y0)/e.rx;
        float y=(-s*x0+c*y0)/e.ry;
        float r2=x*x+y*y;
        if(r2>0.9 and r2<1.1) good.push_back(i); //select only points that fit the ellipse well
    }

    if(good.size()<5) return;
    e=fitEllipse(good); //refit ellipse to only good points

    Point2f centre(WIDTH-e.x,e.y);

    s=sin(e.t);
    c=cos(e.t);

    float rz=max(e.rx,e.ry)/FOCAL_WIDTH;
    vector<float> angles(good.size());
    for(uint8 i=0;i<good.size();i++)
    {
        float x0=points[good[i]].x-e.x;
        float y0=points[good[i]].y-e.y;
        float x=( c*x0+s*y0)/e.rx;
        float y=(-s*x0+c*y0)/e.ry;
        angles[i]=ellipse_to_circle(-atan2(y,x)-pi/2,rz);
    }
    float base_angle=min_rounding(angles);

    array<uint8,16> rounded;
    fill(rounded.begin(),rounded.end(),3);
    array<float,16> size{};
    for(uint8 i=0;i<good.size();i++)
    {
        uint8 z=round((angles[i]-base_angle)*8/pi+16);
        z%=16;
        auto &p=points[good[i]];
        if(size[z]<p.size) size[z]=p.size,rounded[z]=p.colour;
    }

#if 0
    for(int i=0;i<rounded.size();i++)
    {
        float phi=circle_to_ellipse(i*pi/8+base_angle,rz)+pi/2;
        float x=cos(phi)*e.rx;
        float y=-sin(phi)*e.ry;
        line(image,centre*SCALE,centre*SCALE+Point2f(-x*cos(e.t)+y*sin(e.t),x*sin(e.t)+y*cos(e.t))*SCALE,colours[rounded[i]],1,CV_AA);
    }
#else
    bool matched=false;
    pair<uint8,uint8> id;
    for(uint8 j=0;j<seqs.size();j++) for(uint8 k=0;k<16;k++) //try to match observed sequence to one from the table
    {
        uint8 l;
        for(l=0;l<16;l++) if(rounded[l]!=3 and rounded[l]!=seqs[j][15-(k+l)%16]) break;
        if(l==16)
        {
            matched^=true;
            if(not matched) break; //ensure there is only one match
            id=make_pair(j,k);
        }
    }

    if(not matched) return;

    uint8 led0=15-id.second;
    float phi=circle_to_ellipse(led0*pi/8+base_angle,rz)+pi/2; //find angle of led0

    float x=cos(phi)*e.rx;
    float y=-sin(phi)*e.ry;

    //Find the point(x2,y2) on eBug's boundary which represents led0 (where eBug is facing)
    float x_diff = (-x*cos(e.t)+y*sin(e.t)) * SCALE;
    float y_diff = (x*sin(e.t)+y*cos(e.t)) * SCALE;

    QPointF p1,p2,r;
    p1.setX(centre.x*SCALE);
    p1.setY(centre.y*SCALE);
    p2.setX(p1.x()+x_diff);
    p2.setY(p1.y()+y_diff);
    r.setX(e.rx*SCALE);
    r.setY(e.ry*SCALE);

    eBugsInfo.push_back({id.first,p1,p2,r});
    count++;

#endif
}

void knn_graph_partition(uint8 n_blobs,std::vector<eBug> &eBugsInfo,int &count) //constructs 2-nearest neighbour graph and returns connected components
{
    using namespace std;

    static array<uint8,MAX_BLOBS> numbers;
    static bool first=true;
    if(first) iota(numbers.begin(),numbers.end(),0);
    first=false;

    array<vector<uint8>,MAX_BLOBS> knngraph;

    for(uint8 i=0;i<n_blobs;i++) //determine all edges in 2-nearest neighbour graph
    {
        array<uint8,3> neighbours;
        static array<int32_t,MAX_BLOBS> dists;
        for(uint8 j=0;j<n_blobs;j++)
        {
            int32_t x=points[j].x-points[i].x;
            int32_t y=points[j].y-points[i].y;
            dists[j]=x*x+y*y*25; //scale y coordinate by 5 in distance calculation
        }
        partial_sort_copy(numbers.begin(),numbers.begin()+n_blobs,neighbours.begin(),neighbours.end(),
            [](uint8 a,uint8 b){return dists[a]<dists[b];});

        knngraph[i].insert(knngraph[i].end(),neighbours.begin()+1,neighbours.end());
        for(uint8 j=1;j<neighbours.size();j++) knngraph[neighbours[j]].push_back(i);
    }

    static bitset<MAX_BLOBS> done;
    done.reset();

    for(uint8 i=0;i<n_blobs;i++) //partition graph into connected components
    {
        if(done[i]) continue;
        vector<uint8> component;
        queue<uint8> q;
        q.push(i);
        while(not q.empty()) //breadth-first spanning tree search
        {
            auto v=q.front();
            q.pop();

            if(not done[v]) done[v]=true,component.push_back(v);
            for(auto j:knngraph[v])
                if(not done[j]) q.push(j);
        }
        if(component.size()>=5) identify(component, eBugsInfo, count);
    }
}

#endif // DEBRUIJNTRACKING_H
