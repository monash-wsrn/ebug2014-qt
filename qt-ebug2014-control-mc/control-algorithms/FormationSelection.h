#if 0

#ifndef FORMATIONSELECTION_H
#define FORMATIONSELECTION_H
#include <stdint.h>
#include <fstream>
#include <control-algorithms/ControlAlgorithms.h>

using namespace std;
using namespace cv;

class FormationSelection: public ControlAlgorithms{


public:
    FormationSelection(QList<XBeeNode> XBeeNodeList,Buffer<QByteArray> *TxPacketBuffer, vector<EBugData*> eBugData);
    ~FormationSelection();

    int Reset();
    int Init();
    //    int Controller();
    int Start();

    void setxBeeNodeList(QList<XBeeNode> XBeeNodeList);
    void setTxPacketBuffer(Buffer<QByteArray> *TxPacketBuffer);
    void updateEBugData(vector<EBugData*> eBugData);

    int resetComplete;
private:
    QList<XBeeNode> xBeeNodeList;
    Buffer<QByteArray> *txPacketBuffer;
    vector<eBugAPI> eBugsApi;
    vector<EBugData*> eBugData;
    int node_n;
    ofstream logfile;
    int count;
    int FL,FR1,FR2,FR3,FR4;
    float formation_direction;     // in degrees, [-180,180]

    void writeToLog(char * msg);
    XBeeNodeData convertToNodeData(XBeeNode node);

    // Controller
    constexpr static double kAng = 1.5;
    constexpr static double kDist = 30;
    constexpr static double deadZone = 5;
    StepperMotor_Direction_t leftWheelDirection,rightWheelDirection;
    int leftWheelFreq, rightWheelFreq;

    double leaderWController(int targetX, int targetY, int leaderX, int leaderY, double leaderAngle);
    double followerVController(double forwardError);
    double followerWController(double normalError, double forwardError, double followerAngle, double *positionAngleError, bool considerDeadZone);
    int convertVelocitiesToFreq(double V, double ebugW);
    double stdAngleToTarget(double diffX, double diffY);
    double normAngleToTarget(double normalError, double forwardError);
    double correctDisplayAngle(double angle, int opt = 0);
    float calculateErrorVector(float theta_actual, float l_actual, float theta_constraint, float lConstraint, float *normalError, float *forwardError);

    // Formation Selection
    float LocalErrorEstimation(int eBugID, int formationType);
};


#endif // FORMATIONSELECTION_H

#endif
