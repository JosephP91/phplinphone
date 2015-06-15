#ifndef PHPLINPHONE_PHPLINPHONECORE_H
#define PHPLINPHONE_PHPLINPHONECORE_H

#include <phpcpp.h>
#include <linphone/linphonecore.h>
#include <mutex>

/**
 * This class rapresent the linphone core object.
 */
class PhpLinphoneCore : public Php::Base {
private:
    /**
     * A pointer to the linphone core object.
     */
    LinphoneCore *_core;

    /**
     * A mutex used to synchronize the liblinphone calls.
     */
    std::mutex coreMutex;
public:
    /**
     * The constuctor will initialize the core pointer.
     */
    PhpLinphoneCore() : _core(nullptr) {}

    /**
     * This method is directly invoked by PHP interpreter to build an istance of this class.
     * @param parameters an array with the accepted parameters.
     */
    void __construct(Php::Parameters &);

    /**
     * This method is directly invoked by PHP destructor to destroy an istance of this class.
     */
    void __destruct();

    /**
     * Virtual destructor.
     */
    virtual ~PhpLinphoneCore() {}

    /**
     * This method must be called periodically to allow linphone to do background work.
     */
    void iterate();

    /**
     * This method is not visible from PHP, but only from C++, it is used to lock the core object.
     */
    void lock();

    /**
     * This method is not visible from PHP, but only from C++, it is used to unlock the core object.
     */
    void unlock();

    /**
     * This method returns the pointer to the linphone core object.
     */
    LinphoneCore *get();
};


#endif //PHPLINPHONE_PHPLINPHONECORE_H
