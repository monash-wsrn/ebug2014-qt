// $Revision: 422 $
// $Date: 2013-08-11 20:09:38 +1000 (Sun, 11 Aug 2013) $
// $Author: nickdademo@gmail.com $
/************************************************************************/
/* qt-opencv-multithreaded:                                             */
/* A multithreaded OpenCV application using the Qt framework.           */
/*                                                                      */
/* XBeeThread.cpp                                                       */
/*                                                                      */
/* Nick D'Ademo <nickdademo@gmail.com>                                  */
/*                                                                      */
/* Copyright (c) 2012-2013 Nick D'Ademo                                 */
/*                                                                      */
/* Permission is hereby granted, free of charge, to any person          */
/* obtaining a copy of this software and associated documentation       */
/* files (the "Software"), to deal in the Software without restriction, */
/* including without limitation the rights to use, copy, modify, merge, */
/* publish, distribute, sublicense, and/or sell copies of the Software, */
/* and to permit persons to whom the Software is furnished to do so,    */
/* subject to the following conditions:                                 */
/*                                                                      */
/* The above copyright notice and this permission notice shall be       */
/* included in all copies or substantial portions of the Software.      */
/*                                                                      */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS  */
/* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   */
/* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    */
/* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     */
/* SOFTWARE.                                                            */
/*                                                                      */
/************************************************************************/

#include "XBeeThread.h"

XBeeThread::XBeeThread(Buffer<QByteArray> *txPacketBuffer, XBeeConnectDialog::Settings p) : QThread(), txPacketBuffer(txPacketBuffer)
{
    // Initialize members
    doStop=false;
    this->threadSleep_ms = p.threadSleep_ms;
    // Attempt to open serial port
    port = new QSerialPort();
    port->setPortName(p.name);
    if(port->open(QIODevice::ReadWrite))
    {
        // Set serial parameters
        if (port->setBaudRate(p.baudRate)
                && port->setDataBits(p.dataBits)
                && port->setParity(p.parity)
                && port->setStopBits(p.stopBits)
                && port->setFlowControl(p.flowControl))
        {
            // Signal/slot connection for serial RX
            connect(port, SIGNAL(readyRead()), this, SLOT(slotRead()));
            // Signal/slot connection for serial TX
            connect(this, SIGNAL(writeData(QByteArray)), this, SLOT(write(QByteArray)));
        }
        // Parameters could not set: close port
        else
            port->close();
    }

}

XBeeThread::~XBeeThread()
{
    // Close serial port if open
    if(port->isOpen())
        port->close();
}

void XBeeThread::run()
{
    while(1)
    {
        ////////////////////////////////
        // Stop thread if doStop=TRUE //
        ////////////////////////////////
        doStopMutex.lock();
        if(doStop)
        {
            doStop=false;
            doStopMutex.unlock();
            break;
        }
        doStopMutex.unlock();
        /////////////////////////////////
        /////////////////////////////////

        // Wait for packet to be present in queue and send (blocking)
        QByteArray packetToSend = txPacketBuffer->get();//if there is no packet to send, XBeeThread is blocked(waiting)
        // Send packet (via signal/slot connection)
        emit writeData(packetToSend);
        msleep(threadSleep_ms);

    }
    qDebug() << "Stopping XBee thread...";
}

void XBeeThread::stop()
{
    QMutexLocker locker(&doStopMutex);
    doStop=true;
}

void XBeeThread::write(const QByteArray &data)
{
    port->write(data);
}

void XBeeThread::slotRead()
{
    // Read all bytes available
    int bytesAvail = port->bytesAvailable();
    QByteArray temp = port->read(bytesAvail);
    // Handle packet (byte-by-byte)
    for(int i=0;i<temp.length();i++)
        readPacket((unsigned char)temp.at(i));
}

void XBeeThread::doNodeDiscovery()
{
    // Local variables
    QByteArray packet;
    // Packet has a fixed size of 8 bytes
    packet.resize(8);
    // Create packet
    packet[0]=0x7e; // Delimiter
    packet[1]=0x00; // Length (MSB)
    packet[2]=0x04; // Length (LSB)
    packet[3]=0x08; // AT Command API frame type
    packet[4]=0x01; // Frame ID (set to non-zero value)
    packet[5]=0x4e; // AT command ('N')
    packet[6]=0x44; // AT command ('D')
    // Calculate checksum and save in packet array
    packet[7]=calculateChecksum(&packet);
    // Send packet
    txPacketBuffer->add(packet);
}

char XBeeThread::calculateChecksum(QByteArray *packet)
{
    // Local variable(s)
    int packetLength;
    int checksumTotal=0;
    // Save packet length from packet fields
    packetLength=((packet->at(1)<<8)&0xFF)|(packet->at(2)&0xFF);
    // Calculate checksum
    for(int i=0;i<packetLength;i++)
        checksumTotal+=packet->at(3+i);
    // Return checksum value
    return (0xFF-checksumTotal);
}

void XBeeThread::readPacket(unsigned char data)
{
    // Static variables
    static unsigned char packet[MAX_XBEE_RXPACKET_LENGTH-3];
    static int index=0;
    static int checksumTotal=0;
    static unsigned char lengthMSB=0;
    static unsigned char lengthLSB=0;
    static int  packetLength=0;

    // New packet start before previous packeted completed: discard previous packet and start over
    if((index>0)&&(data==XBEE_START_DELIMITER))
    {
        index=0;
        checksumTotal=0;
    }

    // Read packet
    switch(index)
    {
        case 0:
            if(data==XBEE_START_DELIMITER)
                index++;
            break;
        case 1:
            lengthMSB=data;
            index++;
            break;
        case 2:
            lengthLSB=data;
            index++;
            // Save packet length
            packetLength=((lengthMSB<<8)&0xFF)|(lengthLSB&0xFF);
            break;
        default:
            // ABORT: Maximum packet size exceeded
            if(index>MAX_XBEE_RXPACKET_LENGTH)
            {
                // Reset variables
                index=0;
                checksumTotal=0;
                break;
            }

            // Checksum
            checksumTotal+=data;

            // END OF PACKET: Verify checksum
            if(index==(packetLength+3))
            {
                // Only take action if checksum passes
                if((checksumTotal&0xFF)==0xFF)
                {
                    // Save API ID/Frame Type
                    unsigned char apiID = packet[0];
                    // ZigBee Receive Packet
                    if(apiID==0x90)
                    {
                    }
                    // AT Command Response
                    else if(apiID==0x88)
                    {
                        handleATCommandResponse(packet);
                    }
                }
                else
                    qDebug()<<"ERROR: XBee checksum fail.";
                // Reset variables
                index=0;
                checksumTotal=0;
            }
            // SAVE PACKET: Starting from Offset=3 (after Length)
            else
            {
                // Save packet
                packet[index-3]=data;
                index++;
            }
            break;
    }
}

void XBeeThread::handleATCommandResponse(unsigned char *packet)
{
    // NODE DISCOVERY
    if((packet[2]==0x4e)&&(packet[3]==0x44))
    {
        // Acquire mutex
        protectXBeeNodeList.lock();
        // Local variables
        int n=0;
        // Create node structure
        XBeeNode node;
        // Save command status regardless of value
        node.commandStatus=packet[4];
        // Command Status
        switch(node.commandStatus)
        {
            // OK
            case 0x00:
                // Save node addresses
                node.MY=(packet[5]<<8)|packet[6];
                node.SH_high=(packet[7]<<8)|packet[8];
                node.SH_low=(packet[9]<<8)|packet[10];
                node.SL_high=(packet[11]<<8)|packet[12];
                node.SL_low=(packet[13]<<8)|packet[14];
                // Save NI string: (Default: (n=1) packet[15)=0x20, packet[16)=0x00)
                node.NI.clear();
                while(packet[15+n]!=0x00)
                {
                    node.NI.append(packet[15+n]);
                    n++;
                }
                // Save remaining data
                node.parentNetworkAddress=(packet[15+n+1]<<8)|packet[15+n+2];
                node.deviceType=packet[15+n+3];
                node.status=packet[15+n+4];
                node.profileID=(packet[15+n+5]<<8)|packet[15+n+6];
                node.manufacturerID=(packet[15+n+7]<<8)|packet[15+n+8];
                // Add to list
                xBeeNodeList.append(node);
                // TODO covert node to XBeeNodeData and set in ebugAPI
                break;
            // ERROR
            case 0x01:
                break;
            // INVALID COMMAND
            case 0x02:
                break;
            // INVALID PARAMETER
            case 0x03:
                break;
            // TX FAILURE
            case 0x04:
                break;
        }
        // Release mutex
        protectXBeeNodeList.unlock();
    }
}

void XBeeThread::nodeDiscoveryTimeout()
{
    // Wait for current node discovery iteration to finish (if in progress)
    QMutexLocker locker(&protectXBeeNodeList);
    // Send data to controlThread and GUI thread
    emit newXBeeNodeData(xBeeNodeList);
}
