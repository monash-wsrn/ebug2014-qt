#include "commsin.h"

CommsIn::CommsIn(QObject *parent) : QThread(parent)
{
    QObject::connect(&messageTranslator, SIGNAL(translationDone(QList<dataRobotLocation>)), SLOT(dataReadyToSend(QList<dataRobotLocation>)));
    QObject::connect(&messageTranslator, SIGNAL(ledsDone(QList<QString>)), SLOT(dataLedsTranslated(QList<QString>)));
}

/**
 * @brief CommsIn::~CommsIn
 * Called when object deleted.
 * Closes the serial port.
 */
CommsIn::~CommsIn()
{
    //Stop thread
    quit=true;
    QThread::wait();
    //Close port
    qDebug("Closing port");
    port.close();
}

void CommsIn::startComms(const QString &portName)
{
    //Stop thread if already running
    quit=true;
    QThread::wait();

    //Close port
    port.close();
    port.setPortName(portName);
    if(!port.open(QIODevice::ReadOnly)){
        qDebug("Can't open port!");
        return;
    }
    qDebug("CommsIn: Port open.");

    //Start thread
    QThread::start();

}

/**
 * @brief CommsIn::run
 * Gets the number of LEDS in message from
 * first 4 bytes then waits until all LED data
 * is on buffer. When all data is in buffer it is
 * copied out and sent to the messageTranslator
 */
void CommsIn::run(){
    //Set stopping flag to true
    quit=false;
    fillingBuffer = false;
    while(!quit)
    {
        qDebug("CommsIn: Running.");
        if(port.waitForReadyRead(timeoutWait))
        {
            qDebug("CommsIn: Reading port.");
            //Data ready to read so read it
            QByteArray bytarRawRead = port.readAll();
            while(port.waitForReadyRead(10)){
//TODO: Getting "double-free or corruption" error here
                bytarRawRead.append(port.readAll());
            }
            qDebug("CommsIn: Adding read to buffer.");
            //Add read data to buffer
            mutexBuffer.lock();
                for(int i=0; i<bytarRawRead.size(); i++)
                {
                    buffer.append(bytarRawRead.at(i));
                }
            mutexBuffer.unlock();

            //Process buffer
            if(fillingBuffer)
            {
                //Check if message on buffer is size of numLeds
                mutexBuffer.lock();
                    int availableBytes = buffer.size();
                mutexBuffer.unlock();
                if(availableBytes>=numLeds)
                {
                    //Copy off comlete message and send to translator
                    fillingBuffer=false;
                    QByteArray bytarCompleteMessage;
                    mutexBuffer.lock();
                    for(int i=0; i<numLeds; i++)
                            bytarCompleteMessage.append(buffer.takeFirst());
                    mutexBuffer.unlock();
                    messageTranslator.translate(bytarCompleteMessage);
                }else{
                    //Need more data on buffer
                    qDebug("CommsIn: Need more data on buffer before translate");
                }
            }
            else
            {
                qDebug("CommsIn: Getting first 4 bytes for numLeds");
                //Get number of LEDS from firs 4 bytes
                mutexBuffer.lock();
                    int availableBytes = buffer.size();
                    qDebug() << availableBytes;
                mutexBuffer.unlock();
                if(availableBytes>=4)
                {
                    numLeds=0;
                    mutexBuffer.lock();
                        for(int i=0; i<4; i++)
                        {
                            numLeds=buffer.takeFirst();
                            numLeds=numLeds<<4;
                        }
                    mutexBuffer.unlock();
                    if(numLeds>0)
                         fillingBuffer=true;
                }
            }

        }else{
            qDebug("Reached timout with no message.");
        }





    }//while(!quit)

}



/**
 * @brief CommsIn::dataReadyToSend
 * Trigged by translator finishing message.
 * Emmits newRobotData signal containing tranlated data.
 */
void CommsIn::dataReadyToSend(QList<dataRobotLocation> listRobotLocations)
{
    emit newRobotData(listRobotLocations);
}



/**
 * @brief CommsIn::dataLedsTranslated
 * Intermediate passing of translated led messages from translator
 */
void CommsIn::dataLedsTranslated(QList<QString> lststrLeds)
{
    emit newLedData(lststrLeds);
}


///**
//* @brief CommsIn::read
//* Trigged by port.readReady() so starts when any data on serial port.
//* Copies all data from serial port into buffer using buffer mutex lock.
//*/
//void CommsIn::read(){
//    qDebug("Adding incomming message to buffer.");

//    QByteArray bytarRead;
//    bytarRead = port->readAll();
//    emit newRawData(bytarRead);
//    mutexBuffer.lock();
//    for(int i=0; i<bytarRead.size(); i++)
//    {
//        buffer.append(bytarRead.at(i));
//    }
//    mutexBuffer.unlock();
//}

/**
 * @brief CommsIn::getPortInfo
 * Generates a list of strings containing information about
 * the connected serial port and returns it.
 * @return list of strings of port:
 * name, description, manufacturer name, system location/path,
 * vendor id, product id
 */
QStringList CommsIn::getPortInfo(){
    QSerialPortInfo infoPort(port);
    QStringList strlstInfoPort;
    strlstInfoPort << infoPort.portName()
         << infoPort.description()
         << infoPort.manufacturer()
         << infoPort.systemLocation()
         << (infoPort.hasVendorIdentifier() ? QString::number(infoPort.vendorIdentifier(), 16) : QString(""))
         << (infoPort.hasProductIdentifier() ? QString::number(infoPort.productIdentifier(), 16) : QString(""));

    return strlstInfoPort;
}
