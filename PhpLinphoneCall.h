#ifndef PHPLINPHONE_PHPLINPHONECALL_H
#define PHPLINPHONE_PHPLINPHONECALL_H

#include <phpcpp.h>
#include <vector>
#include <thread>
#include <sstream>
#include <string>
#include <linphone/linphonecore.h>
#include "PhpLinphoneCore.h"

/**
 * This class rapresent a generic linphone call.
 */
class PhpLinphoneCall : public Php::Base {
private:
    /**
     * A pointer to the core class object.
     */
    PhpLinphoneCore *_core;

    /**
     * A pointer to the call object.
     */
    LinphoneCall *_call;

    /*
     * Variable that indicates if the call ring timeout is expired.
     */
    bool ringExpired;

    /**
     * Variable that indicates if the call timeout is expired.
     */
    bool callExpired;

    /**
     * Variable that indicates the ring timeout.
     */
    int ringTimeout;

    /**
     * Variable that indicates the call timeout.
     */
    int callTimeout;

    /**
     * Variable that contains the dtmf, if user pressed it.
     */
    int dtmf;

    /**
     * the user to call.
     */
    std::string user;

    /*
     * A thread object that checks if the time specified by 'ringTimeout' has passed.
     */
    std::thread timeChecker;

    /*
     * A thread object that checks if the time specified by 'callTimeout' has passed.
     */
    std::thread callChecker;

    /**
     * A mutex used to lock the calls vector (which is static)
     */
    static std::mutex theCallsMutex;

    /**
     * A static vector containing all the calls.
     */
    static std::vector<PhpLinphoneCall *> theCalls;
public:
    /**
     * Constructor. Gives a default value to all the attributes.
     */
    PhpLinphoneCall() : _core(nullptr), _call(nullptr), ringExpired(false), callExpired(false), ringTimeout(0), callTimeout(0), dtmf(-1) {}

    /**
     * This method is directly invoked by php interpreter to build the object.
     * @param parameters an array with the accepted parameters.
     */
    void __construct(Php::Parameters &);

    /**
     * This method is directly invoked by php when this object goes out of scope.
     */
    void __destruct();

    /**
     * Virtual destructor.
     */
    virtual ~PhpLinphoneCall() {}

    /**
     * This method allow to the invite an user to a call.
     * @param parameters an array with the accepted parameters.
     */
    void invite(Php::Parameters &);

    /**
     * This method starts a call. Note that it must be called after invite(), or the call will fail.
     */
    void start();

    /**
     * This method hangs up a call.
     */
    void hangup();

    /**
     * This method will be not visible by PHP. It is avaiable only from C++ and is used to expire a call ring after the
     * ringTimeout is passed.
     */
    void ringExpire();

    /**
     * This method will be not visible by PHP. It is avaiable only from C++ and is used to expire a call after the
     * callTimeout is passed.
     */
    void callExpire();

    /**
     * This method allow to specify a ring timeout.
     * @param parameters an array with the accepted parameters.
     */
    void setRingTimeout(Php::Parameters &);

    /**
     * This method allo to specifiy a call timeout. The call timeout will start when the user picks up the call.
     * @param parameters an array with the accepted parameters.
     */
    void setCallTimeout(Php::Parameters &);

    /**
     * This method will be not visible from PHP. It is avaiable only from C++ and is used to specify the DTMF pressed by
     * the remote user.
     * @param an integer representing the DTMF pressed.
     */
    void setDtmf(int);

    /**
     * This is the method representing the work that the timer checker thread must do, if a timeout is specified.
     */
    void timeCheck();

    /*
     * This is the method representing the work that the call checker thread must do, if a call timeout is specified.
     */
    void callCheck();

    /**
     * This method returns true if there an error occurred during the call.
     * @return bool true if an error occurred, else false.
     */
    Php::Value hasError() const;

    /**
     * This method returns the error occurred.
     * @return string a string contaning the error reason.
     */
    Php::Value getError() const;

    /**
     * This method returns true if the call has been released.
     * @return bool true if the call has been released, else false.
     */
    Php::Value isReleased() const;

    /**
     * This method returns true if the call is ended.
     * @return bool true if the call is ended, else false.
     */
    Php::Value isEnded() const;

    /**
     * This method returns true if the call ring is expired.
     * @return bool true if the ring timeout is expired, else false.
     */
    Php::Value isRingExpired() const;

    /**
     * This method returns true if the call is expired.
     * @return bool true if the call timeout is expired, else false.
     */
    Php::Value isCallExpired() const;

    /**
     * This method returns true if there is a running stream.
     * @return bool true if there is a running stream, else false.
     */
    Php::Value isStreamRunning() const;

    /**
     * This method returns the DTMF pressed by the remote user.
     * @return string the DTMF pressed or null
     */
    Php::Value getDtmf() const;

    /**
     * This method returns the invited user.
     * @return string the called user.
     */
    Php::Value getUser() const;

    /**
     * This method will be not visible from PHP. It is avaiable only from C++ and is used to return the pointer to the
     * linphone call.
     * @return LinphoneCall * a pointer to a linphone call.
     */
    LinphoneCall *get() const;

    /**
     * This method will be not visible from PHP. It is avaiable only from C++ and is used to push a call into the
     * calls vector.
     */
    void addCall();

    /**
     * This method will be not visible from PHP. It is avaiable only from C++ and is used to pick up a call from the
     * calls vector and do some operations on it.
     */
    static PhpLinphoneCall *selectCall(const LinphoneCall *);

    /**
     * This method will be not visible from PHP. It is avaiable only from C++ and is used to remove a call from the
     * calls vector.
     */
    static void removeCall(const LinphoneCall *);
};

#endif