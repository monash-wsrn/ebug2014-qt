/************************************************************************/
/* eBugAPI.cpp                                                          */
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

#include "eBugAPI.h"

eBugAPI::eBugAPI(InterfaceType_t interfaceType)
{
    /* Set eBug Packet frame start byte */
    if(interfaceType==XBEE)
        eBugPacketFrameStart = EBUG_PACKET_FRAMESTART_XBEE;
    else if(interfaceType==EXPANSION_HEADER)
        eBugPacketFrameStart = EBUG_PACKET_FRAMESTART_EH;

    /* Save interface type */
    this->interfaceType = interfaceType;
}

void eBugAPI::setInterfaceType(InterfaceType_t interfaceType)
{
    /* Set eBug Packet frame start byte */
    if(interfaceType==XBEE)
        eBugPacketFrameStart = EBUG_PACKET_FRAMESTART_XBEE;
    else if(interfaceType==EXPANSION_HEADER)
        eBugPacketFrameStart = EBUG_PACKET_FRAMESTART_EH;

    /* Save interface type */
    this->interfaceType = interfaceType;
}

InterfaceType_t eBugAPI::getInterfaceType(void)
{
    return interfaceType;
}

void eBugAPI::setXBeeNodeData(struct XBeeNodeData xBeeNodeData)
{
    this->xBeeNodeData.commandStatus         = xBeeNodeData.commandStatus;
    this->xBeeNodeData.MY                    = xBeeNodeData.MY;
    this->xBeeNodeData.SH_high               = xBeeNodeData.SH_high;
    this->xBeeNodeData.SH_low                = xBeeNodeData.SH_low;
    this->xBeeNodeData.SL_high               = xBeeNodeData.SL_high;
    this->xBeeNodeData.SL_low                = xBeeNodeData.SL_low;
    this->xBeeNodeData.NI                    = xBeeNodeData.NI;
    this->xBeeNodeData.parentNetworkAddress  = xBeeNodeData.parentNetworkAddress;
    this->xBeeNodeData.deviceType            = xBeeNodeData.deviceType;
    this->xBeeNodeData.status                = xBeeNodeData.status;
    this->xBeeNodeData.profileID             = xBeeNodeData.profileID;
    this->xBeeNodeData.manufacturerID        = xBeeNodeData.manufacturerID;
}

struct XBeeNodeData eBugAPI::getXBeeNodeData(void)
{
    return xBeeNodeData;
}

char eBugAPI::calculateChecksum(const std::vector<char> packet)
{
    /* Local variables */
    int startIndex=0;
    int packetLength=0;
    int checksumTotal=0;

    /* Set interface-dependent variables */
    if(interfaceType==XBEE)
    {
        startIndex=3;
        packetLength=((packet[1]<<8)&0xFF)+(packet[2]&0xFF);
    }
    else if(interfaceType==EXPANSION_HEADER)
    {
        startIndex=2;
        packetLength=packet[1];
    }

    /* Calculate checksum */
    for(int i=0;i<packetLength;i++)
        checksumTotal+=packet[startIndex+i];

    /* Return checksum value */
    return (0xFF-checksumTotal);
}

std::vector<char> eBugAPI::eBugPacketToXBeePacket(const std::vector<char> eBugPacket)
{
    /* Create packet and set size */
    std::vector<char> packet(eBugPacket.size()+18);

    /* Delimiter */
    packet[0]=0x7E;
    /* Length */
    if((14+eBugPacket.size())>255)
    {
        packet[1]=((14+eBugPacket.size())>>8)&0xFF; // MSB
        packet[2]=(14+eBugPacket.size())&0xFF;      // LSB
    }
    else
    {
        packet[1]=0x00;                             // MSB
        packet[2]=14+eBugPacket.size();             // LSB
    }
    packet[3]=0x10; // ZigBee Transmit Request frame type
    packet[4]=0x00; // Frame ID (set to zero: no Transmit Status packet sent from destination)
    /* Serial Number (64-bit destination address) */
    packet[5]=(xBeeNodeData.SH_high>>8)&0xFF;       // MSB
    packet[6]=xBeeNodeData.SH_high&0xFF;
    packet[7]=(xBeeNodeData.SH_low>>8)&0xFF;
    packet[8]=xBeeNodeData.SH_low&0xFF;
    packet[9]=(xBeeNodeData.SL_high>>8)&0xFF;
    packet[10]=xBeeNodeData.SL_high&0xFF;
    packet[11]=(xBeeNodeData.SL_low>>8)&0xFF;
    packet[12]=xBeeNodeData.SL_low&0xFF;            // LSB
    /* Network Address (16-bit) */
    packet[13]=(xBeeNodeData.MY>>8)&0xFF;           // MSB
    packet[14]=(xBeeNodeData.MY)&0xFF;              // LSB
    /* Broadcast Radius */
    packet[15]=0x00;
    /* Options */
    packet[16]=0x00;
    /* Payload: eBug Packet */
    for(unsigned int i=0;i<eBugPacket.size();i++)
        packet[17+i]=eBugPacket[i];
    /* Checksum */
    packet[17+eBugPacket.size()]=calculateChecksum(packet);

    /* Return packet to caller */
    return packet;
}

std::vector<char> eBugAPI::XBee_ATCommand_ND(void)
{
    /* Packet has a fixed size of 8 bytes */
    std::vector<char> packet(8);

    /* Create packet */
    packet[0]=0x7e; // Delimiter
    packet[1]=0x00; // Length (MSB)
    packet[2]=0x04; // Length (LSB)
    packet[3]=0x08; // AT Command API frame type
    packet[4]=0x01; // Frame ID (set to non-zero value)
    packet[5]=0x4e; // AT command ('N')
    packet[6]=0x44; // AT command ('D')
    /* Checksum */
    packet[7]=calculateChecksum(packet);

    /* Return packet to caller */
    return packet;
}

////////////
// SYSTEM //
////////////

/* RESPONSE: NONE */
std::vector<char> eBugAPI::System_SendData(SendData_Destination_t dest, uint8_t *data, uint8_t length, uint8_t options)
{
    /* Packet has a fixed size of (6 + length) bytes */
    std::vector<char> packet(6+length);

    /* Create packet */
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=4+length;             // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x00;                 // Type: System
    packet[4]=0x00;                 // Sub-Type: SendData
    packet[5]=dest;
    
    /* Set payload */
    for(int i=0; i<length; i++)
        packet[6+i]=data[i];
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[6+length]=calculateChecksum(packet);   
    else
        packet[6+length]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

//////////////////
// STEPPERMOTOR //
//////////////////

/* RESPONSE: NONE */
std::vector<char> eBugAPI::StepperMotor_LeftStep(uint16_t freqHz, uint16_t nSteps, StepperMotor_Direction_t direction, StepperMotor_Mode_t mode, bool rgbON, uint8_t options)
{
    /* Packet has a fixed size of 11 bytes */
    std::vector<char> packet(11);

    /* Create packet */
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x08;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x01;                 // Type: StepperMotor
    packet[4]=0x00;                 // Sub-Type: LeftStep
    
    packet[5]=(freqHz>>8)&0xFF;     // Frequency (upper-byte)
    packet[6]=(freqHz)&0xFF;        // Frequency (lower-byte)
    packet[7]=(nSteps>>8)&0xFF;     // Number of steps (upper-byte)
    packet[8]=(nSteps)&0xFF;        // Number of steps (lower-byte)
    packet[9]=(mode&0xF)|(direction<<4)|(rgbON<<5);
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[10]=calculateChecksum(packet);   
    else
        packet[10]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::StepperMotor_RightStep(uint16_t freqHz, uint16_t nSteps, StepperMotor_Direction_t direction, StepperMotor_Mode_t mode, bool rgbON, uint8_t options)
{
    /* Packet has a fixed size of 11 bytes */
    std::vector<char> packet(11);

    /* Create packet */
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x08;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x01;                 // Type: StepperMotor
    packet[4]=0x01;                 // Sub-Type: RightStep
    
    packet[5]=(freqHz>>8)&0xFF;     // Frequency (upper-byte)
    packet[6]=(freqHz)&0xFF;        // Frequency (lower-byte)
    packet[7]=(nSteps>>8)&0xFF;     // Number of steps (upper-byte)
    packet[8]=(nSteps)&0xFF;        // Number of steps (lower-byte)
    packet[9]=(mode&0xF)|(direction<<4)|(rgbON<<5);
    
    /* Checksum */
    if((options & 0x80)==0x80)
        packet[10]=calculateChecksum(packet);
    else
        packet[10]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::StepperMotor_LeftRightStep(uint16_t freqHz1, uint16_t nSteps1, StepperMotor_Direction_t direction1, StepperMotor_Mode_t mode1, bool rgbON1,
                                                      uint16_t freqHz2, uint16_t nSteps2, StepperMotor_Direction_t direction2, StepperMotor_Mode_t mode2, bool rgbON2,
                                                      uint8_t options)
{
    /* Packet has a fixed size of 16 bytes */
    std::vector<char> packet(16);

    /* Create packet */
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x0D;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x01;                 // Type: StepperMotor
    packet[4]=0x05;                 // Sub-Type: LeftRightStep
    
    packet[5]=(freqHz1>>8)&0xFF;    // Frequency (upper-byte)
    packet[6]=(freqHz1)&0xFF;       // Frequency (lower-byte)
    packet[7]=(nSteps1>>8)&0xFF;    // Number of steps (upper-byte)
    packet[8]=(nSteps1)&0xFF;       // Number of steps (lower-byte)
    packet[9]=(mode1&0xF)|(direction1<<4)|(rgbON1<<5);
    
    packet[10]=(freqHz2>>8)&0xFF;   // Frequency (upper-byte)
    packet[11]=(freqHz2)&0xFF;      // Frequency (lower-byte)
    packet[12]=(nSteps2>>8)&0xFF;   // Number of steps (upper-byte)
    packet[13]=(nSteps2)&0xFF;      // Number of steps (lower-byte)
    packet[14]=(mode2&0xF)|(direction2<<4)|(rgbON2<<5);
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[15]=calculateChecksum(packet);   
    else
        packet[15]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::StepperMotor_LeftRightStop(bool disable1, bool disable2, uint8_t options)
{
    /* Packet has a fixed size of 8 bytes */
    std::vector<char> packet(8);

    /* Create packet */
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x05;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x01;                 // Type: StepperMotor
    packet[4]=0x04;                 // Sub-Type: LeftRightStop
    packet[5]= disable1 ? 0x01 : 0x00;
    packet[6]= disable2 ? 0x01 : 0x00;
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[7]=calculateChecksum(packet);    
    else
        packet[7]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::StepperMotor_LeftStop(bool disable, uint8_t options)
{
    /* Packet has a fixed size of 7 bytes */
    std::vector<char> packet(7);

    /* Create packet */ 
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x04;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x01;                 // Type: StepperMotor
    packet[4]=0x02;                 // Sub-Type: LeftStop
    packet[5]= disable ? 0x01 : 0x00;
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[6]=calculateChecksum(packet);    
    else
        packet[6]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::StepperMotor_RightStop(bool disable, uint8_t options)
{
    /* Packet has a fixed size of 7 bytes */
    std::vector<char> packet(7);

    /* Create packet */ 
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x04;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x01;                 // Type: StepperMotor
    packet[4]=0x03;                 // Sub-Type: RightStop
    packet[5]= disable ? 0x01 : 0x00;
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[6]=calculateChecksum(packet);    
    else
        packet[6]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::StepperMotor_CWStep(uint16_t freqHz, uint16_t nSteps, StepperMotor_Mode_t mode, bool rgbON, uint8_t options)
{

    return StepperMotor_LeftRightStep(freqHz,nSteps,FORWARDS,mode,rgbON,freqHz,nSteps,BACKWARDS,mode,rgbON,options);
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::StepperMotor_CCWStep(uint16_t freqHz, uint16_t nSteps, StepperMotor_Mode_t mode, bool rgbON, uint8_t options)
{

    return StepperMotor_LeftRightStep(freqHz,nSteps,BACKWARDS,mode,rgbON,freqHz,nSteps,FORWARDS,mode,rgbON,options);
}

/////////////
// RGBLEDs //
/////////////

/* RESPONSE: NONE */
std::vector<char> eBugAPI::RGBLEDs_SetAllRed(uint16_t value, uint8_t options)
{
    /* Packet has a fixed size of 8 bytes */
    std::vector<char> packet(8);

    /* Create packet */ 
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x05;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x02;                 // Type: RGBLEDs
    packet[4]=0x00;                 // Sub-Type: SetAllRed
    packet[5]=(value>>8)&0xFF;
    packet[6]=(value)&0xFF;
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[7]=calculateChecksum(packet);    
    else
        packet[7]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::RGBLEDs_SetAllGreen(uint16_t value, uint8_t options)
{
    /* Packet has a fixed size of 8 bytes */
    std::vector<char> packet(8);

    /* Create packet */   
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x05;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x02;                 // Type: RGBLEDs
    packet[4]=0x01;                 // Sub-Type: SetAllGreen
    packet[5]=(value>>8)&0xFF;
    packet[6]=(value)&0xFF;
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[7]=calculateChecksum(packet);    
    else
        packet[7]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::RGBLEDs_SetAllBlue(uint16_t value, uint8_t options)
{
    /* Packet has a fixed size of 8 bytes */
    std::vector<char> packet(8);

    /* Create packet */
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x05;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x02;                 // Type: RGBLEDs
    packet[4]=0x02;                 // Sub-Type: SetAllBlue
    packet[5]=(value>>8)&0xFF;
    packet[6]=(value)&0xFF;
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[7]=calculateChecksum(packet);    
    else
        packet[7]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::RGBLEDs_SetAll(uint16_t value, uint8_t options)
{
    /* Packet has a fixed size of 8 bytes */
    std::vector<char> packet(8);

    /* Create packet */ 
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x05;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x02;                 // Type: RGBLEDs
    packet[4]=0x03;                 // Sub-Type: SetAll
    packet[5]=(value>>8)&0xFF;
    packet[6]=(value)&0xFF;
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[7]=calculateChecksum(packet);    
    else
        packet[7]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::RGBLEDs_SetAllOff(uint8_t options)
{
    /* Packet has a fixed size of 6 bytes */
    std::vector<char> packet(6);

    /* Create packet */
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x03;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x02;                 // Type: RGBLEDs
    packet[4]=0x04;                 // Sub-Type: SetAllOff
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[5]=calculateChecksum(packet);    
    else
        packet[5]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::RGBLEDs_SetRed(uint8_t led, uint16_t value, uint8_t options)
{
    /* Packet has a fixed size of 9 bytes */
    std::vector<char> packet(9);

    /* Create packet */  
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x06;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x02;                 // Type: RGBLEDs
    packet[4]=0x05;                 // Sub-Type: SetRed
    packet[5]=led;
    packet[6]=(value>>8)&0xFF;
    packet[7]=(value)&0xFF;
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[8]=calculateChecksum(packet);    
    else
        packet[8]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::RGBLEDs_SetGreen(uint8_t led, uint16_t value, uint8_t options)
{
    /* Packet has a fixed size of 9 bytes */
    std::vector<char> packet(9);

    /* Create packet */ 
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x06;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x02;                 // Type: RGBLEDs
    packet[4]=0x06;                 // Sub-Type: SetGreen 
    packet[5]=led;
    packet[6]=(value>>8)&0xFF;
    packet[7]=(value)&0xFF;
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[8]=calculateChecksum(packet);    
    else
        packet[8]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::RGBLEDs_SetBlue(uint8_t led, uint16_t value, uint8_t options)
{
    /* Packet has a fixed size of 9 bytes */
    std::vector<char> packet(9);

    /* Create packet */  
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x06;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x02;                 // Type: RGBLEDs
    packet[4]=0x07;                 // Sub-Type: SetBlue
    packet[5]=led;
    packet[6]=(value>>8)&0xFF;
    packet[7]=(value)&0xFF;
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[8]=calculateChecksum(packet);    
    else
        packet[8]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::RGBLEDs_SetMultiple(uint16_t mask1, uint16_t mask2, uint16_t mask3, uint16_t value, uint8_t options)
{
    /* Packet has a fixed size of 14 bytes */
    std::vector<char> packet(14);

    /* Create packet */  
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x0B;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x02;                 // Type: RGBLEDs
    packet[4]=0x08;                 // Sub-Type: SetMultiple
    packet[5]=(mask1>>8)&0xFF;
    packet[6]=(mask1)&0xFF;
    packet[7]=(mask2>>8)&0xFF;
    packet[8]=(mask2)&0xFF;
    packet[9]=(mask3>>8)&0xFF;
    packet[10]=(mask3)&0xFF;
    packet[11]=(value>>8)&0xFF;
    packet[12]=(value)&0xFF;
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[13]=calculateChecksum(packet);   
    else
        packet[13]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::RGBLEDs_SetMultipleHold(uint16_t mask1, uint16_t mask2, uint16_t mask3, uint16_t value, uint16_t mask, uint8_t options)
{
    /* Packet has a fixed size of 16 bytes */
    std::vector<char> packet(16);

    /* Create packet */  
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x0D;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x02;                 // Type: RGBLEDs
    packet[4]=0x09;                 // Sub-Type: SetMultipleHold
    packet[5]=(mask1>>8)&0xFF;
    packet[6]=(mask1)&0xFF;
    packet[7]=(mask2>>8)&0xFF;
    packet[8]=(mask2)&0xFF;
    packet[9]=(mask3>>8)&0xFF;
    packet[10]=(mask3)&0xFF;
    packet[11]=(value>>8)&0xFF;
    packet[12]=(value)&0xFF;
    packet[13]=(mask>>8)&0xFF;
    packet[14]=(mask)&0xFF;
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[15]=calculateChecksum(packet);   
    else
        packet[15]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::RGBLEDs_SetAllHold(uint16_t value, uint16_t mask, uint8_t options)
{
    // Packet has a fixed size of 10 bytes
    std::vector<char> packet(10);

    /* Create packet */  
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x07;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x02;                 // Type: RGBLEDs
    packet[4]=0x0A;                 // Sub-Type: SetAllHold
    packet[5]=(value>>8)&0xFF;
    packet[6]=(value)&0xFF;
    packet[7]=(mask>>8)&0xFF;
    packet[8]=(mask)&0xFF;
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[9]=calculateChecksum(packet);    
    else
        packet[9]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/* RESPONSE: NONE */
std::vector<char> eBugAPI::RGBLEDs_SetAllRGB(uint16_t r_value, uint16_t g_value, uint16_t b_value, uint8_t options)
{
    // Packet has a fixed size of 12 bytes
    std::vector<char> packet(12);

    /* Create packet */  
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x07;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x02;                 // Type: RGBLEDs
    packet[4]=0x0B;                 // Sub-Type: SetAllRGB
    packet[5]=(r_value>>8)&0xFF;
    packet[6]=(r_value)&0xFF;
    packet[7]=(g_value>>8)&0xFF;
    packet[8]=(g_value)&0xFF;
    packet[9]=(b_value>>8)&0xFF;
    packet[10]=(b_value)&0xFF;
    
    /* Checksum */
    if((options&0x80)==0x80)
        packet[11]=calculateChecksum(packet);    
    else
        packet[11]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}

/////////
// ADC //
/////////

/* RESPONSE: 16-bit value read from ADC (divide this value by 20 to get temperature in degrees Celcius). */
std::vector<char> eBugAPI::ADC_LM35GetTemperature(uint8_t options)
{
    /* Packet has a fixed size of 6 bytes */
    std::vector<char> packet(6);

    /* Create packet */   
    packet[0]=eBugPacketFrameStart; // Frame Start
    packet[1]=0x03;                 // Packet Length
    packet[2]=options;              // Options
    packet[3]=0x03;                 // Type: ADC
    packet[4]=0x00;                 // Sub-Type: LM35GetTemperature

    /* Checksum */
    if((options&0x80)==0x80)
        packet[5]=calculateChecksum(packet);    
    else
        packet[5]=0x00;

    /* Return packet to caller */
    if(interfaceType==XBEE)
        return eBugPacketToXBeePacket(packet);
    else if(interfaceType==EXPANSION_HEADER)
        return packet;
    return packet;
}
