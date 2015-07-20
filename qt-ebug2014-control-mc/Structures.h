// $Revision: 422 $
// $Date: 2013-08-11 20:09:38 +1000 (Sun, 11 Aug 2013) $
// $Author: nickdademo@gmail.com $
/************************************************************************/
/* qt-opencv-multithreaded:                                             */
/* A multithreaded OpenCV application using the Qt framework.           */
/*                                                                      */
/* Structures.h                                                         */
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

#ifndef STRUCTURES_H
#define STRUCTURES_H

// Qt
#include <QtCore/QRect>
// Local
#include <Config.h>

struct ImageProcessingSettings{
    int smoothType;
    int smoothParam1;
    int smoothParam2;
    double smoothParam3;
    double smoothParam4;
    int dilateNumberOfIterations;
    int erodeNumberOfIterations;
    int flipCode;
    double cannyThreshold1;
    double cannyThreshold2;
    int cannyApertureSize;
    bool cannyL2gradient;
};

struct ImageProcessingFlags{
    bool grayscaleOn;
    bool smoothOn;
    bool dilateOn;
    bool erodeOn;
    bool flipOn;
    bool cannyOn;
};

struct MouseData{
    QRect selectionBox;
    bool leftButtonRelease;
    bool rightButtonRelease;
};

struct ThreadStatisticsData{
    int averageFPS;
    int nFramesProcessed;
};

struct XBeeNode{
    unsigned char commandStatus;
    unsigned int MY;
    unsigned int SH_high;
    unsigned int SH_low;
    unsigned int SL_high;
    unsigned int SL_low;
    QString NI;
    unsigned int parentNetworkAddress;
    unsigned char deviceType;
    unsigned char status;
    unsigned int profileID;
    unsigned int manufacturerID;
};
// ########## Sherry added 12/06/2013
//// eBugData structure definition
//struct EBugData{
//    int id;
//    int x;
//    int y;
//    double angle;
//};

typedef struct relativePos{
    int id;
    float distance;
    float angle;
}relativePos;

struct FormationConstraint {
    double angle1;
    double angle2;
    int dist1;
    int dist2;
};

enum FormationType{
    Max_coverage = 0,
    Dual_line,
    Line,
    Wheel
};

enum FormationRole{
    Follower1 = 0,
    Follower2,
    Follower3,
    Follower4
};
#if 0
static int MaxCoverageConstraints[4][4] =
{
    // 2 constraits, max 2 leaders -> 4 columns
    // dist to leader 1, dist to leader 2, angle to leader 1, angle to leader 2
    {FORMATION_DIST,0,-30, 0},                   // Follower1
    {FORMATION_DIST,FORMATION_DIST,30,90},       // Follower2
    {FORMATION_DIST,FORMATION_DIST,30,-30},      // Follower3
    {FORMATION_DIST,FORMATION_DIST,30,90}        // Follower4
};


static float DualLineConstraints[4][4] =
{
    // 2 constraits, max 2 leaders -> 4 columns
    // dist to leader 1, dist to leader 2, angle to leader 1, angle to leader 2
    {DUAL_DIST,0,-90, 0},                   // Follower1
    {DUAL_DIST,FORMATION_DIST,0,45},       // Follower2
    {DUAL_DIST,DUAL_DIST,0,-90},        // Follower3
    {DUAL_DIST,FORMATION_DIST,0,45}        // Follower4
};

static int LineConstraints[4][2] =
{
    // 2 constraits, max 1 leaders -> 2 columns
    // dist to leader, angle to leader
    {FORMATION_DIST,0},       // Follower1
    {FORMATION_DIST,0},       // Follower2
    {FORMATION_DIST,0},       // Follower3
    {FORMATION_DIST,0}        // Follower4
};

static float WheelConstraints[4][2] =
{
    // 2 constraits, max 1 leaders -> 2 columns
    // dist to neighbours, dist to center
    {WHEEL_RADIUS,54},       // Follower1
    {WHEEL_RADIUS,54},       // Follower2
    {WHEEL_RADIUS,54},       // Follower3
    {WHEEL_RADIUS,54}        // Follower4
};


//// define the angle and distance constraints for various constraint types
//static FormationConstraint constraints[3][4] =
//{
//    {{210,-1,0,-1},{150,90,0,0},{210,270,0,0},{150,210,0,0}}, // max coverage
//    {{180,-1,0,-1}}, // line
//    {{270,-1,0,-1},{180,135,0,0},{180,270,0,0}} // square
//};

#endif // STRUCTURES_H
#endif
