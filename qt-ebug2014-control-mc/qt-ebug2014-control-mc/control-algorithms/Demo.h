/*******************************************************************/
/*  Written by Erwin Mochtar Wijaya on 05/02/2014                 */
/*  $Revision: 448                                               */
/*  Task: Controlling 1 eBug to perform random walk             */
/*  while maintaining its position to always stay              */
/* inside the rectangle from camera's view (640x480 pixels)   */
/*************************************************************/

#ifndef DEMO_H
#define DEMO_H

#include "Structures.h"
#include "EBugData.h"
#include "Config.h"
#include <ebug-api-cpp/eBugAPI.h>
#include <stdint.h>
#include <fstream>
#include <control-algorithms/ControlAlgorithms.h>
#include "PIDController.h"
#include <unistd.h> //TODO: Added by MatthewBoys
#include <Buffer.h>


using namespace std;

class Demo{

public:
    Demo(QList<XBeeNode> XBeeNodeList,Buffer<QByteArray> *TxPacketBuffer);

    void Reset(); //reset the eBugs at the beginning
    void Start(int ID,int xtarget,int ytarget,double angleTargetX, double angleTargetY, u_int16_t min_distance, double Fx, double Fy, double avg_angle); //ID of the Ebug
    void Stop(int ID); //ID of the Ebug

    void StopAll() {
          for(int i =0; i<node_n; i++){
          stopMove(i);
          usleep(10000);
          }
        }

    void setxBeeNodeList(QList<XBeeNode> XBeeNodeList);
    void setTxPacketBuffer(Buffer<QByteArray> *TxPacketBuffer);
    void updateEBugData(int ID,EBugData dataComponent);
    int getResetNumber();
    vector<char> packetToSend;
    QList<int> STAGE;

    /*********************************Function to Control EBug's Movements*******************************/
    void rotateCW(int ebugID,double speed);
    void rotateCCW(int ebugID,double speed);
    void rotate(int ebugID, double speed);
    void moveForward(int ebugID, double leftwheelFreq, double rightwheelFreq);
    void moveForward2(int ebugID);
    void moveBackward(int ebugID, double leftwheelFreq, double rightwheelFreq);
    void stopMove(int ebugID);
    void collisionDetected(int ID) {collisionID[ID] = true;}
    void noCollision (int ID){collisionID[ID] = false;}
    bool getCollisionState(int ID) {return collisionID[ID];}
    bool getBoundaryState(int ID) {return reachedBoundary[ID];}
    bool reachedTarget(int ID) {return targetReached[ID];}
    void setCollisionAngle(int ID, double angle) {collisionAngle[ID] = angle;}
    void resetEbugTimer(int ID) {TimeData[ID].restart();}
    void ebugTrapped (int ID, bool state) {EbugTrapped[ID] = state;}

    //added Monday 25 August 2014
    void SteerRight(int ID,uint16_t wheelFreq);
    void SteerLeft(int ID,uint16_t wheelFreq);

    QList<PIDController> angleController;
    QList<PIDController> speedController;

    /****************************************************************************************************/

private:
    QList<XBeeNode> xBeeNodeList;
    Buffer<QByteArray> *txPacketBuffer;
    vector<eBugAPI> eBugsApi;
    QList<EBugData> eBugData;
    int node_n;
    XBeeNodeData convertToNodeData(XBeeNode node);

    QList<double> escapeAngle,xvector,yvector,theta; //arena divided into 4 squares
    QList<bool> reachedBoundary; //returns true when eBug is close to boundaries
    QList<int> boundaries;//stores number from 1-4 to indicate which of the 4 boundaries is the eBug closest to
    QList<int> direction; //stores number from 1-4 to indicate 4 compass directions(EAST->1,NORTH->2,WEST->3,SOUTH->4)
    QList<int> action;// 8 distinct cases depending on boundary and ebug's angle
    QList<int> targetAngle;
    QList<bool> collisionID;
    QList<double> collisionAngle;
    QList<QTime> TimeData;
    QList<bool> EbugTrapped;
    QList<uint16_t> wheelFreq;
    QList<double> leftWheelFreq;
    QList<double> rightWheelFreq;
    QList<double> wheelAcc;
    QList<bool> targetReached;
    double Kp_lin,Ki_lin,Kd_lin,step_time,lin_errorThreshold,lin_integral_error;
    double Kp_ang,Ki_ang,Kd_ang,ang_errorThreshold,ang_integral_error;
};

#endif // DEMO_H
