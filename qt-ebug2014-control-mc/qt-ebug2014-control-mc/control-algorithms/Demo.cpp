/*******************************************************************/
/*  Written by Erwin Mochtar Wijaya on 05/02/2014                 */
/*  $Revision: 448                                               */
/*  Task: Controlling 1 eBug to perform random walk             */
/*  while maintaining its position to always stay              */
/* inside the rectangle from camera's view (640x480 pixels)   */
/*************************************************************/

#include <control-algorithms/Demo.h>
#include <QTime>

Demo::Demo(QList<XBeeNode> XBeeNodeList,Buffer<QByteArray> *TxPacketBuffer){

    this->xBeeNodeList = XBeeNodeList;
    this->txPacketBuffer = TxPacketBuffer;
    ///////////////////// Create eBugAPI objects for each eBug node detected //////////

    // get number of nodes
    this->node_n= XBeeNodeList.count();

    step_time = 0.120; //in seconds

    //Parameters tuned based on Ziegler-Nichols Method
    Kp_ang = 0.2*Ku_ang;
    Ki_ang = 0.5*Kp_ang/Pu;
    Kd_ang = Kp_ang*Pu/8; //reduce overshoot
    ang_errorThreshold = 45; //with error_thres = 0(PD Controller)
    ang_integral_error = 10; //not used

    Kp_lin = 0.45*Ku_lin;//with error_thres = 0(PD Controller)
    Ki_lin = 1*Kp_lin/Pu;
    Kd_lin = Kp_lin*Pu/3;//reduce overshoot
    lin_errorThreshold = 100; //50 pixels error to activate integral component
    lin_integral_error = 10;

    PIDController ControlAngle, ControlSpeed;
    ControlAngle.Initialise(Kp_ang,Ki_ang,Kd_ang,ang_errorThreshold,ang_integral_error,step_time);
    ControlSpeed.Initialise(Kp_lin,Ki_lin,Kd_lin,lin_errorThreshold,lin_integral_error,step_time);

    // create a vector of eBugAPI objects
    for (int i=0; i < node_n; i++){
        eBugAPI eBugapi(XBEE);
        eBugapi.setXBeeNodeData(this->convertToNodeData(this->xBeeNodeList.at(i)));
        this->eBugsApi.push_back(eBugapi);

        EBugData ebugdata;
        QPointF pos(120.0f,60.0f);
        ebugdata.id = i;
        ebugdata.angle = 0.0f;
        ebugdata.position = (i+1)*pos;
        this->eBugData.append(ebugdata);

        QTime time;
        TimeData.append(time);

        int stage = 0;
        STAGE.append(stage);
        reachedBoundary.append(false);
        escapeAngle.append(0.0);
        boundaries.append(0);
        direction.append(0);
        action.append(0);
        targetAngle.append(0);
        collisionID.append(false);
        collisionAngle.append(0.0);
        EbugTrapped.append(false);
        xvector.append(0.0);
        yvector.append(0.0);
        theta.append(0.0);
        wheelFreq.append(0);
        rightWheelFreq.append(0);
        leftWheelFreq.append(0);
        wheelAcc.append(0.0);
        targetReached.append(false);

        angleController.append(ControlAngle);
        speedController.append(ControlSpeed);
    }

}

void Demo::Reset(){

    //////////////////////////////////////////////////////
    //////////// Set LED IDs for each eBug ///////////////
    //////////////////////////////////////////////////////
    static int seqs[15][16]={
        //{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //RED
        //{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, //GREEN
        //{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}, //BLUE
        {1,1,2,1,2,0,2,2,1,0,1,2,2,2,1,1}, //****
        {0,1,1,0,1,2,1,2,2,2,2,0,2,1,1,1}, //****
        {1,1,1,2,2,0,0,2,1,0,2,2,1,2,1,2}, //****
        {0,1,0,2,0,0,0,1,1,2,1,1,2,2,1,1}, //****
        {2,2,1,0,0,2,0,2,1,2,2,1,2,2,0,1}, //****
        {0,0,1,0,1,1,1,0,2,1,0,1,1,2,0,1}, //***
        {1,2,0,0,2,2,1,1,2,0,2,1,0,0,1,1}, //**
        {0,1,0,0,0,1,0,2,1,2,0,1,2,1,0,1},
        {2,0,1,1,1,1,0,0,0,2,2,2,2,2,1,0},
        {0,2,0,1,0,2,2,0,2,2,2,0,1,0,1,2},
        {0,1,2,0,1,1,2,2,2,0,0,1,2,1,1,0},
        {1,0,0,2,2,0,1,1,0,2,2,2,1,2,0,0},
        {2,1,1,0,2,0,2,2,0,0,0,2,1,2,1,0},
        {0,1,2,0,0,0,0,0,1,2,2,0,2,0,0,2},
        {0,2,1,1,2,1,0,0,0,0,2,0,0,1,1,0},
    };

    uint64_t n=0;
    for(int LEDID = 0; LEDID < node_n; LEDID++){ // assigning eBugs' ID starting from 1
        n = 0;
        for(int i=0;i<16;i++)
            n|=uint64_t(1)<<(i*3+seqs[LEDID][i]); //set alternating red, green, blue LEDs

        // send XBee commands to set LEDs
        std::vector<char> packetToSend= eBugsApi[LEDID].RGBLEDs_SetMultiple(n>>32,n<<32>>48,n<<48>>48,4095,0x00);
        txPacketBuffer->add(QByteArray::fromRawData(packetToSend.data(), packetToSend.size()));
        usleep(100000);

#if 0
        std::vector<char> RGBLEDs_SetAllRed(uint16_t value, uint8_t options);
        std::vector<char> RGBLEDs_SetAllGreen(uint16_t value, uint8_t options);
        std::vector<char> RGBLEDs_SetAllBlue(uint16_t value, uint8_t options);
        std::vector<char> RGBLEDs_SetAll(uint16_t value, uint8_t options);
        std::vector<char> RGBLEDs_SetAllOff(uint8_t options);
        std::vector<char> RGBLEDs_SetRed(uint8_t led, uint16_t value, uint8_t options);
        std::vector<char> RGBLEDs_SetGreen(uint8_t led, uint16_t value, uint8_t options);
        std::vector<char> RGBLEDs_SetBlue(uint8_t led, uint16_t value, uint8_t options);
        std::vector<char> RGBLEDs_SetMultiple(uint16_t mask1, uint16_t mask2, uint16_t mask3, uint16_t value, uint8_t options);
        std::vector<char> RGBLEDs_SetMultipleHold(uint16_t mask1, uint16_t mask2, uint16_t mask3, uint16_t value, uint16_t mask, uint8_t options);
        std::vector<char> RGBLEDs_SetAllHold(uint16_t value, uint16_t mask, uint8_t options);
        std::vector<char> RGBLEDs_SetAllRGB(uint16_t r_value, uint16_t g_value, uint16_t b_value, uint8_t options);
#endif
    }


}

void Demo::Start(int ID, int xtarget, int ytarget, double angleTargetX, double angleTargetY, u_int16_t min_dist, double Fx, double Fy,double avg_angle){

    /*******************************RANDOM WALK WITH COLLISION AVOIDANCE**************************************/
#if 0
    /*****************************************************Control Algorithm********************************************************/

    //Check whether the eBug is approaching one of the 4 boundaries(top,bottom,left,right)
    if (!reachedBoundary[ID]&&(eBugData[ID].position.x()<75||eBugData[ID].position.x()>565||eBugData[ID].position.y()<75||eBugData[ID].position.y()>405)) {
        /*******Search for the direction to avoid boundaries with least rotation*******/

        if(eBugData[ID].position.x()<75) // Reached the left boundary
        {
            if(cos(eBugData[ID].angle*PI/180.0) < 0) {
                //qDebug() <<"Reached the left boundary\n";
                boundaries[ID] = 1;

                if(eBugData[ID].angle > 180) direction[ID] = 7;
                else direction[ID] = 3;

                reachedBoundary[ID] = true;
                action[ID] = direction[ID]*boundaries[ID];
                targetAngle[ID] = (direction[ID]-1)*45;
                if (targetAngle[ID] == 0) targetAngle[ID] = 355; //to avoid condition which is always false
                }
            escapeAngle[ID] = 0.0;
        }

        else if(eBugData[ID].position.x()>565) // Reached the right boundary
        {
            if(cos(eBugData[ID].angle*PI/180.0) > 0) {
                //qDebug() <<"Reached the right boundary\n";
                boundaries[ID] = 3;

                if(eBugData[ID].angle > 270) direction[ID] = 7;
                else if(eBugData[ID].angle < 90) direction[ID] = 3;

                reachedBoundary[ID] = true;
                action[ID] = direction[ID]*boundaries[ID];
                targetAngle[ID] = (direction[ID]-1)*45;
                if (targetAngle[ID] == 0) targetAngle[ID] = 355; //to avoid condition which is always false
                }
            escapeAngle[ID] = 180.0;
        }

        else if(eBugData[ID].position.y()<75) // Reached the top boundary
        {
            if(sin(eBugData[ID].angle*PI/180.0) > 0) {
                //qDebug() <<"Reached the top boundary\n";
                boundaries[ID] = 2;

                if(eBugData[ID].angle > 90) direction[ID] = 5;
                else direction[ID] = 1;

                reachedBoundary[ID] = true;
                action[ID] = direction[ID]*boundaries[ID];
                targetAngle[ID] = (direction[ID]-1)*45;
                if (targetAngle[ID] == 0) targetAngle[ID] = 355; //to avoid condition which is always false
                }
            escapeAngle[ID] = 270.0;
        }

        else if(eBugData[ID].position.y()>405) // Reached the bottom boundary
        {
            if(sin(eBugData[ID].angle*PI/180.0) < 0) {
                //qDebug() <<"Reached the bottom boundary\n";
                boundaries[ID] = 4;

                if(eBugData[ID].angle > 270) direction[ID] = 1;
                else direction[ID] = 5;

                reachedBoundary[ID] = true;
                action[ID] = direction[ID]*boundaries[ID];
                targetAngle[ID] = (direction[ID]-1)*45;
                if (targetAngle[ID] == 0) targetAngle[ID] = 355; //to avoid condition which is always false
            }
            escapeAngle[ID] = 90.0;
        }

        xvector[ID] = cos(escapeAngle[ID]*PI/180.0) + cos(collisionAngle[ID]*PI/180.0);
        yvector[ID] = sin(escapeAngle[ID]*PI/180.0) + sin(collisionAngle[ID]*PI/180.0);
        theta[ID] = atan2(yvector[ID],xvector[ID])*180.0/PI;
        if(theta[ID] < 0.0) theta[ID] += 360.0;      
        qDebug() <<"Theta = " << theta[ID] <<"col angle" << collisionAngle[ID] << " ID =  " <<ID<< endl;
        collisionAngle[ID] = theta[ID];

    } //end outer if

  /*********************************************END OF BOUNDARY CHECKING*************************************************/

    bool cond1 = collisionAngle[ID] < 10 || collisionAngle[ID] > 350;
    bool cond2 = eBugData[ID].angle > 15 && eBugData[ID].angle < 345;
    bool cond3 = abs(collisionAngle[ID] - eBugData[ID].angle) > 25;
    bool cond4 = eBugData[ID].position.x()>100 && eBugData[ID].position.x()<540;
    bool cond5 = eBugData[ID].position.y()>100 && eBugData[ID].position.y()<380;

    if (EbugTrapped[ID]) {
        TimeData[ID].restart();
        stopMove(ID);
        qDebug() << "Ebug ID =  "<<ID<<" trapped mode \n";
    }

    else if(collisionID[ID]?(cond1?cond2:cond3):0) { //collision detected
        if(cond4 && cond5) action[ID] = 1;
        TimeData[ID].restart();

        double rotation;

        if(reachedBoundary[ID]) collisionAngle[ID] = theta[ID];

        qDebug() << "Collision angle of eBug ID "<< ID <<" = " << collisionAngle[ID] << "  current angle" << eBugData[ID].angle <<cond1<<cond3<<" collision mode \n";
        /*****************Case when target angle is bigger than eBug's angle*************/
        if (collisionAngle[ID] > eBugData[ID].angle) {
            rotation = (collisionAngle[ID]-eBugData[ID].angle);
            if (rotation<180) rotateCCW(ID,5);
            else rotateCW(ID,5);
            qDebug() << "";
        }
        /*******************************************************************************/

        else { //Case when target angle < ebug's angle
            rotation = (eBugData[ID].angle-collisionAngle[ID]);
            if (rotation<180) rotateCW(ID,5);
            else rotateCCW(ID,5);
            qDebug() << "";
        }
    }

    else if(reachedBoundary[ID]) { //boundary detected
        //if(cond4 && cond5) action[ID] = 1;
        TimeData[ID].restart();//avoid another rotation after avoidance
        qDebug() << "Ebug ID =  "<<ID<<" located at "<<eBugData[ID].position <<" reached boundary mode....\n";

        switch(action[ID]){
            case 1: //Last step:move forward away from the boundaries after performing rotation
                    (STAGE[ID] == 1)?rotateCCW(ID,20):rotateCW(ID,20);
                    reachedBoundary[ID] = false;
                    action[ID] = 1;
                    qDebug() <<STAGE[ID]<<endl;
                    break;
            case 7://head south->CCW
                    if(abs(eBugData[ID].angle-targetAngle[ID]) > 10) rotateCCW(ID,10);
                    else action[ID] = 1;
                    STAGE[ID] = 1;
                    qDebug() <<STAGE[ID]<<endl;
                    break;
            case 3://head north->CW
                    if(abs(eBugData[ID].angle-targetAngle[ID]) > 10) rotateCW(ID,10);
                    else action[ID] = 1;
                    STAGE[ID] = 0;
                    qDebug() <<STAGE[ID]<<endl;
                    break;
            case 21://head south->CW
                    if(abs(eBugData[ID].angle-targetAngle[ID]) > 10) rotateCW(ID,10);
                    else action[ID] = 1;
                    STAGE[ID] = 0;
                    qDebug() <<STAGE[ID]<<endl;
                    break;
            case 9://head north->CCW
                    if(abs(eBugData[ID].angle-targetAngle[ID]) > 10) rotateCCW(ID,10);
                    else action[ID] = 1;
                    STAGE[ID] = 1;
                    qDebug() <<STAGE[ID]<<endl;
                    break;
            case 10://head west->CCW
                    if(abs(eBugData[ID].angle-targetAngle[ID]) > 10) rotateCCW(ID,10);
                    else action[ID] = 1;
                    STAGE[ID] = 1;
                    qDebug() <<STAGE[ID]<<endl;
                    break;
            case 2:;//head east->CW
                    if(eBugData[ID].angle < 350 && eBugData[ID].angle > 10) rotateCW(ID,5);
                    else action[ID] = 1;
                    STAGE[ID] = 0;
                    qDebug() <<STAGE[ID]<<endl;
                    break;
            case 4://head east->CCW
                    if(eBugData[ID].angle < 350 && eBugData[ID].angle > 10) rotateCCW(ID,5);
                    else action[ID] = 1;
                    STAGE[ID] = 1;
                    qDebug() <<STAGE[ID]<<endl;
                    break;
            case 20://head west->CW
                    if(abs(eBugData[ID].angle-targetAngle[ID]) > 10) rotateCW(ID,10);
                    else action[ID] = 1;
                    STAGE[ID] = 0;
                    qDebug() <<STAGE[ID]<<endl;
                    break;
            default:
                reachedBoundary[ID] = false;
                action[ID] = 0;
                targetAngle[ID] = 0;
                break;
            }
        }

    //command the eBug to move forward when its not close to boundaries yet
    else  {
        qDebug() <<STAGE[ID]<<"if zero I rotate clockwise"<<endl;
        if(TimeData[ID].elapsed() < 4500) moveForward2(ID);
        else if (TimeData[ID].elapsed() < 6000) (STAGE[ID] == 0)?rotateCW(ID,(6000-TimeData[ID].elapsed())/20):rotateCCW(ID,(6000-TimeData[ID].elapsed())/20);
        else TimeData[ID].start();
        STAGE[ID] = 4;
        qDebug() << "Ebug ID =  "<<ID<<" normal mode forward/random rotation\n";

        }

#else
     /*******************************DYNAMIC LEADER-FOLLOWER CHASING VIRTUAL TARGETS**************************************/
    #if 0
    double rot_speed,linear_speed,K_dist;
    double K_rot,distance_err; //steering constant

    double angle = eBugData[ID].angle;
    double x1 = eBugData[ID].position.x();
    double y1 = eBugData[ID].position.y();

    double x2 = xtarget;
    double y2 = ytarget;

    double xdiff = (x2 - x1); //x+ to the right
    double ydiff = (y1 - y2); //y+ running downwards

    double xdiff2 = angleTargetX - x1;
    double ydiff2 = y1 - angleTargetY;

    distance_err = qSqrt(qPow(xdiff,2)+qPow(ydiff,2));
    K_dist = speedController[ID].Update(distance_err-min_dist);

    targetAngle[ID] = (atan2(ydiff2,xdiff2)*180.0/PI);
    targetAngle[ID] = (targetAngle[ID] < 0.0)?(targetAngle[ID]+360.0):targetAngle[ID];

    double angle_err = targetAngle[ID] - angle;
    angle_err = (angle_err>180)?(angle_err-360.0):(angle_err<(-180.0)?angle_err+360.0:angle_err); //limit angle error bet 180 and -180
    K_rot = angleController[ID].Update(angle_err)/180.0;


    //rot_speed = K_rot*MAX_STEPPER_MOTOR_FREQ/2;
    //linear_speed = K_dist*MAX_STEPPER_MOTOR_FREQ/2;
    //rot_speed = (rot_speed>0.5*MAX_STEPPER_MOTOR_FREQ)?0.5*MAX_STEPPER_MOTOR_FREQ:((rot_speed < -0.5*MAX_STEPPER_MOTOR_FREQ)?-0.5*MAX_STEPPER_MOTOR_FREQ:rot_speed);
    //linear_speed = (linear_speed>0.5*MAX_STEPPER_MOTOR_FREQ)?0.5*MAX_STEPPER_MOTOR_FREQ:((linear_speed < -0.5*MAX_STEPPER_MOTOR_FREQ)?-0.5*MAX_STEPPER_MOTOR_FREQ:linear_speed);

    rot_speed = K_rot*MAX_STEPPER_MOTOR_FREQ;
    //rot_speed = (rot_speed>0.45*MAX_STEPPER_MOTOR_FREQ)?0.45*MAX_STEPPER_MOTOR_FREQ:((rot_speed < -0.45*MAX_STEPPER_MOTOR_FREQ)?-0.45*MAX_STEPPER_MOTOR_FREQ:rot_speed);
    linear_speed = K_dist*MAX_STEPPER_MOTOR_FREQ*0.25;
    //linear_speed = (linear_speed>0.65*MAX_STEPPER_MOTOR_FREQ)?0.65*MAX_STEPPER_MOTOR_FREQ:((linear_speed < -0.65*MAX_STEPPER_MOTOR_FREQ)?-0.65*MAX_STEPPER_MOTOR_FREQ:linear_speed);
    rot_speed = (rot_speed>0.4*MAX_STEPPER_MOTOR_FREQ)?0.4*MAX_STEPPER_MOTOR_FREQ:((rot_speed < -0.4*MAX_STEPPER_MOTOR_FREQ)?-0.4*MAX_STEPPER_MOTOR_FREQ:rot_speed);
    linear_speed = (linear_speed>0.4*MAX_STEPPER_MOTOR_FREQ)?0.4*MAX_STEPPER_MOTOR_FREQ:((linear_speed < -0.4*MAX_STEPPER_MOTOR_FREQ)?-0.4*MAX_STEPPER_MOTOR_FREQ:linear_speed);

    if(fabs(distance_err-min_dist) < 5) {
        targetReached[ID] = true;
        speedController[ID].reset();
        angleController[ID].reset();
    }
    else targetReached[ID] = false;

    //qDebug() <<"K_rot = "<<K_dist<<"K_rot = "<<rot_speed<<"angle error = " <<angle_err<<"distance error - 40 = "<< distance_err-40 << endl;
    //rotate(ID,rot_speed);
    //moveForward(ID,linear_speed,linear_speed);
    if(!collisionID[ID]) moveForward(ID,linear_speed-rot_speed,linear_speed+rot_speed);
    else stopMove(ID);
    #else
    /*******************************DECENTRALIZED FORMATION CONTROL BASED ON MORSE POTENTIAL**************************************/
    double rot_speed,linear_speed,angle_diff,polar_moment,avg_orientation_error;

    angle_diff = (avg_angle - eBugData[ID].angle);
    angle_diff = (angle_diff>180.0)?(angle_diff-360.0):(angle_diff<(-180.0)?angle_diff+360.0:angle_diff);
    angle_diff = angle_diff*PI/180.0; //convert into radian

    linear_speed = Fx*cos(eBugData[ID].angle*PI/180.0) + Fy*sin(eBugData[ID].angle*PI/180.0);
    linear_speed *= 2000;
    linear_speed = (linear_speed > 4000)?4000:linear_speed;

    polar_moment = Cp*(Fy*cos(eBugData[ID].angle*PI/180.0) - Fx*sin(eBugData[ID].angle*PI/180.0));
    polar_moment *= 1500;
    polar_moment = (polar_moment > 3000) ? 3000 : polar_moment;

    avg_orientation_error = Ct*(angle_diff);
    avg_orientation_error *= 1500;
    avg_orientation_error = (avg_orientation_error > 1500) ? 1500 : avg_orientation_error;

    rot_speed = polar_moment + avg_orientation_error;
/*
    qDebug() << "Robot ID = " << ID << endl;
    qDebug() << "cos ebugangle = " << cos(eBugData[ID].angle*PI/180.0) << "sin ebugangle = " << sin(eBugData[ID].angle*PI/180.0) << endl;
    qDebug() << "Fx = " << Fx << "  Fy = " << Fy << "  polarmoment = " << polar_moment <<"  orientation error = " << avg_orientation_error << endl;
    qDebug() << linear_speed << rot_speed << endl;
*/
    moveForward(ID,linear_speed-rot_speed,linear_speed+rot_speed);

    #endif

#endif

    }
    /**************************************************End of Control Algorithm****************************************************/

XBeeNodeData Demo::convertToNodeData(XBeeNode node){
    XBeeNodeData result;
    result.commandStatus = node.commandStatus;
    result.deviceType = node.deviceType;
    result.manufacturerID = node.manufacturerID;
    result.MY = node.MY;
    result.NI = node.NI.toUtf8().constData();
    result.parentNetworkAddress = node.parentNetworkAddress;
    result.profileID = node.profileID;
    result.SH_high = node.SH_high;
    result.SH_low = node.SH_low;
    result.SL_high = node.SL_high;
    result.SL_low = node.SL_low;
    result.status = node.status;
    return result;
}

void Demo::setxBeeNodeList(QList<XBeeNode> XBeeNodeList){
    this->xBeeNodeList = XBeeNodeList;
}

void Demo::setTxPacketBuffer(Buffer<QByteArray> *TxPacketBuffer){
    this->txPacketBuffer = TxPacketBuffer;
}

void Demo::updateEBugData(int ID, EBugData dataComponent){
    if(ID < node_n) eBugData[ID] = dataComponent;
}

void Demo::rotateCW(int ebugID,double speed)
{
    int nSteps;// number of steps size taken
    nSteps = (45*3200)/90;//calculate number of steps to turn CW a certain degree
    packetToSend = eBugsApi[ebugID].StepperMotor_LeftRightStep(speed,nSteps,FORWARDS,SIXTEENTH,false,speed,nSteps,BACKWARDS,SIXTEENTH,false,0x00);
    txPacketBuffer->add(QByteArray::fromRawData(packetToSend.data(), packetToSend.size()));
}

void Demo::rotateCCW(int ebugID,double speed)
{
    int nSteps;// number of steps size taken
    nSteps = (45*3200)/180;//calculate number of steps to turn CCW a certain degree
    packetToSend = eBugsApi[ebugID].StepperMotor_LeftRightStep(speed,nSteps,BACKWARDS,SIXTEENTH,false,speed,nSteps,FORWARDS,SIXTEENTH,false,0x00);
    txPacketBuffer->add(QByteArray::fromRawData(packetToSend.data(), packetToSend.size()));
}

void Demo::rotate(int ebugID, double speed)
{
    StepperMotor_Direction_t left_direction,right_direction;
    int nSteps;// number of steps size taken

    //positive counterclockwise
    if(speed > 0.0) {
        left_direction = BACKWARDS;
        right_direction = FORWARDS;
    }

    else {
        left_direction = FORWARDS;
        right_direction = BACKWARDS;
    }

    speed = round(abs(speed));
    nSteps = (45*3200)/90;//calculate number of steps to turn a certain degree

    packetToSend = eBugsApi[ebugID].StepperMotor_LeftRightStep(speed,nSteps,left_direction,SIXTEENTH,false,speed,nSteps,right_direction,SIXTEENTH,false,0x00);
    txPacketBuffer->add(QByteArray::fromRawData(packetToSend.data(), packetToSend.size()));
}

void Demo::moveForward(int ebugID,double leftwheelFreq,double rightwheelFreq)
{
    StepperMotor_Direction_t left_direction,right_direction;
    uint16_t leftspeed,rightspeed;
    if(leftwheelFreq > 0.0)
        left_direction = FORWARDS;
    else left_direction = BACKWARDS;

    if(rightwheelFreq > 0.0)
        right_direction = FORWARDS;
    else right_direction = BACKWARDS;

    leftspeed = round(abs(leftwheelFreq));
    rightspeed = round(abs(rightwheelFreq));

    packetToSend = eBugsApi[ebugID].StepperMotor_LeftRightStep(leftspeed,1600,left_direction,SIXTEENTH,false,rightspeed,1600,right_direction,SIXTEENTH,false,0x00);
    txPacketBuffer->add(QByteArray::fromRawData(packetToSend.data(), packetToSend.size()));
}

void Demo::moveForward2(int ebugID)
{
    packetToSend = eBugsApi[ebugID].StepperMotor_LeftRightStep(0x0200,500,FORWARDS,QUARTER,false,0x0200,500,FORWARDS,QUARTER,false,0x00);
    txPacketBuffer->add(QByteArray::fromRawData(packetToSend.data(), packetToSend.size()));
}

void Demo::moveBackward(int ebugID,double leftwheelFreq,double rightwheelFreq)
{
    StepperMotor_Direction_t left_direction,right_direction;
    uint16_t leftspeed,rightspeed;
    if(leftwheelFreq > 0.0)
        left_direction = BACKWARDS;
    else left_direction = FORWARDS;

    if(rightwheelFreq > 0.0)
        right_direction = BACKWARDS;
    else right_direction = FORWARDS;

    leftspeed = round(abs(leftwheelFreq));
    rightspeed = round(abs(rightwheelFreq));

    packetToSend = eBugsApi[ebugID].StepperMotor_LeftRightStep(leftspeed,500,left_direction,SIXTEENTH,false,rightspeed,500,right_direction,SIXTEENTH,false,0x00);
    txPacketBuffer->add(QByteArray::fromRawData(packetToSend.data(), packetToSend.size()));
}

void Demo::stopMove(int ebugID)
{
    packetToSend = eBugsApi[ebugID].StepperMotor_LeftRightStop(true,true, 0x00);
    txPacketBuffer->add(QByteArray::fromRawData(packetToSend.data(), packetToSend.size()));
}

void Demo::SteerRight(int ebugID, uint16_t wheelFreq) {
    packetToSend = eBugsApi[ebugID].StepperMotor_LeftRightStep(wheelFreq,500,FORWARDS,SIXTEENTH,false,round(wheelFreq*0.8),500,FORWARDS,SIXTEENTH,false,0x00);
    txPacketBuffer->add(QByteArray::fromRawData(packetToSend.data(), packetToSend.size()));
}

void Demo::SteerLeft(int ebugID, uint16_t wheelFreq) {
    packetToSend = eBugsApi[ebugID].StepperMotor_LeftRightStep(round(wheelFreq*0.8),500,FORWARDS,SIXTEENTH,false,wheelFreq,500,FORWARDS,SIXTEENTH,false,0x00);
    txPacketBuffer->add(QByteArray::fromRawData(packetToSend.data(), packetToSend.size()));
}
