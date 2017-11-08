// #################################
// # Author: Timon Heim
// # Email: timon.heim at cern.ch
// # Project: Yarr
// # Description: Fei4 Selftrigger
// # Comment: Nothing fancy
// ################################

#include "Fei4Exttrigger.h"

Fei4Exttrigger::Fei4Exttrigger(Bookkeeper *k) : ScanBase(k) {
    triggerFrequency = 1e4;
    triggerTime = 30;
    verbose = false;
}

// Initialize Loops
void Fei4Exttrigger::init() {
    // Loop 1: Trigger
    std::shared_ptr<Fei4TriggerLoop> triggerLoop(new Fei4TriggerLoop);
    triggerLoop->setVerbose(verbose);
    triggerLoop->setTrigFreq(triggerFrequency);
    triggerLoop->setTrigTime(triggerTime);
    // triggerLoop->setTrigCnt(0); // Activated time mode
    triggerLoop->setNoWord(); // Fei4 exttrigger just sends data
    triggerLoop->setExtTrigger();

    // Loop 2: Data gatherer
    std::shared_ptr<StdDataGatherer> dataLoop(new StdDataGatherer);
    dataLoop->setVerbose(verbose);
    dataLoop->connect(g_data);

    this->addLoop(triggerLoop);
    this->addLoop(dataLoop);
    
    engine.init();
}

// Do necessary pre-scan configuration
void Fei4Exttrigger::preScan() {
    unsigned hitDiscCfg = g_fe->getValue(&Fei4::HitDiscCnfg);
    std::cout << "hitDiscCfg = " << hitDiscCfg << std::endl;
    g_fe->writeRegister(&Fei4::Trig_Lat, 235);
    g_fe->writeRegister(&Fei4::HitOr, 0); // do not trigger on HitOr pulses (GateHitOr)
    g_fe->writeRegister(&Fei4::Trig_Count, 5);
    while(!g_tx->isCmdEmpty());
}

