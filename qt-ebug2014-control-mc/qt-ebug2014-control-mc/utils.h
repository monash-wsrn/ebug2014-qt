// $Revision$
// $Date$
// $Author$

#ifndef UTILS_H
#define UTILS_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

float PointsDist(int ax, int ay, int bx, int by){
    return sqrt(pow(bx-ax,2)+pow(by-ay,2));
}


#endif // UTILS_H
