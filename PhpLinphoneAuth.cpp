#include "PhpLinphoneAuth.h"

// Implementation of PHP constructor.
void PhpLinphoneAuth::__construct(Php::Parameters &parameters) {
    this->core = (PhpLinphoneCore *)parameters[0].implementation();
}

// Implementation of PHP destructor.
void PhpLinphoneAuth::__destruct() {
    if (this->from != nullptr) {
        linphone_address_unref(this->from);
    }
    if (this->info != nullptr) {
        linphone_auth_info_destroy(this->info);
    }
}

// Implementation of setUsername method.
void PhpLinphoneAuth::setUsername(Php::Parameters &parameters) {
    std::string user = parameters[0];
    this->username = user;
    this->from = linphone_address_new((this->username).c_str());
    if (this->from == nullptr) {
        Php::error<<"The username specified is not a valid sip URI."<<std::endl;
    }
}

// Implementation of setPassword method.
void PhpLinphoneAuth::setPassword(Php::Parameters &parameters) {
    this->password = parameters[0].stringValue();
}

// Implementation of build method.
void PhpLinphoneAuth::build() {
    if (this->from != nullptr) {
        this->info = linphone_auth_info_new(linphone_address_get_username(this->from),NULL,(this->password).c_str(),NULL,NULL,NULL);
        if (this->info == nullptr) {
            Php::error<<"Error while building the authentication info."<<std::endl;
        }
    } else {
        Php::error<<"The address is not set yet. Set the username first with setUsername method."<<std::endl;
    }
    linphone_core_add_auth_info((this->core)->get(),this->info);
}

// Implementation of get method.
LinphoneAuthInfo *PhpLinphoneAuth::get() {
    return this->info;
}

// Implementation of getFrom method.
LinphoneAddress *PhpLinphoneAuth::getFrom() {
    return this->from;
}

// Implementation of getUsername method.
std::string PhpLinphoneAuth::getUsername() const {
    return this->username;
}