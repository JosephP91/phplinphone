#include <iostream>

#include "PhpLinphoneConfig.h"

// Implementation of the set method.
void PhpLinphoneConfig::set(Php::Parameters &parameters) {
    std::string filePath = parameters[0];
    if (filePath.empty()) {
        Php::error<<"The config file specified does not exists."<<std::endl;
    }
    (this->filePath).assign(filePath);
}

// Implemenation of the get method.
const char *PhpLinphoneConfig::get() const {
    return (this->filePath).c_str();
}