#include "PhpLinphoneCore.h"
#include "PhpLinphoneFactoryConfig.h"
#include "PhpLinphoneConfig.h"
#include "PhpLinphoneCall.h"

// Handler for the DTMF press. It selects the call from the calls vector and set the DTMF for it.
static void dtmfHandler(LinphoneCore *lc, LinphoneCall *call, int dtmf) {
    PhpLinphoneCall *theCall = PhpLinphoneCall::selectCall(call);
    if (theCall != nullptr) {
        #ifdef DEBUG
        Php::out<<"[DEBUG - MAIN THREAD] Settings dtmf in the call object"<<std::endl;
        #endif
        theCall->setDtmf(dtmf);
    } else {
        #ifdef DEBUG
        Php::out<<"[DEBUG - MAIN THREAD] Call not found!"<<std::endl;
        #endif
    }
}

// Implementation of the PHP constructor. It initialize the handlers table and create a core object.
void PhpLinphoneCore::__construct(Php::Parameters &parameters) {
    LinphoneCoreVTable vTable = {0};
    vTable.dtmf_received = dtmfHandler;
    const unsigned long size = parameters.size();
    if (size == 1) {
        PhpLinphoneFactoryConfig *factoryConfig = (PhpLinphoneFactoryConfig *)parameters[0].implementation();
        this->_core = linphone_core_new(&vTable,nullptr,factoryConfig->get(),nullptr);
    } else if (size == 2) {
        PhpLinphoneFactoryConfig *factoryConfig = (PhpLinphoneFactoryConfig *)parameters[0].implementation();
        PhpLinphoneConfig *config = (PhpLinphoneConfig *)parameters[1].implementation();
        this->_core = linphone_core_new(&vTable,config->get(),factoryConfig->get(),nullptr);
    } else if (size == 0) {
        this->_core = linphone_core_new(&vTable,nullptr,nullptr,nullptr);
    }
}

// Imlpementation of the PHP destructor. It destroys the core object.
void PhpLinphoneCore::__destruct() {
    if (this->_core != nullptr) {
        linphone_core_destroy(this->_core);
    }
}

// Implementation of the iterate method.
void PhpLinphoneCore::iterate() {
    this->lock();
    linphone_core_iterate(this->_core);
    this->unlock();
    ms_usleep(50000);
}

// Implementation of the lock method.
void PhpLinphoneCore::lock() {
    (this->coreMutex).lock();
}

// Implementation of the unlock method.
void PhpLinphoneCore::unlock() {
    (this->coreMutex).unlock();
}

// Implementation of the get method.
LinphoneCore *PhpLinphoneCore::get() {
    return this->_core;
}