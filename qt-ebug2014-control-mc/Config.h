// $Revision: 422 $
// $Date: 2013-08-11 20:09:38 +1000 (Sun, 11 Aug 2013) $
// $Author: nickdademo@gmail.com $
/************************************************************************/
/* qt-opencv-multithreaded:                                             */
/* A multithreaded OpenCV application using the Qt framework.           */
/*                                                                      */
/* Config.h                                                             */
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

#ifndef CONFIG_H
#define CONFIG_H

// Qt
#include <QtWidgets>
// QtSerialPort
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#define DEBUG                               1

// XBEE DEFAULTS
// Default serial device parameters
#define DEFAULT_SERIAL_BAUDRATE             QSerialPort::Baud115200
#define DEFAULT_SERIAL_DATABITS             QSerialPort::Data8
#define DEFAULT_SERIAL_STOPBITS             QSerialPort::OneStop
#define DEFAULT_SERIAL_PARITY               QSerialPort::NoParity
#define DEFAULT_SERIAL_FLOWCONTROL          QSerialPort::NoFlowControl
// Default XBee packet buffer size
#define DEFAULT_XBEE_PACKET_BUFFER_SIZE     100
// Default XBee thread sleep time (ms)
#define DEFAULT_XBEE_THREAD_SLEEP_MS        1
// Drop packet if buffer is full
#define DEFAULT_DROP_PACKETS                false
// Node Discovery
#define DEFAULT_XBEE_ND_DURATION            10
#define DEFAULT_MIN_XBEE_ND_DURATION        10
// XBee Start Delimiters
#define XBEE_START_DELIMITER                0x7E
// Position of API ID field in XBee packet (where index=0 corresponds to the Start Delimiter)
#define API_ID_INDEX                        3
// The RF Data contained in the ZigBee API frame is the eBug Packet
// Note: XBee ZB firmware includes a command (ATNP) that returns the maximum number of RF payload bytes
//       that can be sent in a unicast transmission (API TX Request Frame).
// The following assumes: Security not enabled, source routing and fragmentation not used
#define MAX_EBUG_PACKET_LENGTH              84
// Maximum size (in bytes) of the ZigBee Receive Packet (API ID=0x90)
// 16 bytes = header + checksum
#define MAX_XBEE_RXPACKET_LENGTH            (16+MAX_EBUG_PACKET_LENGTH)

// FPS statistics queue lengths
#define PROCESSING_FPS_STAT_QUEUE_LENGTH    32
#define CAPTURE_FPS_STAT_QUEUE_LENGTH       32

// Image buffer size
#define DEFAULT_IMAGE_BUFFER_SIZE           1
// Drop frame if buffer is full
#define DEFAULT_DROP_FRAMES                 false
// Thread priorities
#define DEFAULT_CAP_THREAD_PRIO             QThread::NormalPriority
#define DEFAULT_PROC_THREAD_PRIO            QThread::HighPriority
#define DEFAULT_XBEE_THREAD_PRIO            QThread::NormalPriority

// IMAGE PROCESSING
// Smooth
#define DEFAULT_SMOOTH_TYPE                 0 // Options: [BLUR=0,GAUSSIAN=1,MEDIAN=2]
#define DEFAULT_SMOOTH_PARAM_1              3
#define DEFAULT_SMOOTH_PARAM_2              3
#define DEFAULT_SMOOTH_PARAM_3              0
#define DEFAULT_SMOOTH_PARAM_4              0
// Dilate
#define DEFAULT_DILATE_ITERATIONS           1
// Erode
#define DEFAULT_ERODE_ITERATIONS            1
// Flip
#define DEFAULT_FLIP_CODE                   0 // Options: [x-axis=0,y-axis=1,both axes=-1]
// Canny
#define DEFAULT_CANNY_THRESHOLD_1           10
#define DEFAULT_CANNY_THRESHOLD_2           00
#define DEFAULT_CANNY_APERTURE_SIZE         3.2
#define DEFAULT_CANNY_L2GRADIENT            false

// Control Algorithms
#define SENSOR_RANGE                                250
#define THRESHOLD                                   50
#define FORMATION_DIST                              SENSOR_RANGE-THRESHOLD
#define DUAL_DIST                                   FORMATION_DIST/1.4142
#define WHEEL_RADIUS                                (FORMATION_DIST/2)/0.5878
#define GOAL_X										320
#define GOAL_Y										240
#define PI 											3.1415926
#define V_TO_FREQ_CONSTANT							19.60784
#define W_TO_FREQ_CONSTANT 							8.6949
#define MAX_STEPPER_MOTOR_FREQ 						9000
#define MIN_STEPPER_MOTOR_FREQ                      500
#define MAX_FORMATION_TYPE 							3
#define Pu                                          0.55 //seconds
#define Ku_ang                                      3.6
#define Ku_lin                                      0.045
#define Ca                                          180 //230 before
#define Cr                                          240
#define la                                          200
#define lr                                          75
#define Cp                                          1
#define Ct                                          1.25


#endif // CONFIG_H
