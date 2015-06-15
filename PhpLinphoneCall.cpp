#include "PhpLinphoneCall.h"
#include <iostream>

// Declaration of the static vector which contains the calls.
std::vector<PhpLinphoneCall *> PhpLinphoneCall::theCalls;
// Declaration of the static mutex used to lock and unlock the calls vector.
std::mutex PhpLinphoneCall::theCallsMutex;

// Php constructor implementation.
void PhpLinphoneCall::__construct(Php::Parameters &parameters) {
    this->_core = (PhpLinphoneCore *)parameters[0].implementation();
}

// Php destructor implementation.
void PhpLinphoneCall::__destruct() {
    this->hangup();
}

// Invite method implementation.
void PhpLinphoneCall::invite(Php::Parameters &parameters) {
    this->user = parameters[0].stringValue();
    if ((this->user).empty()) {
        Php::error<<"Invalid user to invite"<<std::endl;
    }
    this->_call = linphone_core_invite((this->_core)->get(),(this->user).c_str());
    if (this->_call == nullptr) {
        Php::error<<"Can not invite user "<<this->user<<std::endl;
    }
}

// This is the work for the timer checker thread.
void PhpLinphoneCall::timeCheck() {
    for (int i = 0 ; i < this->ringTimeout; ++i) {
        (this->_core)->lock();
        if (this->isEnded() or this->isReleased() or this->hasError()) {
            #ifdef DEBUG
            Php::out<<"[DEBUG - TIME THREAD] Call is ended.";
            #endif
            break;
        }
        if (this->isStreamRunning()) {
            #ifdef DEBUG
            Php::out<<"[DEBUG - TIME THREAD] User answered."<<std::endl;
            Php::out<<"[DEBUG - TIME THREAD] I will spawn a call checker thread ...";
            #endif
            if (this->callTimeout > 0) {
                // Spawn call checker thread.
                this->callChecker = std::thread(&PhpLinphoneCall::callCheck, this);
            }
            break;
        }
        if (i+1 == this->ringTimeout) {
            #ifdef DEBUG
            Php::out<<"[DEBUG - TIME THREAD] Ring timeout is expired.";
            #endif
            this->ringExpire();
            break;
        }
        #ifdef DEBUG
        Php::out<<"[DEBUG - TIME THREAD] Waiting "<<i+1<<" seconds."<<std::endl;
        #endif
        (this->_core)->unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    #ifdef DEBUG
    Php::out<<" I will terminate."<<std::endl;
    #endif
    (this->_core)->unlock();
}

// This is the work for the call checker thread.
void PhpLinphoneCall::callCheck() {
    for (int i = 0; i < this->callTimeout; ++i) {
        (this->_core)->lock();
        if (this->isEnded() or this->isReleased() or this->hasError()) {
            #ifdef DEBUG
            Php::out<<"[DEBUG - CALL THREAD] Call is ended.";
            #endif
            break;
        }
        if (i + 1 == this->callTimeout) {
            #ifdef DEBUG
            Php::out<<"[DEBUG - CALL THREAD] Call timeout is expired!";
            #endif
            this->callExpire();
            break;
        }
        #ifdef DEBUG
        Php::out<<"[DEBUG - CALL THREAD] I will wait "<<i+1<<" seconds."<<std::endl;
        #endif
        (this->_core)->unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    #ifdef DEBUG
    Php::out<<" I will terminate."<<std::endl;
    #endif
    (this->_core)->unlock();
}

/**
 * Implementation of the start method. Acquires a reference to the call, pushes it into the calls vector and, then
 * spawn a thread that checks if the timeout specified has passed.
 */
void PhpLinphoneCall::start() {
    if (this->_call == nullptr) {
        Php::error<<"Call is not initialized. Call invite method first!"<<std::endl;
    }
    linphone_call_ref(this->_call);
    this->addCall();
    #ifdef DEBUG
    Php::out<<"[DEBUG - MAIN THREAD] Spawning time checker thread ..."<<std::endl;
    #endif
    if (this->ringTimeout > 0) {
        this->timeChecker = std::thread(&PhpLinphoneCall::timeCheck, this);
    }
}

/*
 * Implementation of the hangup method. It terminates the call and releases the reference from the core, to it. Then,
 * the timeout threads are joined, if they are in a joinable state
 */
void PhpLinphoneCall::hangup() {
    if (this->_call) {
        PhpLinphoneCall::removeCall(this->_call);
        (this->_core)->lock();
        if (!this->isEnded() and !this->isReleased()) {
            linphone_core_terminate_call((this->_core)->get(), this->_call);
            #ifdef DEBUG
            Php::out<<"[DEBUG - MAIN THREAD] Call terminated."<<std::endl;
            #endif
            linphone_call_unref(this->_call);
            #ifdef DEBUG
            Php::out<<"[DEBUG - MAIN THREAD] Call reference has been released."<<std::endl;
            #endif
        }
        (this->_core)->unlock();
        #ifdef DEBUG
        Php::out<<"[DEBUG - MAIN THREAD] Check if time checker can be terminated ... "<<std::endl;
        #endif
        if ((this->timeChecker).joinable()) {
            #ifdef DEBUG
            Php::out<<"[DEBUG - MAIN THREAD] Yes, it can be terminated. Joining it ... ";
            #endif
            (this->timeChecker).join();
            #ifdef DEBUG
            Php::out<<"joined!"<<std::endl;
            #endif
        }
        #ifdef DEBUG
        Php::out<<"[DEBUG - MAIN THREAD] No, maybe it is already terminated."<<std::endl;
        Php::out<<"[DEBUG - MAIN THREAD] Check if the call checker can be terminated ... "<<std::endl;
        #endif
        if ((this->callChecker).joinable()) {
            #ifdef DEBUG
            Php::out<<"[DEBUG - MAIN THREAD] Yes, it can be terminated. Joining it ... ";
            #endif
            (this->callChecker).join();
            #ifdef DEBUG
            Php::out<<"joined!"<<std::endl;
            #endif
        }
        #ifdef DEBUG
        Php::out<<"[DEBUG - MAIN THREAD] No, maybe it is already terminated."<<std::endl;
        #endif
    }
}

// Implementation of the expire method.
void PhpLinphoneCall::ringExpire() {
    this->ringExpired = true;
}

void PhpLinphoneCall::callExpire() {
    this->callExpired = true;
}

// Implementation of the timeout setter method.
void PhpLinphoneCall::setRingTimeout(Php::Parameters &parameters) {
    this->ringTimeout = parameters[0];
    if (this->ringTimeout <= 0 or this->ringTimeout > 60) {
        Php::error<<"Attempt to set an invalid ring timeout! Will set it to 30 seconds"<<std::endl;
    }
}

// Implementation of the call timeout setter method.
void PhpLinphoneCall::setCallTimeout(Php::Parameters &parameters) {
    this->callTimeout = parameters[0];
    if (this->callTimeout <= 0 or this->callTimeout > 60) {
        Php::error<<"Attempt to set an invalid call timeout! Will set it to 30 seconds!"<<std::endl;
    }
}

// Implementation of the dtmf setter method.
void PhpLinphoneCall::setDtmf(int dtmf) {
    this->dtmf = dtmf;
}

// Implementation of the isEnded method.
Php::Value PhpLinphoneCall::isEnded() const {
    return this->_call ? linphone_call_get_state(this->_call) == LinphoneCallEnd : true;
}

// Implementation of the hasError method.
Php::Value PhpLinphoneCall::hasError() const {
    return this->_call ? linphone_call_get_state(this->_call) == LinphoneCallError : false;
}

// Implementation of getError method.
Php::Value PhpLinphoneCall::getError() const {
    const char *reason = linphone_reason_to_string(linphone_call_get_reason(this->_call));
    return reason != nullptr ? std::string(reason) : nullptr;
}

Php::Value PhpLinphoneCall::isReleased() const {
    return this->_call ? linphone_call_get_state(this->_call) == LinphoneCallReleased : true;
}

// Implementation of the isStreamRunning method.
Php::Value PhpLinphoneCall::isStreamRunning() const {
    return this->_call ? linphone_call_get_state(this->_call) == LinphoneCallStreamsRunning : false;
}

// Implementation of the isRingExpired method.
Php::Value PhpLinphoneCall::isRingExpired() const {
    return this->ringExpired;
}

// Implementation of the isCallExpired method.
Php::Value PhpLinphoneCall::isCallExpired() const {
    return this->callExpired;
}

// Implementation of the getDtmf method. It returns the casted value for the dtmf previously specified.
Php::Value PhpLinphoneCall::getDtmf() const {
    if (this->dtmf != -1) {
        std::stringstream stream;
        stream<< static_cast<char>(this->dtmf);
        return stream.str();
    }
    return nullptr;
}

// Implementation of the getUser method.
Php::Value PhpLinphoneCall::getUser() const {
    return this->user;
}

// Implementation of the get method. It returns a pointer to the LinphoneCall object.
LinphoneCall *PhpLinphoneCall::get() const {
    return this->_call;
}

// Implementation of the addCall method. It locks the calls vector, pushes the calls into it, and release the lock.
void PhpLinphoneCall::addCall() {
    theCallsMutex.lock();
    theCalls.push_back(this);
    #ifdef DEBUG
    Php::out<<"[DEBUG - MAIN THREAD] Call pushed in the calls vector. Current size: "<<theCalls.size()<<std::endl;
    #endif
    theCallsMutex.unlock();
}

/**
 * Implementation of the selectCall method. It locks the calls vector, walks throught its elements and finds the pointer
 * with the same address of the one specified as parameter. If a call is found, is returned.
 */
PhpLinphoneCall *PhpLinphoneCall::selectCall(const LinphoneCall *theCall) {
    theCallsMutex.lock();
    unsigned long size = theCalls.size();
    for (unsigned long i = 0; i < size; ++i) {
        PhpLinphoneCall *aCall = theCalls.at(i);
        if (aCall->get() == theCall) {
            theCallsMutex.unlock();
            return aCall;
        }
    }
    theCallsMutex.unlock();
    return nullptr;
}

/**
 * Implementation of the removeCall method. It locks the calls vector, walks throught its elements and finds the pointer
 * with the same memory address of the one specified as parameters. If a call is found, is removed from the calls vector.
 */
void PhpLinphoneCall::removeCall(const LinphoneCall *theCall) {
    theCallsMutex.lock();
    unsigned long size = theCalls.size();
    for (unsigned long i = 0; i < size; ++i) {
        if ((theCalls.at(i))->get() == theCall) {
            theCalls.erase(theCalls.begin()+i);
            #ifdef DEBUG
            Php::out<<"[DEBUG - MAIN THREAD] Removed call from the calls array. Current size: "<<theCalls.size()<<std::endl;
            #endif
            break;
        }
    }
    theCallsMutex.unlock();
}