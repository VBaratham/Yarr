#ifndef FEI4EXTTRIGGER_H
#define FEI4EXTTRIGGER_H

// #################################
// # Author: Timon Heim
// # Email: timon.heim at cern.ch
// # Project: Yarr
// # Description: Fei4 Noise Scan
// # Comment: Nothing fancy
// ################################

#include "ScanBase.h"

#include "AllStdActions.h"
#include "AllFei4Actions.h"

class Fei4Exttrigger : public ScanBase {
    public:
        Fei4Exttrigger(Bookkeeper *k);
        
        void init();
        void preScan();
        void postScan() {}

    private:
        double triggerFrequency;
        unsigned triggerTime; 
        bool verbose;
};

#endif
