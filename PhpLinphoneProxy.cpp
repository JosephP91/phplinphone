#include "PhpLinphoneProxy.h"

// Implementation of PHP constructor.
void PhpLinphoneProxy::__construct(Php::Parameters &parameters) {
    this->_core = (PhpLinphoneCore *)parameters[0].implementation();
    this->_auth = (PhpLinphoneAuth *)parameters[1].implementation();
    this->_config = linphone_core_create_proxy_config((this->_core)->get());
    if (this->_config == nullptr) {
        Php::error << "Can not create config proxy!" << std::endl;
    }
}

// Implementation of configure method.
void PhpLinphoneProxy::configure() {
    std::string username = (this->_auth)->getUsername();
    if (username.empty()) {
        Php::error<<"Error while setting the identity. Invalid username."<<std::endl;
    }
    LinphoneAddress *address = (this->_auth)->getFrom();
    if (address == nullptr) {
        Php::error<<"Error while setting the identity. Invalid address."<<std::endl;
    }
    linphone_proxy_config_set_identity(this->_config,username.c_str());
    linphone_proxy_config_set_server_addr(this->_config,linphone_address_get_domain(address));
    linphone_proxy_config_enable_register(this->_config,TRUE);
    linphone_core_add_proxy_config((this->_core)->get(),this->_config);
    linphone_core_set_default_proxy_config((this->_core)->get(),this->_config);
}

// Implementation of authenticate method.
void PhpLinphoneProxy::authenticate() {
    while (!this->isRegistered() and !this->hasError()) {
        (this->_core)->iterate();
    }
}

// Implementation if disable method.
void PhpLinphoneProxy::disable() {
    linphone_core_get_default_proxy((this->_core)->get(),&(this->_config));
    linphone_proxy_config_edit(this->_config);
    linphone_proxy_config_enable_register(this->_config,FALSE);
    linphone_proxy_config_done(this->_config);
}

// Implementation of clear method.
void PhpLinphoneProxy::clear() {
    while (!this->isClear() and !this->hasError()) {
        (this->_core)->iterate();
    }
}

// Implementation of isRegistered method.
Php::Value PhpLinphoneProxy::isRegistered() const {
    return linphone_proxy_config_is_registered(this->_config) == TRUE;
}

// Implementation of isClear method.
Php::Value PhpLinphoneProxy::isClear() const {
    return linphone_proxy_config_get_state(this->_config) != LinphoneRegistrationCleared;
}

// Implementation of hasError method.
Php::Value PhpLinphoneProxy::hasError() const {
    return linphone_proxy_config_get_state(this->_config) == LinphoneRegistrationFailed;
}

// Implementation of getError method.
Php::Value PhpLinphoneProxy::getError() const {
    const char *reason = linphone_reason_to_string(linphone_proxy_config_get_error(this->_config));
    return reason != nullptr ? std::string(reason) : nullptr;
}