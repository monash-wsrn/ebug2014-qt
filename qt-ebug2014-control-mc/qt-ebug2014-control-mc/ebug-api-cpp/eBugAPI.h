/************************************************************************/
/* eBugAPI.h                                                            */
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

/* COMPATIBLE EBUG FIRMWARE VERSION(s): v1.18 */

/* API functions are in the format: Type_SubType() */

////////////////
// Interfaces //
////////////////
// 1. Expansion Header  :   eBug Packet with Frame Start=0x7E
// 2. XBee              :   XBee Transmit Request API packet with payload containing an eBug Packet with Frame Start=0x00

////////////////////////
// eBug Packet Format //
////////////////////////
// Packet[0]            :   Frame Start [Interface: XBee=0x00, Expansion Header=0x7E]
// Packet[1]            :   Packet Length (number of bytes between Packet Length and Checksum)
// Packet[2]            :   Options {Checksum Enable (1-bit), Class (4-bits), Reserved (3-bits)} where Class => 0x0 [eBug Command], 0x1 [eBug Response]
// Packet[3]            :   Type
// Packet[4]            :   Sub-Type
// Packet[5+n]          :   Payload (n bytes)
// Packet[5+n+1]        :   Checksum [Optional: set to 0x00 if unused]

#ifndef EBUGAPI_H
#define EBUGAPI_H

#include <vector>
#include <string>
#include <inttypes.h>

#define EBUG_PACKET_FRAMESTART_XBEE 0x00
#define EBUG_PACKET_FRAMESTART_EH   0x7E

/* Structure definitions */
struct XBeeNodeData {
    unsigned char commandStatus;
    unsigned int MY;
    unsigned int SH_high;
    unsigned int SH_low;
    unsigned int SL_high;
    unsigned int SL_low;
    std::string NI;
    unsigned int parentNetworkAddress;
    unsigned char deviceType;
    unsigned char status;
    unsigned int profileID;
    unsigned int manufacturerID;
};

/* Enumerations */
typedef enum {
    XBEE                    = 0,
    EXPANSION_HEADER        = 1
} InterfaceType_t;

typedef enum {
    FULL                    = 0,
    HALF                    = 1,
    QUARTER                 = 2,
    EIGHTH                  = 3,
    SIXTEENTH               = 8
} StepperMotor_Mode_t;

typedef enum {
    BACKWARDS               = 0,
    FORWARDS                = 1
} StepperMotor_Direction_t;

typedef enum {
    DST_XBEE                = 0,
    DST_EXPANSION_HEADER    = 1
} SendData_Destination_t;

/* Class definition */
class eBugAPI
{
    public:
        eBugAPI(InterfaceType_t intefaceType);
        void setInterfaceType(InterfaceType_t interfaceType);
        InterfaceType_t getInterfaceType(void);
        void setXBeeNodeData(struct XBeeNodeData xBeeNodeData);
        struct XBeeNodeData getXBeeNodeData(void);
        std::vector<char> XBee_ATCommand_ND(void);
        /* SYSTEM */
        std::vector<char> System_SendData(SendData_Destination_t dest, uint8_t *data, uint8_t length, uint8_t options);
        /* STEPPERMOTOR */
        std::vector<char> StepperMotor_LeftStep(uint16_t freqHz, uint16_t nSteps, StepperMotor_Direction_t direction, StepperMotor_Mode_t mde, bool rgbON, uint8_t options);
        std::vector<char> StepperMotor_RightStep(uint16_t freqHz, uint16_t nSteps, StepperMotor_Direction_t direction, StepperMotor_Mode_t mode, bool rgbON, uint8_t options);
        std::vector<char> StepperMotor_LeftRightStep(uint16_t freqHz1, uint16_t nSteps1, StepperMotor_Direction_t direction1, StepperMotor_Mode_t mode1, bool rgbON1,
                                                     uint16_t freqHz2, uint16_t nSteps2, StepperMotor_Direction_t direction2, StepperMotor_Mode_t mode2, bool rgbON2,
                                                     uint8_t options);
        std::vector<char> StepperMotor_LeftRightStop(bool disable1, bool disable2, uint8_t options);
        std::vector<char> StepperMotor_LeftStop(bool disable, uint8_t options);
        std::vector<char> StepperMotor_RightStop(bool disable, uint8_t options);
        std::vector<char> StepperMotor_CWStep(uint16_t freqHz, uint16_t nSteps, StepperMotor_Mode_t mode, bool rgbON, uint8_t options);
        std::vector<char> StepperMotor_CCWStep(uint16_t freqHz, uint16_t nSteps, StepperMotor_Mode_t mode, bool rgbON, uint8_t options);
        /* RGBLEDs */
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
        /* ADC */
        std::vector<char> ADC_LM35GetTemperature(uint8_t options);
        
    private:
        InterfaceType_t interfaceType;
        struct XBeeNodeData xBeeNodeData;
        char eBugPacketFrameStart;
        char calculateChecksum(std::vector<char> packet);
        std::vector<char> eBugPacketToXBeePacket(const std::vector<char> eBugPacket);
};
                
#endif // EBUGAPI_H
