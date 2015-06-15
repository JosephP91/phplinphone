#ifndef PHPLINPHONE_PHPLINPHONEAUTH_H
#define PHPLINPHONE_PHPLINPHONEAUTH_H

#include <phpcpp.h>
#include <linphone/linphonecore.h>
#include <iostream>

#include "PhpLinphoneCore.h"

/**
 * This class rapresent a linphone authentication object.
 */
class PhpLinphoneAuth : public Php::Base {
private:
    /**
     * The authentication username.
     */
    std::string username;

    /**
     * The authentication password.
     */
    std::string password;

    /**
     * A pointer to linphone core object.
     */
    PhpLinphoneCore *core;

    /**
     * A pointer to linphone authentication informations.
     */
    LinphoneAuthInfo *info;

    /**
     * A pointer to the address of the user to call.
     */
    LinphoneAddress *from;
public:
    /**
     * Constructor gives a default value to the attributes.
     */
    PhpLinphoneAuth() : core(nullptr), info(nullptr), from(nullptr) {}

    /**
     * This method is automatically invoked by PHP to build an istance of this class.
     * @param parameters an array with the accepted parameters.
     */
    void __construct(Php::Parameters &);

    /**
     * Virtual destructor.
     */
    virtual ~PhpLinphoneAuth() {}

    /**
     * This method is automatically invoked by PHP when this object goes out of scope.
     */
    void __destruct();

    /**
     * This method allows to specify an authentication username.
     */
    void setUsername(Php::Parameters &);

    /**
     * This method allows to specifiy an authentication password.
     */
    void setPassword(Php::Parameters &);

    /**
     * This method builds the authentication object.
     */
    void build();

    /**
     * This method is not visible from PHP. It is visible only from C++ and it returns the linphone authentication info.
     */
    LinphoneAuthInfo *get();

    /**
     * This method is not visible from PHP. It is visible only from C++ and it returns the linphone address to call.
     */
    LinphoneAddress *getFrom();

    /**
     * This method returns the username of the called user.
     */
    std::string getUsername() const;
};

#endif //PHPLINPHONE_PHPLINHONEAUTH_H