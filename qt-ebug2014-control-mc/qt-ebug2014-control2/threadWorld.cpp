#include "threadWorld.h"

ThreadWorld::ThreadWorld() :
    ThreadableQObject()
{

}

/** TODO: Erase
 * @brief ThreadWorld::run
 * Processing thread of world. Has no function.
 */
void ThreadWorld::run(){
    forever{
        qDebug("World Thread Running");
        uiWorldEventNew("World ticking.");
        QThread::sleep(1);
    }
}


/** TODO:Implement
 * @brief ThreadWorld::uiCommsInAddConnection
 * Opens dialog to add new incomming connection
 * then adds it the the listCommsIn
 */
void ThreadWorld::uiCommsInAddConnection(){
    //Open add connection dialog
    //Create new connection from user info
    //Add new connection to listCommsIn
}

/** TODO: Implement
 * @brief ThreadWorld::uiCommsInDeleteConnection
 * Remove a communication thread requested by user/param
 * @param idCommsIn: ID of connection to delete
 */
void ThreadWorld::uiCommsInDeleteConnection(int idCommsIn){
    //Find incomming connection with requested id
    //Delete requested connection (using deleteLater() )
    emit uiCommsInListUpdate("Updated comms list");
}

/** TODO: Implement
 * @brief ThreadWorld::uiCommsOutAddConnection
 * Opens a dialog to add a new outgoing connection
 * and adds it to the listCommsOut. Note that
 */
void ThreadWorld::uiCommsOutAddConnection(){

}

/**
 * @brief ThreadWorld::uiCommsOutDeleteConnection
 * Remove a communication thread requested by user/param
 * @param idCommsOut: ID of connection to delete
 */
void ThreadWorld::uiCommsOutDeleteConnection(int idCommsOut){
    //Find outgoing connection with requested id
    //Delete requested connection  (using deleteLater() )
    emit uiCommsOutListUpdate("Updated listCommsOut");
}
