#include "commsin.h"

CommsIn::CommsIn(QString strPortName) : ThreadableQObject()
{
    //Set up serial port connected to requested address
    port = new QSerialPort(strPortName);
    port->open(QIODevice::ReadOnly);
    connect(port, SIGNAL(readyRead()), this, SLOT(read()));

    //Set up message translator
    messageTranslator = new MessageTranslator();
    connect(messageTranlator, SIGNAL(translationDone(QList<dataRobotLocation>)), this, SLOT(dataReadyToSend(QList<dataRobotLocation>));

    waitingForMoreData = false;
}

/**
 * @brief CommsIn::~CommsIn
 * Called when object deleted.
 * Closes the serial port.
 */
CommsIn::~CommsIn()
{
    qDebug("Closing port");
    port->close();
}

/**
 * @brief CommsIn::run
 * Gets the number of LEDS in message from
 * first 4 bytes then waits until all LED data
 * is on buffer. When all data is in buffer it is
 * copied out and sent to the messageTranslator
 */
void CommsIn::run(){
    forever{
        if(waitingForMoreData)
        {
            //Check if complete message on buffer
            mutexBuffer.lock();
                int availableBytes = buffer.size();
            mutexBuffer.unlock();
            if(availableBytes>=numLeds)
            {
                //Copy off comlete message and send to translator
                waitingForMoreData=false;
                QByteArray bytarCompleteMessage;
                mutexBuffer.lock();
                for(int i=0; i<numLeds; i++)
                        bytarCompleteMessage.append(buffer.pop_front());
                mutexBuffer.unlock();
                messageTranslator.translate(bytarCompleteMessage);
            }
        }
        else
        {
            //Get number of LEDS from firs 4 bytes
            mutexBuffer.lock();
                int availableBytes = buffer.size();
            mutexBuffer.unlock();
            if(availableBytes>=4)
            {
                numLeds=0;
                mutexBuffer.lock();
                    for(int i=0; i<4; i++)
                    {
                        numLeds=buffer.pop_front();
                        numLeds=numLeds<<4;
                    }
                mutexBuffer.unlock();
                if(numLeds>0)
                     waitingForMoreData=true;
            }
        }
    }
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
* @brief CommsIn::read
* Trigged by port.readReady() so starts when any data on serial port.
* Copies all data from serial port into buffer using buffer mutex lock.
*/
void CommsIn::read(){
    qDebug("Adding incomming message to buffer.");

    QByteArray bytarRead;
    bytarRead = port->readAll();
    mutexBuffer.lock();
    for(int i=0; i<bytarRead.size(); i++)
    {
        buffer.append(bytarRead.at(i));
    }
    mutexBuffer.unlock();
}

/**
 * @brief CommsIn::getPortInfo
 * Generates a list of strings containing information about
 * the connected serial port and returns it.
 * @return list of strings of port:
 * name, description, manufacturer name, system location/path,
 * vendor id, product id
 */
QStringList CommsIn::getPortInfo(){
    QSerialPortInfo infoPort(*port);
    QStringList strlstInfoPort;
    strlstInfoPort << infoPort.portName()
         << infoPort.description()
         << infoPort.manufacturer()
         << infoPort.systemLocation()
         << (infoPort.hasVendorIdentifier() ? QString::number(infoPort.vendorIdentifier(), 16) : QString(""))
         << (infoPort.hasProductIdentifier() ? QString::number(infoPort.productIdentifier(), 16) : QString(""));

    return strlstInfoPort;
}
