#ifndef PHPLINPHONE_PHPLINPHONEPROXY_H
#define PHPLINPHONE_PHPLINPHONEPROXY_H

#include <phpcpp.h>
#include <linphone/linphonecore.h>

#include "PhpLinphoneCore.h"
#include "PhpLinphoneAuth.h"

/**
 * This object represent a generic linphone proxy object.
 */
class PhpLinphoneProxy : public Php::Base {
private:
    /**
     * A pointer to the linphone proxy configuration object.
     */
    LinphoneProxyConfig *_config;

    /**
     * A pointer to the linphone core object.
     */
    PhpLinphoneCore *_core;

    /**
     * A pointer to the linphone authentication object.
     */
    PhpLinphoneAuth *_auth;
public:
    /**
     * The constructor will initiualize the attributes with their default values.
     */
    PhpLinphoneProxy() : _config(nullptr), _core(nullptr), _auth(nullptr) {}

    /**
     * Virtual destructor.
     */
    virtual ~PhpLinphoneProxy() {}

    /**
     * This method is directly invoked by PHP to build and istance of this class.
     * @param parameters an array with the accepted parameters.
     */
    void __construct(Php::Parameters &);

    /**
     * This method allows to start the proxy configuration.
     */
    void configure();

    /*
     * This method allows to start the authentication on the proxy.
     */
    void authenticate();

    /**
     * This method allows to disable a proxy for registration.
     */
    void disable();

    /**
     * This method allows to clear the registration for this proxy.
     */
    void clear();

    /**
     * This method returns true if the registration is ok, else false.
     * @return bool true if the registration is ok, else false.
     */
    Php::Value isRegistered() const;

    /**
     * This method returns ture if the registration has been cleared, else false.
     * @return bool true if the registration has been cleared, else false.
     */
    Php::Value isClear() const;

    /**
     * This method returns true if the registration has errors, else flase.
     * @return bool true if the registration has errors, else false.
     */
    Php::Value hasError() const;

    /**
     * This method returns the error reason.
     * @return a string containing the error reason.
     */
    Php::Value getError() const;
};

#endif