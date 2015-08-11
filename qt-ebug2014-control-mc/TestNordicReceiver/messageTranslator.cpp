#include "messageTranslator.h"

MessageTranslator::MessageTranslator(QObject *parent) : QThread(parent)
{
}

MessageTranslator::~MessageTranslator()
{
}

void MessageTranslator::run()
{
}

void MessageTranslator::translate(QByteArray btyarMessage)
{
    qDebug("Translator: Got new message to translate");

    //Convert bytes to LED data
    QList<dataLed> listLeds = getLeds(btyarMessage);

    QList<QString> lststrLedsString;
    for(int i=0; i<listLeds.size(); i++){
        dataLed gotLED = listLeds.at(i);
        lststrLedsString.append(QString::number(i));
        lststrLedsString.append(": (");
        lststrLedsString.append(QString::number(gotLED.x));
        lststrLedsString.append(",");
        lststrLedsString.append(QString::number(gotLED.y));
        lststrLedsString.append(") Colour: ");
        lststrLedsString.append(QString::number(gotLED.colour));
        lststrLedsString.append(" Size: ");
        lststrLedsString.append(QString::number(gotLED.size));
    }

    qDebug("Translator: got some LEDs!");
    emit ledsDone(lststrLedsString);
    //Set up 2-nearest Neighbour graph
    //TODO

}

/**
 * @brief MessageTranslator::getLeds
 * Converts a raw byte message to a vector of
 * LEDS objects, each containing x,y,colour and size
 * @param message Raw byte array to split
 * @return  List of LED objects matching input
 */
QList<MessageTranslator::dataLed> MessageTranslator::getLeds(QByteArray message)
{
    QList<dataLed> listLeds;
    for(int i=0; i<message.size(); i++)
    {
        dataLed newLed;
        char byteCurrent = message.at(i);
        newLed.x = getBits(0,10,byteCurrent);
        newLed.y = getBits(11,20,byteCurrent);
        newLed.colour = getBits(21,22,byteCurrent);
        newLed.size = getBits(23,31,byteCurrent);
        qDebug("Translator: Added LED");
        //Add new led data
        listLeds.append(newLed);
    }
    return listLeds;
}

/**
 * @brief MessageTranslator::getBits
 * Sub function of getLeds. Extracts the bits from index 'from' to
 * 'to' inclusive (first index is 0).
 * @param from: Starting bit to include (0 is initial)
 * @param to: Last bit to include. Assumes to-from<16
 * @param byte: Char of bits to take from.
 * @return Integer of returned bits.
 */
int MessageTranslator::getBits(int from, int to, char byte)
{
    int chopStart=byte>>from;
    return chopStart & (int)(pow(2,to-from)-1);
}


////constructs 2-nearest neighbour graph
//QVector<dataRobotLocation> MessageTranslator::knn_graph_partition(QVector<dataLed> vectLeds)
//{

//    QVector<dataRobotLocation> listRobotDataLocations;

//    using namespace std;

//    array<int,MAX_LEDS> numbers;
//    iota(numbers.begin(),numbers.end(),0);

//    array<std::vector<int>,MAX_LEDS> knngraph;

//    for(int i=0;i<numLeds;i++) //determine all edges in 2-nearest neighbour graph
//    {
//        array<int,3> neighbours;
//        static array<int32_t,MAX_LEDS> dists;
//        for(int j=0;j<numLeds;j++)
//        {
//            int32_t x=vectLeds[j].x-vectLeds[i].x;
//            int32_t y=vectLeds[j].y-vectLeds[i].y;
//            dists[j]=x*x+y*y; //scale y coordinate by 5 in distance calculation
//        }
//        partial_sort_copy(numbers.begin(),numbers.begin()+numLeds,neighbours.begin(),neighbours.end(),
//            [](int a,int b){return dists[a]<dists[b];});

//        knngraph[i].insert(knngraph[i].end(),neighbours.begin()+1,neighbours.end());
//        for(int j=1;j<neighbours.size();j++) knngraph[neighbours[j]].push_back(i);
//    }
////TODO
//    static bitset<MAX_LEDS> done;
//    done.reset();

//    for(int i=0;i<numLeds;i++) //partition graph into connected components
//    {
//        if(done[i]) continue;
//        std::vector<int> component;
//        queue<int> q;
//        q.push(i);
//        while(not q.empty()) //breadth-first spanning tree search
//        {
//            auto v=q.front();
//            q.pop();

//            if(not done[v]) done[v]=true,component.push_back(v);
//            for(auto j:knngraph[v])
//                if(not done[j]) q.push(j);
//        }
//        if(component.size()>=5){
//            //Only bother if 5+ LED points are found
//            std::vector<int> vecLedsGood = getGoodLEDPoints(component);
//            if(vecLedsGood.size()>=5){
//                //Only bother with identify if 5+ 'good' led points found
//                dataRobotLocation dataIdentifiedRobot = identify(vecLedsGood);
//                if(dataIdentifiedRobot.robotId!=-1){
//                    //Check the id for the "No match found=-1" flag
//                    qDebug("Adding identified robot data!");
//                    listRobotDataLocations.push_back(dataIdentifiedRobot);
//                }
//            }
//        }
//    }


//    return listRobotDataLocations;

//}


//std::vector<int> MessageTranslator::getGoodLEDPoints(std::vector<int> vecLeds){
//    dataEllipse ellipseFitted=fitEllipse(vecLeds); //fit ellipse to all leds in component

//    float s=sin(ellipseFitted.t);
//    float c=cos(ellipseFitted.t);
//    std::vector<int> vecLedsGood;
//    for(auto i:vecLeds)
//    {
//        float x0=points[i].x-ellipseFitted.x;
//        float y0=points[i].y-ellipseFitted.y;
//        float x=( c*x0+s*y0)/ellipseFitted.rx;
//        float y=(-s*x0+c*y0)/ellipseFitted.ry;
//        float r2=x*x+y*y;
//        if(r2>0.9 and r2<1.1) vecLedsGood.push_back(i); //select only points that fit the ellipse well
//    }

//    return vecLedsGood;
//}

//dataRobotLocation MessageTranslator::identify(std::vector<int> good)
//{
//    const std::array<std::array<int,16>,15> ledSequences{{
//    //{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
//    {{1,1,2,1,2,0,2,2,1,0,1,2,2,2,1,1}}, //****
//    {{0,1,1,0,1,2,1,2,2,2,2,0,2,1,1,1}}, //****
//    {{1,1,1,2,2,0,0,2,1,0,2,2,1,2,1,2}}, //****
//    {{0,1,0,2,0,0,0,1,1,2,1,1,2,2,1,1}}, //****
//    {{2,2,1,0,0,2,0,2,1,2,2,1,2,2,0,1}}, //****
//    {{0,0,1,0,1,1,1,0,2,1,0,1,1,2,0,1}}, //***
//    {{1,2,0,0,2,2,1,1,2,0,2,1,0,0,1,1}}, //**
//    {{0,1,0,0,0,1,0,2,1,2,0,1,2,1,0,1}},
//    {{2,0,1,1,1,1,0,0,0,2,2,2,2,2,1,0}},
//    {{0,2,0,1,0,2,2,0,2,2,2,0,1,0,1,2}},
//    {{0,1,2,0,1,1,2,2,2,0,0,1,2,1,1,0}},
//    {{1,0,0,2,2,0,1,1,0,2,2,2,1,2,0,0}},
//    {{2,1,1,0,2,0,2,2,0,0,0,2,1,2,1,0}},
//    {{0,1,2,0,0,0,0,0,1,2,2,0,2,0,0,2}},
//    {{0,2,1,1,2,1,0,0,0,0,2,0,0,1,1,0}},
//    }};


//    dataEllipse ellipseFitted=fitEllipse(good); //refit ellipse to only good points

//    QPointF centre(WIDTH-ellipseFitted.x,ellipseFitted.y);

//    float s=sin(ellipseFitted.t);
//    float c=cos(ellipseFitted.t);

//    float rz=std::max(ellipseFitted.rx,ellipseFitted.ry)/FOCAL_WIDTH;
//    std::vector<float> angles(good.size());
//    for(int i=0;i<good.size();i++)
//    {
//        float x0=points[good[i]].x-ellipseFitted.x;
//        float y0=points[good[i]].y-ellipseFitted.y;
//        float x=( c*x0+s*y0)/ellipseFitted.rx;
//        float y=(-s*x0+c*y0)/ellipseFitted.ry;
//        angles[i]=ellipse_to_circle(-atan2(y,x)-M_PI/2,rz);
//    }
//    float base_angle=min_rounding(angles);

//    std::array<int,16> rounded;
//    std::fill(rounded.begin(),rounded.end(),3);
//    std::array<float,16> size{};
//    for(int i=0;i<good.size();i++)
//    {
//        int z=round((angles[i]-base_angle)*8/M_PI+16);
//        z%=16;
//        auto &p=points[good[i]];
//        if(size[z]<p.size) size[z]=p.size,rounded[z]=p.colour;
//    }

//    bool matched=false;
//    std::pair<int,int> id;
//    for(int j=0;j<ledSequences.size();j++)
//    {
//        for(int k=0;k<16;k++) //try to match observed sequence to one from the table
//        {
//            int l;
//            for(l=0;l<16;l++)
//                if(rounded[l]!=3 and rounded[l]!=ledSequences[j][15-(k+l)%16]) break;
//            if(l==16)
//            {
//                matched^=true;
//                if(not matched)
//                    break; //ensure there is only one match
//                id=std::make_pair(j,k);
//            }
//        }
//    }

//    if(not matched){
//        //Found no match of the LED pattern
//        dataRobotLocation noMatchRobot;
//        noMatchRobot.robotId = -1;
//        qDebug("Found no match for this LED pattern.");
//        return noMatchRobot;
//    }

//    int led0=15-id.second;
//    float angleLed0=circle_to_ellipse(led0*M_PI/8+base_angle,rz)+M_PI/2; //find angle of led0

//    float x=cos(angleLed0)*ellipseFitted.rx;
//    float y=-sin(angleLed0)*ellipseFitted.ry;

//    //Find the point(x2,y2) on eBug's boundary which represents led0 (where eBug is facing)
//    float x_diff = (-x*cos(ellipseFitted.t)+y*sin(ellipseFitted.t)) * SCALE ;
//    float y_diff = (x*sin(ellipseFitted.t)+y*cos(ellipseFitted.t)) *  SCALE ;
//    float angle = -atan2(y_diff,x_diff)*180/M_PI;
//    if (angle < 0) angle += 360;

//    QPointF ptRobotCentre;//,pointRobotDirection,pointRobotRadius;
//    ptRobotCentre.setX(centre.x() * SCALE );
//    ptRobotCentre.setY(centre.y() * SCALE );
//    //pointRobotDirection.setX(pointRobotCentre.x()+x_diff);
//    //pointRobotDirection.setY(pointRobotCentre.y()+y_diff);
//    //pointRobotRadius.setX(ellipseFitted.rx * SCALE );
//    //pointRobotRadius.setY(ellipseFitted.ry * SCALE );

//    dataRobotLocation newRobotData;
//    newRobotData.robotId = id.first;
//    newRobotData.worldPossition = ptRobotCentre;
//    newRobotData.worldOrientation = angle;
//    return newRobotData;

//    //eBugsInfo.push_back({id.first,pointRobotCentre,pointRobotDirection,pointRobotRadius,angle});
//   // count++;
//}



//MessageTranslator::dataEllipse MessageTranslator::fitEllipse(std::vector<int> &component)
//{
//    using namespace Eigen;

//    float xoffset,yoffset;
//    Vector3f vx,vy;
//    {
//        int n;
//        Matrix<float,32,1> x,y,size;
//        float xrange,yrange;
//        {
//            n=component.size();
//            if(n>32) n=32;
//            for(int i=0;i<n;i++)
//            {
//                x[i]=points[component[i]].x;
//                y[i]=points[component[i]].y;
//                size[i]=points[component[i]].size;
//            }
//            for(int i=n;i<32;i++) x[i]=y[i]=size[i]=0;

//            float xmin=x.segment(0,n).minCoeff();
//            float ymin=y.segment(0,n).minCoeff();
//            float xmax=x.segment(0,n).maxCoeff();
//            float ymax=y.segment(0,n).maxCoeff();

//            xoffset=(xmax+xmin)/2;
//            yoffset=(ymax+ymin)/2;
//            xrange=(xmax-xmin)/2;
//            yrange=(ymax-ymin)/2;

//            for(int i=0;i<n;i++)
//            {
//                x[i]-=xoffset;
//                x[i]/=xrange;
//                y[i]-=yoffset;
//                y[i]/=yrange;
//            }
//        }

//        Matrix3f A,B,C,D;

//        {
//            Matrix<float,32,1> xx=x.array()*x.array(),xy=x.array()*y.array(),yy=y.array()*y.array();
//            Matrix<float,32,1> xxs=xx.array()*size.array(),xys=xy.array()*size.array(),yys=yy.array()*size.array();
//            A<<xx.dot(xxs),xx.dot(xys),xx.dot(yys),
//               xx.dot(xys),xy.dot(xys),xy.dot(yys),
//               xx.dot(yys),xy.dot(yys),yy.dot(yys);
//            C<<  xxs.sum(),  xys.sum(),x.dot(size),
//                 xys.sum(),  yys.sum(),y.dot(size),
//               x.dot(size),y.dot(size), size.sum();
//            B<<x.dot(xxs),x.dot(xys),x.dot(yys),
//               x.dot(xys),x.dot(yys),y.dot(yys),
//                   C(0,0),    C(0,1),    C(1,1);
//            D<<   0,   0,-0.5,
//                  0,   1,   0,
//               -0.5,   0,   0;
//        }

//        C=-C.inverse()*B;
//        D*=(A+B.transpose()*C);

//        vx=eig(D);
//        vy=C*vx;

//        vx[0]*=yrange*yrange;
//        vx[1]*=xrange*yrange;
//        vx[2]*=xrange*xrange;
//        vy[0]*=xrange*yrange*yrange;
//        vy[1]*=xrange*xrange*yrange;
//        vy[2]*=xrange*xrange*yrange*yrange;
//    }

//    float t=0.5*atan2(vx[1],vx[0]-vx[2]);
//    float cost=cos(t);
//    float sint=sin(t);

//    float cos_squared=cost*cost,cos_sin=cost*sint,sin_squared=sint*sint;
//    float Au= vy[0]*cost+vy[1]*sint;
//    float Av=-vy[0]*sint+vy[1]*cost;
//    float Auu=vx[0]*cos_squared+vx[1]*cos_sin+vx[2]*sin_squared;
//    float Avv=vx[0]*sin_squared-vx[1]*cos_sin+vx[2]*cos_squared;

//    float tuCentre=-Au/(2*Auu);
//    float tvCentre=-Av/(2*Avv);
//    float wCentre=vy[2]-Auu*tuCentre*tuCentre-Avv*tvCentre*tvCentre;
//    float uCentre=tuCentre*cost-tvCentre*sint;
//    float vCentre=tuCentre*sint+tvCentre*cost;

//    float Ru=sqrt(fabs(wCentre/Auu));
//    float Rv=sqrt(fabs(wCentre/Avv));

//    uCentre+=xoffset;
//    vCentre+=yoffset;

//    if(t<0) t+=4*atan(1);

//    return {uCentre,vCentre,Ru,Rv,t};
//}

//float MessageTranslator::ellipse_to_circle(float phi,float rz) //projects ellipse angle onto 3D circle
//{
//    float K=rz*cos(phi);
//    return 2*atan2(sqrt(1-K*K)-cos(phi),K+sin(phi));
//}

//float MessageTranslator::min_rounding(std::vector<float> &angles) //minimise rounding when selecting 16 evenly placed points on circle
//{
//    float angle=0,min=INFINITY;
//    for(auto j:angles)
//    {
//        float r=0;
//        for(auto k:angles)
//        {
//            float x=(k-j)*8/M_PI+16;
//            x-=round(x);
//            r+=x*x;
//        }
//        if(r<min)
//        {
//            min=r;
//            angle=j;
//        }
//    }
//    return angle;
//}

//float MessageTranslator::circle_to_ellipse(float theta,float rz) //projects circle angle onto ellipse in image
//{
//    return atan2(rz+sin(theta),cos(theta));
//}


//Eigen::Vector3f MessageTranslator::eig(Eigen::Matrix3f &M)
//{
//    using namespace Eigen;

//    float q=M.trace()/3;
//    Matrix3f A=M-q*Matrix3f::Identity();
//    float det=A.determinant();

//    Matrix3f B=A*A;
//    float p2=B.trace()/6;
//    float p=sqrt(p2);
//    det/=(p*p2);

//    #if 0
//    float z=sqrt(det+2);
//    z=z/2+1;
//    if(det>-1) z=(z/2+.99985)*(1.0145-.00725*det);
//    else z=z/1.75+0.921620629618769-0.0393396851906154*det;
//    #else
//    float z=2*cos(acos(det/2)/3);
//    #endif

//    float e0=q+p*z;
//    z/=2;
//    float e1=q+p*(sqrt(3-3*z*z)-z);

//    Matrix3f E0=M-e0*Matrix3f::Identity();
//    Matrix3f E1=M-e1*Matrix3f::Identity();

//    return (E0*E1).col(0);
//}
