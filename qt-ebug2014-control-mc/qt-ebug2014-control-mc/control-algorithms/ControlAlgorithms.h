#if 0

// $Revision: 426 $  $Date: 2013-11-24 14:47:09 +1100 (Sun, 24 Nov 2013) $
#ifndef CONTROLALGORITHMS_H
#define CONTROLALGORITHMS_H

#include "Structures.h"
#include "EBugData.h"

#include "Config.h"
#include <ebug-api-cpp/eBugAPI.h>
#include "XBeeThread.h"
class ControlAlgorithms {

public:
    virtual int Reset()
    { return (0); }
    virtual int Init()
    { return (0); }
//    virtual int Controller()
//    { return (0); }
    virtual int Start()
    { return (0); }

private:
    int node_n;     // number of nodes


};

#endif // CONTROLALGORITHMS_H

#endif
