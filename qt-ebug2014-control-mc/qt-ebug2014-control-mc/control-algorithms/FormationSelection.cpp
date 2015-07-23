#if 0

// $Revision: 426 $  $Date: 2013-11-24 14:47:09 +1100 (Sun, 24 Nov 2013) $
#include <control-algorithms/FormationSelection.h>


FormationSelection::FormationSelection(QList<XBeeNode> XBeeNodeList,Buffer<QByteArray> *TxPacketBuffer, std::vector<EBugData*> eBugData){

    this->xBeeNodeList = XBeeNodeList;
    this->txPacketBuffer = TxPacketBuffer;
    this->eBugData = eBugData;

    resetComplete = 0;
    count = 0;
    formation_direction = 0;

    FL = -1;					// Formation Leader
    FR1 = -1;                // Follower Robot 1
    FR2 = -1;                // Follower Robot 2
    FR3 = -1;                // Follower Robot 3
    FR4 = -1;                // Follower Robot 4

    ///////////////////// Create eBugAPI objects for each eBug node detected //////////

    // get number of nodes
    this->node_n=xBeeNodeList.count();

    // create a vector of eBugAPI objects
    for (int i=0; i < node_n; i++){
        eBugAPI eBugapi(XBEE);
        eBugapi.setXBeeNodeData(this->convertToNodeData(this->xBeeNodeList.at(i)));
        this->eBugsApi.push_back(eBugapi);
    }
}

FormationSelection::~FormationSelection(){}

void FormationSelection::setxBeeNodeList(QList<XBeeNode> XBeeNodeList){
    this->xBeeNodeList = XBeeNodeList;
}

void FormationSelection::setTxPacketBuffer(Buffer<QByteArray> *TxPacketBuffer){
    this->txPacketBuffer = TxPacketBuffer;
}

void FormationSelection::updateEBugData(vector<EBugData*> eBugData){
    this->eBugData = eBugData;
}

int FormationSelection::Reset(){
    count = 0;

    // open a log file
    logfile.open ("Sherry_log.txt");
    logfile << "Sherry's log file.\n";
    logfile << "Reset...\n";


    //////////////////////////////////////////////////////
    //////////// Set LED IDs for each eBug ///////////////
    //////////////////////////////////////////////////////

    static int seqs[15][16]={
        {2,2,1,0,0,2,0,2,1,2,2,1,2,2,0,1},
        {0,1,0,0,0,1,0,2,1,2,0,1,2,1,0,1},
        {2,0,1,1,1,1,0,0,0,2,2,2,2,2,1,0},
        {0,2,0,1,0,2,2,0,2,2,2,0,1,0,1,2},
        {1,1,2,1,2,0,2,2,1,0,1,2,2,2,1,1},
        {0,1,2,0,1,1,2,2,2,0,0,1,2,1,1,0},
        {1,0,0,2,2,0,1,1,0,2,2,2,1,2,0,0},
        {2,1,1,0,2,0,2,2,0,0,0,2,1,2,1,0},
        {1,2,0,0,2,2,1,1,2,0,2,1,0,0,1,1},
        {0,1,1,0,1,2,1,2,2,2,2,0,2,1,1,1},
        {0,1,2,0,0,0,0,0,1,2,2,0,2,0,0,2},
        {0,0,1,0,1,1,1,0,2,1,0,1,1,2,0,1},
        {0,1,0,2,0,0,0,1,1,2,1,1,2,2,1,1},
        {1,1,1,2,2,0,0,2,1,0,2,2,1,2,1,2},
        {0,2,1,1,2,1,0,0,0,0,2,0,0,1,1,0},
    };
    uint64_t n=0;
    for(int LEDID = 0; LEDID < node_n; LEDID++){
        n = 0;
        for(int i=0;i<16;i++)
            n|=uint64_t(1)<<(i*3+seqs[LEDID][i]); //set alternating red, green, blue LEDs
        // send XBee commands to set LEDs
        std::vector<char> packetToSend= eBugsApi[LEDID].RGBLEDs_SetMultiple(n>>32,n<<32>>48,n<<48>>48,4095,0x00);
        txPacketBuffer->add(QByteArray::fromRawData(packetToSend.data(), packetToSend.size()));
        // sleep 5 ms
        usleep(10000);
    }

    if(this->eBugData.size() == node_n){
        //////////////////////////////////////////////////
        ////////// find every eBug's neighbours //////////
        //////////////////////////////////////////////////

        // clean neighbour before updating
        for(unsigned int i = 0; i < eBugData.size(); i++){
            eBugData[i]->neighboursPos.clear();
        }

        relativePos temp;

        for (int i=0; i<node_n; i++){

            for (int j = i+1; j<node_n; j++)
            {

                if (eBugData[i]->getDistanceTo(eBugData[j]) <= SENSOR_RANGE){
                    temp.id = eBugData[j]->getID();
                    temp.distance = eBugData[i]->getDistanceTo(eBugData[j]);
#if DEBUG
                    cout << "distance "<<eBugData[j]->getID()<<" to " << eBugData[i]->getID()<< " = " << temp.distance<< endl;
#endif
                    temp.angle = correctDisplayAngle(eBugData[i]->getAngleBetween(eBugData[j]));
                    eBugData[i]->neighboursPos.push_back(temp);

                    temp.id = eBugData[i]->getID();
                    temp.angle = eBugData[j]->getAngleBetween(eBugData[i]);
                    eBugData[j]->neighboursPos.push_back(temp);
                }
            }
        }
#if DEBUG
        cout << "eBug neighbours sizes"<< endl;
        for(unsigned int i = 0; i < eBugData.size(); i++)
            cout << eBugData[i]->neighboursPos.size()<<endl;
#endif



        //////////////////////////////////////////////////
        ////////// assign leaders for each eBug //////////
        //////////////////////////////////////////////////

        /// FORMATION LEADER
        // calculate distance of each eBug to goal and select the closest one as leader
        FL = 0;

        // distance to goal vector
        vector<float> dis2goal(node_n);
        dis2goal[0]=sqrt(pow(GOAL_X-eBugData[0]->getPosition().x,2)+pow(GOAL_Y-eBugData[0]->getPosition().y,2));

        for (int i=1; i<node_n; i++){
            dis2goal[i]=sqrt(pow(GOAL_X-eBugData[i]->getPosition().x,2)+pow(GOAL_Y-eBugData[i]->getPosition().y,2));
            if(dis2goal[i]<=dis2goal[i-1]){
                FL = eBugData[i]->getID();
                eBugData[FL]->leadersID[0] = FL;
                eBugData[FL]->leadersID[0] = FL;
            }
        }
        // assign formation direction
        formation_direction = eBugData[FL]->getAngle();


        //////////// assign leaders to the rest  ///////////////

        /// find Follower Robot 1 and Follower Robot 2

        int d_temp[eBugData[FL]->neighboursPos.size()];				// distances to neighbours

        // calculate FL distances to neighbours
        for (unsigned int j=0; j<eBugData[FL]->neighboursPos.size(); j++){
            d_temp[j] = eBugData[FL]->getDistanceTo(eBugData[eBugData[FL]->neighboursPos[j].id]);
        }
        FR1 = 0;
        // nearest robot, primarily assigned to be FR1;
        for (unsigned int j=1; j<eBugData[FL]->neighboursPos.size();j++){
            if (d_temp[j]<=d_temp[j-1]){
                FR1 = eBugData[FL]->neighboursPos[j].id;
            }
        }
        FR2 = 0;
        if (eBugData[FL]->neighboursPos.size()>1){  // FR2 is seen by FL
            // second nearest, primarily assigned to be FR2;
            for (unsigned int j=1; j<eBugData[FL]->neighboursPos.size();j++){
                if ((d_temp[j]<=d_temp[j-1])&&(eBugData[FL]->neighboursPos[j].id!= FR1)){
                    FR2 = eBugData[FL]->neighboursPos[j].id;
                }
            }

            // for position 1, find distance of candidate FR1 and FR2 to desired FR1 position
            float* forwardError, normalError;

            /*
            float dis1 = calculateErrorVector(eBugData[FR1]->getAngleBetween(eBugData[FL])-formation_direction, eBugData[FR1]->getDistanceTo(eBugData[FL]), MaxCoverageConstraints[Follower1][2],MaxCoverageConstraints[Follower1][0], &normalError, &forwardError);
            float dis2 = calculateErrorVector(eBugData[FR2]->getAngleBetween(eBugData[FL])-formation_direction, eBugData[FR2]->getDistanceTo(eBugData[FL]), MaxCoverageConstraints[Follower1][2],MaxCoverageConstraints[Follower1][0], &normalError, &forwardError);
            if (dis1>dis2){
                int temp = FR1;
                FR1 = FR2;
                FR2 = temp;
            }
            */

            eBugData[FR1]->formationRole = Follower1;
            eBugData[FR1]->leadersID[0] = FL;
            eBugData[FR1]->leadersID[1] = FR1;

            eBugData[FR2]->formationRole = Follower2;
            eBugData[FR2]->leadersID[0] = FL;
            eBugData[FR2]->leadersID[1] = FR1;
        }else{
            // FR2 is not seen by FL
            qDebug()<< "FR2 is not seen by FL!";
            // find a nearest eBug to FR1, assign this FR2
            for (unsigned int j=1; j<eBugData[FR1]->neighboursPos.size();j++){
                if (eBugData[FR1]->getDistanceTo(eBugData[eBugData[FR1]->neighboursPos[j].id])<=eBugData[FR1]->getDistanceTo(eBugData[eBugData[FR1]->neighboursPos[j-1].id])){
                    FR2 = eBugData[FR1]->neighboursPos[j].id;
                }
            }
            eBugData[FR2]->formationRole = Follower2;
            eBugData[FR2]->leadersID[0] = FR1;
            eBugData[FR2]->leadersID[1] = FR2;
        }

        if (node_n > 3){
            FR3 = 0;
            // the robot  (relative to FR4) nearer to FR1 is FR3
            for (unsigned int j=1; j<eBugData[FR1]->neighboursPos.size();j++){
                if ((eBugData[FR1]->getDistanceTo(eBugData[eBugData[FR1]->neighboursPos[j].id])<=eBugData[FR1]->getDistanceTo(eBugData[eBugData[FR1]->neighboursPos[j-1].id]))&&(j!= FR2)&&(j!=FL)){
                    FR3 = eBugData[FR1]->neighboursPos[j].id;
                }
            }


//            // check FR2's neighbours, select closest to desired position to be FR4
//            for (unsigned int j=1; j<eBugData[FR2]->neighboursPos.size();j++){
//                float dis = calculateErrorVector(eBugData[neighboursPos[j]]->getAngleBetween(eBugData[FR4])-formation_direction, eBugData[neighboursPos[j]]->getDistanceTo(eBugData[FR2]), MaxCoverageConstraints[Follower4][3],MaxCoverageConstraints[Follower4][1], &normalError, &forwardError);
//            }



            eBugData[FR3]->formationRole = Follower3;
            eBugData[FR3]->leadersID[0] = FR1;
            eBugData[FR3]->leadersID[1] = FR2;
        }

        // registered node number can't be bigger than 5
        if (node_n > 4){
            FR4 = 0;
            for (int i = 0; i<node_n; i++){
                if ((i!=FL)&&(i!=FR1)&&(i!=FR2)&&(i!=FR3)){
                    FR4 = i;
                    eBugData[FR4]->formationRole = Follower4;
                    eBugData[FR4]->leadersID[0] = FR2;
                    eBugData[FR4]->leadersID[1] = FR3;
                }
            }
        }

#if DEBUG
        cout<< "Formation Positions"<<endl;
        cout <<"FL = " <<FL<<endl;
        cout <<"FR1 = "<<FR1<<endl;
        cout <<"FR2 = "<<FR2<<endl;
        cout <<"FR3 = "<<FR3<<endl;
        cout <<"FR4 = "<<FR4<<endl;
#endif

    }

    return 0;
}

int FormationSelection::Init(){


    return 0;
}


int FormationSelection::Start(){

    for (int i =0; i<node_n;i++){

        // Local Error Estimation
        float error_MC,error_DL,error_L,error_C;
        if (i == FL){
            // no error estimation
        } else {
            error_MC = LocalErrorEstimation(i,Max_coverage);
            //            error_DL = LocalErrorEstimation(i,dual_line);
            //            error_L = LocalErrorEstimation(i,line);
            //            error_C = LocalErrorEstimation(i,circle);
        }
    }
    return 0;
}


float FormationSelection::LocalErrorEstimation(int eBugID, int formationType){

    float error_Leader1, error_Leader2, ave_error;
    int formationRole = eBugData[eBugID]->formationRole;

    ////////////*************
    if (formationType == Max_coverage){
        // estimate travelling distance
        error_Leader1 = sqrt((pow(MaxCoverageConstraints[formationRole][0] - eBugData[eBugID]->getDistanceTo(eBugData[eBugData[eBugID]->leadersID[0]]),2) + pow(MaxCoverageConstraints[formationRole][2] - eBugData[eBugID]->getAngleBetween(eBugData[eBugData[eBugID]->leadersID[0]]),2))/2);
        error_Leader2 = sqrt((pow(MaxCoverageConstraints[formationRole][1] - eBugData[eBugID]->getDistanceTo(eBugData[eBugData[eBugID]->leadersID[1]]),2) + pow(MaxCoverageConstraints[formationRole][3] - eBugData[eBugID]->getAngleBetween(eBugData[eBugData[eBugID]->leadersID[1]]),2))/2);
        ave_error = (error_Leader1+error_Leader2)/2;
        if (formationRole == Follower1){
            ave_error = error_Leader1;
        }
    }else if(formationType == Dual_line){
        error_Leader1 = sqrt((pow(DualLineConstraints[formationRole][0] - eBugData[eBugID]->getDistanceTo(eBugData[eBugData[eBugID]->leadersID[0]]),2) + pow(DualLineConstraints[formationRole][2] - eBugData[eBugID]->getAngleBetween(eBugData[eBugData[eBugID]->leadersID[0]]),2))/2);
        error_Leader2 = sqrt((pow(DualLineConstraints[formationRole][1] - eBugData[eBugID]->getDistanceTo(eBugData[eBugData[eBugID]->leadersID[1]]),2) + pow(DualLineConstraints[formationRole][3] - eBugData[eBugID]->getAngleBetween(eBugData[eBugData[eBugID]->leadersID[1]]),2))/2);
        ave_error = (error_Leader1+error_Leader2)/2;
        if (formationRole == Follower1){
            ave_error = error_Leader1;
        }

    }else if(formationType == Line){

    }else if(formationType == Wheel){

    }


    ///////////////*********

    //    // error of leader 1
    //    if (thiseBugData->leadersID[0] != thiseBugData->getID()){
    //        //        int constraint1Err =
    //        //        error1 = pow(thiseBugData->getAngleBetween(eBugData[thiseBugData->leadersID[0]]), 2);

    //    }
    int localError = 0;
    return localError;
}


// ensures that all angles are within -180 to 180 degrees
double FormationSelection::correctDisplayAngle(double angle, int opt)
{
    double minAngle;
    double maxAngle;
    if (opt)
    {
        minAngle = 0;
        maxAngle = 360;
    }
    else
    {
        minAngle = -180;
        maxAngle = 180;
    }

    while (angle < minAngle)
    {
        angle += 360;
    }

    while (angle > maxAngle)
    {
        angle -= 360;
    }

    return angle;
} // correctDisplayAngle()


// theta_actual = getAngleBetween leader eBug - formation_direction
// return the distance from the actual position to its desired position
float FormationSelection::calculateErrorVector(float theta_actual, float l_actual, float theta_constraint, float lConstraint, float *normalError, float *forwardError)
{
    *normalError = lConstraint*sin(PI/180*theta_constraint) - l_actual*sin(PI/180*theta_actual);
    *forwardError =  lConstraint*cos(PI/180*theta_constraint) - l_actual*cos(PI/180*theta_actual);
#if DEBUG
    qDebug("normalError: %.2f forwardError: %.2f", *normalError, *forwardError);
#endif

    return sqrt(pow(*normalError,2)+pow(*forwardError, 2));
} // calculateErrorVector()




XBeeNodeData FormationSelection::convertToNodeData(XBeeNode node){
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

#endif
